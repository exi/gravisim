#ifndef PLANETSIM_H_
#define PLANETSIM_H_

#include <vector>
#include <list>

class Cplanetsim {

	public:
	
		struct Tplanet {
			Tplanet (){
				x=0;
				y=0;
				vx=0;
				vy=0;
				weight=1;
				radius=1;
			}
			double x;
			double y;
			double vx;
			double vy;
			double weight;
			int radius;
		};
		
		struct TplanetCoord {
			TplanetCoord (){
				x=0;
				y=0;
				weight=1;
				radius=1;
			};
			TplanetCoord (double x, double y,double weight,int radius):
				x(x),y(y),weight(weight),radius(radius) { };
			double x;
			double y;
			double weight;
			int radius;
		};
		
		struct TplanetForce {
			TplanetForce (){
				x=0;
				y=0;
				id=0;
			};
			TplanetForce (double x, double y,int id):
				x(x),y(y),id(id) { };
			double x;
			double y;
			int id;
		};
		
		struct TplanetJob {
			TplanetJob (){
				i=0;
			};
			TplanetJob (int i):
				i(i) { };
			int i;
		};
		
		typedef std::vector<Tplanet> Tplanets;
		typedef std::vector<TplanetCoord> TplanetCoords;
		typedef std::vector<TplanetForce> TplanetForces;
		typedef std::list<TplanetForce> TplanetForcesList;
	
		Tplanets planets;
	
	public:
	
		Cplanetsim(int corecount,double scale):corecount(corecount),scale(scale) {};
		Cplanetsim(int corecount):corecount(corecount),scale(100) {};
		Cplanetsim():corecount(2),scale(100) {};
			
		void addPlanet(double x, double y, double vx, double vy, double weight);
		void addPlanet(Tplanet);
		
		void delPlanet(int nr);		
		
		int mergePlanets(int p1,int p2);
		
		Tplanet getPlanet(int nr);
		int calcRadius(double weight);
		int recalcRadius(int nr);
		
		void move();
		void setScale(double scale);
		
		double dist(int p1, int p2);
		double dist(Tplanet p1, Tplanet p2);
		double sx(int p1, int p2);
		double sx(Tplanet p1, Tplanet p2);
		double sy(int p1, int p2);
		double sy(Tplanet p1, Tplanet p2);
		double dsx(int p);
		double dsy(int p);
		double ax(double Fx, int p);
		double ay(double Fy, int p);
		
		TplanetCoords exportPlanetCoords();
		
		int corecount;
		double scale;
	
};

#endif /*PLANETSIM_H_*/
