/// \file hadronic/Hadr02/src/EventAction.cc
/// \brief Implementation of the EventAction class

#include "EventAction.hh"
#include "G4Event.hh"
#include "HistoManager.hh"
#include "EventActionMessenger.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4DynamicParticle.hh"

#include "G4Track.hh"
#include "G4ThreeVector.hh"

#include "G4UImanager.hh"
#include "G4ios.hh"
#include <G4RunManager.hh>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
EventAction::EventAction()
: G4UserEventAction(),
fEventMessenger(0),
fSelectedEvents(),
fNSelected(0),
fDebugStarted(false){

  G4cout << "Start EventAction Constructor . . ." << G4endl;
  
  fEventMessenger = new EventActionMessenger(this);
  
  time_t seed = time( NULL );
  fRandomEngine = new CLHEP::HepJamesRandom( static_cast< long >( seed ) );
  fRandomGauss = new CLHEP::RandGauss( fRandomEngine );
  
  fdebug=0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
EventAction::~EventAction(){
    delete fEventMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void EventAction::BeginOfEventAction(const G4Event* evt){
    if (fdebug>1)
        std::cout << "EventAction::BeginOfEventAction(const G4Event* evt)"
        << std::endl
        << "------------------------------------------------------------------"
        << std::endl;
    
    // New event
    G4int nEvt = evt->GetEventID();
    G4cout << "---> Event # " << nEvt << " Start" << G4endl;

    if (fNSelected>0) {
        for(G4int i=0; i<fNSelected; i++) {
            if (nEvt == fSelectedEvents[i]) {
                G4UImanager::GetUIpointer()->ApplyCommand("/random/saveThisEvent");
                G4UImanager::GetUIpointer()->ApplyCommand("/tracking/verbose  2");
                fDebugStarted = true;
                break;
            }
        }
    }
    
    // Initialize user actions
    HistoManager* man = HistoManager::GetPointer();
    man->BeginOfEvent();
    if ( man->GetVerbose() > 0 ) {
        G4cout << "EventAction: Event # " << nEvt << " started" << G4endl;
    }
    
    ClearEventTrackVector();
    if (fdebug>1)
        std::cout << "done EventAction::BeginOfEventAction(const G4Event* evt)"
        << std::endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void EventAction::EndOfEventAction(const G4Event* evt){
    if (fdebug>1)
        std::cout << "EventAction::EndOfEventAction(const G4Event* evt)"
        << std::endl;
    
    // kill event if there was no interaction in target
    // namely, if there is only one track
    int Ntracks = eventTracks.size();
    G4cout << "Ntracks in Event " << evt->GetEventID() << ": " << Ntracks << G4endl;
    if (fdebug>1) std::cout << "Ntracks in event " << evt->GetEventID() << ":" << Ntracks << std::endl;
    //    if (Ntracks<2) return;
        
    HistoManager* man = HistoManager::GetPointer();
    man->EndOfEvent();
    
    
    
    // Look for the primary particle' track,
    // and ask what is its momentum and direction after passing through the target
    PrimaryName = evt -> GetPrimaryVertex() -> GetPrimary() -> GetParticleDefinition() -> GetParticleName () ;
    PrimaryInitialMomentum = PrimaryPreStepMomenta.at(0);
    PrimaryFinalMomentum = PrimaryPreStepMomenta.at( PrimaryPreStepMomenta.size()-1 );
    WritePrimaryMomentumToCSV();

    G4cout << "---> Event # " << evt->GetEventID() << " End" << G4endl;

    if (fdebug>1) std::cout << "done EventAction::EndOfEventAction(const G4Event* evt)" << std::endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void EventAction::WritePrimaryMomentumToCSV(){
    
    G4String csvfilename = "events.csv";
    std::ofstream csvfile;
    csvfile.open( csvfilename  , std::ios_base::app );
    
    if (fdebug>1) {
        
        std::cout << PrimaryName
        << std::endl
        << "PrimaryInitialMomentum: ("
        << PrimaryInitialMomentum.x() << ","
        << PrimaryInitialMomentum.y() << ","
        << PrimaryInitialMomentum.z() << ") GeV/c"
        << std::endl
        << "PrimaryFinalMomentum: ("
        << PrimaryFinalMomentum.x() << ","
        << PrimaryFinalMomentum.y() << ","
        << PrimaryFinalMomentum.z() << ") GeV/c"
        << std::endl ;
    }

    // To Dien from Erez:
    // Here I write the initial and final momenta of the primary particle to the csv-file
    // this is just an example and what I had thought you need for start
    // but you can add more data here to the output file
    // just remember to add them to the header as well, in
    // RunAction::OpenCSVFile()
    
    G4double Pi_x = PrimaryInitialMomentum.x();
    G4double Pi_y = PrimaryInitialMomentum.y();
    G4double Pi_z = PrimaryInitialMomentum.z();
    G4double Pi = sqrt( Pi_x*Pi_x + Pi_y*Pi_y + Pi_z*Pi_z );
    
    G4double Pf_x = PrimaryFinalMomentum.x();
    G4double Pf_y = PrimaryFinalMomentum.y();
    G4double Pf_z = PrimaryFinalMomentum.z();
    G4double Pf = sqrt( Pf_x*Pf_x + Pf_y*Pf_y + Pf_z*Pf_z );
    
    csvfile
    << PrimaryName                  << ","
    << Pi_x/ (CLHEP::GeV)           << ","
    << Pi_y/ (CLHEP::GeV)           << ","
    << Pi_z/ (CLHEP::GeV)           << ","
    << Pi/ (CLHEP::GeV)             << ","
    << Pf_x/ (CLHEP::GeV)           << ","
    << Pf_y/ (CLHEP::GeV)           << ","
    << Pf_z/ (CLHEP::GeV)           << ","
    << Pf/ (CLHEP::GeV)             << ","
    << std::endl ;
    
    // close csv file
    csvfile.close();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void EventAction::AddEventToDebug(G4int val)  {
    fSelectedEvents.push_back(val);
    fNSelected++;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::RecordEventTrack(const G4Track * track){
    // save the (new) track in buffer
    if (fdebug>1)
        std::cout << "Run::RecordEventTrack(const G4Track * track)" << std::endl;
    
    G4Track * t = new G4Track(*track);
    t->SetTrackID( track->GetTrackID()  );
    t->SetParentID( track->GetParentID() );
    eventTracks.push_back(t);
    
    if (fdebug>1)
        std::cout
        << "recorded track " << track->GetTrackID() << " in " << t->GetTrackID() << std::endl
        << "done Run::RecordEventTrack(const G4Track * track)"
        << ", in eventTracks there are " << eventTracks.size() << " tracks"
        << std::endl;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void EventAction::ClearEventTrackVector(){
    eventTracks.clear();
    breakup_fragments.clear();
    event_particles.clear();
    Zeff = 0;
    Nprotons = Npions = NHe3 = NHe4 = NLi6 = NLi7 = NBe8 = NBe9 = NBe10 = NBe11 = NB8 = NB9 = NB10 = NB11 = 0 ;
}



