/// \file hadronic/Hadr02/src/DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Tubs.hh"
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
  : G4VUserDetectorConstruction(), fRadius(10.*cm), fTargetMaterial(0), fWorldMaterial(0),
    fTargetSD(0), fLogicTarget(0), fLogicWorld (0), fDetectorMessenger(0)
{
  fDetectorMessenger = new DetectorMessenger(this);
  
  // Prepare sensitive detectors
  fTargetSD = new TargetSD("targetSD");
  G4SDManager::GetSDMpointer()->AddNewDetector(fTargetSD);
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::~DetectorConstruction() {
  delete fDetectorMessenger;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  
  // The target
  fRadius             = 10. * mm / 2; // define the target radius
  G4double target_dz  = 5. * mm; // define the target length
  G4double worldR     = 1.*m;
  G4double worldZ     = 2.*m;
  SetTargetMaterial();
  SetWorldMaterial("G4_AIR");
  
  // World
  G4Tubs* solidW = new G4Tubs("World",0.,worldR,worldZ,0.,twopi);
  fLogicWorld = new G4LogicalVolume( solidW,fWorldMaterial,"World");
  G4VPhysicalVolume* world = new G4PVPlacement(0,G4ThreeVector(), fLogicWorld,"World",0,false,0);
  G4VisAttributes zero = G4VisAttributes::GetInvisible();
  fLogicWorld->SetVisAttributes(zero);
  
  // Target volume
  G4Tubs* solidA = new G4Tubs("Target",0.,fRadius, target_dz,0.,twopi);
  fLogicTarget = new G4LogicalVolume( solidA, fTargetMaterial,"Target");
  fLogicTarget->SetSensitiveDetector(fTargetSD);

  physTarget = new G4PVPlacement(0,
				 G4ThreeVector(0.0,0.0, target_dz/2 ), // location
				 fLogicTarget, "Target",fLogicWorld,false,0);

  G4VisAttributes regCcolor(G4Colour(0.8, 0.3, 0.2));
  fLogicTarget->SetVisAttributes(regCcolor);
    
   
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  return world;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetTargetMaterial() {
  
    // Set the material to be 40Ca
    // We can use G4_Ca here, but since additional targets will be simulated,
    // I give here an example of how to define a specified material
    // effective A = 40.08 g/mole
    // mass density near room temperature 1.55 g/cm3
    G4int ncomponents, natoms;
    G4String symbol;
    G4double Aeff, z, density;      //z=mean number of protons;
    G4Element* Ca40 = new G4Element("Calcium", symbol = "Ca",  z = 20, Aeff = 40.08 * g/mole );

    G4Material* CalciumMaterial = new G4Material("CalciumMaterial", density = 1.55 * g/cm3, ncomponents=1);
    CalciumMaterial->AddElement(Ca40, natoms=1);
    
    fTargetMaterial = CalciumMaterial;
    HistoManager::GetPointer()->SetTargetMaterial(fTargetMaterial);
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetWorldMaterial(const G4String& mat){

  // search the material by its name
  G4Material* material = G4NistManager::Instance()->FindOrBuildMaterial(mat);
  
  if (material && material != fWorldMaterial) {
    fWorldMaterial = material;
    if(fLogicWorld) { fLogicWorld->SetMaterial(fWorldMaterial); }
    G4RunManager::GetRunManager()->PhysicsHasBeenModified();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::SetTargetRadius(G4double val)  {
  if(val > 0.0) {
    fRadius = val;
    G4RunManager::GetRunManager()->GeometryHasBeenModified();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
