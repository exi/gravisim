#ifndef GRAVENV_H_
#define GRAVENV_H_

#ifndef CORECOUNT
#define CORECOUNT 2
#endif

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

#include "dlib/pipe.h"
#include "dlib/threads.h"
#include "dlib/misc_api.h"
#include "planets.h"
#include "force.h"
#include <vector>
#include <list>
#include <iostream>
#include <stdio.h>

#ifndef DEBUG
#define DEBUG false
#endif
#define DBG(arg) if(DEBUG==true) arg

class Cgravenv : public Cplanets
{

    private:
        typedef boost::mutex::scoped_lock scoped_lock;
        Cplanets rplanets;
        boost::thread* pThread;

    public:
        struct Job{
            Job():start(0),end(0) {}
            Job(int start, int end):start(start),end(end) {}

            int start,end;
        };

        Cgravenv() : job_pipe(CORECOUNT*2),stopThreads(0) {
            activeThreads=0;
            for(int i=0;i<CORECOUNT;i++) {
                pThread = new boost::thread(
                        boost::bind( 
                            &Cgravenv::thread,             // member function
                            this ) );
            }
        }

        ~Cgravenv () {
            stopThreads = true;
            job_pipe.disable();
            {
                scoped_lock lock(stop_mutex);
                if(activeThreads>0) {
                    DBG(fprintf(stderr,"activeThreads:%d\n",activeThreads));
                    while(activeThreads>0)
                        stopcond.wait(lock);
                }
            }
        }

        void work();

        void thread();

    protected:	
        dlib::pipe<Cgravenv::Job>::kernel_1a job_pipe;
        boost::mutex p_mutex;
        boost::mutex *pl_mutex;
        boost::mutex stop_mutex;
        boost::condition stopcond;
        boost::condition workcond;
        bool stopThreads;
        int processcount;
        int activeThreads;

};

#endif /*CALCULATOR_H_*/
