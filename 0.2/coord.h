#ifndef COORD_H_
#define COORD_H_

#include <math.h>
#include <sstream>

class Ccoord {
	public:
		Ccoord():x(new double(0)),y(new double(0)) {}
		Ccoord(double tx,double ty):x(new double(tx)),y(new double (ty)) {}
		Ccoord(const Ccoord &coord):x(new double(*coord.x)),y(new double(*coord.y)) {}
		
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
		inline double sx(double tx) { return (*x - tx); };
		inline double sx(Ccoord &c) { return (*x - c.getX()); }; 
		inline double sy(double ty) { return (*y - ty); }
		inline double sy(Ccoord &c) { return (*y - c.getY()); };  
		inline double getX() { return *x; };
		inline double getY() { return *y; };
		void setX(double tx) 
		{
			if(x)
				delete x; 
			x = new double(tx); 
		};
		void setY(double ty)
		{
			if(y)
				delete y; 
			y = new double(ty); 
		};
		void setC(double tx, double ty)
		{	
			if(x)
				delete x;
			if(y)
				delete y;
			x = new double(tx);
			y = new double(ty);
		}
	
	protected:
		double *x;
		double *y;	
};

#endif /*COORD_H_*/
