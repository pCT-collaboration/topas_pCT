// Extra Class for use by ForbildThorax

/* 
 * File:   HalfSpace.cc
 * Author: swr
 * 
 * Created on November 5, 2017, 3:01 PM
 */

#include "HalfSpace.hh"

HalfSpace::HalfSpace() {
    _normal = new G4ThreeVector();
    _distance = 0.0;
}

HalfSpace::HalfSpace(G4ThreeVector *normal, double distance) {
    _normal = normal;
    _distance = distance;
}

HalfSpace::~HalfSpace() {
}

