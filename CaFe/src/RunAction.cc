/// \file hadronic/Hadr02/src/RunAction.cc
/// \brief Implementation of the RunAction class
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"
#include "HistoManager.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4NistManager.hh"
#include "G4Element.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "Run.hh"
#include "G4RunManager.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
: G4UserRunAction(){

  G4cout << "Start RunAction Constructor . . ." << G4endl;

  fdebug = 0;
  id = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction(){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* run){
  id = run->GetRunID();
  G4cout << "===============" << G4endl;
  G4cout << "= Run " << id << " START =" << G4endl;
  G4cout << "===============" << G4endl;
  
  HistoManager::GetPointer()->BeginOfRun();
  
  OpenCSVFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::EndOfRunAction(const G4Run*){
  
  G4cout << "===================" << G4endl;
  G4cout << "= Run " << id << " END =" << G4endl;
  G4cout << "===================" << G4endl;
  
  HistoManager::GetPointer()->EndOfRun();
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::OpenCSVFile(){
    // This function has to be synced with
    // EventAction::WritePrimaryMomentumToCSV( )
    //
    
    G4String csvfilename = "events.csv";
    std::ofstream csvfile;
    
    if (fdebug) std::cout << "opening " << csvfilename << std::endl;
    
    csvfile.open( csvfilename  , std::ios_base::out );
    
    if (fdebug) std::cout << "opened csv file " << csvfilename << std::endl;
    
    csvfile
    << "PrimaryParticle"   << ","
    << "P init x  [GeV/c]"   << ","
    << "P init y  [GeV/c]"   << ","
    << "P init z  [GeV/c]"   << ","
    << "P init    [GeV/c]"     << ","
    << "P final x [GeV/c]"   << ","
    << "P final y [GeV/c]"   << ","
    << "P final z [GeV/c]"   << ","
    << "P final   [GeV/c]"     << ","
    << std::endl;
    csvfile.close();
    
    if (fdebug>1)
        std::cout << "done Run::OpenCSVFile()...." << std::endl;

}

