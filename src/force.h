#ifndef FORCE_H_
#define FORCE_H_

struct Cforce {
	typedef double Tforce;
	Tforce fx,fy;
	Cforce(Tforce fx,Tforce fy):fx(fx),fy(fy) {};
	Cforce(const Cforce& f):fx(f.fx),fy(f.fy) {};
	Cforce():fx(0.0),fy(0.0) {}
	
};

#endif /*FORCE_H_*/
