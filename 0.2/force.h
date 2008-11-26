#ifndef FORCE_H_
#define FORCE_H_

struct Cforce {
	double fx,fy;
	Cforce(double fx,double fy):fx(fx),fy(fy) {};
	Cforce(const Cforce& f):fx(f.fx),fy(f.fy) {};
	Cforce():fx(0),fy(0) {}
	
};

#endif /*FORCE_H_*/
