#ifndef GRAVCALC_H_
#define GRAVCALC_H_

#include "planetsim.h"
#include "dlib/threads.h"
#include "dlib/misc_api.h"  // for dlib::sleep
#include "dlib/pipe.h"
#include "dlib/logger.h"

using namespace dlib;

class gravcalc : private multithreaded_object
{
public:

    gravcalc( Cplanetsim& ps, int corecount) : job_pipe(50),ps(ps),corecount(corecount) {
    	
    	job_pipe.enable();
    	sig = new signaler(p_mutex);
		// register 3 threads
		for(int i=0;i<corecount;i++)
			register_thread(*this,&gravcalc::thread);
		processcount = 0;
		// start the 3 threads we registered above
    	//fprintf(stderr,"\n const finished \n");
    	start();
    }    
    
     ~gravcalc () {
       // dlog << LINFO << "destructing pipe object: job_pipe is empty";
		//fprintf(stderr,"\n disabling \n");
		//stopit();
      //  dlog << LINFO << "destructing pipe object: all threads have ended";
      delete sig;

     }
     
    void stopit(); 
	
    dlib::pipe<Cplanetsim::TplanetJob>::kernel_1a job_pipe;
    Cplanetsim ps;
    Cplanetsim::TplanetForces forces;
    mutex f_mutex;
    mutex p_mutex;
    int processcount;
    int corecount;
    signaler *sig;
    

private:
    void thread ();

};


#endif /*GRAVCALC_H_*/
