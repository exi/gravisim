#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

#include "gravenv.h"
#include "force.h"
#include "camera.h"
#include <vector>
#include <list>
#include <iostream>
#define GRAV 6.67428

#ifndef DEBUG
#define DEBUG false
#endif
#define DBG(arg) if(DEBUG==true) arg

void Cgravenv::work() {

    DBG(fprintf(stderr,"->work \n"));
    rplanets = *this;
    pl_mutex = new boost::mutex[(int)planets.size()];
    DBG(fprintf(stderr,"enabling pipe \n"));
    job_pipe.enable();
    processcount = 0;
    int maxProcesscount = 0;

    //start();

    {
        DBG(fprintf(stderr,"adding jobs \n"));
        int start=0,end;
        int packetsize = (int)((double)planets.size()+1.0)*((double)planets.size()/2.0);
        DBG(fprintf(stderr,"packetsize\n"));
        packetsize = ceil(packetsize/CORECOUNT);
        DBG(fprintf(stderr,"loop\n"));

        while(start<(int)planets.size()) {// calc forces
            end = start;
            int thispacket = (int)planets.size() - end;

            while(thispacket<packetsize && end<(int)planets.size()) {
                ++end;
                thispacket += (int)planets.size() - end;
            }

            if(end>=(int)planets.size()) {
                end=(int)planets.size() - 1;
            }

            Job j(start,end);
            job_pipe.enqueue(j);
            start = end + 1;
            {
                scoped_lock lock(p_mutex);
                ++maxProcesscount;
            }
        }
        DBG(fprintf(stderr,"jobs added \n"));
    }
    DBG(fprintf(stderr,"out\n"));

    {
        scoped_lock lock(p_mutex);
        if(processcount!=maxProcesscount) {
            while (processcount!=maxProcesscount) 
                workcond.wait(lock);
        }
    }

    delete [ ] pl_mutex;

    {
        DBG(fprintf(stderr,"collisions\n"));
        int size=(int)planets.size();
        if(size > 1)
            for(int i=0;i<size;++i)
                if(i<size) {
                    (*this)[i].vStep();
                    for(int e=i-1;e>=0;--e) {
                        if( (*this)[i].dist((*this)[e]) < (*this)[i].getRadius() + (*this)[e].getRadius() ){
                            (*this)[i]+=(*this)[e];
                            delPlanet(e);
                            --size;
                            --i;
                        }
                    }
                }
    }
    DBG(fprintf(stderr,"out\n"));
}

void Cgravenv::thread() {
    // Here we loop on jobs from the job_pipe.
    DBG(fprintf(stderr,"\n thread start\n"));

    Job j;
    while (job_pipe.dequeue(j) && !stopThreads)
    {
        int offset = j.start;
        std::vector<Cforce> tforces((int)rplanets.size()-offset);
        DBG(fprintf(stderr,"\n job range: %i - %i\n",j.start,j.end));

        for(int i=j.start;i<=j.end;++i)
            for(int e=i+1;e<(int)rplanets.size();++e)
                if(i!=e) {    		
                    DBG(fprintf(stderr,"\n thread=> calc start\n"));
                    double d = rplanets[i].dist(rplanets[e]);
                    if(d==0)++d;
                    double gravw = ( 1 / ( rplanets[i].getWeight() + rplanets[e].getWeight() ) ) * rplanets[e].getWeight();
                    double F = ( -GRAV * ( ( rplanets[i].getWeight() * rplanets[e].getWeight() ) / d ) );
                    double F1 = F * gravw;
                    double F2 = F * ( 1 - gravw);
                    double sina = rplanets[i].sy(rplanets[e]) / d;
                    double sina2 = -sina;
                    double cosa = rplanets[i].sx(rplanets[e]) / d;
                    double cosa2 = -cosa;
                    tforces[i-offset].fx+=cosa * F1;
                    tforces[i-offset].fy+=sina * F1;
                    tforces[e-offset].fx+=cosa2 * F2;
                    tforces[e-offset].fy+=sina2 * F2;
                    DBG(fprintf(stderr,"\n thread=> calc end\n"));
                }
        for(int i=0;i<(int)tforces.size();++i) {
                scoped_lock locker(pl_mutex[i+offset]);
                (*this)[i+offset].adjustSpeed(tforces[i]);
        }
        {
            scoped_lock locker(p_mutex);
            ++processcount;
            workcond.notify_all();
        }
    }
    {
        scoped_lock lock(stop_mutex);
        --activeThreads;
        stopcond.notify_all();
    }
    DBG(fprintf(stderr,"\n thread stop\n"));
}
