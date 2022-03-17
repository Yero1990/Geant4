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
#include "FTFP_BERT.hh"
#include "FTFP_INCLXX.hh"
#include <iostream>

using namespace std;


int main(int argc,char** argv) {

  
  //detect interactive mode (if no arguments) and define UI session
  G4UIExecutive* ui = nullptr;
  if (argc == 1) ui = new G4UIExecutive(argc,argv);


  
  //choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
  
  //Construct the default run manager
  G4RunManager * runManager = new G4RunManager();
  
  //set mandatory DetectorConstruction initialization class
  DetectorConstruction * fDetector = new DetectorConstruction();
  runManager->SetUserInitialization(fDetector);

  //set mandatory PhysicsList initialization class
  G4PhysListFactory factory;
  G4VModularPhysicsList* physicsList = new FTFP_INCLXX; //FTFP_BERT;
  runManager->SetUserInitialization(physicsList);

  
  HistoManager::GetPointer()->SetPhysicsList(physicsList);


  //set mandatory PrimaryGenerationAction action class
  runManager->SetUserAction(new PrimaryGeneratorAction());

  
  //set optional user action classes
  runManager->SetUserAction(new RunAction());

  EventAction * evt = new EventAction();
  runManager->SetUserAction(evt);
  
  StackingAction* stackingAction = new StackingAction(evt);
  runManager->SetUserAction(stackingAction);
  
  TrackingAction* trackingAction = new TrackingAction();
  
  SteppingAction* steppingAction = new SteppingAction(trackingAction, fDetector, evt);
  runManager->SetUserAction(steppingAction);

  //
  runManager->Initialize();
  
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

