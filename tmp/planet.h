#ifndef PLANET_H_
#define PLANET_H_

#define PI 3.1415926535897932
#define ROH 2
#define FAKTOR 3.0 / ( 4.0 * ROH * PI )
#include "mcoord.h"
#include "force.h"

#include <iostream>

class Cplanet : public Cmcoord {
	protected:
		double *weight;
		double *radius;
		
			
	public:
		typedef long double Ta,Tforce;
	
		Cplanet():Cmcoord(0.0,0.0,0.0,0.0),weight(new double(0)),radius(new double(0.0))
			{
				setRadius();
			}
			
		Cplanet(Tcoord tx,Tcoord ty,Tmcoord tvx,Tmcoord tvy,double weight)
			:Cmcoord(tx,ty,tvx,tvy),
			weight(new double(weight)),
			radius(new double(0.0))
			{
				setRadius();
			}
			
		Cplanet(const Cplanet &p)
			:Cmcoord(*p.x,*p.y,*p.vx,*p.vy),
			weight(new double(*p.weight)),
			radius(new double(0.0))
			{
				setRadius();
			}
		
		~Cplanet() {
			delete(radius);
			delete(weight);
		}

		Cplanet& operator=(Cplanet &p) {
			setRadius();
			setWeight(p.getWeight());
			setX(p.getX());
			setY(p.getY());
			setVx(p.getVx());
			setVy(p.getVy());
			return (*this);			
		}
		
		/*Cplanet& operator=(Cplanet p) {
			setRadius();
			setWeight(p.getWeight());
			setX(p.getX());
			setY(p.getY());
			setVx(p.getVx());
			setVy(p.getVy());
			return (*this);
		}*/
		
		const Cplanet operator+(Cplanet &p); //COLLISION!!!
		
		Cplanet& operator+=(const Cplanet &p); //COLLISION!!!
		inline double getRadius() { return (*radius); }
		const inline double getWeight() const { return (*weight); }
		
		//void adjustSpeed(Cforce &f);
		void adjustSpeed(Cforce f);
		
		void setRadius() {
			delete radius;
			radius = new double(sqrt(FAKTOR * getWeight())*100);	
		}
			
		void setWeight(double w) {
			delete weight;
			weight = new double(w);	
			setRadius();
		}
				
		//HELPER
		inline Ta ax(Tforce Fx) {
			return (Fx / getWeight());
		}

		inline Ta ay(Tforce Fy) {
			return (Fy / getWeight());
		}
		
};

#endif /*PLANET_H_*/
