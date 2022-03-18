/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "SteppingAction.hh"
#include "TrackingAction.hh"
#include "EventAction.hh"
#include "HistoManager.hh"
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "PrimaryGeneratorAction.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(TrackingAction* TrAct, DetectorConstruction* det, EventAction* event)
: G4UserSteppingAction(),fTrackingAction(TrAct),fDetector(det),fEventAction(event)
{
  G4cout << "Start SteppingAction Constructor . . ." << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SteppingAction::UserSteppingAction(const G4Step* aStep) {

    int fdebug=1;

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

 
    if (fdebug>1){
        std::cout <<"SteppingAction::UserSteppingAction(const G4Step* aStep) " << std::endl;
    }
    
    const G4StepPoint* startPoint = aStep->GetPreStepPoint();
    const G4StepPoint* endPoint   = aStep->GetPostStepPoint();
    
    
    
    const G4VProcess* process   = endPoint->GetProcessDefinedStep();
    
    G4ThreeVector p_start= startPoint->GetMomentum();
    G4ThreeVector p_end  = endPoint->GetMomentum();
    G4double ekin   = endPoint->GetKineticEnergy();
    G4double trackl = aStep->GetTrack()->GetTrackLength();
    G4double time   = aStep->GetTrack()->GetLocalTime();
    G4int trackID = aStep->GetTrack()->GetTrackID();
    G4double phi_f      = endPoint -> GetPosition().phi();
    G4double theta_f    = endPoint -> GetPosition().theta();
    
    G4bool pAtBoundary = false;
    if (fdebug==1){
      if(aStep->GetTrack()->GetParticleDefinition()->GetParticleName()=="proton"){
	
	
	//C.Y.  useful line to define whether the particle has reached the target volume boundary or not. This might be useful if we
	// want to know how many particles actually managed to leave the target. see: https://geant4-forum.web.cern.ch/t/how-to-detect-particle-without-killing-it/3209/5
	//G4bool boundaryHit = endPoint->GetStepStatus() == fGeomBoundary && startPoint->GetPhysicalVolume()->GetName() == "World" ;
	//G4cout << "startPoint->GetPhysicalVolume()->GetName(): " << startPoint->GetPhysicalVolume()->GetName() << G4endl;
	//G4cout << "startPoint->GetStepStatus(): " << startPoint->GetStepStatus() << G4endl;
	//G4cout << "endPoint->GetStepStatus(): " << endPoint->GetStepStatus() << G4endl;
	
	/*
	  G4cout << "fGeomBoundary ID: " << fGeomBoundary << G4endl;      
	  G4cout << "fWorldBoundary ID: " << fWorldBoundary << G4endl;
	  G4cout << "PreStepPoint Status: " << aStep->GetPreStepPoint()->GetStepStatus() << G4endl;
	  G4cout << "PostStepPoint Status: " << aStep->GetPostStepPoint()->GetStepStatus() << G4endl;
	*/

	
	if (aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary && trackID==1) {

	  pAtBoundary = true;
	  analysisManager->FillH1(0, pAtBoundary);

	  /*
	  std::cout << "Step ends on geometry boundary ! ! !" << std::endl;
	  G4cout << "Step #: " << aStep->GetTrack()->GetCurrentStepNumber() << G4endl;
	  G4cout << "Step Track ID: " << aStep->GetTrack()->GetTrackID() << G4endl;
	  G4cout << "Step Particle: "<< aStep->GetTrack()->GetParticleDefinition()->GetParticleName() << G4endl;
	  G4cout << "Step Process: " << process->GetProcessName() << G4endl;
	  G4cout << "Step Track Length [mm]: " << trackl/CLHEP::mm << G4endl;
	  G4cout << "Step Time [n]: " << time/CLHEP::ns << G4endl;
	  G4cout << "Start Step Pos: (x,y,z)"      << "("
		 << startPoint->GetPosition().x() << ","
		 << startPoint->GetPosition().y() << ","
		 << startPoint->GetPosition().z() << ") [mm]"
		 << "->";
	  
	  G4cout << "End Step Pos: (x,y,z)" << "("
		 << endPoint->GetPosition().x() << ","
		 << endPoint->GetPosition().y() << ","
		 << endPoint->GetPosition().z() << ") [mm]"
		 << ", step length: " << aStep->GetStepLength()/CLHEP::mm << " mm "
		 << G4endl;
	  G4cout << "Start Step Momentum: (Px,Py,Pz, P)"      << "("
		 << startPoint->GetMomentum().x() << ","
		 << startPoint->GetMomentum().y() << ","
		 << startPoint->GetMomentum().z() << ","
		 << startPoint->GetMomentum().mag() << ") [GeV/c]"
		 << "->";
	  
	  G4cout << "End Step Momentum: (Px,Py,Pz,P)" << "("
		 << endPoint->GetMomentum().x() << ","
		 << endPoint->GetMomentum().y() << ","
		 << endPoint->GetMomentum().z() << ","
		 << endPoint->GetMomentum().mag() << ") [GeV/c]"
		 << G4endl;

	  G4cout << "End Step (theta, phi): " << "("
		 << endPoint->GetPosition().theta()/CLHEP::deg << ","
		 << endPoint->GetPosition().phi()/CLHEP::deg << ") [deg]"
		 << G4endl;
	  */
	  analysisManager->FillH1(1, trackID);
	  analysisManager->FillH1(2, endPoint->GetMomentum().mag()/CLHEP::GeV);
	  analysisManager->FillH1(3, endPoint->GetPosition().theta()/CLHEP::deg);
	  analysisManager->FillH1(4, endPoint->GetPosition().phi()/CLHEP::deg);
	  G4double xval = endPoint->GetPosition().phi()/CLHEP::deg;
	  G4double yval = endPoint->GetPosition().theta()/CLHEP::deg;

	  analysisManager->FillH2(0, xval,  yval);
	  analysisManager->FillH2(1, endPoint->GetPosition().theta()/CLHEP::deg, endPoint->GetMomentum().mag()/CLHEP::GeV);
	  analysisManager->FillH2(2, endPoint->GetPosition().phi()/CLHEP::deg, endPoint->GetMomentum().mag()/CLHEP::GeV);


	}
	
      }
    }
    
    // incident primary particle - the most important one
    // we record its pre-step and post-step momentum
    // in our EventAction object
    if (aStep->GetTrack()->GetTrackID() == 1) {
        
        fEventAction    -> SetPrimaryPreStepMomentum ( p_start );
        fEventAction    -> SetPrimaryPostStepMomentum( p_end );
        fTrackingAction -> UpdateTrackInfo(ekin,trackl,time);
        
        //        G4AnalysisManager::Instance()->FillH1(7,ekin);
    }
    
    if (fdebug>1){
        
        const G4StepPoint* startPoint = aStep->GetPreStepPoint();
        process   = endPoint->GetProcessDefinedStep();
        G4LogicalVolume* lVolume = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
        
        
        std::cout
        << "track id: "<< aStep->GetTrack()->GetTrackID() << ", "
        << "particle: "<< aStep->GetTrack()->GetParticleDefinition ()->GetParticleName() << ", "
        << std::endl;
        
        std::cout << "process: " << process->GetProcessName() << std::endl;
        
        std::cout << ""      << "("
        << startPoint->GetPosition().x() << ","
        << startPoint->GetPosition().y() << ","
        << startPoint->GetPosition().z() << ")"
        << "->";
        
        std::cout << "("
        << endPoint->GetPosition().x() << ","
        << endPoint->GetPosition().y() << ","
        << endPoint->GetPosition().z() << ")"
        << ", step length: " << aStep->GetStepLength()/CLHEP::mm << " mm "
        << std::endl;
        
        G4double Ek_end     = endPoint->GetKineticEnergy();
        G4double Ek_start   = startPoint->GetKineticEnergy();
        G4double Edep = Ek_start - Ek_end;
        std::cout
        << "Ek_start: " << Ek_start/CLHEP::keV  << " keV, "
        << "Ek_end: " << Ek_end/CLHEP::keV      << " keV, "
        << "Edep: " << Edep/CLHEP::keV
        << " keV"
        << std::endl;

        
    }
    
    
    // record the track energy deposition
    // if the particle was knocked out by the primary neutron
    // (namely, if the track parent is the primary)
    //   In order to find the point spread function (PSF) I would like to calculate the radial distribution of the total absorbed proton energy for all neutrons.
    // As you know the range of few MeV protons in plastics can be significant(1-2mm), so one has to follow the proton track as it loses the energy within the slab.
    // I expect the FWHM of the deposited energy to be few hundreds microns. In addition neutron scattering by both H and C will further broaden the shoulders of the PSF. I think that 1M incident neutrons will be sufficient to provide the answer. Only radial distribution is of interest so the energy is integrated on the thickness z.
    //    if (aStep->GetTrack()->GetTrackID() == 1) return;
    
    
    auto theTrack   = aStep->GetTrack();
    auto pDef       = theTrack->GetParticleDefinition();
    auto PDG        = pDef->GetPDGEncoding();
    G4bool isProton             = (PDG == 2212);
    G4bool KnockedOutByPrimary  = (theTrack->GetParentID() == 1);
    G4bool isFirstKnockout      = (theTrack->GetTrackID() == 2);
    
    // is in scintillator at the start of the step?
    G4LogicalVolume* lVolume    = startPoint->GetTouchableHandle() ->GetVolume()->GetLogicalVolume();
    if (fdebug>2) std::cout << "G4bool isInScintillator     = FALSE;" << std::endl;
    G4bool isInScintillator     = FALSE;
//    if (lVolume == fDetector->GetLogicScintillator()) isInScintillator = TRUE;
    if (fdebug>2) std::cout << "isInScintillator: " << isInScintillator << std::endl;
    
    
    if ( KnockedOutByPrimary && isFirstKnockout && isInScintillator ){
        // the fulfillement of this if statement is unique to the first proton that was knocked out by the neutron,
        // if its the first particle that was knocked out by the neutron
        // end point radial distance from center
        
        G4int eventNumber = G4RunManager::GetRunManager()-> GetCurrentEvent()->GetEventID();
        
        // distance from the center is defined as the perpendicular radial distance of the average positions before and after the step
        G4double x_i        = startPoint -> GetPosition().x();
        G4double y_i        = startPoint -> GetPosition().y();
        G4double x_f        = endPoint -> GetPosition().x();
        G4double y_f        = endPoint -> GetPosition().y();
        G4double phi_f      = endPoint -> GetPosition().phi();
        G4double theta_f    = endPoint -> GetPosition().theta();
        G4double x          = (x_i+x_f)/2;
        G4double y          = (y_i+y_f)/2;
        G4double Ek_end     = endPoint->GetKineticEnergy();
        G4double Ek_start   = startPoint->GetKineticEnergy();
        G4double Edep       = Ek_start - Ek_end;
        
    }
    
    if (fdebug>1){
        std::cout
        << "done SteppingAction::UserSteppingAction(const G4Step* aStep) "
        << std::endl
        << "------------------------------------------------------------------"
        << std::endl;
    }
}
