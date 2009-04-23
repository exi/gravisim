#include "gravenv.h"
#include "camera.h"
#include <SDL.h>
#include <sstream>
#include <string>
#include <time.h>
#include <vector>
#include <unistd.h>

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
    int height=600;
    int width=800;
    char c;
    int lastframe=0, curframe=0,frametime=0;
    unsigned long int ticker = 1;
    std::vector<int> frames;

    //keys
    bool BLeft=false,BRight=false;
    bool BUp=false,BDown=false;
    bool Bi=false,Bo=false;
    bool BE=false,BB=false;
    bool BF=false;
    double ftsquare=0;
    srand ( time(NULL) );
    Uint32 g_Black,g_red;

    /* parse our width, height options */
    while ((c = getopt(argc, argv, "w:h:")) != -1) {
        switch (c) {
            case 'w':
                width = atoi(optarg);
                break;
            case 'h':
                height = atoi(optarg);
                break;
        }
    }

    Cgravenv psim;
    Ccamera cam(&psim);
    cam.setHeight(height);
    cam.setWidth(width);
    cam.setMode(Ccamera::Tmodes(0));

    {
        bool mass=false;
        if(mass)
            for(int i=0;i<800;i++)
                psim.addPlanet(rand() % 1000000 - 500000,
                        rand() % 1000000 - 500000,
                        0,
                        0,
                        rand() % 5000000 +1);

        if(!mass){
            psim.addPlanet(000,000,0,0,99999999);
            psim.addPlanet(400,7000,30000,0,5000000);
            psim.addPlanet(400,-6000,-30000,0,5000000);
            psim.addPlanet(7000,400,0,-30000,5000000);
        }
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "SDL konnte nicht initialisiert werden: %s\n",SDL_GetError());
        exit(1);
    }

    screen = SDL_SetVideoMode(width,height, 0, 0);
    if (!screen) {
        fprintf(stderr, "Konnte Bildschirmmodus nicht setzen: %s\n",SDL_GetError());
        exit(1);
    }

    g_Black = SDL_MapRGB(screen->format, 0, 0, 0);
    g_red = SDL_MapRGB(screen->format, 255, 0, 0);
    curframe=SDL_GetTicks();
    while(running)
    {
        lastframe=curframe;
        curframe=SDL_GetTicks();
        frametime = curframe-lastframe;
        if(frametime<30 && !BF) usleep(33333-frametime*1000);

        if(frames.size()>=10)
            frames.erase(frames.begin());

        frames.push_back(frametime);
        ftsquare = 0;
        for(int i=0;i<(int)frames.size();++i)
            ftsquare+=frames[i];
        ftsquare/=frames.size();

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
                        case SDLK_i:
                            Bi=true;
                            break;
                        case SDLK_o:
                            Bo=true;
                            break;
                        case SDLK_c:
                            cam.center();
                            break;
                        case SDLK_b:
                            if(BB) BB=false;
                            else BB=true;
                            break;
                        case SDLK_e:
                            if(BE) BE=false;
                            else BE=true;
                            break;
                        case SDLK_f:
                            if(BF) BF=false;
                            else BF=true;
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
                        case SDLK_i:
                            Bi=false;
                            break;
                        case SDLK_o:
                            Bo=false;
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
            cam.moveLeft();
        if(BRight)
            cam.moveRight();
        if(BUp)
            cam.moveUp();
        if(BDown)
            cam.moveDown();
        if(Bi)
            cam.zoomIn();
        if(Bo)
            cam.zoomOut();
        psim.work();
        cam.work();

        if(ticker % 5 == 0 && BE)
            psim.addPlanet(rand() % width + ( cam.getPosX()/100 - ( width / 2)),
                    rand() % height + ( cam.getPosY()/100 - (height / 2)),
                    rand() % 20000 - 10000,
                    rand() % 20000 - 10000,
                    rand() % 2000000 +1);
        if(BB) {
            double x=(rand() % (int)(cam.xmax - cam.xmin) + cam.xmin);
            double y=(rand() % (int)(cam.ymax - cam.ymin) + cam.ymin);
            if(x==0) ++x;
            if(y==0) ++y;
            psim.addPlanet(     x*1.001/100.0,
                    y*1.001/100.0,
                    rand() % 20000 - 10000,
                    rand() % 20000 - 10000,
                    rand() % 2000000 +1);
        }
        if(ticker % 10 == 0){
            std::cerr<<"\r "<<1000.0/ftsquare<<" fps, ";
            std::cerr<<psim.size()<<" objects, ";
            std::cerr<<((psim.size()+1)*(psim.size()/2.0))/CORECOUNT<<" calc/core--------------";
        }

        for(int i=0;i<(int)cam.view.size();++i) {
            circle(screen, cam.view[i].x, cam.view[i].y,cam.view[i].radius, g_red);
        }

        ++ticker;
        SDL_UpdateRect(screen, 0, 0, 0, 0);
    }

    if(screen) SDL_FreeSurface(screen);
    fprintf(stderr,"\ndone\n");
    return 0;
}
