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
#include <math.h>

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

    int fdebug=0;

     
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

 
    if (fdebug>1){
        std::cout <<"SteppingAction::UserSteppingAction(const G4Step* aStep) " << std::endl;
    }


    G4int evNum = G4RunManager::GetRunManager()-> GetCurrentEvent()->GetEventID();

    
    
    // Get the variables for pre- (start of step), along and post (end of step) for each aStep
    const G4StepPoint* startPoint = aStep->GetPreStepPoint();
    const G4StepPoint* endPoint   = aStep->GetPostStepPoint();
    
    const G4VProcess* process   = endPoint->GetProcessDefinedStep();

    
    // get particle position at the (start, end) of each step
    G4double pos_xi  = startPoint->GetPosition().x()/CLHEP::mm;               // particle x-comp. of 3-position at start of step
    G4double pos_yi  = startPoint->GetPosition().y()/CLHEP::mm;               // particle y-comp. of 3-position at start of step
    G4double pos_zi  = startPoint->GetPosition().z()/CLHEP::mm;               // particle z-comp. 3-position at start of step
    G4double pos_i   = startPoint->GetPosition().mag()/CLHEP::mm;             // particle magnitude of 3-position at start of step

    G4double pos_xf  = endPoint->GetPosition().x()/CLHEP::mm;                 // particle 3-position at end of step
    G4double pos_yf  = endPoint->GetPosition().y()/CLHEP::mm;                 // particle 3-position at end of step
    G4double pos_zf  = endPoint->GetPosition().z()/CLHEP::mm;                 // particle 3-position at end of step
    G4double pos_f   = endPoint->GetPosition().mag()/CLHEP::mm;               // particle magnitude of 3-position at end of step

    // get particle momenta at the (start, end) of each step
    G4double p_xi  = startPoint->GetMomentum().x()/CLHEP::GeV;                 // particle x-comp. of 3-momentum at start of step
    G4double p_yi  = startPoint->GetMomentum().y()/CLHEP::GeV;                 // particle y-comp. of 3-momentum at start of step
    G4double p_zi  = startPoint->GetMomentum().z()/CLHEP::GeV;                 // particle z-comp. 3-momentum at start of step
    G4double p_i   = startPoint->GetMomentum().mag()/CLHEP::GeV;               // particle magnitude of 3-momentum at start of step

    G4double p_xf  = endPoint->GetMomentum().x()/CLHEP::GeV;                 // particle 3-momentum at end of step
    G4double p_yf  = endPoint->GetMomentum().y()/CLHEP::GeV;                 // particle 3-momentum at end of step
    G4double p_zf  = endPoint->GetMomentum().z()/CLHEP::GeV;                 // particle 3-momentum at end of step
    G4double p_f   = endPoint->GetMomentum().mag()/CLHEP::GeV;               // particle magnitude of 3-momentum at end of step

    // get particle local time and star/end of each step
    G4double ti_local   = startPoint->GetLocalTime()/CLHEP::ns;             // particle magnitude of 3-position at start of step
    G4double tf_local   = endPoint->GetLocalTime()/CLHEP::ns;             // particle magnitude of 3-position at start of step

    // get other particle step information
    G4double ekin_start    = startPoint->GetKineticEnergy()/CLHEP::MeV;           // particle kinetic energy at end of each step
    G4double ekin_end      = endPoint->GetKineticEnergy()/CLHEP::MeV;           // particle kinetic energy at end of each step

    G4double phi_i         = startPoint->GetPosition().phi()/CLHEP::deg;          // particle end-step out-of-plane azimuth angle [-180, 180] deg (this will need to be converted to [0,360] deg later
    G4double theta_i       = startPoint->GetPosition().theta()/CLHEP::deg;        // particle end-step in-plane "polar" angle
    G4double phi_i_v2      = phi_i + ceil(-phi_i/360)*360;           //particle azimuthal angle [0,360] deg
    
    G4double phi_f             = endPoint->GetPosition().phi()/CLHEP::deg;          // particle end-step out-of-plane azimuth angle [-180, 180] deg (this will need to be converted to [0,360] deg later
    G4double theta_f           = endPoint->GetPosition().theta()/CLHEP::deg;        // particle end-step in-plane "polar" angle
    G4double phi_f_v2          = phi_f + ceil(-phi_f/360.)*360.;           //particle azimuthal angle [0,360] deg
    G4double track_length      = aStep->GetTrack()->GetTrackLength()/CLHEP::mm;     // cumulative particle track length up to and including aStep
    G4double track_time        = aStep->GetTrack()->GetLocalTime()/CLHEP::ns;       // cumulative particle time up to and including aStep
    G4double edep_total        = aStep->GetTotalEnergyDeposit()/CLHEP::MeV;         // total energy deposited in volume at each step (non-cumulative)
    G4double edep_nonionizing  = aStep->GetNonIonizingEnergyDeposit()/CLHEP::MeV;

    G4int    trackID       = aStep->GetTrack()->GetTrackID();                   // trackID of each step (trackID changes if particle identity is lost)
    

    G4String step_particle = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();  //get particle name participating along the step
    G4int post_step_status  = aStep->GetPostStepPoint()->GetStepStatus();  // get the step status at the end of the step (this is mainly used to check boundary conditions)

    
    G4bool pAtBoundary = false;


  
    // Define central angle/momentum for later calculation of momentum acceptance, and dtheta, dphi
    // Recall: delta = (P - P0)/P0 * 100 [%]  where P0 is the central momentum (i.e., the initial proton momentum)
    // dtheta = theta-theta0,  dphi = phi - phi0  | P0 = 1.325 * GeV, th0 = 66.9*deg, phi0 = 180.*deg
    
    G4double P0 = 1.325;
    G4double th0 = 66.9;
    G4double phi0 = 180.;
    
    G4double delta = (p_f - P0)/P0 * 100.;
    G4double dth = theta_f - th0;
    G4double dphi = phi_f_v2 - phi0;
    
	
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

    // set to true if step particle is a primary proton (originated at vertex) and has reached the geometry boundary 
    pAtBoundary = (step_particle=="proton") && (post_step_status == fGeomBoundary) && (trackID==1);


    if(step_particle=="proton") {


      
      
      if(fdebug==1){
	// These statements are printed to screen for educational/understanding GEANT4 step tracking purposes
	G4cout  << "" << G4endl;
	G4cout << "-----------------" << G4endl;
	G4cout << "Ev Num: " << evNum << G4endl;
	G4cout << "Step #: " << aStep->GetTrack()->GetCurrentStepNumber() << G4endl;
	G4cout << "-----------------" << G4endl;
	G4cout  << "" << G4endl;
	G4cout << "Step Track ID: " << trackID << G4endl;
	G4cout << "Step Particle: "<< step_particle << G4endl;
	G4cout << "Step Process: " << process->GetProcessName() << G4endl;
	G4cout << "Step Track Length [mm]: " << track_length  << G4endl;
	G4cout << "Step Time [ns]: "         << track_time  << G4endl;
	G4cout << "(ti_local, tf_local, dt_local) [ns]: "<< ti_local << ", " << tf_local << ", " << (tf_local-ti_local) <<  G4endl;
	G4cout << "Start Step Pos: (x,y,z)"      << "("
	       << pos_xi << ","
	       << pos_yi << ","
	       << pos_zi << ") [mm]"
	       << "->";
	G4cout << "End Step Pos: (x,y,z)" << "("
	       << pos_xf << ","
	       << pos_yf << ","
	       << pos_zf << ") [mm]"
	       << ", step length: " << aStep->GetStepLength()/CLHEP::mm << " mm "
	       << G4endl;
	G4cout << "Start Step Momentum: (Px,Py,Pz, P)"      << "("
	       << p_xi << ","
	       << p_yi << ","
	       << p_zi << ","
	       << p_i << ") [GeV/c]"
	       << "->";
	G4cout << "End Step Momentum: (Px,Py,Pz,P)" << "("
	       << p_xf << ","
	       << p_yf << ","
	       << p_zf << ","
	       << p_f << ") [GeV/c]"
	       << G4endl;
	G4cout << "Start Step (theta_i, phi_i, phi_i_v2): " << "("
	       << theta_i << ","
	       << phi_i << ","
	       << phi_i_v2 << ") [deg]"
	       << G4endl;
	G4cout << "End Step (theta_f, phi_f, phi_f_v2): " << "("
	       << theta_f << ","
	       << phi_f << ","
	       << phi_f_v2 << ") [deg]"
	       << G4endl;
	G4cout << "(ekin_start, ekin_end) [MeV]: " << ekin_start << ", " << ekin_end << G4endl;
	G4cout << "(edep_total, edep_noionizing) [MeV]: " << edep_total << ", " << edep_nonionizing << G4endl;
	G4cout << "momentum acceptance (delta) = " << delta << " % " << G4endl; 
	G4cout << "dtheta (th-th0) [deg] = " << dth << " [deg] " << G4endl; 
	G4cout << "dphi (phi-phi0) [deg] = " << dphi << " [deg] " << G4endl; 
	
      G4cout << "End Step @ Volume Boundary ?" << pAtBoundary << G4endl;
       
      }
    

      if(pAtBoundary) {

	
	// Fill 1D Histos
	analysisManager->FillH1(0, pAtBoundary);
	analysisManager->FillH1(1, trackID);
	analysisManager->FillH1(2, p_f);
	analysisManager->FillH1(3, theta_f);
	analysisManager->FillH1(4, phi_f);
	analysisManager->FillH1(5, phi_f_v2);
	analysisManager->FillH1(6, track_length);
	analysisManager->FillH1(7, track_time);
	//analysisManager->FillH1();
	  
	// Fill 2D Histos
	analysisManager->FillH2(0, phi_f, theta_f);
	analysisManager->FillH2(1, phi_f_v2, theta_f);
	
	analysisManager->FillH2(2, theta_f,  p_f);
	analysisManager->FillH2(3, phi_f,    p_f);
	analysisManager->FillH2(4, phi_f_v2, p_f);

	analysisManager->FillH2(5, dphi, dth);
	analysisManager->FillH2(6, dth, delta);
	analysisManager->FillH2(7, dphi, delta);
	
	//analysisManager->FillH2();
	
      } // end volume boundary condition

     
      
    } // end proton requirement condition


      
    // incident primary particle - the most important one
    // we record its pre-step and post-step momentum
    // in our EventAction object
    if (aStep->GetTrack()->GetTrackID() == 1) {
      
      fEventAction    -> SetPrimaryPreStepMomentum ( startPoint->GetPosition() );
      fEventAction    -> SetPrimaryPostStepMomentum( endPoint->GetPosition() );
      fTrackingAction -> UpdateTrackInfo(ekin_end,track_length,track_time);
        
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
    // I expect the FWHM of the deposited energy to be few hundreds microns. In addition neutron scattering by both H and C will further broaden the shoulders of the PSF.
    //I think that 1M incident neutrons will be sufficient to provide the answer. Only radial distribution is of interest so the energy is integrated on the thickness z.
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
