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

    // in this context, beam particle is NOT the accelerator incident beam on target, but rather
    // a starting primary particle generated to start our simulation
    G4ParticleDefinition* beam_particle = particleTable->FindParticle("proton");

    G4double zVertex = 0.0*mm;    //primary particle vertex (set at 0 mm on z-vertex)
    G4double theta_p = 66.4*deg;  //primary particle ("proton") scattering angle
    G4double Pf = 1.325 * GeV;    // proton momentum
    G4double mp = beam_particle->GetPDGMass();  //PDG mass is returned in unites of MeV
    G4double Tp = std::sqrt(Pf*Pf + mp*mp) - mp;  // kinetic energy
  
    
    fParticleGun -> SetParticleDefinition( beam_particle );
    fParticleGun -> SetParticleMomentumDirection(G4ThreeVector(-sin(theta_p), 0, cos(theta_p)));
    fParticleGun -> SetParticleEnergy( Tp );
    
    
    fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,zVertex));
    fParticleGun->GeneratePrimaryVertex(event);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
