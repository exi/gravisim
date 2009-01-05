#ifndef GRAVENV_H_
#define GRAVENV_H_

#ifndef CORECOUNT
#define CORECOUNT 2
#endif

#include "dlib/threads.h"
#include "dlib/misc_api.h"  // for dlib::sleep
#include "dlib/pipe.h"
#include "dlib/logger.h"
#include "planets.h"
#include "force.h"
#include <vector>
#include <list>
#include <iostream>

using namespace dlib;

class Cgravenv : 	private multithreaded_object,
    public Cplanets
{

    private:
	Cplanets rplanets;

    public:
	struct Job{
	    Job():i(0) {}
	    Job(int i):i(i) {}

	    int i;	
	};

	Cgravenv() : job_pipe(CORECOUNT*2),finished(0) {
	    psig = new signaler(np_mutex);
	    for(int i=0;i<CORECOUNT;i++) {
		register_thread(*this,&Cgravenv::thread);
		fprintf(stderr,"thread start \n");
	    }
	}

	~Cgravenv () {
	    stop();
	    wait();
	    delete psig;
	}

	void work();

	int getPCount() { 
	    p_mutex.lock();
	    int p = processcount;
	    p_mutex.unlock();
	    return p;
	} 

	int size() {
	    pl_mutex.lock();
	    int p = planets.size();
	    pl_mutex.unlock();
	    return p;
	};

	// Ccamera::Tview exportCoords();
    protected:	
	dlib::pipe<Cgravenv::Job>::kernel_1a job_pipe;
	mutex p_mutex;
	mutex pl_mutex;
	mutex np_mutex;
	bool finished;
	int processcount;
	int maxProcesscount;
	int corecount;
	signaler *psig;


    private:
	void thread ();

};

#endif /*CALCULATOR_H_*/
