#ifndef CLUSTERBUILD_H_
#define CLUSTERBUILD_H_

#include "planets.h"

class Cclusterbuild {
	
	protected:
		Cplanets* psim;
	
	public:
		Cclusterbuild(Cplanets* psim):psim(psim) {}
		
		void addCluster(int pcount,int mcount,double x, double y);
	
	
};

#endif /*CLUSTERBUILD_H_*/
