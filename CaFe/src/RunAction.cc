/// \file hadronic/Hadr02/src/RunAction.cc
/// \brief Implementation of the RunAction class
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"
#include "HistoManager.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4NistManager.hh"
#include "G4AnalysisManager.hh"
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


  //C.Y. Create and open ROOTfile for storing variables
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  //C.Y. Open an output file
  analysisManager->OpenFile("cafe_Ca48.root");

  //C.Y. Create Histograms
  // 1D
  analysisManager->CreateH1("0", "(# protons at boundary)", 100, -1, 5);
  analysisManager->CreateH1("1", "(track ID)", 100, 0, 10);
  analysisManager->CreateH1("2", "(Proton Momentum @ Boundary)", 100, 1.31, 1.33);
  analysisManager->CreateH1("3", "(Proton In-Plane Angle #theta @ Boundary)", 60, 66.3, 67.8);
  analysisManager->CreateH1("4", "(Proton Out-Plane Angle #phi @ Boundary [-180,180])", 60, 179.5, 180.5);
  analysisManager->CreateH1("5", "(Proton Out-Plane Angle #phi @ Boundary [0, 360])", 60, 178, 182);
  analysisManager->CreateH1("6", "track path length [mm]", 100, 6.7, 7.1);
  analysisManager->CreateH1("7", "track path time [ns]", 100, 0, 0.05);

  
  // 2D
  analysisManager->CreateH2("0", "(Proton theta vs. phi)[phi: (-180,180)]", 60, 179.5, 180.5, 60, 66.3, 67.8);
  analysisManager->CreateH2("1", "(Proton theta vs. phi)[phi: (0,360)]", 60, 178, 182, 60, 66.3, 67.8);

  analysisManager->CreateH2("2", "(Proton P vs. theta)", 60, 66.3, 67.8, 100, 1.31, 1.33);
  analysisManager->CreateH2("3", "(Proton P vs. phi)[phi: (-180,180)]", 60, 179.5, 180.5, 100, 1.31, 1.33);
  analysisManager->CreateH2("4", "(Proton P vs. phi)[phi: (0,360)]", 60, 178, 182, 100, 1.31, 1.33);

  analysisManager->CreateH2("5", "(Proton dth vs. dphi) [phi:0,360]", 60, -0.07, 0.07, 60, -0.07, 0.07);
  analysisManager->CreateH2("6", "(Proton delta vs. dth) ", 60, -0.07, 0.07, 60, -0.7, 0.7);
  analysisManager->CreateH2("7", "(Proton delta vs. dphi) ", 60, -0.07, 0.07, 60, -0.7, 0.7);

  
  OpenCSVFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void RunAction::EndOfRunAction(const G4Run*){
  
  G4cout << "===================" << G4endl;
  G4cout << "= Run " << id << " END =" << G4endl;
  G4cout << "===================" << G4endl;
  
  HistoManager::GetPointer()->EndOfRun();

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
  
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

