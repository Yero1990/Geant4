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
    
        
private:
    
  DetectorConstruction & operator=(const DetectorConstruction &right);
  DetectorConstruction(const DetectorConstruction&);
  
  TargetSD*        fTargetSD;
  DetectorMessenger* fDetectorMessenger;
  
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

#endif

