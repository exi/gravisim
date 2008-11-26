#include "camera.h"
#include <SDL.h>
#include <sstream>
#include <string>
#include <time.h>

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

int main(int argc, char ** argv) {
	SDL_Surface *screen;
	bool running=true;
	int height=900;
	int width=900;
	int lastframe=0, curframe=0,frametime=0;
	unsigned long int ticker = 0;
	srand ( time(NULL) );
	
	bool BLeft=false,BRight=false;
	bool BUp=false,BDown=false;
	bool BPlus=false,BMinus=false;
	double ftsquare=0;
	srand ( time(NULL) );
	Uint32 g_Black,g_red;
	
	Ccamera psim;
	psim.setHeight(height);
	psim.setWidth(width);
	psim.setMode(Ccamera::Tmodes(0));
	
	for(int i=0;i<500;i++)
		psim.addPlanet(rand() % 10000 - 5000,
					rand() % 10000 - 5000,
					0,
					0,
					rand() % 200000 +1);
	/*psim.addPlanet(000,000,0,0,99999999);
	psim.addPlanet(400,700,030000,0,5000000);
	psim.addPlanet(400,-600,030000,0,5000000);*/
	curframe=SDL_GetTicks();
	
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
		ftsquare=(ftsquare*(double)((double)ticker/((double)ticker+1.0))+(double)frametime*(double)(1.0/((double)ticker+1.0)));
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
						case SDLK_c:
							psim.center();
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
			psim.moveLeft();
		if(BRight)
			psim.moveRight();
		if(BUp)
			psim.moveUp();
		if(BDown)
			psim.moveDown();
		if(BPlus)
			psim.zoomIn();
		if(BMinus)
			psim.zoomOut();
		psim.tick();
			
		/*if(ticker % 5 == 0 && frametime < 100)
			planetsim->addPlanet(rand() % width * scale * 0.5 + ( collect.x*scale - (width * scale / 2) * 0.5),
								rand() % height * scale * 0.5 + ( collect.y*scale - (height * scale / 2 ) * 0.5),
								rand() % 20000 - 20000,
								rand() % 20000 - 20000,
								rand() % 2000000 +1);*/
		
		if(ticker % 10 == 0)
			std::cerr<<"\r "<<frametime<<" ms / frame, "<<psim.size()<<" objects";
		for(int i=0;i<(int)psim.view.size();++i) {
			//fprintf(stderr,"\r %i ms / frame, %i objects %f",frametime,psim.size(), psim.view[i].x);
			circle(screen, psim.view[i].x, psim.view[i].y,psim.view[i].radius, g_red);
		}
		++ticker;
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	}
	
	if(screen) SDL_FreeSurface(screen);
	fprintf(stderr,"\ndone\n");
	return 0;
}
