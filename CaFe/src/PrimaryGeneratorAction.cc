/// \file hadronic/Hadr02/src/PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "HistoManager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
fParticleGun(0),
fHisto(0)
{
    fParticleGun  = new G4ParticleGun(1);
    fParticleGun -> SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
    fParticleGun -> SetParticleEnergy( 1 * GeV );
    fHisto = HistoManager::GetPointer();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4double zVertex = -5.0*mm;
    
    G4ParticleDefinition* beam_particle = particleTable->FindParticle("proton");
    fParticleGun -> SetParticleDefinition( beam_particle );
    fParticleGun -> SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
    fParticleGun -> SetParticleEnergy( 1 * GeV );
    
    
    fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,zVertex));
    fParticleGun->GeneratePrimaryVertex(event);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
