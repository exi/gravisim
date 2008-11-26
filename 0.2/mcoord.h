#ifndef MCOORD_H_
#define MCOORD_H_

#ifndef T
#define T 0.1
#endif

//MOVING COORDINATES

#include "coord.h"

class Cmcoord : public Ccoord {
	protected:
		double *vx;
		double *vy;
		
			
	public:
		Cmcoord():Ccoord(0.0,0.0),vx(new double(0)),vy(new double(0)) {}
		Cmcoord(double tx,double ty,double vx,double vy):Ccoord(tx,ty),vx(new double(vx)),vy(new double(vy)) {}
		Cmcoord(Cmcoord &c)
			:Ccoord(*c.x,*c.y),
			vx(new double(*c.vx)),
			vy(new double(*c.vy))
			{}
			
		~Cmcoord() {
			delete(vx);
			delete(vy);
		}

		virtual Cmcoord& operator=(Cmcoord &c) {
			setV(c.getVx(),c.getVy());
			setX(getX());
			setY(getY());
			return (*this);			
		}
		
		const inline double getVx() const { return (*vx); }
		const inline double getVy() const { return (*vy); }
		
		void setVx(double tvx) {
			if(vx)
				delete vx;
			vx = new double(tvx);	
		}
			
		void setVy(double tvy) {
			if(vy)
				delete vy;
			vy = new double(tvy);	
		}		
		
		void setV(double vx,double vy) {
			setVx(vx);
			setVy(vy);
		}
		
		void vStep() {
			setX( getX() + getVx() * T);
			setY( getY() + getVy() * T);
		}
};

#endif /*MCOORD_H_*/
