/// \file hadronic/Hadr02/src/StackingMessenger.cc
/// \brief Implementation of the StackingMessenger class

#include "StackingMessenger.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "StackingAction.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingMessenger::StackingMessenger(StackingAction* stack)
 : G4UImessenger(),
   fStackAction(stack),
   fKillCmd(0),
   fKillEMCmd(0)
   
{
  fKillCmd = new G4UIcmdWithABool("/testhadr/killAll",this);
  fKillCmd->SetGuidance("  Choice : true false");
  fKillCmd->SetGuidance("Kill all secondaries");
  fKillCmd->SetParameterName("choice",true);
  fKillCmd->SetDefaultValue(false);

  fKillEMCmd = new G4UIcmdWithABool("/testhadr/killEM", this);
  fKillEMCmd->SetGuidance("  Choice : true false");
  fKillEMCmd->SetGuidance("Kill secondary e+, e-, gamma");
  fKillEMCmd->SetParameterName("ch", true);
  fKillEMCmd->SetDefaultValue(true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingMessenger::~StackingMessenger()
{
  delete fKillCmd;
  delete fKillEMCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void StackingMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{     
  if(command == fKillCmd) {
    fStackAction->SetKillAll(fKillCmd->GetNewBoolValue(newValue));
  } else if(command == fKillEMCmd) {
    fStackAction->SetKillEM(fKillEMCmd->GetNewBoolValue(newValue));
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
