/// \file hadronic/Hadr02/include/DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class
#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Material.hh"
#include "G4SystemOfUnits.hh"
#include "G4PVPlacement.hh"

class CheckVolumeSD;
class TargetSD;
class G4LogicalVolume;
class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class DetectorConstruction : public G4VUserDetectorConstruction{
public:
    
    DetectorConstruction();
    virtual ~DetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct();
    
    void SetWorldMaterial(const G4String&);
    void SetTargetMaterial();// fixed material: liquid hydrogen
    
    void SetTargetRadius(G4double val);
    
private:
    
    DetectorConstruction & operator=(const DetectorConstruction &right);
    DetectorConstruction(const DetectorConstruction&);
    
    G4double fRadius;
    
    G4Material*  fTargetMaterial;
    G4Material*  fWorldMaterial;
    
    TargetSD*        fTargetSD;
    
    G4LogicalVolume* fLogicTarget;
    G4LogicalVolume* fLogicWorld;
    
    DetectorMessenger* fDetectorMessenger;
    
    G4PVPlacement *physTarget;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

#endif

