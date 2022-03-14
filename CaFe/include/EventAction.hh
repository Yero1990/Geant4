/// \file hadronic/Hadr02/include/EventAction.hh
/// \brief Definition of the EventAction class

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "Run.hh"
#include "G4RunManager.hh"
#include "G4Track.hh"

#include <vector>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class G4Event;
class EventActionMessenger;
class G4Track;

class EventAction : public G4UserEventAction
{
public: // Without description
    
    int         fdebug = 2;
    G4double    Zresolution = 0.1; // Z-detection resolution for Cerenkov detector
    
    /// CLHEP random engine.
    CLHEP::HepRandomEngine* fRandomEngine;
    
    /// CLHEP random engine used in gaussian smearing.
    CLHEP::RandGauss* fRandomGauss;
    
    
    EventAction();
    virtual ~EventAction();
    
    virtual void BeginOfEventAction(const G4Event*);
    virtual void   EndOfEventAction(const G4Event*);
    
    void AddEventToDebug(G4int val);
    
    
    
    std::vector<G4double> Z_fragments;
    std::vector<G4double> A_fragments;
    G4double Zeff; // sum over Z^2 of all fragments that reach +/- 1.7 deg. detector
    G4double Zsmearedeff; // smeared Z
    G4int Nprotons, Npions, NHe3, NHe4, NLi6, NLi7, NBe8, NBe9, NBe10, NBe11, NB8, NB9, NB10, NB11;
    G4int NfragmentsInAngularCut; // number of nuclear fragments in nuclear cut
    
    G4int Nfragment[10][20];
    std::vector<G4String> breakup_fragments, event_particles;

    
    G4bool  CheckIfPassedTriggerCuts ( ); // check if event passed trigger cuts
    void       ClearEventTrackVector ( );
    void            RecordEventTrack ( const G4Track * track );
    G4bool            InSpectrometer ( G4Track * track, G4String Side );
    std::vector<G4Track*> eventTracks;
    // vector of fragment tracks
    std::vector<G4Track*> GetEventTracks() {return eventTracks;};
    G4bool      event_passed_trigger_cuts;
    
    
    void   WritePrimaryMomentumToCSV ();
    
    
    void   SetPrimaryPreStepMomentum ( G4ThreeVector p )
    { PrimaryPreStepMomenta.push_back(p); };
    
    void  SetPrimaryPostStepMomentum ( G4ThreeVector p )
    { PrimaryPostStepMomenta.push_back(p); };


    G4ThreeVector   GetPrimaryPreStepMomentum ( int i )
    { return PrimaryPreStepMomenta.at(i); };
    
    G4ThreeVector  GetPrimaryPostStepMomentum ( int i )
    { return PrimaryPostStepMomenta.at(i); };

    
private:
    
    EventActionMessenger* fEventMessenger;
    std::vector<G4int>    fSelectedEvents;
    
    G4int       fNSelected;
    G4bool      fDebugStarted;
    
    std::vector<G4ThreeVector>   PrimaryPreStepMomenta;
    std::vector<G4ThreeVector>  PrimaryPostStepMomenta;
    G4String      PrimaryName;
    G4ThreeVector PrimaryInitialMomentum;
    G4ThreeVector PrimaryFinalMomentum;
    
    
};

#endif


