
/// \file hadronic/Hadr02/src/StackingAction.cc
/// \brief Implementation of the StackingAction class
//
#include "StackingAction.hh"

#include "HistoManager.hh"
#include "StackingMessenger.hh"
#include "Run.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include <G4RunManager.hh>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StackingAction::StackingAction(EventAction * evt)
  : G4UserStackingAction(),
    fHistoManager(0),
    fStackMessenger(0),
    fEvent(evt),
    fKillAll(true),
    fKillEM(true)
{
  
  G4cout << "Start StackingAction Constructor . . ." << G4endl;
  
  fStackMessenger = new StackingMessenger(this);
  fHistoManager   = HistoManager::GetPointer();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
StackingAction::~StackingAction(){
    delete fStackMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4ClassificationOfNewTrack
StackingAction::ClassifyNewTrack(const G4Track* track)
{

    int fdebug=0;
    if (fdebug>1) {std::cout << "Start StackingAction::ClassifyNewTrack(const G4Track* track) " << std::endl;}
    
    G4ClassificationOfNewTrack status = fUrgent;
    
    fHistoManager->ScoreNewTrack(track);
    
    if(track->GetDefinition()->GetParticleName()=="proton"){
      G4cout << "Start StackingAction::ClassifyNewTrack . . ." << G4endl;     
      std::cout << "Track #"
		<< track->GetTrackID() << " of "
		<< track->GetDefinition()->GetParticleName()
		<< " P (GeV)= " << track->GetMomentum().mag()/GeV
		<< " Ekin (MeV)= " << track->GetKineticEnergy()/MeV
		<< " ID= " << track->GetParentID()
		<< std::endl;
    }
    
    //    Run* run = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
    
    
    //    G4Event * event = G4RunManager::GetRunManager()-> GetCurrentEvent();
    fEvent -> RecordEventTrack( track );
    
    if (fdebug > 1 ) {
        std::cout << "Track #"
        << track->GetTrackID() << " of "
        << track->GetDefinition()->GetParticleName()
        << " E(MeV)= " << track->GetKineticEnergy()/MeV
        << " ID= " << track->GetParentID()
        << std::endl;
    }
    //    if (track->GetTrackID() == 1) { return status; }
    
    //  //stack or delete secondaries
    //  if (fKillAll)  { status = fKill; }
    //  else if(fKillEM && track->GetDefinition()->GetPDGMass() < MeV)
    //    { status = fKill; }
    
    status = fUrgent;
    
    if (fdebug>1)
        std::cout << "End StackingAction::ClassifyNewTrack(const G4Track* track) " << std::endl;

    return status;
}



