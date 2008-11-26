#include "camera.h"
#include "planets.h"

void Ccamera::transformCoords() {
	Tview tview;
	Cplanets coords = *this;
	Ccoord tcollect;
	bool resized;
	std::pair<double,double> wx;
	std::pair<double,double> wy;
	double tx,ty,tweight=0;
	
	for(int i=0;i<(int)coords.size();i++) {
		coords[i].setX(coords[i].getX() / scale);
		coords[i].setY(coords[i].getY() / scale);		
	}
	
	for(int i=0;i<(int)coords.size();i++) {
		double gravw = ( 1 / ( coords[i].getWeight() + tweight ) ) * coords[i].getWeight();
		tcollect.setX(tcollect.getX() * ( 1 - gravw ) + coords[i].getX() * gravw);
		tcollect.setY(tcollect.getY() * ( 1 - gravw ) + coords[i].getY() * gravw);
		tweight+=coords[i].getWeight();
	}
	
	if(mode==mauto) {
		targetx=tcollect.getX()*scale;
		targety=tcollect.getY()*scale;
	}
	
	//std::cerr<<"\n"<<targetx<<"|"<<targety<<"|"<<positionx<<"|"<<positiony<<",\n";

	tx=positionx/scale;
	ty=positiony/scale;
	
	//std::cerr<<"\n"<<coords[0].getX()<<"|"<<coords[0].getX()<<"|"<<positionx<<"|"<<positiony<<"|"<<scale<<"\n";
	
	double nheight = height / 2;
	double nwidth = width / 2;
	
	wx = std::pair<double,double>((tx - nheight),(tx + nheight));
	wy = std::pair<double,double>((ty - nwidth),(ty + nwidth));	
		 
	resized=false;
	
	for(int i=0;i<(int)coords.size();i++) {
		if(	coords[i].getX() > wx.first - coords[i].getRadius() / scale && 
			coords[i].getX() < wx.second + coords[i].getRadius() / scale && 
			coords[i].getY() > wy.first - coords[i].getRadius() / scale && 
			coords[i].getY() < wy.second + coords[i].getRadius() / scale) {	
				int xdiff = coords[i].getX() - tx;
				int ydiff = coords[i].getY() - ty;
				tview.push_back(
					Cvcoords(
						(nwidth + xdiff),
						(nheight + ydiff),
						((int)coords[i].getRadius() / scale>1?(int)coords[i].getRadius() / scale:1)
					)
				);
		}else
			resized = true;
	}
	
	if(mode==mauto) {
		if(resized) {
			targetscale+=0.01*scale;
		} else {
			targetscale-=0.01*scale;
		}			
	}

	std::swap(view,tview);
}
