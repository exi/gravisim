#ifndef MCOORD_H_
#define MCOORD_H_

#ifndef T
#define T 1.0
#endif

//MOVING COORDINATES

#include "coord.h"

class Cmcoord : public Ccoord {
	
	public:
		typedef long double Tmcoord;
	
	protected:
		Tmcoord *vx;
		Tmcoord *vy;
		
			
	public:
		Cmcoord():Ccoord(0,0),vx(new Tmcoord(0)),vy(new Tmcoord(0)) {}
		Cmcoord(Tcoord tx,Tcoord ty,Tmcoord vx,Tmcoord vy):Ccoord(tx,ty),vx(new Tmcoord(vx)),vy(new Tmcoord(vy)) {}
		Cmcoord(Cmcoord &c)
			:Ccoord(*c.x,*c.y),
			vx(new Tmcoord(*c.vx)),
			vy(new Tmcoord(*c.vy))
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
		
		const inline Tmcoord getVx() const { return (*vx); }
		const inline Tmcoord getVy() const { return (*vy); }
		
		void setVx(Tmcoord tvx) {
			if(vx)
				delete vx;
			vx = new Tmcoord(tvx);	
		}
			
		void setVy(Tmcoord tvy) {
			if(vy)
				delete vy;
			vy = new Tmcoord(tvy);	
		}		
		
		void setV(Tmcoord vx,Tmcoord vy) {
			setVx(vx);
			setVy(vy);
		}
		
		void vStep() {
			setX( getX() + getVx() * T);
			setY( getY() + getVy() * T);
		}
};

#endif /*MCOORD_H_*/
