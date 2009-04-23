#ifndef PLANET_H_
#define PLANET_H_

#ifndef T
#define T 1.0
#endif

#define PI 3.1415926535897932
#define ROH 2
#define FAKTOR 3.0 / ( 4.0 * ROH * PI )
#include "force.h"

#include <iostream>
#include <math.h>

class Cplanet {

    public:
        typedef double Tmcoord;
        typedef double Tcoord;
        typedef double Tweight;
        typedef double Tradius;
        typedef double Ta,Tforce;

    protected:
        Tcoord x;
        Tcoord y;	
        Tmcoord vx;
        Tmcoord vy;
        double weight;
        double radius;

    public:
        Cplanet():
            x(Tcoord(0)),
            y(Tcoord(0)),
            vx(Tmcoord(0)),
            vy(Tmcoord(0)),
            weight(Tweight(0)),
            radius(Tradius(0))
    {
        setRadius();
    }

        Cplanet(Tcoord tx,Tcoord ty,Tmcoord tvx,Tmcoord tvy,double weight)
            :x(Tcoord(tx)),
            y(Tcoord(ty)),
            vx(Tmcoord(tvx)),
            vy(Tmcoord(tvy)),
            weight(Tweight(weight)),
            radius(Tradius(0.0))
    {
        setRadius();
    }

        Cplanet(const Cplanet &p)
            :x(Tcoord(p.x)),
            y(Tcoord(p.y)),
            vx(Tmcoord(p.vx)),
            vy(Tmcoord(p.vy)),
            weight(Tweight(p.weight)),
            radius(Tradius(0.0))
    {
        setRadius();
    }

        ~Cplanet() {
        }

        Cplanet& operator=(Cplanet &p) {
            if(this!=&p) {
                setRadius();
                setWeight(p.getWeight());
                setV(p.getVx(),p.getVy());
                setC(p.getX(),p.getY());
            }
            return (*this);			
        }

        const Cplanet operator+(Cplanet &p); //COLLISION!!!

        Cplanet& operator+=(const Cplanet &p); //COLLISION!!!
        Tradius getRadius() { return (radius); }
        const Tweight getWeight() const { return (weight); }
        const Tmcoord getVx() const { return (vx); }
        const Tmcoord getVy() const { return (vy); }
        double dist(Cplanet &planet) { return sqrt(sx(planet) * sx(planet) + sy(planet) * sy(planet)); };
        Tcoord sx(double tx) { return (x - tx); };
        Tcoord sx(Cplanet &p) { return (x - p.getX()); }; 
        Tcoord sy(double ty) { return (y - ty); }
        Tcoord sy(Cplanet &p) { return (y - p.getY()); };  
        Tcoord getX() { return x; };
        Tcoord getY() { return y; };

        //void adjustSpeed(Cforce &f);
        void adjustSpeed(Cforce f);

        void setRadius() {
            radius = Tradius(sqrt(FAKTOR * getWeight())*100);	
        }

        void setWeight(Tweight w) {
            weight = Tweight(w);	
            setRadius();
        }


        void setVx(Tmcoord tvx) {
            vx = Tmcoord(tvx);	
        }

        void setVy(Tmcoord tvy) {
            vy = Tmcoord(tvy);	
        }		

        void setV(Tmcoord vx,Tmcoord vy) {
            setVx(vx);
            setVy(vy);
        }

        void vStep() {
            setX( getX() + getVx() * T);
            setY( getY() + getVy() * T);
        }

        void setX(Tcoord tx) 
        {
            x = Tcoord(tx); 
        };

        void setY(Tcoord ty)
        {
            y = Tcoord(ty); 
        };

        void setC(Tcoord tx, Tcoord ty)
        {	
            x = Tcoord(tx);
            y = Tcoord(ty);
        }

        //HELPER
        Ta ax(Tforce Fx) {
            return (Fx / getWeight());
        }

        Ta ay(Tforce Fy) {
            return (Fy / getWeight());
        }

};

#endif /*PLANET_H_*/
