#include "gravcalc.h"
#include "dlib/threads.h"
#include "dlib/misc_api.h"  // for dlib::sleep
#include "dlib/pipe.h"
#include "dlib/logger.h"
#include <vector>
#include <list>
#define GRAV 6.67428

using namespace dlib;


void gravcalc::thread() {
//	fprintf(stderr,"\n thread start\n");
    Cplanetsim::TplanetJob j;
    Cplanetsim::TplanetForcesList tforces;
    Cplanetsim::Tplanets planets = ps.planets;
    // Here we loop on jobs from the job_pipe. 
    while (job_pipe.dequeue(j) && should_stop() == false)
    {
    	for(int e=j.i+1;e<(int)planets.size();e++)
    		if(j.i!=e) {
	        	double d = ps.dist(planets[j.i],planets[e]);
				double gravw = ( 1 / ( planets[j.i].weight + planets[e].weight ) ) * planets[e].weight;
				double F = ( -GRAV * ( ( planets[j.i].weight * planets[e].weight ) / d ) );
				double F1 = F * gravw;
				double F2 = F * ( 1 - gravw);
				double sina = ps.sy(planets[j.i],planets[e]) / d;
				double sina2 = -sina;
				double cosa = ps.sx(planets[j.i],planets[e]) / d;
				double cosa2 = -cosa;
				{
					tforces.push_back(Cplanetsim::TplanetForce(cosa * F1,sina * F1,j.i));
					tforces.push_back(Cplanetsim::TplanetForce(cosa2 * F2,sina2 * F2,e));
				}
    		}
    	{
    		auto_mutex locker(p_mutex);
			processcount++;
			sig->signal();
    	}
    }

    auto_mutex locker(f_mutex);
    while(!tforces.empty()) {
    	forces[tforces.front().id].x+=tforces.front().x;
    	forces[tforces.front().id].y+=tforces.front().y;
    	tforces.pop_front();
    }
  //fprintf(stderr,"\n thread stop\n");
}

void gravcalc::stopit() {
	//fprintf(stderr,"\n ding \n");
	job_pipe.disable();
	//fprintf(stderr,"\n stoping \n");
	stop();
	//fprintf(stderr,"\n waiting \n");
    wait();
    //fprintf(stderr,"\n done \n");
}
