#include "clusterbuild.h"
#include <time.h>
#include <iostream>

void Cclusterbuild::addCluster(int pcount,int mcount,double x, double y) {
	//sun
	double cvx = (rand() % 30000 + 500000);
	double cvy = (rand() % 30000 + 500000);
	psim->addPlanet(x,y,cvx,cvy,1e11);
	std::cerr<<"-";
	//planets
	for(int i=0;i<pcount;++i) {
	
		double nx = rand() % 8000000 - 4000000 + x;
		nx+=(nx>x?1000000:-1000000);
		double ny = rand() % 8000000 - 4000000 + x;
		ny+=(ny>y?1000000:-1000000);
		double vx = nx - x;
		double vy = ny - y;
		//um 90 ° kippen
		double t = vy;
		vy = vx;
		vx = -t;
		//einheitsvektor draus machen
		double lange = sqrt(pow(vx,2)+pow(vy,2));
		vx/=lange;
		vy/=lange;
		//neuen vektor berechnen
		double speed = (rand() % 100000 + 500000);
		vx*=speed;
		vy*=speed;
		double weight = 1e9;
		psim->addPlanet(nx,y,vx+cvx,vy+cvy,weight);
		std::cerr<<"+";
		//moon?
		if(mcount>0) {
			for(int e=0;e<mcount;++e) {
				std::cerr<<"_";
				double mx = (rand() % 20000 - 10000) + nx;
				mx+=(mx>nx?40000:-40000);
				double my = (rand() % 20000 - 10000) + ny;
				my+=(my>y?40000:-40000);
				double mvx = mx - nx;
				double mvy = my - ny;
				//um 90 ° kippen
				t = mvy;
				mvy = mvx;
				mvx = -t;
				//einheitsvektor draus machen
				lange = sqrt(pow(mvx,2)+pow(mvy,2));
				mvx/=lange;
				mvy/=lange;
				//neuen vektor berechnen
				double speed = 60000 + (rand() % 10000);
				mvx*=speed;
				mvy*=speed;
				double mweight = 5e6 + (rand() % (int)1e6);
				psim->addPlanet(mx,my,mvx+vx+cvx,mvy+vy+cvy,mweight);	
			}
		}
		
	}	
	std::cerr<<"\n";
}
