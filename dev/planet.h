#ifndef PLANET_H_
#define PLANET_H_

class Cplanet {
	
	public:
		Cplanet():x(0),y(0),vx(0),vy(0),weigth(0),radius(0) {};
		Cplanet(const &Cplanet p):x(p.x),y(p.y),vx(p.vx),vy(p.vy),weigth(p.weight),radius(p.radius) {}
		Cplanet operator=(const &Cplanet)
		{
			x=p.x;
			y=p.y;
			vx=p.vx;
			vy=p.vy;
			weight=p.weight;
			radius=p.radius;
			return this;	
		} 
		
		double getX();
		double getY();
		double getVX();
		double getVY();
		double getWeight();
		double getRadius();
		
	
	private:
		double x,y,vx,vy,weight,radius;
	
};

#endif /*PLANET_H_*/
