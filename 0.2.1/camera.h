#ifndef CAMERA_H_
#define CAMERA_H_

#include "gravenv.h"

class Ccamera: public Cgravenv {
	
	public:
		enum Tmodes { mauto, manual };
	
	protected:
		double positionx;
		double positiony;
		double targetx;
		double targety;
		double targetscale;
		double scale;
		int width;
		int height;
		Tmodes mode;
		
		void transformCoords();
	
	public:
		
		struct Cvcoords {
			int x;
			int y;
			int radius;
			Cvcoords():x(0),y(0),radius(0) {}	
			Cvcoords(int x,int y,int radius):x(x),y(y),radius(radius) {}
			~Cvcoords() {}
		};
		
		typedef std::vector<Cvcoords> Tview;
	
		Ccamera():positionx(0.0),positiony(0.0),targetx(0.0),targety(0.0),targetscale(10000.0),scale(10000.0) {};
		
		void move(double x,double y) {
			targetx = x;
			targetx = y;
		}
		
		void tick() {
			positionx+=(double)(targetx-positionx)*0.1;
			positiony+=(double)(targety-positiony)*0.1;
			scale+=(double)(targetscale-scale)*0.1;
			work();
			transformCoords();	
		}
		
		inline double getScale() { return scale; }
		
		inline double getPosX() { return positionx; } 
		inline double getPosY() { return positiony; }  
		
		void setHeight(int h) {
			height=h;	
		}
		
		void setMode(Tmodes tmode) {
			if(tmode<2 && tmode>=0)
				mode=tmode;
		}
		
		void setScale(double tscale) {
			scale=tscale;	
		}
		
		void setWidth(int w) {
			width=w;
			mode = manual;	
		}
		
		void center() {
			if(mode==manual)
				mode=mauto;
			else
				mode=manual;			
		}
		
		void moveLeft() {
			targetx-=10*scale;
			mode = manual;
		}
		
		void moveRight() {
			targetx+=10*scale;
			mode = manual;
		}
		
		void moveUp() {
			targety-=10*scale;
			mode = manual;
		}
		
		void moveDown() {
			targety+=10*scale;	
			mode = manual;
		}
		
		void zoomIn() {
			targetscale-=0.1*targetscale;
			mode = manual;
		}
		
		void zoomOut() {
			targetscale+=0.1*targetscale;
			mode = manual;
		}
		
		Tview view;
};

#endif /*CAMERA_H_*/
