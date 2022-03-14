
/// \file hadronic/Hadr02/src/UrQMDProtonBuilder.cc
/// \brief Implementation of the UrQMDProtonBuilder class
//
//
//---------------------------------------------------------------------------
//

#ifdef G4_USE_URQMD
#include "UrQMDProtonBuilder.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"
#include "G4HadronicParameters.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UrQMDProtonBuilder::UrQMDProtonBuilder() {
  fMin = 0*MeV;
  fMax = G4HadronicParameters::Instance()->GetMaxEnergy();
  fModel = new G4UrQMD1_3Model();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UrQMDProtonBuilder::~UrQMDProtonBuilder() 
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UrQMDProtonBuilder::Build(G4HadronElasticProcess * )
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UrQMDProtonBuilder::Build(G4ProtonInelasticProcess * aP)
{
  fModel->SetMinEnergy(fMin);
  fModel->SetMaxEnergy(fMax);
  aP->RegisterMe(fModel);
}
#endif //G4_USE_URQMD

