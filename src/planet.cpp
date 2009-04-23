#include "planet.h"
#include <math.h>

const Cplanet Cplanet::operator +(Cplanet &p) {
    Cplanet dummy(*this);
    bool iamsmall=true;
    if(dummy.getWeight()>p.getWeight())
        iamsmall=false;

    double gravw = ( 1 / ( dummy.getWeight() + p.getWeight() ) ) * (iamsmall?p.getWeight():dummy.getWeight());
    dummy.setVx( p.getVx() * gravw + dummy.getVx() * ( 1 - gravw ) );
    dummy.setVy( p.getVy() * gravw + dummy.getVy() * ( 1 - gravw ) );
    dummy.setWeight(dummy.getWeight() + p.getWeight());
    dummy.setRadius();

    return dummy;
}

/* planet collision */
Cplanet& Cplanet::operator +=(const Cplanet &p) {
    Cplanet dest(*this);
    Cplanet del(p);

    if(getWeight()<p.getWeight()){
        std::swap(dest,del);
    }

    double gravw = ( 1 / ( dest.getWeight() + del.getWeight() ) ) * del.getWeight();
    //fprintf(stderr,"\n merging %i and %i\n",dest,del);
    dest.setVx( dest.getVx() + ( del.getVx() * gravw ));
    dest.setVy( dest.getVy() + ( del.getVy() * gravw ));
    dest.setWeight( dest.getWeight() + del.getWeight() );

    std::swap(dest,*this);
    return *this;
}

void Cplanet::adjustSpeed(Cforce f){
    setV( Tmcoord(getVx() + ax(f.fx) * T),Tmcoord( getVy() + ay(f.fy) * T));
}
