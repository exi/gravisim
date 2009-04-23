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

    rplanets = *this;
    pl_mutex = new mutex[(int)planets.size()];
    DBG(fprintf(stderr,"enabling pipe \n"));
    job_pipe.enable();
    processcount = 0;
    maxProcesscount = 0;

    //start();

    {
        DBG(fprintf(stderr,"adding jobs \n"));
        int start=0,end;
        int packetsize = (int)((double)planets.size()+1.0)*((double)planets.size()/2.0);
        packetsize = ceil(packetsize/CORECOUNT);

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
            p_mutex.lock();
            ++maxProcesscount;
            p_mutex.unlock();
        }
        DBG(fprintf(stderr,"jobs added \n"));
    }


    if(getPCount()!=maxProcesscount)
        while (getPCount()!=maxProcesscount) {	
            psig->wait_or_timeout(10);
        }

    DBG(fprintf(stderr,"disabling pipe \n"));
    //job_pipe.disable();

    //stop();
    //wait();

    delete [ ] pl_mutex;

    {
        int size=(int)planets.size();
        for(int i=0;i<size;++i)
            if(i<size) {
                (*this)[i].vStep();
                for(int e=i-1;e>=0;--e) {
                    if( ((*this)[i].dist((*this)[e])) < (*this)[i].getRadius() + (*this)[e].getRadius() ){
                        (*this)[i]+=(*this)[e];
                        delPlanet(e);
                        --size;
                        --i;
                    }
                }
            }
    }
}

void Cgravenv::thread() {
    // Here we loop on jobs from the job_pipe.
    DBG(fprintf(stderr,"\n thread start\n"));

    Job j;
    while (job_pipe.dequeue(j) && !should_stop())
    {
        int offset = j.start;
        std::vector<Cforce> tforces((int)rplanets.size()-offset);
        DBG(fprintf(stderr,"\n job range: %i - %i\n",j.start,j.end));

        for(int i=j.start;i<=j.end;++i)
            for(int e=i+1;e<(int)rplanets.size();++e)
                if(i!=e) {    		
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
                }
        for(int i=0;i<(int)tforces.size();++i) {
            {
                auto_mutex locker(pl_mutex[i+offset]);
                (*this)[i+offset].adjustSpeed(tforces[i]);
            }
        }
        {
            auto_mutex locker(p_mutex);
            ++processcount;
            psig->signal();
        }
    }

    DBG(fprintf(stderr,"\n thread stop\n"));
}
