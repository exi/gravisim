#ifndef COORD_H_
#define COORD_H_

#include <math.h>
#include <iostream>

class Ccoord {
	public:
		typedef long double Tcoord;
	
		Ccoord():x(new Tcoord(0)),y(new Tcoord(0)) {}
		Ccoord(Tcoord tx,Tcoord ty):x(new Tcoord(tx)),y(new Tcoord (ty)) {}
		Ccoord(const Ccoord &coord):x(new Tcoord(*coord.x)),y(new Tcoord(*coord.y)) {}
		
		virtual Ccoord& operator=(Ccoord &c) {
			if(this!=&c) {
				setC(c.getX(),c.getY());
			}
			return *this;
		}
		
		~Ccoord() {
			delete(x);
			delete(y);
		}
	
		double dist(Ccoord &coord) { return sqrt(sx(coord) * sx(coord) + sy(coord) * sy(coord)); };
		inline Tcoord sx(double tx) { return (*x - tx); };
		inline Tcoord sx(Ccoord &c) { return (*x - c.getX()); }; 
		inline Tcoord sy(double ty) { return (*y - ty); }
		inline Tcoord sy(Ccoord &c) { return (*y - c.getY()); };  
		inline Tcoord getX() { return *x; };
		inline Tcoord getY() { return *y; };
		void setX(Tcoord tx) 
		{
			if(x)
				delete x; 
			x = new Tcoord(tx); 
		};
		void setY(Tcoord ty)
		{
			if(y)
				delete y; 
			y = new Tcoord(ty); 
		};
		void setC(Tcoord tx, Tcoord ty)
		{	
			if(x)
				delete x;
			if(y)
				delete y;
			x = new Tcoord(tx);
			y = new Tcoord(ty);
		}
	
	protected:
		Tcoord *x;
		Tcoord *y;	
};

#endif /*COORD_H_*/
