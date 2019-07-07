//
// ********************************************************************
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * TOPAS collaboration.                                             *
// * Use or redistribution of this code is not permitted without the  *
// * explicit approval of the TOPAS collaboration.                    *
// * Contact: Joseph Perl, perl@slac.stanford.edu                     *
// *                                                                  *
// ********************************************************************
//

#ifndef TsScorePCT_hh
#define TsScorePCT_hh

#include "TsVVolumeScorer.hh"

class TsScorePCT : public TsVVolumeScorer
{
public:
    TsScorePCT(TsParameterManager* pM, TsMaterialManager* mM, TsGeometryManager* gM, TsScoringManager* scM, 
               G4String scorerName, G4String quantity, G4String outFileName, G4String outFileType, G4bool isSubScorer);

    virtual ~TsScorePCT();

    G4bool ProcessHits(G4Step*,G4TouchableHistory*);
    void UserHookForEndOfEvent();

private:

    std::ofstream fFileOpen;
    std::ostringstream fBuff;
    G4int fVerbosity;
    G4int fNbOfRegions;
    std::vector<G4Region*> fRegionPlanes;
    G4double fEnergyDeposit;

    G4double fKineticEnergy;
    G4ThreeVector fPosition;
    G4int fEvtID;
    G4bool fScore;
};
#endif
