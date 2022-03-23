/// \file SteppingAction.hh
/// \brief Definition of the SteppingAction class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "DetectorConstruction.hh"

class TrackingAction;
class DetectorConstruction;
class EventAction;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction(TrackingAction*,DetectorConstruction*,EventAction*);
  ~SteppingAction();
  
  virtual void UserSteppingAction(const G4Step*);


private:
  TrackingAction* fTrackingAction;
  DetectorConstruction* fDetector;
  EventAction*       fEventAction;

  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
