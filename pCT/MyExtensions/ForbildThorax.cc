// Component for ForbildThorax

/*
 * File:   ForbildThorax.cc
 * Author: swr
 *
 * Created on October 31, 2017, 2:35 PM
 */

/*
 * The FORBILD Thorax phantom.  http://www.imp.uni-erlangen.de/phantoms/thorax/thorax.htm
 */

#include "G4Box.hh"
#include "G4CutTubs.hh"
#include "G4Ellipsoid.hh"
#include "G4EllipticalTube.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4String.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4SubtractionSolid.hh"
#include "G4Transform3D.hh"
#include "G4Tubs.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"
#include "TsParameterManager.hh"
#include "ForbildThorax.hh"
#include <iomanip>
#include <string>
#include <sstream>

G4double scale;

ForbildThorax::ForbildThorax(TsParameterManager* pM, TsExtensionManager* eM, TsMaterialManager* mM, TsGeometryManager* gM,
			 TsVGeometryComponent* parentComponent, G4VPhysicalVolume* parentVolume, G4String& name) :
TsVGeometryComponent(pM, eM, mM, gM, parentComponent, parentVolume, name)
{
    G4cout << "ForbildThorax::ForbildThorax" << G4endl;
    scale = fPm->GetDoubleParameter(GetFullParmName("scale"), "Length");
}

G4ThreeVector* scaledThreeVector(double x, double y, double z) {
    return new G4ThreeVector(x * scale, y * scale, z * scale);
}

ForbildThorax::~ForbildThorax() {
}

G4VSolid* thorax;

