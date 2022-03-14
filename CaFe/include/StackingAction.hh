
/// \file hadronic/Hadr02/include/StackingAction.hh
/// \brief Definition of the StackingAction class


#ifndef StackingAction_h
#define StackingAction_h 1

#include "G4UserStackingAction.hh"
#include "EventAction.hh"
#include "globals.hh"
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

class HistoManager;
class StackingMessenger;
class G4Track;
class EventAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class StackingAction : public G4UserStackingAction
{
public:
    
    StackingAction(EventAction * evt);
    virtual ~StackingAction();
    
    inline void SetKillAll(G4bool value)  {fKillAll = value;};
    inline void SetKillEM (G4bool value)  {fKillEM  = value;};
    
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);
    
private:
    
    HistoManager*       fHistoManager;
    StackingMessenger*  fStackMessenger;
    EventAction*        fEvent;
    
    G4bool              fKillAll;
    G4bool              fKillEM;
    
    
    
    void OutputTrackToCSV(const G4Track*);
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

