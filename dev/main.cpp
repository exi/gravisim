#include "planetsim.h"
#include "camera.h"
#include <SDL.h>
#include <math.h>
#include <time.h>
#include <utility>
#include <sstream>
#include <memory>

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
	bool running=true;
	int height=900;
	int width=900;
	int lastframe=0, curframe=0,frametime=0;
	double scale=10;
	unsigned long int ticker = 0;
	srand ( time(NULL) );
	int corecount=2;
	
	bool BLeft=false,BRight=false;
	bool BUp=false,BDown=false;
	bool BPlus=false,BMinus=false;
	
	if(argc==2) {
		std::stringstream sstr;
		sstr<<argv[1];
		sstr>>corecount;
	}	
	
	std::auto_ptr<Ccamera> camera( new Ccamera() );
	camera->setHeight(height);
	camera->setWidth(width);
	camera->setScale(scale);
	camera->setResize(true);
	
	Cplanetsim *planetsim = new Cplanetsim(corecount,scale);
	//planetsim->addPlanet(000,000,0,0,99999999);
	planetsim->addPlanet(400,1000,00000,0,50000000);
	planetsim->addPlanet(400,2000,00000,0,50000000);
	/*planetsim->addPlanet(400,300,30000,0,5000000);
	planetsim->addPlanet(400,800,-30000,0,5000000);
	planetsim->addPlanet(400,200,20000,0,5000000);*/
	
	/*planetsim->addPlanet(4000,400,0,-20000,99999999);
	planetsim->addPlanet(4000,100,30000,0,5000000);
	planetsim->addPlanet(4000,800,-30000,0,5000000);
	planetsim->addPlanet(4000,200,20000,0,5000000);
	planetsim->addPlanet(4000,600,-20000,0,5000000);*/
	/*for(int i=0;i<500;i++)
	planetsim->addPlanet(rand() % 10000,
						rand() % 10000,
						0,
						0,
						rand() % 200000 +1);*/
	/*planetsim->addPlanet(400,0,5000,0,99999999);
	planetsim->addPlanet(400,800,-5000,0,99999999);
	planetsim->addPlanet(0,400,0,-5000,99999999);
	planetsim->addPlanet(800,400,0,5000,99999999);*/
	
	Uint32 g_Black,g_red;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "SDL konnte nicht initialisiert werden: %s\n",SDL_GetError());
		exit(1);
	}
	
	screen = SDL_SetVideoMode(height, width, 0, 0);
	if (!screen) {
		fprintf(stderr, "Konnte Bildschirmmodus nicht setzen: %s\n",SDL_GetError());
		exit(1);
	}
	
	g_Black = SDL_MapRGB(screen->format, 0, 0, 0);
	g_red = SDL_MapRGB(screen->format, 255, 0, 0);
	
	while(running)
	{
		lastframe=curframe;
		curframe=SDL_GetTicks();
		frametime = curframe-lastframe;
		if(frametime<33) usleep(33333-frametime*1000);
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
						case SDLK_LEFT:
							BLeft=true;
							break;
						case SDLK_RIGHT:
							BRight=true;
							break;
						case SDLK_UP:
							BUp=true;
							break;
						case SDLK_DOWN:
							BDown=true;
							break;
						case SDLK_PLUS:
							BPlus=true;
							break;
						case SDLK_MINUS:
							BMinus=true;
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym)
					{
						case SDLK_LEFT:
							BLeft=false;
							break;
						case SDLK_RIGHT:
							BRight=false;
							break;
						case SDLK_UP:
							BUp=false;
							break;
						case SDLK_DOWN:
							BDown=false;
							break;
						case SDLK_PLUS:
							BPlus=false;
							break;
						case SDLK_MINUS:
							BMinus=false;
							break;
						default:
							break;
					}
					break;
				case SDL_QUIT:
					running=0;
					break;
			}
		}
		
		if(BLeft)
			camera->moveLeft();
		if(BRight)
			camera->moveRight();
		if(BUp)
			camera->moveUp();
		if(BDown)
			camera->moveDown();
		if(BPlus)
			camera->zoomIn();
		if(BMinus)
			camera->zoomOut();
		
		planetsim->move();
		Cplanetsim::TplanetCoords coords = planetsim->exportPlanetCoords();
		
		coords = camera->calc(coords);	
		Cplanetsim::TplanetCoord collect = camera->getCollect();
			
		/*if(ticker % 5 == 0 && frametime < 100)
			planetsim->addPlanet(rand() % width * scale * 0.5 + ( collect.x*scale - (width * scale / 2) * 0.5),
								rand() % height * scale * 0.5 + ( collect.y*scale - (height * scale / 2 ) * 0.5),
								rand() % 20000 - 20000,
								rand() % 20000 - 20000,
								rand() % 2000000 +1);*/
		
		if(ticker % 1 == 0)
			fprintf(stderr,"\r %i ms / frame, %i objects ",frametime,coords.size());
		
		for(int i=0;i<(int)coords.size();i++) {
			//fprintf(stderr,"\r %i ms / frame, %i objects %f",frametime,coords.size(), coords[i].x);
				circle(screen, coords[i].x, coords[i].y,coords[i].radius, g_red);
		}
		ticker++;
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	}
	
	if(screen) SDL_FreeSurface(screen);
	fprintf(stderr,"\ndone\n");
	
    return 0;
}

