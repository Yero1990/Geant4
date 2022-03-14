/// \file hadronic/Hadr02/include/RunAction.hh
/// \brief Definition of the RunAction class

#ifndef RunAction_h
#define RunAction_h 1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "globals.hh"
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class RunAction : public G4UserRunAction
{
public: // Without description

  RunAction();
  virtual ~RunAction();
 
  virtual void BeginOfRunAction(const G4Run*);
  // In this method histogramms are booked

  virtual void EndOfRunAction(const G4Run*);
  // In this method bookHisto method is called in which histogramms are filled

    
    
    
    int fdebug=2;
    
    
    void            OpenCSVFile ( );
    void OpenEventInfoToCSVFile ( );
    
    

};

#endif

