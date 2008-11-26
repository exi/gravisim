#ifndef PLANETS_H_
#define PLANETS_H_

#include "planet.h"
#include <vector>
#include <iostream>

class Cplanets {
	
	public:
		typedef std::vector<Cplanet*> Tplanets;
	
	protected:
		Tplanets planets;
		const char* wrongIndex() { return "Wrong delete index"; };
		
	public:
		Cplanets() {};
		
		Cplanets(Cplanets& p)			
		{
			for(int i=0;i<(int)p.planets.size();++i)
				planets.push_back(new Cplanet(p[i]));
		
		};
		
		~Cplanets() {
			while(!planets.empty()) {
				delete(planets.back());
				planets.pop_back();				
			}
		}
		
		Cplanets operator=(Cplanets& p) {
			while(!planets.empty())
				delPlanet(0);
			for(int i=0;i<(int)p.planets.size();++i)
				planets.push_back(new Cplanet(p[i]));
			
			return *this;						
		}
		
		Cplanet& operator[](int idx) {
			return *planets.at(idx);	
		}
		
		void addPlanet(Ccoord::Tcoord x,Ccoord::Tcoord y,Cmcoord::Tmcoord vx,Cmcoord::Tmcoord vy,double weight) {
			planets.push_back(new Cplanet(x*100.0,y*100.0,vx,vy,weight));
		}
		
		void clear() {
			while(!planets.empty()) {
				delete(planets.back());
				planets.pop_back();				
			}
		}
		
		void delPlanet(int idx) {
			if((int)planets.size()>idx) {
				delete(planets[idx]);
				Tplanets::iterator iter;
				iter=planets.begin();
				iter+=idx;
				planets.erase(iter);
			}else
				throw wrongIndex();
		}
		
		inline unsigned int size() {
			return (int)planets.size();	
		}
};

#endif /*PLANETS_H_*/
