/// \file hadronic/Hadr02/src/DetectorMessenger.cc
/// \brief Implementation of the DetectorMessenger class
#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "HistoManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction * det)
: G4UImessenger(),
fDetector(det),
fTestDir(0),
fMatCmd(0),
fMat1Cmd(0),
fIonCmd(0),
fRCmd(0),
fLCmd(0),
fEdepCmd(0),
fBinCmd(0),
fNOfAbsCmd(0),
fVerbCmd(0),
fBeamCmd(0)
{
    fTestDir = new G4UIdirectory("/testhadr/");
    fTestDir->SetGuidance(" Hadronic Extended Example.");
    
    fMatCmd = new G4UIcmdWithAString("/testhadr/TargetMat",this);
    fMatCmd->SetGuidance("Select Material for the target");
    fMatCmd->SetParameterName("tMaterial",false);
    fMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fMat1Cmd = new G4UIcmdWithAString("/testhadr/WorldMat",this);
    fMat1Cmd->SetGuidance("Select Material for world");
    fMat1Cmd->SetParameterName("wMaterial",false);
    fMat1Cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fIonCmd = new G4UIcmdWithAString("/testhadr/ionPhysics",this);
    fIonCmd->SetGuidance("Select ion Physics");
    fIonCmd->SetParameterName("DPMJET",false);
    fIonCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fRCmd = new G4UIcmdWithADoubleAndUnit("/testhadr/TargetRadius",this);
    fRCmd->SetGuidance("Set radius of the target");
    fRCmd->SetParameterName("radius",false);
    fRCmd->SetUnitCategory("Length");
    fRCmd->SetRange("radius>0");
    fRCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fLCmd = new G4UIcmdWithADoubleAndUnit("/testhadr/TargetLength",this);
    fLCmd->SetGuidance("Set length of the target");
    fLCmd->SetParameterName("length",false);
    fLCmd->SetUnitCategory("Length");
    fLCmd->SetRange("length>0");
    fLCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fBinCmd = new G4UIcmdWithAnInteger("/testhadr/NumberOfBinsE",this);
    fBinCmd->SetGuidance("Set number of bins for Energy");
    fBinCmd->SetParameterName("NEbins",false);
    fBinCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fNOfAbsCmd = new G4UIcmdWithAnInteger("/testhadr/NumberDivZ",this);
    fNOfAbsCmd->SetGuidance("Set number of slices");
    fNOfAbsCmd->SetParameterName("NZ",false);
    fNOfAbsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fEdepCmd = new G4UIcmdWithADoubleAndUnit("/testhadr/MaxEdep",this);
    fEdepCmd->SetGuidance("Set max energy in histogram");
    fEdepCmd->SetParameterName("edep",false);
    fEdepCmd->SetUnitCategory("Energy");
    fEdepCmd->SetRange("edep>0");
    fEdepCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    fBeamCmd = new G4UIcmdWithABool("/testhadr/DefaultBeamPosition",this);
    fBeamCmd->SetGuidance("show inelastic and elastic cross sections");
    
    fVerbCmd = new G4UIcmdWithAnInteger("/testhadr/Verbose",this);
    fVerbCmd->SetGuidance("Set verbose for ");
    fVerbCmd->SetParameterName("verb",false);
    fVerbCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
    delete fMatCmd;
    delete fMat1Cmd;
    delete fIonCmd;
    delete fRCmd;
    delete fLCmd;
    delete fNOfAbsCmd;
    delete fTestDir;
    delete fBeamCmd;
    delete fVerbCmd;
    delete fEdepCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    HistoManager* h = HistoManager::GetPointer();
    if( command == fMatCmd ) {
        //    fDetector->SetTargetMaterial(newValue);
        std::cout << "target material is fixed to be liquid Hydrogen. If you want to change it, see Hadr02 example.." << std::endl;
        
    } else if( command == fMat1Cmd ) {
        fDetector->SetWorldMaterial(newValue);
    } else if( command == fIonCmd ) {
        h->SetIonPhysics(newValue);
    } else if( command == fRCmd ) {
        fDetector->SetTargetRadius(fRCmd->GetNewDoubleValue(newValue));
    } else if( command == fLCmd ) {
        h->SetTargetLength(fLCmd->GetNewDoubleValue(newValue));
    } else if( command == fNOfAbsCmd ) {
        h->SetNumberOfSlices(fNOfAbsCmd->GetNewIntValue(newValue));
    } else if( command == fBinCmd ) {
        h->SetNumberOfBinsE(fBinCmd->GetNewIntValue(newValue));
    } else if( command == fVerbCmd ) {
        h->SetVerbose(fVerbCmd->GetNewIntValue(newValue));
    } else if (command == fBeamCmd) {
        h->SetDefaultBeamPositionFlag(fBeamCmd->GetNewBoolValue(newValue));
    } else if (command == fEdepCmd) {
        h->SetMaxEnergyDeposit(fEdepCmd->GetNewDoubleValue(newValue));
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