G4VPhysicalVolume* ForbildThorax::Construct() {
	BeginConstruction();

    G4cout << "scale=" << scale << G4endl;
    G4ThreeVector* noTrans = new G4ThreeVector();
    G4RotationMatrix* noRot = new G4RotationMatrix();
    G4ThreeVector* size = scaledThreeVector(20.0, 10.0, 50.0);
    thorax = new G4EllipticalTube(
            "thorax",
            size->getX(),
            size->getY(),
            size->getZ());
    G4VSolid* arm = new G4Tubs(
            "arm",
            0.0,
            5.0 * cm,
            7.5 * cm,
            0.0,
            2.0 * pi);
    G4RotationMatrix* rotation = noRot;
    G4ThreeVector* trans = scaledThreeVector(-20.0, 0.0, 17.5);
    thorax = new G4UnionSolid("thorax+right_arm", thorax, arm, rotation, *trans);
    trans = scaledThreeVector(20.0, 0.0, 17.5);
    thorax = new G4UnionSolid("thorax+both_arms", thorax, arm, rotation, *trans);
    G4String thorax_material = "G4_TISSUE_SOFT_ICRU-4";
	fEnvelopeLog = CreateLogicalVolume("thorax", thorax_material, thorax);
	fEnvelopePhys = CreatePhysicalVolume(fEnvelopeLog);

    size = scaledThreeVector(20.0, 10.0, 11.0);
    G4VSolid* diaphragm = new G4Ellipsoid(
            "diaphragm",
            size->getX(),
            size->getY(),
            size->getZ());
    G4ThreeVector* diaphragm_position = scaledThreeVector(0.0, 0.0, -21.5);
    G4RotationMatrix* diaphragm_rotation = noRot;
    size = scaledThreeVector(7.0, 5.0, 16.0);
    G4VSolid* lung = new G4Ellipsoid(
            "lung",
            size->getX(),
            size->getY(),
            size->getZ());
    G4String lung_material = "G4_LUNG_ICRP";
    G4RotationMatrix* inverse = noRot;
    G4RotationMatrix* new_rotation = noRot;
    G4ThreeVector* position = scaledThreeVector(10.5, 0.0, -5.5);
    G4ThreeVector* new_position = new G4ThreeVector((*diaphragm_position - *position));
    new_position = new G4ThreeVector((*diaphragm_rotation)(*new_position));
    G4VSolid* new_shape = new G4SubtractionSolid("diaphragm+left_lung" , lung, diaphragm, new_rotation, *diaphragm_position);
    G4LogicalVolume* left_lung = CreateLogicalVolume("diaphragm+left_lung", lung_material, new_shape);
    G4VPhysicalVolume* left_lung_phys = CreatePhysicalVolume("diaphragm+left_lung", left_lung, noRot, position, fEnvelopePhys);
    position = scaledThreeVector(-10.5, 0.0, -5.5);
    new_position = new G4ThreeVector((*diaphragm_position -* position));
    new_position = new G4ThreeVector((*diaphragm_rotation)(*new_position));
    new_shape = new G4SubtractionSolid("diaphragm+right_lung" , lung, diaphragm, new_rotation, *diaphragm_position);
    G4LogicalVolume* right_lung = CreateLogicalVolume("diaphragm+right_lung", lung_material, new_shape);
    CreatePhysicalVolume("diaphragm+right_lung", right_lung, noRot, position, fEnvelopePhys);
    G4String tumor_material = "G4_TISSUE_SOFT_ICRU-4";
    size = scaledThreeVector(1.5, 1.5, 1.5);
    G4VSolid* tumor_shape = new G4Ellipsoid(
            "tumor",
            size->getX(),
            size->getY(),
            size->getZ());
    G4LogicalVolume* tumor = CreateLogicalVolume("tumor", tumor_material, tumor_shape);
    position = scaledThreeVector(-2.0, 0.0, 0.0);
    CreatePhysicalVolume("tumor", tumor, noRot, position, left_lung_phys);

    size = scaledThreeVector(3.5, 3.5, 3.5);
    G4VSolid* heart_shape = new G4Ellipsoid(
            "heart",
            size->getX(),
            size->getY(),
            size->getZ());
    position = scaledThreeVector(0.0, 4.0, 0.0);
    G4String heart_material = "G4_MUSCLE_STRIATED_ICRU";
    G4LogicalVolume* heart = CreateLogicalVolume("heart", heart_material, heart_shape);
    CreatePhysicalVolume("heart", heart, noRot, position, fEnvelopePhys);
    G4String aorta_material = "G4_BLOOD_ICRP";
    size = scaledThreeVector(1.0, 1.0, 11.249999906883966);
    position = scaledThreeVector(-2.5, -2.5, -3.750000093116034);
    G4ThreeVector halfSpace1 = G4ThreeVector(0.0, 0.0, -1.0);
    G4ThreeVector halfSpace2 = G4ThreeVector(2.785430e-01, 6.499337e-01, 7.071068e-01);
    G4VSolid* aorta_shape = new G4CutTubs(
            "aorta",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    G4LogicalVolume* aorta = CreateLogicalVolume("aorta", aorta_material, aorta_shape);
    CreatePhysicalVolume("aorta", aorta, noRot, position, fEnvelopePhys);
    size = scaledThreeVector(1.0, 1.0, 2.2077083805936755);
    halfSpace2 = G4ThreeVector(-3.162278e-01, -6.324555e-01, 7.071068e-01);
    aorta_shape = new G4CutTubs(
            "aorta",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    aorta = CreateLogicalVolume("aorta", aorta_material, aorta_shape);
    position = scaledThreeVector(0.5, 4.5, 5.207708380593675);
    CreatePhysicalVolume("aorta", aorta, noRot, position, fEnvelopePhys);
    size = scaledThreeVector(1.0, 1.0, 3.8502513711604007);
    position = scaledThreeVector(-0.9833117538794218, 1.0389392409480158, 7.5);
    halfSpace1 = G4ThreeVector(2.785430e-01, 6.499337e-01, -7.071068e-01);
    halfSpace2 = G4ThreeVector(3.167084e-01, 6.335771e-01, 7.058866e-01);
    aorta_shape = new G4CutTubs(
            "aorta",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    aorta = CreateLogicalVolume("aorta", aorta_material, aorta_shape);
    CreatePhysicalVolume("aorta", aorta, new G4RotationMatrix(-0.404892, -1.57080, 0.404892), position, fEnvelopePhys);
    
    G4String cortical_bone_material = "G4_BONE_CORTICAL_ICRP";
    G4String marrow_material = "G4_ADIPOSE_TISSUE_ICRP";
    size = scaledThreeVector(2.5, 0.8, 25.0);
    G4VSolid* sternum_shape = new G4Box(
            "sternum",
            size->getX(),
            size->getY(),
            size->getZ());
    G4LogicalVolume* sternum = CreateLogicalVolume("sternum", cortical_bone_material, sternum_shape);
    position = scaledThreeVector(0.0, 9.0, 2.5);
    G4VPhysicalVolume* sternum_phys = CreatePhysicalVolume("sternum", sternum, noRot, position, fEnvelopePhys);
    size = scaledThreeVector(2.0, 0.5, 25.0);
    G4VSolid* sternum_marrow_shape = new G4Box(
            "sternum_marrow",
            size->getX(),
            size->getY(),
            size->getZ());
    G4LogicalVolume* sternum_marrow = CreateLogicalVolume("sternum_marrow", marrow_material, sternum_marrow_shape);
    CreatePhysicalVolume("sternum_marrow", sternum_marrow, noRot, noTrans, sternum_phys);
    
    size = scaledThreeVector(0.5, 0.5, 8.5);
    G4VSolid* clavicle_shape = new G4Tubs(
            "clavicle",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi);
    G4LogicalVolume* clavicle = CreateLogicalVolume("clavicle", cortical_bone_material, clavicle_shape);
    position = scaledThreeVector(-10.0, 5.5, 16.0);
    G4VPhysicalVolume* clavicle_right = CreatePhysicalVolume("clavicle_right", clavicle, new G4RotationMatrix(-1.14794, -1.66174, 1.14794), position, fEnvelopePhys);
    position = scaledThreeVector(10.0, 5.5, 16.0);
    G4VPhysicalVolume* clavicle_left = CreatePhysicalVolume("clavicle_left", clavicle, new G4RotationMatrix(-1.99365, -1.47986, 1.99365), position, fEnvelopePhys);
    G4VSolid* clavicle_marrow_shape = new G4Tubs(
            "clavicle_marrow",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi);
    G4LogicalVolume* clavicle_marrow = CreateLogicalVolume("clavicle_marrow", marrow_material, clavicle_marrow_shape);
    CreatePhysicalVolume("clavicle_marrow", clavicle_marrow, noRot, noTrans, clavicle_right);
    CreatePhysicalVolume("clavicle_marrow", clavicle_marrow, noRot, noTrans, clavicle_left);
    
    size = scaledThreeVector(4.5, 0.5, 6.0);
    G4VSolid* scapula_shape = new G4Ellipsoid(
            "scapula",
            size->getX(),
            size->getY(),
            size->getZ()
            );
    G4LogicalVolume* scapula = CreateLogicalVolume("scapula", cortical_bone_material, scapula_shape);
    position = scaledThreeVector(-12.0, -5.0, 15.0);
    G4VPhysicalVolume* scapula_right = CreatePhysicalVolume("scapula_right", scapula, noRot, position, fEnvelopePhys);
    position = scaledThreeVector(12.0, -5.0, 15.0);
    G4VPhysicalVolume* scapula_left = CreatePhysicalVolume("scapula_left", scapula, noRot, position, fEnvelopePhys);
    size = scaledThreeVector(4.0, 0.4, 5.5);
    G4VSolid* scapula_marrow_shape = new G4Ellipsoid(
            "scapula_marrow",
            size->getX(),
            size->getY(),
            size->getZ()
            );
    G4LogicalVolume* scapula_marrow = CreateLogicalVolume("scapula_marrow", marrow_material, scapula_marrow_shape);
    CreatePhysicalVolume("scapula_marrow", scapula_marrow, noRot, noTrans, scapula_right);
    CreatePhysicalVolume("scapula_marrow", scapula_marrow, noRot, noTrans, scapula_left);

    size = scaledThreeVector(2.5, 2.5, 2.5);
    G4VSolid* acromion_shape = new G4Ellipsoid(
            "acromion",
            size->getX(),
            size->getY(),
            size->getZ()
            );
    G4LogicalVolume* acromion = CreateLogicalVolume("acromion", cortical_bone_material, acromion_shape);
    position = scaledThreeVector(-16.5, 0.0, 15.0);
    G4VPhysicalVolume* acromion_right = CreatePhysicalVolume("acromion_right", acromion, noRot, position, fEnvelopePhys);
    position = scaledThreeVector(16.5, 0.0, 15.0);
    G4VPhysicalVolume* acromion_left = CreatePhysicalVolume("acromion_left", acromion, noRot, position, fEnvelopePhys);
    size = scaledThreeVector(2.0, 2.0, 2.0);
    G4VSolid* acromion_marrow_shape = new G4Ellipsoid(
            "acromion_marrow",
            size->getX(),
            size->getY(),
            size->getZ()
            );
    G4LogicalVolume* acromion_marrow = CreateLogicalVolume("acromion_marrow", marrow_material, acromion_marrow_shape);
    CreatePhysicalVolume("acromion_marrow", acromion_marrow, noRot, noTrans, acromion_right);
    CreatePhysicalVolume("acromion_marrow", acromion_marrow, noRot, noTrans, acromion_left);
    
    size = scaledThreeVector(2.5, 2.5, 2.5);
    G4VSolid* humerus_shape = new G4Ellipsoid(
            "humerus",
            size->getX(),
            size->getY(),
            size->getZ()
            );
    G4LogicalVolume* humerus = CreateLogicalVolume("humerus", cortical_bone_material, humerus_shape);
    position = scaledThreeVector(-22.0, 0.0, 15.0);
    G4VPhysicalVolume* humerus_right = CreatePhysicalVolume("humerus_right", humerus, noRot, position, fEnvelopePhys);
    position = scaledThreeVector(22.0, 0.0, 15.0);
    G4VPhysicalVolume* humerus_left = CreatePhysicalVolume("humerus_left", humerus, noRot, position, fEnvelopePhys);
    size = scaledThreeVector(2.0, 2.0, 2.0);
    G4VSolid* humerus_marrow_shape = new G4Ellipsoid(
            "humerus_marrow",
            size->getX(),
            size->getY(),
            size->getZ()
            );
    G4LogicalVolume* humerus_marrow = CreateLogicalVolume("humerus_marrow", marrow_material, humerus_marrow_shape);
    CreatePhysicalVolume("humerus_marrow", humerus_marrow, noRot, noTrans, humerus_right);
    CreatePhysicalVolume("humerus_marrow", humerus_marrow, noRot, noTrans, humerus_left);
    
    G4String trabecular_bone_material = "TrabecularBone";
    size = scaledThreeVector(1.75, 1.75, 1.0);
    G4VSolid* vertebra_1_shape = new G4Tubs(
            "vertebra_1",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi);
    size = scaledThreeVector(2.3430749027719964, 0.2, 1.5);
    G4VSolid* vertebra_2_shape = new G4Box(
            "vertebra_2",
            size->getX(),
            size->getY(),
            size->getZ()
            );
    trans = scaledThreeVector(0.0, -1.4, 0.0);
    G4VSolid* vertebra_shape = new G4UnionSolid("vertebra", vertebra_1_shape, vertebra_2_shape, noRot, *trans);
    size = scaledThreeVector(4.0, 0.95, 1.5);
    G4VSolid* vertebra_3_shape = new G4Box(
            "vertebra_3",
            size->getX(),
            size->getY(),
            size->getZ()
            );
    trans = scaledThreeVector(-2.0, -1.975, 0.0);
    vertebra_shape = new G4UnionSolid("vertebra", vertebra_shape, vertebra_3_shape, noRot, *trans);
    trans = scaledThreeVector(2.0, -1.975, 0.0);
    vertebra_shape = new G4UnionSolid("vertebra", vertebra_shape, vertebra_3_shape, noRot, *trans);
    size = scaledThreeVector(1.6, 0.3, 1.5);
    G4VSolid* vertebra_4_shape = new G4Box(
            "vertebra_4",
            size->getX(),
            size->getY(),
            size->getZ()
            );
    trans = scaledThreeVector(0.0, -2.35, 0.0);
    vertebra_shape = new G4UnionSolid("vertebra", vertebra_shape, vertebra_4_shape, noRot, *trans);
    size = scaledThreeVector(0.8, 1.6, 1.5);
    G4VSolid* vertebra_5_shape = new G4Box(
            "vertebra_5",
            size->getX(),
            size->getY(),
            size->getZ()
            );
    trans = scaledThreeVector(0.0, -3.25, 0.0);
    vertebra_shape = new G4UnionSolid("vertebra", vertebra_shape, vertebra_4_shape, noRot, *trans);
    G4LogicalVolume* vertebra = CreateLogicalVolume("vertebra", cortical_bone_material, vertebra_shape);
    size = scaledThreeVector(1.5, 1.5, 1.0);
    G4VSolid* vertebra_6_shape = new G4Tubs(
            "vertebra_6",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi);
    G4LogicalVolume* vertebra_6 = CreateLogicalVolume("vertebra_6", trabecular_bone_material, vertebra_6_shape);
    size = scaledThreeVector(0.4, 0.4, 1.0);
    G4VSolid* vertebra_7_shape = new G4Tubs(
            "vertebra_7",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi);
    G4LogicalVolume* vertebra_7 = CreateLogicalVolume("vertebra_7", trabecular_bone_material, vertebra_7_shape);
    double spine_z[] = {15.0, 12.0, 9.0, 6.0, 3.0, 0.0, -3.0, -6.0};
    for (uint i = 0; i < sizeof (spine_z) / sizeof (spine_z[0]); ++i) {
        position = scaledThreeVector(0.0, -5.0, spine_z[i]);
        CreatePhysicalVolume("vertebra", vertebra, noRot, position, fEnvelopePhys);
        CreatePhysicalVolume("vertebra_6", vertebra_6, noRot, position, fEnvelopePhys);
        position = scaledThreeVector(0.0, -9.05, spine_z[i]);
        CreatePhysicalVolume("vertebra_7", vertebra_7, noRot, position, fEnvelopePhys);
    }
    
#if 0
    G4RotationMatrix* rib_left_rotation = new G4RotationMatrix(-1.72757, -1.47567, 1.72757);
    G4RotationMatrix rib_left_rotation_inverse = rib_left_rotation->inverse();
    G4ThreeVector *rib_left_position = scaledThreeVector(5.716814809004056, 8.559508872121505, -5.230810921852053);
    size = scaledThreeVector(0.5, 0.5, 2.8341128111577727);
    halfSpace1 = G4ThreeVector(-5.876771e-02, -7.007764e-02, -9.958090e-01);
    halfSpace2 = G4ThreeVector(-6.719852e-02, -8.013074e-02, 9.945167e-01);
    G4VSolid* rib_left_1_shape = new G4CutTubs(
            "rib_left_1",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);

    size = scaledThreeVector(0.5, 0.5, 2.5658347171819935);
    halfSpace1 = G4ThreeVector(-7.882399e-02, -6.872687e-02, -9.945167e-01);
    halfSpace2 = G4ThreeVector(-1.024330e-01, -8.931145e-02, 9.907224e-01);
    G4VSolid* rib_left_2_shape = new G4CutTubs(
            "rib_left_2",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(10.874025519915964, 7.28115352314031, -4.449593319696884);
    trans = new G4ThreeVector();
    *trans = *position - *rib_left_position;
    *trans = (*rib_left_rotation)(*trans);
    rotation = new G4RotationMatrix(-1.91054, -1.36989, 1.91054);
    *rotation = *rotation * rib_left_rotation_inverse;
    G4VSolid* rib_left_shape = new G4UnionSolid("rib_left", rib_left_1_shape, rib_left_2_shape, rotation, *trans);
    
    size = scaledThreeVector(0.5, 0.5, 2.189253583235205);
    halfSpace1 = G4ThreeVector(-1.170750e-01, -6.901139e-02, -9.907224e-01);
    halfSpace2 = G4ThreeVector(-1.658457e-01, -9.775952e-02, 9.812942e-01);
    G4VSolid* rib_left_3_shape = new G4CutTubs(
            "rib_left_3",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(14.966813931887867, 5.290067045607286, -3.232819027871116);
    trans = new G4ThreeVector();
    *trans = *position - *rib_left_position;
    *trans = (*rib_left_rotation)(*trans);
    rotation = new G4RotationMatrix(-2.16082, -1.24305, 2.16082);
    *rotation = *rotation * rib_left_rotation_inverse;
    rib_left_shape = new G4UnionSolid("rib_left", rib_left_shape, rib_left_3_shape, rotation, *trans);
    
    size = scaledThreeVector(0.5, 0.5, 1.828694046243503);
    halfSpace1 = G4ThreeVector(-1.841965e-01, -5.597617e-02, -9.812942e-01);
    halfSpace2 = G4ThreeVector(-2.450449e-01, -7.446715e-02, 9.666476e-01);
    G4VSolid* rib_left_4_shape = new G4CutTubs(
            "rib_left_4",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(17.594545557112774, 2.7811536631136162, -1.6995934052360941);
    trans = new G4ThreeVector();
    *trans = *position - *rib_left_position;
    *trans = (*rib_left_rotation)(*trans);
    rotation = new G4RotationMatrix(-2.55274, -1.10062, 2.55274);
    *rotation = *rotation * rib_left_rotation_inverse;
    rib_left_shape = new G4UnionSolid("rib_left", rib_left_shape, rib_left_4_shape, rotation, *trans);
    
    size = scaledThreeVector(0.5, 0.5, 1.670564138859993);
    halfSpace1 = G4ThreeVector(-2.561104e-01, -1.471204e-07, -9.666475e-01);
    halfSpace2 = G4ThreeVector(-2.561104e-01, 1.471204e-07, 9.666475e-01);
    G4VSolid* rib_left_5_shape = new G4CutTubs(
            "rib_left_5",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(18.5, 2.220446049250313E-16, -1.1102230246251565E-16);
    trans = new G4ThreeVector();
    *trans = *position - *rib_left_position;
    *trans = (*rib_left_rotation)(*trans);
    rotation = new G4RotationMatrix(3.14159, -1.02225, 3.14159);
    *rotation = *rotation * rib_left_rotation_inverse;
    rib_left_shape = new G4UnionSolid("rib_left", rib_left_shape, rib_left_5_shape, rotation, *trans);
    
    size = scaledThreeVector(0.5, 0.5, 1.8286940462435033);
    halfSpace1 = G4ThreeVector(-2.450449e-01, 7.446715e-02, -9.666476e-01);
    halfSpace2 = G4ThreeVector(-1.841965e-01, 5.597617e-02, 9.812942e-01);
    G4VSolid* rib_left_6_shape = new G4CutTubs(
            "rib_left_6",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(17.594545557112774, -2.7811536631136162, 1.6995934052360944);
    trans = new G4ThreeVector();
    *trans = *position - *rib_left_position;
    *trans = (*rib_left_rotation)(*trans);
    rotation = new G4RotationMatrix(2.55274, -1.10062, -2.55274);
    *rotation = *rotation * rib_left_rotation_inverse;
    rib_left_shape = new G4UnionSolid("rib_left", rib_left_shape, rib_left_6_shape, rotation, *trans);
    
    size = scaledThreeVector(0.5, 0.5, 2.189253583235205);
    halfSpace1 = G4ThreeVector(-1.658457e-01, 9.775952e-02, -9.812942e-01);
    halfSpace2 = G4ThreeVector(-1.170750e-01, 6.901139e-02, 9.907224e-01);
    G4VSolid* rib_left_7_shape = new G4CutTubs(
            "rib_left_7",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(14.966813931887867, -5.290067045607285, 3.2328190278711153);
    trans = new G4ThreeVector();
    *trans = *position - *rib_left_position;
    *trans = (*rib_left_rotation)(*trans);
    rotation = new G4RotationMatrix(2.16082, -1.24305, -2.16082);
    *rotation = *rotation * rib_left_rotation_inverse;
    rib_left_shape = new G4UnionSolid("rib_left", rib_left_shape, rib_left_7_shape, rotation, *trans);
    
    size = scaledThreeVector(0.5, 0.5, 2.5658347171819926);
    halfSpace1 = G4ThreeVector(-1.024330e-01, 8.931145e-02, -9.907224e-01);
    halfSpace2 = G4ThreeVector(-7.882399e-02, 6.872687e-02, 9.945167e-01);
    G4VSolid* rib_left_8_shape = new G4CutTubs(
            "rib_left_8",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(10.874025519915964, -7.28115352314031, 4.449593319696884);
    trans = new G4ThreeVector();
    *trans = *position - *rib_left_position;
    *trans = (*rib_left_rotation)(*trans);
    rotation = new G4RotationMatrix(1.91054, -1.36989, -1.91054);
    *rotation = *rotation * rib_left_rotation_inverse;
    rib_left_shape = new G4UnionSolid("rib_left", rib_left_shape, rib_left_8_shape, rotation, *trans);
    
    size = scaledThreeVector(0.5, 0.5, 2.834112811157772);
    halfSpace1 = G4ThreeVector(-6.719852e-02, 8.013074e-02, -9.945167e-01);
    halfSpace2 = G4ThreeVector(-5.876771e-02, 7.007764e-02, 9.958090e-01);
    G4VSolid* rib_left_9_shape = new G4CutTubs(
            "rib_left_9",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(5.716814809004056, -8.559508872121505, 5.230810921852054);
    trans = new G4ThreeVector();
    *trans = *position - *rib_left_position;
    *trans = (*rib_left_rotation)(*trans);
    rotation = new G4RotationMatrix(1.72757, -1.47567, -1.72757);
    *rotation = *rotation * rib_left_rotation_inverse;
    rib_left_shape = new G4UnionSolid("rib_left", rib_left_shape, rib_left_9_shape, rotation, *trans);

    G4RotationMatrix* rib_right_rotation = new G4RotationMatrix(1.72757, -1.47567, -1.72757);
    G4RotationMatrix rib_right_rotation_inverse = rib_right_rotation->inverse();
    G4ThreeVector *rib_right_position = scaledThreeVector(-5.716814809004056, 8.559508872121505, -5.230810921852053);
    size = scaledThreeVector(0.5, 0.5, 2.8341128111577727);
    halfSpace1 = G4ThreeVector(5.876771e-02, -7.007764e-02, -9.958090e-01);
    halfSpace2 = G4ThreeVector(6.719852e-02, -8.013074e-02, 9.945167e-01);
    G4VSolid* rib_right_1_shape = new G4CutTubs(
            "rib_right_1",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);

    size = scaledThreeVector(0.5, 0.5, 2.5658347171819935);
    halfSpace1 = G4ThreeVector(7.882399e-02, -6.872687e-02, -9.945167e-01);
    halfSpace2 = G4ThreeVector(1.024330e-01, -8.931145e-02, 9.907224e-01);
    G4VSolid* rib_right_2_shape = new G4CutTubs(
            "rib_right_2",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(-10.874025519915964, 7.28115352314031, -4.449593319696884);
    trans = new G4ThreeVector();
    *trans = *position - *rib_right_position;
    *trans = (*rib_right_rotation)(*trans);
    rotation = new G4RotationMatrix(1.91054, -1.36989, -1.91054);
    *rotation = *rotation * rib_right_rotation_inverse;
    G4VSolid* rib_right_shape = new G4UnionSolid("rib_right", rib_right_1_shape, rib_right_2_shape, rotation, *trans);
    
    size = scaledThreeVector(0.5, 0.5, 2.189253583235205);
    halfSpace1 = G4ThreeVector(1.170750e-01, -6.901139e-02, -9.907224e-01);
    halfSpace2 = G4ThreeVector(1.658457e-01, -9.775952e-02, 9.812942e-01);
    G4VSolid* rib_right_3_shape = new G4CutTubs(
            "rib_right_3",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(-14.966813931887867, 5.290067045607286, -3.232819027871116);
    trans = new G4ThreeVector();
    *trans = *position - *rib_right_position;
    *trans = (*rib_right_rotation)(*trans);
    rotation = new G4RotationMatrix(2.16082, -1.24305, -2.16082);
    *rotation = *rotation * rib_right_rotation_inverse;
    rib_right_shape = new G4UnionSolid("rib_right", rib_right_shape, rib_right_3_shape, rotation, *trans);

    size = scaledThreeVector(0.5, 0.5, 1.828694046243503);
    halfSpace1 = G4ThreeVector(1.841965e-01, -5.597617e-02, -9.812942e-01);
    halfSpace2 = G4ThreeVector(2.450449e-01, -7.446715e-02, 9.666476e-01);
    G4VSolid* rib_right_4_shape = new G4CutTubs(
            "rib_right_4",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(-17.594545557112774, 2.7811536631136162, -1.6995934052360941);
    trans = new G4ThreeVector();
    *trans = *position - *rib_right_position;
    *trans = (*rib_right_rotation)(*trans);
    rotation = new G4RotationMatrix(2.55274, -1.10062, -2.55274);
    *rotation = *rotation * rib_right_rotation_inverse;
    rib_right_shape = new G4UnionSolid("rib_right", rib_right_shape, rib_right_4_shape, rotation, *trans);
    
    size = scaledThreeVector(0.5, 0.5, 1.670564138859993);
    halfSpace1 = G4ThreeVector(2.561104e-01, -1.471204e-07, -9.666475e-01);
    halfSpace2 = G4ThreeVector(2.561104e-01, 1.471204e-07, 9.666475e-01);
    G4VSolid* rib_right_5_shape = new G4CutTubs(
            "rib_right_5",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(-18.5, 2.220446049250313E-16, -1.1102230246251565E-16);
    trans = new G4ThreeVector();
    *trans = *position - *rib_right_position;
    *trans = (*rib_right_rotation)(*trans);
    rotation = new G4RotationMatrix(3.14159, -1.02225, 3.14159);
    *rotation = *rotation * rib_right_rotation_inverse;
    rib_right_shape = new G4UnionSolid("rib_right", rib_right_shape, rib_right_5_shape, rotation, *trans);
        
    size = scaledThreeVector(0.5, 0.5, 1.8286940462435033);
    halfSpace1 = G4ThreeVector(2.450449e-01, 7.446715e-02, -9.666476e-01);
    halfSpace2 = G4ThreeVector(1.841965e-01, 5.597617e-02, 9.812942e-01);
    G4VSolid* rib_right_6_shape = new G4CutTubs(
            "rib_right_6",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(-17.594545557112774, -2.7811536631136162, 1.6995934052360944);
    trans = new G4ThreeVector();
    *trans = *position - *rib_right_position;
    *trans = (*rib_right_rotation)(*trans);
    rotation = new G4RotationMatrix(-2.55274, -1.10062, 2.55274);
    *rotation = *rotation * rib_right_rotation_inverse;
    rib_right_shape = new G4UnionSolid("rib_right", rib_right_shape, rib_right_6_shape, rotation, *trans);

    size = scaledThreeVector(0.5, 0.5, 2.189253583235205);
    halfSpace1 = G4ThreeVector(1.658457e-01, 9.775952e-02, -9.812942e-01);
    halfSpace2 = G4ThreeVector(1.170750e-01, 6.901139e-02, 9.907224e-01);
    G4VSolid* rib_right_7_shape = new G4CutTubs(
            "rib_right_7",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(-14.966813931887867, -5.290067045607285, 3.2328190278711153);
    trans = new G4ThreeVector();
    *trans = *position - *rib_right_position;
    *trans = (*rib_right_rotation)(*trans);
    rotation = new G4RotationMatrix(-2.16082, -1.24305, 2.16082);
    *rotation = *rotation * rib_right_rotation_inverse;
    rib_right_shape = new G4UnionSolid("rib_right", rib_right_shape, rib_right_7_shape, rotation, *trans);
    
    size = scaledThreeVector(0.5, 0.5, 2.5658347171819926);
    halfSpace1 = G4ThreeVector(1.024330e-01, 8.931145e-02, -9.907224e-01);
    halfSpace2 = G4ThreeVector(7.882399e-02, 6.872687e-02, 9.945167e-01);
    G4VSolid* rib_right_8_shape = new G4CutTubs(
            "rib_right_8",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(-10.874025519915964, -7.28115352314031, 4.449593319696884);
    trans = new G4ThreeVector();
    *trans = *position - *rib_right_position;
    *trans = (*rib_right_rotation)(*trans);
    rotation = new G4RotationMatrix(-1.91054, -1.36989, 1.91054);
    *rotation = *rotation * rib_right_rotation_inverse;
    rib_right_shape = new G4UnionSolid("rib_right", rib_right_shape, rib_right_8_shape, rotation, *trans);
    
    size = scaledThreeVector(0.5, 0.5, 2.834112811157772);
    halfSpace1 = G4ThreeVector(6.719852e-02, 8.013074e-02, -9.945167e-01);
    halfSpace2 = G4ThreeVector(5.876771e-02, 7.007764e-02, 9.958090e-01);
    G4VSolid* rib_right_9_shape = new G4CutTubs(
            "rib_right_9",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(-5.716814809004056, -8.559508872121505, 5.230810921852054);
    trans = new G4ThreeVector();
    *trans = *position - *rib_right_position;
    *trans = (*rib_right_rotation)(*trans);
    rotation = new G4RotationMatrix(-1.72757, -1.47567, 1.72757);
    *rotation = *rotation * rib_right_rotation_inverse;
    rib_right_shape = new G4UnionSolid("rib_right", rib_right_shape, rib_right_9_shape, rotation, *trans);

    G4LogicalVolume* rib_right = CreateLogicalVolume("rib_right", cortical_bone_material, rib_right_shape);

    G4LogicalVolume* rib_left = CreateLogicalVolume("rib_left", cortical_bone_material, rib_left_shape);
    rib_left_rotation = new G4RotationMatrix(-1.72757, -1.47567, 1.72757);
    rib_left_position = scaledThreeVector(5.716815297096183, 8.55950879496925, -11.230810874703465);
    G4VPhysicalVolume* rib_left_1_phys = CreatePhysicalVolume("rib_left_1", rib_left, rib_left_rotation, rib_left_position, fEnvelopePhys);
    rib_right_position = scaledThreeVector(-5.716814809004056, 8.559508872121505, -11.230810874703465);
    G4VPhysicalVolume* rib_right_1_phys = CreatePhysicalVolume("rib_right_1", rib_right, rib_right_rotation, rib_right_position, fEnvelopePhys);
    rib_left_position = scaledThreeVector(5.716815053050119, 8.559508833545378, -8.23081089827776);
    G4VPhysicalVolume* rib_left_2_phys = CreatePhysicalVolume("rib_left_2", rib_left, rib_left_rotation, rib_left_position, fEnvelopePhys);
    rib_right_position = scaledThreeVector(-5.716814809004056, 8.559508872121505, -8.23081089827776);
    G4VPhysicalVolume* rib_right_2_phys = CreatePhysicalVolume("rib_right_2", rib_right, rib_right_rotation, rib_right_position, fEnvelopePhys);
    rib_left_position = scaledThreeVector(5.716814809004056, 8.559508872121505, -5.230810921852053);
    G4VPhysicalVolume* rib_left_3_phys = CreatePhysicalVolume("rib_left_3", rib_left, rib_left_rotation, rib_left_position, fEnvelopePhys);
    rib_right_position = scaledThreeVector(-5.716814809004056, 8.559508872121505, -5.230810921852053);
    G4VPhysicalVolume* rib_right_3_phys = CreatePhysicalVolume("rib_right_3", rib_right, rib_right_rotation, rib_right_position, fEnvelopePhys);
    rib_left_position = scaledThreeVector(5.716814564957993, 8.559508910697632, -2.230810945426347);
    G4VPhysicalVolume* rib_left_4_phys = CreatePhysicalVolume("rib_left_4", rib_left, rib_left_rotation, rib_left_position, fEnvelopePhys);
    rib_right_position = scaledThreeVector(-5.716814809004056, 8.559508872121505, -2.230810945426347);
    G4VPhysicalVolume* rib_right_4_phys = CreatePhysicalVolume("rib_right_4", rib_right, rib_right_rotation, rib_right_position, fEnvelopePhys);
    rib_left_position = scaledThreeVector(5.716814320911929, 8.55950894927376, 0.7691890309993594);
    G4VPhysicalVolume* rib_left_5_phys = CreatePhysicalVolume("rib_left_5", rib_left, rib_left_rotation, rib_left_position, fEnvelopePhys);
    rib_right_position = scaledThreeVector(-5.716814809004056, 8.559508872121505, 0.7691890309993594);
    G4VPhysicalVolume* rib_right_5_phys = CreatePhysicalVolume("rib_right_5", rib_right, rib_right_rotation, rib_right_position, fEnvelopePhys);

    rib_right_rotation = new G4RotationMatrix(1.72757, -1.47567, -1.72757);
    rib_right_rotation_inverse = rib_right_rotation->inverse();
    rib_right_position = scaledThreeVector(-5.716814809004056, 8.559508872121505, -5.230810921852053);
    size = scaledThreeVector(0.4, 0.4, 2.8341128111577727);
    halfSpace1 = G4ThreeVector(5.876771e-02, -7.007764e-02, -9.958090e-01);
    halfSpace2 = G4ThreeVector(6.719852e-02, -8.013074e-02, 9.945167e-01);
    G4VSolid* rib_right_marrow_1_shape = new G4CutTubs(
            "rib_right_marrow_1",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);

    size = scaledThreeVector(0.4, 0.4, 2.5658347171819935);
    halfSpace1 = G4ThreeVector(7.882399e-02, -6.872687e-02, -9.945167e-01);
    halfSpace2 = G4ThreeVector(1.024330e-01, -8.931145e-02, 9.907224e-01);
    G4VSolid* rib_right_marrow_2_shape = new G4CutTubs(
            "rib_right_marrow_2",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(-10.874025519915964, 7.28115352314031, -4.449593319696884);
    trans = new G4ThreeVector();
    *trans = *position - *rib_right_position;
    *trans = (*rib_right_rotation)(*trans);
    rotation = new G4RotationMatrix(1.91054, -1.36989, -1.91054);
    *rotation = *rotation * rib_right_rotation_inverse;
    G4VSolid* rib_right_marrow_shape = new G4UnionSolid("rib_right_marrow", rib_right_marrow_1_shape, rib_right_marrow_2_shape, rotation, *trans);
    
    size = scaledThreeVector(0.4, 0.4, 2.189253583235205);
    halfSpace1 = G4ThreeVector(1.170750e-01, -6.901139e-02, -9.907224e-01);
    halfSpace2 = G4ThreeVector(1.658457e-01, -9.775952e-02, 9.812942e-01);
    G4VSolid* rib_right_marrow_3_shape = new G4CutTubs(
            "rib_right_marrow_3",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(-14.966813931887867, 5.290067045607286, -3.232819027871116);
    trans = new G4ThreeVector();
    *trans = *position - *rib_right_position;
    *trans = (*rib_right_rotation)(*trans);
    rotation = new G4RotationMatrix(2.16082, -1.24305, -2.16082);
    *rotation = *rotation * rib_right_rotation_inverse;
    rib_right_marrow_shape = new G4UnionSolid("rib_right_marrow", rib_right_marrow_shape, rib_right_marrow_3_shape, rotation, *trans);
    
    size = scaledThreeVector(0.4, 0.4, 1.828694046243503);
    halfSpace1 = G4ThreeVector(1.841965e-01, -5.597617e-02, -9.812942e-01);
    halfSpace2 = G4ThreeVector(2.450449e-01, -7.446715e-02, 9.666476e-01);
    G4VSolid* rib_right_marrow_4_shape = new G4CutTubs(
            "rib_right_marrow_4",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(-17.594545557112774, 2.7811536631136162, -1.6995934052360941);
    trans = new G4ThreeVector();
    *trans = *position - *rib_right_position;
    *trans = (*rib_right_rotation)(*trans);
    rotation = new G4RotationMatrix(2.55274, -1.10062, -2.55274);
    *rotation = *rotation * rib_right_rotation_inverse;
    rib_right_marrow_shape = new G4UnionSolid("rib_right_marrow", rib_right_marrow_shape, rib_right_marrow_4_shape, rotation, *trans);
    
    size = scaledThreeVector(0.4, 0.4, 1.670564138859993);
    halfSpace1 = G4ThreeVector(2.561104e-01, -1.471204e-07, -9.666475e-01);
    halfSpace2 = G4ThreeVector(2.561104e-01, 1.471204e-07, 9.666475e-01);
    G4VSolid* rib_right_marrow_5_shape = new G4CutTubs(
            "rib_right_marrow_5",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(-18.5, 2.220446049250313E-16, -1.1102230246251565E-16);
    trans = new G4ThreeVector();
    *trans = *position - *rib_right_position;
    *trans = (*rib_right_rotation)(*trans);
    rotation = new G4RotationMatrix(3.14159, -1.02225, 3.14159);
    *rotation = *rotation * rib_right_rotation_inverse;
    rib_right_marrow_shape = new G4UnionSolid("rib_right_marrow", rib_right_marrow_shape, rib_right_marrow_5_shape, rotation, *trans);
    
    size = scaledThreeVector(0.4, 0.4, 1.8286940462435033);
    halfSpace1 = G4ThreeVector(2.450449e-01, 7.446715e-02, -9.666476e-01);
    halfSpace2 = G4ThreeVector(1.841965e-01, 5.597617e-02, 9.812942e-01);
    G4VSolid* rib_right_marrow_6_shape = new G4CutTubs(
            "rib_right_marrow_6",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(-17.594545557112774, -2.7811536631136162, 1.6995934052360944);
    trans = new G4ThreeVector();
    *trans = *position - *rib_right_position;
    *trans = (*rib_right_rotation)(*trans);
    rotation = new G4RotationMatrix(-2.55274, -1.10062, 2.55274);
    *rotation = *rotation * rib_right_rotation_inverse;
    rib_right_marrow_shape = new G4UnionSolid("rib_right_marrow", rib_right_marrow_shape, rib_right_marrow_6_shape, rotation, *trans);
    
    size = scaledThreeVector(0.4, 0.4, 2.189253583235205);
    halfSpace1 = G4ThreeVector(1.658457e-01, 9.775952e-02, -9.812942e-01);
    halfSpace2 = G4ThreeVector(1.170750e-01, 6.901139e-02, 9.907224e-011);
    G4VSolid* rib_right_marrow_7_shape = new G4CutTubs(
            "rib_right_marrow_7",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(-14.966813931887867, -5.290067045607285, 3.2328190278711153);
    trans = new G4ThreeVector();
    *trans = *position - *rib_right_position;
    *trans = (*rib_right_rotation)(*trans);
    rotation = new G4RotationMatrix(-2.16082, -1.24305, 2.16082);
    *rotation = *rotation * rib_right_rotation_inverse;
    rib_right_marrow_shape = new G4UnionSolid("rib_right_marrow", rib_right_marrow_shape, rib_right_marrow_7_shape, rotation, *trans);
    
    size = scaledThreeVector(0.4, 0.4, 2.5658347171819926);
    halfSpace1 = G4ThreeVector(1.024330e-01, 8.931145e-02, -9.907224e-01);
    halfSpace2 = G4ThreeVector(7.882399e-02, 6.872687e-02, 9.945167e-01);
    G4VSolid* rib_right_marrow_8_shape = new G4CutTubs(
            "rib_right_marrow_8",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(-10.874025519915964, -7.28115352314031, 4.449593319696884);
    trans = new G4ThreeVector();
    *trans = *position - *rib_right_position;
    *trans = (*rib_right_rotation)(*trans);
    rotation = new G4RotationMatrix(-1.91054, -1.36989, 1.91054);
    *rotation = *rotation * rib_right_rotation_inverse;
    rib_right_marrow_shape = new G4UnionSolid("rib_right_marrow", rib_right_marrow_shape, rib_right_marrow_8_shape, rotation, *trans);
    
    size = scaledThreeVector(0.4, 0.4, 2.834112811157772);
    halfSpace1 = G4ThreeVector(6.719852e-02, 8.013074e-02, -9.945167e-01);
    halfSpace2 = G4ThreeVector(5.876771e-02, 7.007764e-02, 9.958090e-0);
    G4VSolid* rib_right_marrow_9_shape = new G4CutTubs(
            "rib_right_marrow_9",
            0.0,
            size->getX(),
            size->getZ(),
            0.0,
            2.0 * pi,
            halfSpace1,
            halfSpace2);
    position = scaledThreeVector(-5.716814809004056, -8.559508872121505, 5.230810921852054);
    trans = new G4ThreeVector();
    *trans = *position - *rib_right_position;
    *trans = (*rib_right_rotation)(*trans);
    rotation = new G4RotationMatrix(-1.72757, -1.47567, 1.72757);
    *rotation = *rotation * rib_right_rotation_inverse;
    rib_right_marrow_shape = new G4UnionSolid("rib_right_marrow", rib_right_marrow_shape, rib_right_marrow_9_shape, rotation, *trans);
    
    G4LogicalVolume* rib_right_marrow = CreateLogicalVolume("rib_right_marrow", marrow_material, rib_right_marrow_shape);
    CreatePhysicalVolume("rib_right_marrow_1", rib_right_marrow, noRot, new G4ThreeVector(), rib_right_1_phys);
#endif

	InstantiateChildren(fEnvelopePhys);
	return fEnvelopePhys;
}
