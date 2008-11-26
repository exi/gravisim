#include "planetsim.h"
#include <SDL.h>
#include <math.h>
#include <time.h>
#include <utility>


void circle(SDL_Surface *screen, int x, int y, int r, Uint32 color)
{
	int y1, y2;
	SDL_Rect rect;
	for(y1 = -r, y2 = r; y1; ++y1, --y2)
	{
		int xr = (int)(sqrt(r*r - y1*y1) + 0.5);
		rect.x = x - xr;
		rect.y = y + y1;
		rect.w = 2 * xr;
		rect.h = 1;
		SDL_FillRect(screen, &rect, color);
		rect.y = y + y2;
		rect.h = 1;
		SDL_FillRect(screen, &rect, color);
	}
	rect.x = x - r;
	rect.y = y;
	rect.w = 2 * r;
	rect.h = 1;
	SDL_FillRect(screen, &rect, color);
}


int main(int argc, char *argv[])
{
	SDL_Surface *screen;
	int running;
	int height=900;
	int width=900;
	int lastframe=0, curframe=0;
	unsigned long int ticker = 0;
	srand ( time(NULL) );
	
	
	Cplanetsim planetsim;
	/*planetsim.addPlanet(400,400,0,20000,99999999);
	planetsim.addPlanet(400,100,30000,0,5000000);
	planetsim.addPlanet(400,800,-30000,0,5000000);
	planetsim.addPlanet(400,200,20000,0,5000000);
	
	planetsim.addPlanet(4000,400,0,-20000,99999999);
	planetsim.addPlanet(4000,100,30000,0,5000000);
	planetsim.addPlanet(4000,800,-30000,0,5000000);
	planetsim.addPlanet(4000,200,20000,0,5000000);
	planetsim.addPlanet(4000,600,-20000,0,5000000);*/
	for(int i=0;i<500;i++)
		planetsim.addPlanet(rand() % 1000,
							rand() % 1000,
							0,
							0,
							200000);
	/*planetsim.addPlanet(400,0,5000,0,99999999);
	planetsim.addPlanet(400,800,-5000,0,99999999);
	planetsim.addPlanet(0,400,0,-5000,99999999);
	planetsim.addPlanet(800,400,0,5000,99999999);*/
	
	Uint32 g_Black,g_red;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "SDL konnte nicht initialisiert werden: %s\n",SDL_GetError());
		exit(1);
	}
	
	screen = SDL_SetVideoMode(height, width, 0, 0);
	if (!screen) {
		fprintf(stderr, "Konnte Bildschirmmodus nicht setzen: %s\n",
		SDL_GetError());
		exit(1);
	}
	
	g_Black = SDL_MapRGB(screen->format, 0, 0, 0);
	g_red = SDL_MapRGB(screen->format, 255, 0, 0);
	
	while(running)
	{
		lastframe=curframe;
		curframe=SDL_GetTicks();
		
		SDL_Event event;
		SDL_FillRect(screen, 0, g_Black);
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
							running=0;
							break;
						default:
							break;
					}
				case SDL_QUIT:
					running=0;
					break;
			}
		}
		
		planetsim.move();
		Cplanetsim::TplanetCoords coords = planetsim.exportPlanetCoords();
		
		Cplanetsim::TplanetCoord collect;
		for(int i=0;i<(int)coords.size();i++) {
			double gravw = ( 1 / ( coords[i].weight + collect.weight ) ) * coords[i].weight;
			collect.x = collect.x * ( 1 - gravw ) + coords[i].x * gravw;
			collect.y = collect.y * ( 1 - gravw ) + coords[i].y * gravw;
			collect.weight+=coords[i].weight;
		}
		
			
		if(ticker % 10 == 0 && (int)coords.size() < 300)
			planetsim.addPlanet(rand() % width + ( collect.x - width / 2 ),
								rand() % height + ( collect.y - height / 2 ),
								rand() % 20000 - 10000,
								rand() % 20000 - 10000
								,rand() % 200000 +1);
		
		fprintf(stderr,"\r planets: %d \t %i / frame",coords.size(),(curframe-lastframe));
		std::pair<double,double> wx((collect.x - width / 2),(collect.x + width / 2));
		std::pair<double,double> wy((collect.y - height / 2),(collect.y + height / 2));
		
		
		for(int i=0;i<(int)coords.size();i++) {
			//circle(screen, coords[i].x, coords[i].y, (int)coords[i].weight, g_red);
			if(coords[i].x > wx.first - coords[i].radius && coords[i].x < wx.second + coords[i].radius && coords[i].y > wy.first - coords[i].radius && coords[i].y < wy.second + coords[i].radius)
				circle(screen, coords[i].x - wx.first, coords[i].y - wy.first , (int)coords[i].radius, g_red);
		}
		delete coords;
		
		ticker++;
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	}
	
	if(screen) SDL_FreeSurface(screen);
	fprintf(stderr,"done\n");
	
    return 0;
}

