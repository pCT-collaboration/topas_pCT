//
// ********************************************************************
// *                                                                  *
// *                                                                  *
// * This file was obtained from Topas MC Inc under the license       *
// * agreement set forth at http://www.topasmc.org/registration       *
// * Any use of this file constitutes full acceptance of              *
// * this TOPAS MC license agreement.                                 *
// *                                                                  *
// ********************************************************************
//

#include "TsExtensionManager.hh"

#include "TsParameterManager.hh"

// Add an include like this for each of your new components, filters or scorers:
#include "TsScorePCT.hh"
//#include "TsMyComponent1.hh"
//#include "TsMyMagneticField1.hh"
//#include "TsMyFilter1.hh"
//#include "TsMyParticleSource1.hh"
//#include "TsMyParticleGenerator1.hh"
//#include "TsMyScorer1.hh"
//#include "TsMyOutcomeModel1.hh"
//#include "TsMyPhysicsList1.hh"

TsVGeometryComponent* TsExtensionManager::InstantiateComponent(TsParameterManager* pM, TsMaterialManager* mM, TsGeometryManager* gM,
															   TsVGeometryComponent* pc, G4VPhysicalVolume* pv, G4String childCompType, G4String childName)
{
	// Add code below for each of your new components
	//if (childCompType=="MyComponent1")
    //	return new TsMyComponent1(pM, this, mM, gM, pc, pv, childName);

	return 0;
}


TsVMagneticField* TsExtensionManager::InstantiateMagneticField(TsParameterManager* pM, TsGeometryManager* gM,
										   TsVGeometryComponent* geometryComponent, G4String magneticFieldType)
{
	// Add code below for each of your new magnetic field types
	//if (magneticFieldType=="mymagneticfield1")
    //	return new TsMyMagneticField1(pM, gM, geometryComponent);
	return 0;
}


TsVFilter* TsExtensionManager::InstantiateFilter(TsParameterManager* pM, TsMaterialManager* mM, TsGeometryManager* gM,
												 TsFilterManager* fM, TsVFiltered* filtered, TsVFilter* parentFilter)
{
	// Add a block like this for each of your new filters
    //G4String filterName = "OnlyIncludeParticlesOfTwiceAtomicNumber";
    //pM->RegisterFilterName(filterName);
    //if (pM->ParameterExists(filtered->GetFullParmName(filterName)))
	//	parentFilter = new TsMyFilter1(filterName, pM, mM, gM, fM, filtered, parentFilter, false);

	return parentFilter;
}


TsSource* TsExtensionManager::InstantiateParticleSource(TsParameterManager* pM, TsSourceManager* psM,
														G4String sourceType, G4String sourceName)
{
	// Add code below for each of your new particle sources
	//if (sourceType=="myparticlesource1")
    //	return new TsMyParticleSource1(pM, psM, sourceName);

	return 0;
}


TsVGenerator* TsExtensionManager::InstantiateParticleGenerator(TsParameterManager* pM, TsGeometryManager* gM, TsGeneratorManager* pgM,
															   G4String sourceType, G4String sourceName)
{
	// Add code below for each of your new particle sources
	//if (sourceType=="myparticlesource1")
    //	return new TsMyParticleGenerator1(pM, gM, pgM, sourceName);

	return 0;
}


TsVScorer* TsExtensionManager::InstantiateScorer(TsParameterManager* pM, TsMaterialManager* mM, TsGeometryManager* gM, TsScoringManager* scM,
												 G4String currentScorerName, G4String quantityName,
												 G4String quantityNameLower, G4String outFileName, G4String outFileType, G4bool isSubScorer)
{
	// Add a block like this for each of your new scorers (with quantity name all lower case)
	if (quantityNameLower=="energydepositperevent")
		return new TsScorePCT(pM, mM, gM, scM, currentScorerName, quantityName, outFileName, outFileType, isSubScorer);

	return 0;
}


TsVOutcomeModel* TsExtensionManager::InstantiateOutcomeModel(TsParameterManager* pM, G4String parmPrefix, G4String modelName) {
	// Add a block like this for each of your new outcome models (with modelName all lower case)
	//if (modelName=="myoutcomemodel1")
	//	return new TsMyOutcomeModel1(pM, parmPrefix);

	return 0;
}


G4VUserPhysicsList* TsExtensionManager::InstantiatePhysicsList(TsParameterManager* pM, G4String physicsListName) {
	// Add a block like this for each of your new physics lists (with physicsListName all lower case)
	//if (physicsListName=="myphysicslist1")
	//	return new TsMyPhysicsList1(pM);

	return 0;
}


void TsExtensionManager::BeginSession(TsParameterManager* pM) {
	//G4cout << "TsExtensionManager::BeginSession" << G4endl;
}


void TsExtensionManager::BeginRun(TsParameterManager* pM, const G4Run*) {
	//G4cout << "TsExtensionManager::BeginRun" << G4endl;
}


void TsExtensionManager::BeginHistory(TsParameterManager* pM, const G4Run*, const G4Event*) {
	//G4cout << "TsExtensionManager::BeginHistory" << G4endl;
}


void TsExtensionManager::EndHistory(TsParameterManager* pM, const G4Run*, const G4Event*) {
	//G4cout << "TsExtensionManager::EndHistory" << G4endl;
}


void TsExtensionManager::EndRun(TsParameterManager* pM, const G4Run*) {
	//G4cout << "TsExtensionManager::EndRun" << G4endl;
}


void TsExtensionManager::EndSession(TsParameterManager* pM) {
	//G4cout << "TsExtensionManager::EndSession" << G4endl;
}
