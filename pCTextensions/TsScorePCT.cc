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
#include "TsScorePCT.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"

#include "G4SystemOfUnits.hh"
#include "G4SteppingManager.hh"
#include "G4VTouchable.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RegionStore.hh"

TsScorePCT::TsScorePCT(TsParameterManager* pM, TsMaterialManager* mM, TsGeometryManager* gM, TsScoringManager* scM,
                       G4String scorerName, G4String quantity, G4String outFileName, G4String outFileType, G4bool isSubScorer)
:TsVVolumeScorer(pM, mM, gM, scM, scorerName, quantity, outFileName, outFileType, isSubScorer)

{
    SetUnit("");
    G4String fileName = fPm->GetStringParameter(GetFullParmName("OutputFile"));

    fileName += "_RawTuple_float64.bin";
    fFileOpen.open(fileName, std::ios::out|std::ios::binary);

    fVerbosity = fPm->GetIntegerParameter("Ts/SequenceVerbosity");
    fEnergyDeposit = 0.0;
}


TsScorePCT::~TsScorePCT() {
}


G4bool TsScorePCT::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
    if (!fIsActive) {
        fSkippedWhileInactive++;
        return false;
    }
    //if ( aStep->GetTrack()->GetKineticEnergy() > 0 ) { //aStep->GetTrack()->GetParentID() == 0  ) { // Only primaries
	// for particles that scapes from the detector, the position and the energy are scored
/*    	G4StepPoint* postPoint = aStep->GetPreStepPoint();
   	fKineticEnergy = postPoint->GetKineticEnergy()/MeV;
	fPosition = postPoint->GetPosition();*/

	G4double energyDeposit = aStep->GetTotalEnergyDeposit()/MeV;

	// Dummy scorer

        AccumulateHit(aStep, energyDeposit);
	// True scorer
	fEnergyDeposit += energyDeposit/MeV;
     //   return true;
   // }
    return false;
}

void TsScorePCT::UserHookForEndOfEvent() {
	if ( fEnergyDeposit > 0 ) {
		G4float tmp;

		fEvtID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
		fBuff.write( reinterpret_cast<char*>( &fEvtID ), sizeof fEvtID);
/*		tmp = static_cast<G4float>(fPosition.x()/mm);
		fBuff.write( reinterpret_cast<char*>( &tmp ), sizeof tmp);*/
		tmp = static_cast<G4float>(fEnergyDeposit);
		fBuff.write( reinterpret_cast<char*>( &tmp ), sizeof tmp);

		fFileOpen << fBuff.str();
		fBuff.str("");
	}
	fEnergyDeposit = 0.0;
}
