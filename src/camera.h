#ifndef CAMERA_H_
#define CAMERA_H_

#include "planets.h"

class Ccamera {

    public:
        enum Tmodes { mauto, manual };

    protected:

        Cplanets *sim;
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

        struct Cvcoord {
            int x;
            int y;
            int radius;
            Cvcoord():x(0),y(0),radius(0) {}	
            Cvcoord(int x,int y,int radius):x(x),y(y),radius(radius) {}
            ~Cvcoord() {}
        };

        typedef std::vector<Cvcoord> Tview;

        Ccamera(Cplanets *sim):sim(sim),positionx(0.0),positiony(0.0),targetx(0.0),targety(0.0),targetscale(10000.0),scale(10000.0) {};

        void move(double x,double y) {
            targetx = x;
            targetx = y;
        }

        void work() {
            positionx+=(double)(targetx-positionx)*0.1;
            positiony+=(double)(targety-positiony)*0.1;
            scale+=(double)(targetscale-scale)*0.1;
            transformCoords();	
        }

        double getScale() { return scale; }

        double getPosX() { return positionx; } 
        double getPosY() { return positiony; }  

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
        }

        void zoomOut() {
            targetscale+=0.1*targetscale;
        }

        Tview view;

        double xmin;
        double xmax;
        double ymin;
        double ymax;
};

#endif /*CAMERA_H_*/
