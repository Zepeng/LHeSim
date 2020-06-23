///////////////////////////////////////////////////////////////////
//
// Apr/2015  E. Nacher -> DetectorConstruction.cc
//
///////////////////////////////////////////////////////////////////

#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"

#include "G4LogicalBorderSurface.hh"
#include "G4OpticalSurface.hh"

using namespace CLHEP;

DetectorConstruction::DetectorConstruction()
{ }

DetectorConstruction::~DetectorConstruction()
{ }

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  bool fCheckOverlaps = true;
  //
  G4double length = 6*cm;
  //
  G4double ring_R1 = 59.4*cm;
  G4double ring_R2 = 60*cm;
  //
  G4double detector_dZ = 20*cm;
  //
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* default_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* cu_barrel = nist->FindOrBuildMaterial("G4_Cu");
  G4Material* al_barrel = nist->FindOrBuildMaterial("G4_Al");
  G4Material* glass = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");
  //define a vacuum
  G4double atomicNumber = 1.; 
  G4double massOfMole = 1.008*g/mole; 
  G4double vacuumdensity = 1.e-15*g/cm3; 
  G4double pressure = 3.e-8*pascal; 
  G4Material* Vacuum = new G4Material("interGalactic", atomicNumber, massOfMole, vacuumdensity);

  //     
  // World
  //
  G4double world_sizeXY = 2.*ring_R2;
  G4double world_sizeZ  = 1.2*detector_dZ;
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ); //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        Vacuum,         //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      fCheckOverlaps);       // checking overlaps 
                 
  //
  // Outer barrel  
  //
  G4Tubs* solidAlBarrel =
    new G4Tubs("AlBarrel", ring_R1, ring_R2, 0.5*detector_dZ, 0., twopi);
      
  G4LogicalVolume* logicAlBarrel =                         
    new G4LogicalVolume(solidAlBarrel,       //its solid
                        al_barrel,         //its material
                        "AlBarrel");         //its name
                                 
  //
  // place outer barrel in world
  //                    
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicAlBarrel,           //its logical volume
                    "AlBarrel",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    fCheckOverlaps);         // checking overlaps 
  
  //
  // copper inner barrel 
  //
  G4Tubs* solidCuBarrel =
    new G4Tubs("CuBarrel", 19.4*cm, 20*cm, 0.5*length, 0., twopi);
      
  G4LogicalVolume* logicCuBarrel =                         
    new G4LogicalVolume(solidCuBarrel,           //its solid
                        cu_barrel,         //its material
                        "CuBarrel");             //its name
                 
  //
  // place inner barrel in world
  //                    
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicCuBarrel,           //its logical volume
                    "CuBarrel",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    fCheckOverlaps);         // checking overlaps 
 
  //copper device holder
  G4Box *outerBox = new G4Box("Outer Box",4.8*cm/2.,3.0*cm/2.,1.8*cm/2.);
  G4Box *innerBox = new G4Box("Inner Box",4.0*cm/2.,0.5*cm/2.,1.8*cm/2.);
  G4SubtractionSolid *hollowBox = new G4SubtractionSolid("Hollow Box",outerBox,innerBox);
  
  G4LogicalVolume* logicCuHolder =                         
    new G4LogicalVolume(hollowBox,           //its solid
                        cu_barrel,         //its material
                        "CuHolder");             //its name
                 
  //
  // place copper holder in world
  //                    
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicCuHolder,           //its logical volume
                    "CuHolder",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    fCheckOverlaps);         // checking overlaps 
 
  //
  // glass cylinder 
  //
  G4Tubs* solidGlass =
    new G4Tubs("GlassCylinder", 0.05*cm, 2*cm, 0.25*cm, 0., twopi);
      
  G4LogicalVolume* logicGlass =                         
    new G4LogicalVolume(solidGlass,           //its solid
                        glass,         //its material
                        "GlassCylinder");             //its name
                 
  //
  // place glass holder in world
  //                    
  G4RotationMatrix* rotm  = new G4RotationMatrix();
  rotm->rotateX(90*deg); 
  new G4PVPlacement(rotm,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicGlass,           //its logical volume
                    "GlassCylinder",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    fCheckOverlaps);         // checking overlaps 
 
  G4double z, a, density;
  density = 0.125*g/cm3;
  a = 4.003*g/mole;
  
  G4Material* lhe = new G4Material("liquidHelium", z=2., a, density);
  //
  // target  
  //
  G4double target_radius = 5*cm;
  G4double target_dZ = 5*cm;  
    
  G4Tubs* solidTarget =
    new G4Tubs("Target", 0., target_radius, 0.5*target_dZ, 0., twopi);
      
  G4LogicalVolume* logicTarget =                         
    new G4LogicalVolume(solidTarget,        //its solid
                        lhe,         //its material
                        "TargetLV");        //its name
               
  //
  // place target in world
  //                    
  new G4PVPlacement(rotm,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicTarget,            //its logical volume
                    "Target",               //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    fCheckOverlaps);         // checking overlaps 
                                          
  // Visualization attributes
  //
  G4Colour colour(1.,0.,0.); G4VisAttributes attribs(colour);
  logicCuBarrel->SetVisAttributes (G4VisAttributes::GetInvisible());
  logicCuBarrel->SetVisAttributes (attribs);
  G4Colour colour1(0.,1.,0.); G4VisAttributes attribs1(colour1);
  logicAlBarrel->SetVisAttributes (G4VisAttributes::GetInvisible());    
  logicAlBarrel->SetVisAttributes (attribs1);
  logicCuHolder->SetVisAttributes (G4VisAttributes::GetInvisible());    
  logicCuHolder->SetVisAttributes (attribs);
  G4Colour colour2(0.,0.,1.); G4VisAttributes attribs2(colour2);
  logicGlass->SetVisAttributes (G4VisAttributes::GetInvisible());   
  logicGlass->SetVisAttributes(attribs2);
  logicTarget->SetVisAttributes(attribs);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl; 

  //always return the physical World
  //
  return physWorld;
	
}
