
/// \file hadronic/Hadr02/include/PrimaryGeneratorAction.hh
/// \brief Definition of the PrimaryGeneratorAction class
//
//
/////////////////////////////////////////////////////////////////////////
//
// PrimaryGeneratorAction
//
// Created: 31.01.03 V.Ivanchenko
//
// Modified:
// 04.06.2006 Adoptation of hadr01 (V.Ivanchenko)
//
////////////////////////////////////////////////////////////////////////
//

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4Event;
class G4ParticleGun;
class HistoManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:

  PrimaryGeneratorAction();
  virtual ~PrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event*);

public:
  G4ParticleGun* GetParticleGun() { return fParticleGun; };
  void SelectAction(G4int i) { fSelectedAction = i; };    
  G4int GetSelectedAction()  { return fSelectedAction; };

private:
  PrimaryGeneratorAction & operator=(const PrimaryGeneratorAction &right);
  PrimaryGeneratorAction(const PrimaryGeneratorAction&);

  G4ParticleGun*   fParticleGun;
  HistoManager*    fHisto;
  G4int                    fSelectedAction;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


