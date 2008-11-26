#include "planetsim.h"
#include "gravcalc.h"
#include <math.h>
#include <vector>
#include <list>
#include <algorithm>

#define T 0.1
#define PI 3.1415926535897932
#define ROH 9.0
#define FAKTOR 3.0 / ( 4.0 * ROH * PI )

void Cplanetsim::addPlanet(double x, double y, double vx, double vy, double weight) {
	
	Tplanet dummy;
	dummy.x=x * 100;
	dummy.y=y * 100;
	dummy.vx=vx;
	dummy.vy=vy;
	dummy.weight=weight;
	dummy.radius=calcRadius(weight);
	this->planets.push_back(dummy);	
}

void Cplanetsim::addPlanet(Tplanet planet) {
	
	this->planets.push_back(planet);
	
}

void Cplanetsim::delPlanet(int nr) {
	Tplanets::iterator iter;
	iter=planets.begin();
	for(int i=0;i<nr;i++)
		iter++;
	planets.erase(iter);
}

int Cplanetsim::mergePlanets(int p1, int p2) {
	int dest=p1;
	int del=p1;
	if(planets[p1].weight>planets[p2].weight){
		dest=p1;
		del=p2;
	}else{
		dest=p2;
		del=p1;
	}
	double gravw = ( 1 / ( planets[dest].weight + planets[del].weight ) ) * planets[del].weight;
	//fprintf(stderr,"\n merging %i and %i\n",dest,del);
	planets[dest].vx += ( planets[del].vx * gravw );
	planets[dest].vy += ( planets[del].vy * gravw );
	planets[dest].weight += planets[del].weight;
	planets[dest].radius = recalcRadius(dest);

	delPlanet(del);
	return dest;
}

Cplanetsim::Tplanet Cplanetsim::getPlanet(int nr) {
	
	if(nr<(int)planets.size())
		return this->planets[nr];
		
	Tplanet dummy ;
	return dummy;
}

int Cplanetsim::recalcRadius(int nr) {
	return calcRadius(planets[nr].weight);
}

int Cplanetsim::calcRadius(double weight) {
	//fprintf(stderr,"nr:%f\n",pow(FAKTOR * weight ,(1.0 / 3.0)));
	return (int)(pow(FAKTOR * weight ,(1.0 / 3.0)));
}

void Cplanetsim::move() {
	
	TplanetForces forces;
	bool goon;
	bool loop;
	//fprintf(stderr,"\n constructing \n");
	{
		gravcalc gc(*this,corecount);
		int processcount=0;
	//	fprintf(stderr,"\n preadding \n");
		{
			for(int i=0;i<(int)planets.size();i++)
				gc.forces.push_back(TplanetForce());
		}
	//	fprintf(stderr,"\n adding \n");
		for(int i=0;i<(int)planets.size();i++) { // calc forces
			TplanetJob j;
			j.i=i;
			gc.job_pipe.enqueue(j);
			processcount++;
		}
		//fprintf(stderr,"\n %d in queue \n",processcount);
		{
			auto_mutex locker(gc.p_mutex);
			goon = gc.processcount==processcount;
		}
		if(goon)
			loop = false;
		else
			loop = true;
		{
			auto_mutex locker(gc.p_mutex);
			while (gc.processcount!=processcount) {
				gc.sig->wait();
			}
		}
		gc.stopit();
		//fprintf(stderr,"\n queue done \n",processcount);
		
		forces = gc.forces;
	}
//	fprintf(stderr,"\n destructed \n");
	//std::sort(forces.begin(),forces.end());

	for(int i=0;i<(int)forces.size();i++) { // append forces
		planets[i].vx += ax(forces[i].x, i) * T;
		planets[i].vy += ay(forces[i].y, i) * T;
		planets[i].x += dsx(i);
		planets[i].y += dsy(i);
	}
	
	for(int i=0;i<(int)planets.size();i++)
		for(int e=0;e<(int)planets.size();e++)
			if(i!=e) {
				double d = dist(i,e) / 100;
				double r = planets[i].radius + planets[e].radius;
				if( d < r )
						mergePlanets(i,e);
			}
}

void Cplanetsim::setScale(double scale) {
	this->scale=scale;
	for(int i=0;i<(int)planets.size();++i) {
		planets[i].radius = recalcRadius(i);
	}
}

double Cplanetsim::dist(int p1, int p2) {
	return sqrt(sx(p1,p2) * sx(p1,p2) + sy(p1,p2) * sy(p1,p2));
}

double Cplanetsim::dist(Tplanet p1, Tplanet p2) {
	return sqrt(sx(p1,p2) * sx(p1,p2) + sy(p1,p2) * sy(p1,p2));
}

double Cplanetsim::sx(int p1, int p2) {
	return planets[p1].x - planets[p2].x;
}

double Cplanetsim::sx(Tplanet p1, Tplanet p2) {
	return p1.x - p2.x;
}

double Cplanetsim::sy(int p1, int p2) {
	return planets[p1].y - planets[p2].y;
}

double Cplanetsim::sy(Tplanet p1, Tplanet p2) {
	return p1.y - p2.y;
}

double Cplanetsim::dsx(int p) {
	return planets[p].vx * T;
}

double Cplanetsim::dsy(int p) {
	return planets[p].vy * T;
}

double Cplanetsim::ax(double Fx, int p) {
	return Fx / planets[p].weight;
}

double Cplanetsim::ay(double Fy, int p) {
	return Fy / planets[p].weight;
}

Cplanetsim::TplanetCoords Cplanetsim::exportPlanetCoords() {
	
	TplanetCoords dummy;
	for(int i=0;i<(int)planets.size();i++) {
		//fprintf(stderr,"\n %f %f",planets[i].x,planets[i].y);
		dummy.push_back(TplanetCoord(planets[i].x / 100,planets[i].y / 100,planets[i].weight,planets[i].radius));
	}
	return dummy;
}
