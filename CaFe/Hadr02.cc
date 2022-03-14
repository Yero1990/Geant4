#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"
#include "DetectorConstruction.hh"
#include "G4PhysListFactory.hh"
#include "G4VModularPhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "StackingAction.hh"
#include "SteppingAction.hh"
#include "TrackingAction.hh"
#include "HistoManager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "UrQMD.hh"
#include "CRMC_FTFP_BERT.hh"
#include <iostream>

using namespace std;


int main(int argc,char** argv) {

  
  //detect interactive mode (if no arguments) and define UI session
  G4UIExecutive* ui = nullptr;
  if (argc == 1) ui = new G4UIExecutive(argc,argv);

  cout << "# of arguments: " << argc << endl;
  
  //choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
  
  //Construct the default run manager
  G4RunManager * runManager = new G4RunManager();
  
  //set mandatory initialization classes
  
  DetectorConstruction * fDetector = new DetectorConstruction();
  runManager->SetUserInitialization(fDetector);
  
  G4PhysListFactory factory;
  G4VModularPhysicsList* phys = 0;
  
  // default Physics List for this example
  G4String physName = "FTFP_BERT"; //"QBBC";
  //G4String physName = "UrQMD";
  
  // Physics List name defined via 2nd argument
  if (argc==3) { physName = argv[2]; }
  else {
    char* path = std::getenv("PHYSLIST");
    if (path) { physName = G4String(path); }
  }
  if ( physName == "UrQMD" ) {
    phys = new UrQMD;
  } else if ( physName == "CRMC_FTFP_BERT" ) {
    phys = new CRMC_FTFP_BERT;
  } else {
        phys = factory.GetReferencePhysList( physName );
  }

  cout << "physName ------------- > " << physName << endl;
  // Physics List is defined via environment variable PHYSLIST
  if(!phys) {
    G4cout << "Hadr02 FATAL ERROR: Physics List is not defined"
	   << G4endl;
    return 1;
  }
  
  runManager->SetUserInitialization(phys);
  HistoManager::GetPointer()->SetPhysicsList(phys);
  runManager->SetUserAction(new PrimaryGeneratorAction());
  
  //set user action classes
  runManager->SetUserAction(new RunAction());
  EventAction * evt = new EventAction();
  runManager->SetUserAction(evt);
  
  StackingAction* stackingAction = new StackingAction(evt);
  runManager->SetUserAction(stackingAction);
  
  TrackingAction* trackingAction = new TrackingAction();
  SteppingAction* steppingAction = new SteppingAction(trackingAction, fDetector, evt);
  runManager->SetUserAction(steppingAction);

  
  //initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
    
  //get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  
  if (ui)  {
    //interactive mode
    UImanager->ApplyCommand("/control/execute vis.mac");
    ui->SessionStart();
    delete ui;
  }
  else  {
    //batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  
  //job termination
  delete visManager;
  delete runManager;
}
