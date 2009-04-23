#include "camera.h"
#include "planets.h"

void Ccamera::transformCoords() {
    Tview tview;
    Cplanets coords(*sim);
    std::pair<int,int> tcollect;
    bool resized;
    std::pair<double,double> wx;
    std::pair<double,double> wy;
    double tx,ty,tweight=0;

    for(int i=0;i<(int)coords.size();i++) {

        if(i==0) {
            xmin=xmax=coords[i].getX();
            ymin=ymax=coords[i].getY();
        }
        if(xmin>coords[i].getX()) xmin=coords[i].getX();
        if(xmax<coords[i].getX()) xmax=coords[i].getX();
        if(ymin>coords[i].getY()) ymin=coords[i].getY();
        if(ymax<coords[i].getY()) ymax=coords[i].getY();

        coords[i].setX(coords[i].getX() / scale);
        coords[i].setY(coords[i].getY() / scale);
    }

    for(int i=0;i<(int)coords.size();i++) {
        double gravw = ( 1 / ( coords[i].getWeight() + tweight ) ) * coords[i].getWeight();
        tcollect.first = (tcollect.first * ( 1 - gravw ) + coords[i].getX() * gravw);
        tcollect.second = (tcollect.second * ( 1 - gravw ) + coords[i].getY() * gravw);
        tweight+=coords[i].getWeight();
    }

    if(mode==mauto) {
        targetx=tcollect.first*scale;
        targety=tcollect.second*scale;
    }

    tx=positionx/scale;
    ty=positiony/scale;

    double nheight = height / 2;
    double nwidth = width / 2;

    wx = std::pair<double,double>((tx - nwidth),(tx + nwidth));
    wy = std::pair<double,double>((ty - nheight),(ty + nheight));	

    resized=false;

    for(int i=0;i<(int)coords.size();i++) {
        if(	coords[i].getX() > wx.first - coords[i].getRadius() / scale && 
                coords[i].getX() < wx.second + coords[i].getRadius() / scale && 
                coords[i].getY() > wy.first - coords[i].getRadius() / scale && 
                coords[i].getY() < wy.second + coords[i].getRadius() / scale) {	
            int xdiff = coords[i].getX() - tx;
            int ydiff = coords[i].getY() - ty;
            tview.push_back(
                    Cvcoord(
                        (nwidth + xdiff),
                        (nheight + ydiff),
                        ((int)coords[i].getRadius() / scale>1?(int)coords[i].getRadius() / scale:1)
                        )
                    );
        }else
            resized = true;
    }

    std::swap(view,tview);
}
