#include "camera.h"
#include "planetsim.h"

void Ccamera::transformCoords() {
	Cplanetsim::TplanetCoords tcoords;
	Cplanetsim::TplanetCoord tcollect;
	bool resized;
	std::pair<double,double> wx;
	std::pair<double,double> wy;
	double tx,ty;
	
	for(int i=0;i<(int)coords.size();i++) {
		coords[i].x/=scale;
		coords[i].y/=scale;
		coords[i].radius/=scale;
	}
	
	for(int i=0;i<(int)coords.size();i++) {
		double gravw = ( 1 / ( coords[i].weight + tcollect.weight ) ) * coords[i].weight;
		tcollect.x = tcollect.x * ( 1 - gravw ) + coords[i].x * gravw;
		tcollect.y = tcollect.y * ( 1 - gravw ) + coords[i].y * gravw;
		tcollect.weight+=coords[i].weight;
	}
	
	if(!resize) {
		tx=cx/scale;
		ty=cy/scale;
	}else{
		tx=collect.x;
		ty=collect.y;
	}
	
	double nheight = height / 2;
	double nwidth = width / 2;
	
	wx = std::pair<double,double>((tx - nheight),(tx + nheight));
	wy = std::pair<double,double>((ty - nwidth),(ty + nwidth));	
		 
	resized=false;
	
	for(int i=0;i<(int)coords.size();i++) {
		if(	coords[i].x > wx.first - coords[i].radius && 
			coords[i].x < wx.second + coords[i].radius && 
			coords[i].y > wy.first - coords[i].radius && 
			coords[i].y < wy.second + coords[i].radius) {	
				int xdiff = coords[i].x - tx;
				int ydiff = coords[i].y - ty;
				
				//fprintf(stderr,"\n cox: %f coy:%f [%d] cx:%f cy:%f w:%f \n",tcollect.x,tcollect.y,i,coords[i].x,coords[i].y,coords[i].weight);
				
				tcoords.push_back(
					Cplanetsim::TplanetCoord(
						(nwidth + xdiff),
						(nheight + ydiff),
						0,
						((int)coords[i].radius>1?(int)coords[i].radius:1)
					)
				);
		}else
			resized = true;
	}
	
	if(resize) {
		if(resized) {
			scale+=0.01;
		} else {
			scale-=0.01;
		}			
	}

	std::swap(ncoords,tcoords);
	std::swap(collect,tcollect);
}

void Ccamera::setHeight(int h) {
	height=h;	
}

void Ccamera::setResize(bool rs) {
	resize=rs;	
}

void Ccamera::setScale(double tscale) {
	scale=tscale;	
}

void Ccamera::setWidth(int w) {
	width=w;	
}

void Ccamera::moveLeft() {
	cx-=50;
}

void Ccamera::moveRight() {
	cx+=50;
}

void Ccamera::moveUp() {
	cy-=50;
}

void Ccamera::moveDown() {
	cy+=50;	
}

void Ccamera::zoomIn() {
	if(!resize)
		scale-=0.3;
}

void Ccamera::zoomOut() {
	if(!resize)
		scale+=0.3;
}

Cplanetsim::TplanetCoords Ccamera::calc(Cplanetsim::TplanetCoords co) {
	coords=co;
	transformCoords();
	return (ncoords);	
}

Cplanetsim::TplanetCoords Ccamera::getCoords() {
	return ncoords;	
}

Cplanetsim::TplanetCoord Ccamera::getCollect() {
	return collect;
}
