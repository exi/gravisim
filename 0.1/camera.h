#ifndef CAMERA_
#define CAMERA_

#include "planetsim.h"

class Ccamera {
	
	private:
		Cplanetsim::TplanetCoord collect;
		Cplanetsim::TplanetCoords coords;
		Cplanetsim::TplanetCoords ncoords;
		int cx;
		int cy;
		int width;
		int height;
		double scale;
		bool resize;

	public:
		Ccamera(): cx(0),cy(0),width(0),height(0),scale(10),resize(true)
		{}
		
		void transformCoords();
		
		void setHeight(int h);
		void setResize(bool rs);
		void setScale(double scale);
		void setWidth(int w);
		
		void moveLeft();
		void moveRight();
		void moveUp();
		void moveDown();
		
		void zoomIn();
		void zoomOut();
		
		Cplanetsim::TplanetCoords calc(Cplanetsim::TplanetCoords co);
		
		Cplanetsim::TplanetCoords getCoords();
		Cplanetsim::TplanetCoord getCollect();
	
};

#endif /*CAMERA_*/
