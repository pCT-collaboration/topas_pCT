/* 
 * File:   HalfSpace.hh
 * Author: swr
 *
 * Created on November 5, 2017, 3:01 PM
 */

#ifndef HALFSPACE_HH
#define HALFSPACE_HH

#include "G4ThreeVector.hh"

class HalfSpace {
public:
    HalfSpace();
    HalfSpace(G4ThreeVector *normal, double distance);
    virtual ~HalfSpace();
    G4ThreeVector *_normal;
    double _distance;
private:

};

#endif /* HALFSPACE_HH */

