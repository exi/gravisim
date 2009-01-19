#include "gravenv.h"
#include "force.h"
#include "camera.h"
#include <vector>
#include <list>
#include <iostream>
#define GRAV 6.67428

void Cgravenv::work() {
    job_pipe.enable();

    rplanets = *this;
    processcount = 0;
    maxProcesscount = 0;
    pl_mutex = new mutex[(int)planets.size()];

    start();

    for(int i=0;i<(int)planets.size();i++) { // calc forces
	Job dummy;
	dummy.i=i;
	job_pipe.enqueue(dummy);
	p_mutex.lock();
	++maxProcesscount;
	p_mutex.unlock();
    }


    {
	if(getPCount()!=maxProcesscount)
	    while (getPCount()!=maxProcesscount) {	
		psig->wait_or_timeout(10);
	    }
    }

    job_pipe.disable();
    stop();
    wait();

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
    Job j;
    std::vector<Cforce> tforces((int)rplanets.size());
    for(int i=0;i<(int)tforces.size();++i)
	tforces[i].fx=tforces[i].fy=0.0;
    while (job_pipe.dequeue(j) && should_stop() == false)
    {
	for(int e=j.i+1,s=0;e<(int)rplanets.size();++e,s+=2)
	    if(j.i!=e) {    		
		double d = rplanets[j.i].dist(rplanets[e]);
		if(d==0)++d;
		double gravw = ( 1 / ( rplanets[j.i].getWeight() + rplanets[e].getWeight() ) ) * rplanets[e].getWeight();
		double F = ( -GRAV * ( ( rplanets[j.i].getWeight() * rplanets[e].getWeight() ) / d ) );
		double F1 = F * gravw;
		double F2 = F * ( 1 - gravw);
		double sina = rplanets[j.i].sy(rplanets[e]) / d;
		double sina2 = -sina;
		double cosa = rplanets[j.i].sx(rplanets[e]) / d;
		double cosa2 = -cosa;
		tforces[j.i].fx+=cosa * F1;
		tforces[j.i].fy+=sina * F1;
		tforces[e].fx+=cosa2 * F2;
		tforces[e].fy+=sina2 * F2;
	    }
	{
	    auto_mutex locker(p_mutex);
	    ++processcount;
	    psig->signal();
	}
    }
    psig->signal();
    for(int i=0;i<(int)tforces.size();++i) {
	{
	    auto_mutex locker(pl_mutex[i]);
	    (*this)[i].adjustSpeed(tforces[i]);
	}
    }
    tforces.clear();
    //fprintf(stderr,"\n thread stop\n");
}
