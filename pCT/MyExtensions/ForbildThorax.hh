/* 
 * File:   ForbildThorax.hh
 * Author: swr
 *
 * Created on October 31, 2017, 2:35 PM
 */

#ifndef FORBILDTHORAX_HH
#define FORBILDTHORAX_HH

#include "HalfSpace.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"
#include <cstddef>
#include <string>

#include "TsVGeometryComponent.hh"

class ForbildThorax : public TsVGeometryComponent
{    
public:
	ForbildThorax(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
				  TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name);
	~ForbildThorax();
	
	G4VPhysicalVolume* Construct();
};

#endif /* FORBILDTHORAX_HH */

