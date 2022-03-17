/// \file hadronic/Hadr02/src/DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4RunManager.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4UnitsTable.hh"
#include "G4ios.hh"

#include "TargetSD.hh"
#include "G4SDManager.hh"
#include "HistoManager.hh"
#include "G4NistManager.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(), fTargetSD(0), fDetectorMessenger(0)
{
  G4cout << "Calling DetectorConstruction::DetectorConstruction() . . ." << G4endl;
  fDetectorMessenger = new DetectorMessenger(this);
  
  // Prepare sensitive detectors
  fTargetSD = new TargetSD("targetSD");
  G4SDManager::GetSDMpointer()->AddNewDetector(fTargetSD);
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::~DetectorConstruction() {
  G4cout << "Calling DetectorConstruction::~DetectorConstruction() . . ." << G4endl;

  delete fDetectorMessenger;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4cout << "Calling DetectorConstruction::Construct . . ." << G4endl;

  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  //-----------------------------------
  // C.Y. Create World Mother Volume
  //-----------------------------------
  

  // Geometrical World Volume
  G4double world_sizeX = 2.*cm;  // +/- 2 cm along x-axis 
  G4double world_sizeY = 2.*cm;  // +/- 2 cm ''
  G4double world_sizeZ = 2.*cm;  // +/- 2 cm ''
  G4Box* solidWorld = new G4Box("World",         // its name
				world_sizeX,     // its size along x-axis
				world_sizeY,     //  '' along y-axis 
				world_sizeZ );   //  '' along z-axis

  // Logical World Volume
  G4Material* Air = nist->FindOrBuildMaterial("G4_AIR");
  G4LogicalVolume* logicalWorld = new G4LogicalVolume(solidWorld,  // its solid
						      Air,         // its material
						      "World");    // its name
   
  // Physical World Volume
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0,                      // no rotation
						  G4ThreeVector(),   // world volume with origin at (0,0,0)
						  logicalWorld,           // its logical volume 
						  "World",                // its name
						  0,                      // its mother volume (none)
						  false,                  // no boolean operation
						  0,                      // copy number
						  true);                  // overlaps check

  G4VisAttributes zero = G4VisAttributes::GetInvisible();
  logicalWorld->SetVisAttributes(zero);
  
  //-----------------------------------
  // C.Y. Create Target Volume
  //-----------------------------------

  // Geometrical Target Volume
  G4double fRadius    = 6.35  * mm;  // define the target radius
  G4double fThick     = 4.301 * mm; // define the target length (thickness)
  //"cylindrical 'hockey puck' shape"
  G4Tubs* solidA      = new G4Tubs("Target",    // its name
				   0.,          // inner radius
				   fRadius,     // outer radius
				   fThick,      // length ("thickness")
				   0.,          // phi coverage (min phi)
				   twopi);      // phi coverage (max phi)
  
  // Logical Target Volume
  G4double Ca48_density = 1.86 * g/cm3;
  G4double Ca48_mass = 47.952553 * g/mole;

  G4Isotope* Ca48 = new G4Isotope("Ca48", 20, 48, Ca48_mass);
  G4Element* elCa = new G4Element("Calcium", "Ca", 1 );
  elCa->AddIsotope(Ca48, 100.*perCent);  //assume target is 100% Ca48 composition

  G4Material* targMat = new G4Material("Calcium", Ca48_density,  1 );
  targMat->AddElement(elCa, 1);
  
  G4LogicalVolume* logicalTarget = new G4LogicalVolume(solidA,       // its solid
						      targMat,      // its material
						      "Target");    // its name
  logicalTarget->SetSensitiveDetector(fTargetSD);

  
  HistoManager::GetPointer()->SetTargetMaterial(targMat);
  G4RunManager::GetRunManager()->PhysicsHasBeenModified();


   // Physical Target Volume
  new G4PVPlacement(0,                      // no rotation
		    G4ThreeVector(0,0,0),    // target volume with origin at (0,0,0) of world
		    logicalTarget,           // its logical volume 
		    "Target",                // its name
		    logicalWorld,            // its mother volume 
		    false,                   // no boolean operation
		    0,                       // copy number
		    true);                   // overlaps check
  

  // add color of target for better visual
  G4VisAttributes tgtColor(G4Colour(0.5,  // red
				    0.5,  // green
				    0.5,  // blue
				    0.1)); // transparency (alpha)
  
  logicalTarget->SetVisAttributes(tgtColor);

   
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  return physWorld;
}
