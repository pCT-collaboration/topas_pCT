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

#ifndef TsExtensionManager_hh
#define TsExtensionManager_hh

#include "globals.hh"

class TsParameterManager;
class TsMaterialManager;
class TsGeometryManager;
class TsFilterManager;
class TsSourceManager;
class TsScoringManager;

class TsGeneratorManager;
class TsVGeometryComponent;
class TsVMagneticField;
class TsVFilter;
class TsVFiltered;
class TsSource;
class TsVGenerator;
class TsVScorer;
class TsVOutcomeModel;
class G4VPhysicalVolume;
class G4VUserPhysicsList;

class G4Run;
class G4Event;

class TsExtensionManager
{
public:
	TsExtensionManager() {;};
	~TsExtensionManager() {;};    
	
	TsVGeometryComponent* InstantiateComponent(TsParameterManager* pM, TsMaterialManager* mM, TsGeometryManager* gM,
											   TsVGeometryComponent* pc, G4VPhysicalVolume* pv, G4String childCompType, G4String childName);
	
	TsVMagneticField* InstantiateMagneticField(TsParameterManager* pM, TsGeometryManager* gM,
											   TsVGeometryComponent* geometryComponent, G4String magneticFieldType);
	
	TsVFilter* InstantiateFilter(TsParameterManager* pM, TsMaterialManager* mM, TsGeometryManager* gM,
								 TsFilterManager* fM, TsVFiltered* filtered, TsVFilter* parentFilter);
	
	TsSource* InstantiateParticleSource(TsParameterManager* pM, TsSourceManager* psM, G4String sourceType, G4String sourceName);
	
	TsVGenerator* InstantiateParticleGenerator(TsParameterManager* pM, TsGeometryManager* gM, TsGeneratorManager* pgM, G4String sourceType, G4String sourceName);
	
	TsVScorer* InstantiateScorer(TsParameterManager* pM, TsMaterialManager* mM, TsGeometryManager* gM, TsScoringManager* scM,
								 G4String currentScorerName, G4String quantityName,
								 G4String quantityNameLower, G4String outFileName, G4String outFileType, G4bool isSubScorer);
	
	TsVOutcomeModel* InstantiateOutcomeModel(TsParameterManager* pM, G4String parmPrefix, G4String modelName);
	
	G4VUserPhysicsList* InstantiatePhysicsList(TsParameterManager* pM, G4String physicsListName);

	void BeginSession(TsParameterManager* pM);
	void BeginRun(TsParameterManager* pM, const G4Run*);
	void BeginHistory(TsParameterManager* pM, const G4Run*, const G4Event*);
	void EndHistory(TsParameterManager* pM, const G4Run*, const G4Event*);
	void EndRun(TsParameterManager* pM, const G4Run*);
	void EndSession(TsParameterManager* pM);
};
#endif
