/// \file hadronic/Hadr02/src/HistoManager.cc
/// \brief Implementation of the HistoManager class
//
//
//---------------------------------------------------------------------------
//
// ClassName:   HistoManager
//
//
// Author:      V.Ivanchenko 30/01/01
//
// Modified:
// 04.06.2006 Adoptation of hadr01 (V.Ivanchenko)
// 16.11.2006 Add beamFlag (V.Ivanchenko)
// 16.10.2012 Renamed G4IonFTFPBinaryCascadePhysics as G4IonPhysics (A.Ribon)
//
//----------------------------------------------------------------------------
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "HistoManager.hh"
#include "G4UnitsTable.hh"
#include "G4Neutron.hh"
#include "G4Proton.hh"
#include "G4AntiProton.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4PionPlus.hh"
#include "G4PionMinus.hh"
#include "G4PionZero.hh"
#include "G4KaonPlus.hh"
#include "G4KaonMinus.hh"
#include "G4KaonZeroShort.hh"
#include "G4KaonZeroLong.hh"
#include "G4Deuteron.hh"
#include "G4Triton.hh"
#include "G4He3.hh"
#include "G4Alpha.hh"
#include "Histo.hh"
#include "globals.hh"
#include "G4VModularPhysicsList.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4RunManager.hh"
#include "DetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4BuilderType.hh"
#include "G4SystemOfUnits.hh"

#include "IonUrQMDPhysics.hh"
#include "IonHIJINGPhysics.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

HistoManager* HistoManager::fManager = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

HistoManager* HistoManager::GetPointer()
{
    if(!fManager) {
        static HistoManager manager;
        fManager = &manager;
    }
    return fManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

HistoManager::HistoManager()
{
    fVerbose=  0;
    fNSlices   = 100;
    fNBinsE    = 100;
    fNHisto    = 20;
    fLength    = 300.*mm;
    fEdepMax   = 1.0*GeV;
    
    fPrimaryDef= 0;
    fPrimaryKineticEnergy = 0.0;
    fMaterial  = 0;
    fBeamFlag  = true;
    
    fHisto     = new Histo();
    fHisto->SetVerbose(fVerbose);
    fNeutron   = G4Neutron::Neutron();
    fPhysList  = 0;
    fIonPhysics= 0;
    Initialise();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HistoManager::Initialise()
{
    fAbsZ0       = -0.5*fLength;
    fNevt       = 0;
    fNelec      = 0;
    fNposit     = 0;
    fNgam       = 0;
    fNstep      = 0;
    fNions      = 0;
    fNdeut      = 0;
    fNalpha     = 0;
    fNkaons     = 0;
    fNmuons     = 0;
    fNcpions    = 0;
    fNpi0       = 0;
    fNneutron   = 0;
    fNproton    = 0;
    fNaproton   = 0;
    
    fEdepEvt    = 0.0;
    fEdepSum    = 0.0;
    fEdepSum2   = 0.0;
    
    
    // Erez
    fN_2H   = 0;
    fN_3H   = 0;
    fN_3He  = 0;
    fN_4He  = 0;
    fN_5He  = 0;
    fN_6Li  = 0;
    fN_7Li  = 0;
    fN_8Be  = 0;
    fN_9Be  = 0;
    fN_10B  = 0;
    fN_10Be = 0;
    fN_11B  = 0;
    fN_11Be = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

HistoManager::~HistoManager()
{
    delete fHisto;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HistoManager::BookHisto(){
    fHisto->Add1D("0","Energy deposition (MeV/mm/event) in the target",
                  fNSlices,0.0,fLength/mm,MeV/mm);
    fHisto->Add1D("1","Log10 Energy (GeV) of gammas",fNBinsE,-5.,5.,1.0);
    fHisto->Add1D("2","Log10 Energy (GeV) of electrons",fNBinsE,-5.,5.,1.0);
    fHisto->Add1D("3","Log10 Energy (GeV) of positrons",fNBinsE,-5.,5.,1.0);
    fHisto->Add1D("4","Log10 Energy (GeV) of protons",fNBinsE,-5.,5.,1.0);
    fHisto->Add1D("5","Log10 Energy (GeV) of neutrons",fNBinsE,-5.,5.,1.0);
    fHisto->Add1D("6","Log10 Energy (GeV) of charged pions",fNBinsE,-4.,6.,1.0);
    fHisto->Add1D("7","Log10 Energy (GeV) of pi0",fNBinsE,-4.,6.,1.0);
    fHisto->Add1D("8","Log10 Energy (GeV) of charged kaons",fNBinsE,-4.,6.,1.0);
    fHisto->Add1D("9","Log10 Energy (GeV) of neutral kaons",fNBinsE,-4.,6.,1.0);
    fHisto->Add1D("10","Log10 Energy (GeV) of deuterons and tritons",
                  fNBinsE,-5.,5.,1.0);
    fHisto->Add1D("11","Log10 Energy (GeV) of He3 and alpha",fNBinsE,-5.,5.,1.0);
    fHisto->Add1D("12","Log10 Energy (GeV) of Generic Ions",fNBinsE,-5.,5.,1.0);
    fHisto->Add1D("13","Log10 Energy (GeV) of muons",fNBinsE,-4.,6.,1.0);
    fHisto->Add1D("14","Log10 Energy (GeV) of pi+",fNBinsE,-4.,6.,1.0);
    fHisto->Add1D("15","Log10 Energy (GeV) of pi-",fNBinsE,-4.,6.,1.0);
    fHisto->Add1D("16","X Section (mb) of Secondary Fragments Z with E>1 GeV (mb)"
                  ,25,0.5,25.5,1.0);
    fHisto->Add1D("17","Secondary Fragment A E>1 GeV",50,0.5,50.5,1.0);
    fHisto->Add1D("18","Secondary Fragment Z E<1 GeV",25,0.5,25.5,1.0);
    fHisto->Add1D("19","Secondary Fragment A E<1 GeV",50,0.5,50.5,1.0);
    fHisto->Add1D("20","X Section (mb) of Secondary Fragments Z (mb) ",
                  25,0.5,25.5,1.0);
    fHisto->Add1D("21","Secondary Fragment A ",50,0.5,50.5,1.0);
    
    
    fHisto->Add1D("21","Secondary Fragment A ",50,0.5,50.5,1.0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void HistoManager::BeginOfRun(){
    Initialise();
    BookHisto();
    fHisto->Book();
    
    if(fVerbose > 0) {
        G4cout << "HistoManager: Histograms are booked and run has been started"
        <<G4endl<<"  BeginOfRun (After fHisto->book)"<< G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void HistoManager::EndOfRun(){
    G4bool DoPrints = FALSE;
    if (fdebug>2) DoPrints = TRUE;
    if (DoPrints) {
        G4cout << "HistoManager: End of run actions are started" << G4endl;
        
        // Average values
        G4cout<<"========================================================"<<G4endl;
    }
    G4double x = (G4double)fNevt;
    if(fNevt > 0) { x = 1.0/x; }
    
    G4double xe = x*fNelec;
    G4double xg = x*fNgam;
    G4double xp = x*fNposit;
    G4double xs = x*fNstep;
    G4double xn = x*fNneutron;
    G4double xpn = x*fNproton;
    G4double xap = x*fNaproton;
    G4double xpc = x*fNcpions;
    G4double xp0 = x*fNpi0;
    G4double xpk = x*fNkaons;
    G4double xpm = x*fNmuons;
    G4double xid = x*fNdeut;
    G4double xia = x*fNalpha;
    G4double xio = x*fNions;
    
    
    
    
    fEdepSum  *= x;
    fEdepSum2 *= x;
    fEdepSum2 -= fEdepSum*fEdepSum;
    if(fEdepSum2 > 0.0) fEdepSum2 = std::sqrt(fEdepSum2);
    else               fEdepSum2 = 0.0;
    
    if (DoPrints){
        G4cout                         << "Beam particle                        "
        << fPrimaryDef->GetParticleName() <<G4endl;
        G4cout                         << "Beam Energy(GeV)                     "
        << fPrimaryKineticEnergy/GeV <<G4endl;
        G4cout                         << "Number of events                     "
        << fNevt <<G4endl;
        G4cout << std::setprecision(4) << "Average energy deposit (GeV)         "
        << fEdepSum/GeV
        << "   RMS(GeV) " << fEdepSum2/GeV << G4endl;
        
        std::cout<<"-------------------------------------------------------"<<std::endl;
        
        //  G4cout << std::setprecision(4) << "Average number of steps              "
        // << xs << G4endl;
        PrintAverageNumberOfIons( x, fNgam,    "gamma   ");
        PrintAverageNumberOfIons( x, fNelec,   "e-      ");
        PrintAverageNumberOfIons( x, fNposit,  "e+      ");
        PrintAverageNumberOfIons( x, fNneutron,"neutrons");
        PrintAverageNumberOfIons( x, fNproton, "protons ");
        PrintAverageNumberOfIons( x, fNcpions, "pi+/-   ");
        PrintAverageNumberOfIons( x, fNpi0,    "pi0     ");
        PrintAverageNumberOfIons( x, fNkaons,  "kaons   ");
        PrintAverageNumberOfIons( x, fNmuons,  "muons   ");
        PrintAverageNumberOfIons( x, fNions,   "ions    ");
        
        std::cout<<"-------------------------------------------------------"<<std::endl;
        
        PrintAverageNumberOfIons( x, fN_2H,     "2H     ");
        PrintAverageNumberOfIons( x, fN_3H,     "3H     ");
        PrintAverageNumberOfIons( x, fN_3He,    "3He    ");
        PrintAverageNumberOfIons( x, fN_4He,    "4He    ");
        PrintAverageNumberOfIons( x, fN_5He,    "5He    ");
        PrintAverageNumberOfIons( x, fN_6Li,    "6Li    ");
        PrintAverageNumberOfIons( x, fN_7Li,    "7Li    ");
        PrintAverageNumberOfIons( x, fN_8Be,    "8Be    ");
        PrintAverageNumberOfIons( x, fN_9Be,    "9Be    ");
        PrintAverageNumberOfIons( x, fN_10B,    "10B    ");
        PrintAverageNumberOfIons( x, fN_10Be,   "10Be   ");
        PrintAverageNumberOfIons( x, fN_11B,    "11B    ");
        PrintAverageNumberOfIons( x, fN_11Be,   "11Be   ");
        
        G4cout<<"========================================================"<<G4endl;
        G4cout<<G4endl;
    }
    // normalise histograms
    for(G4int i=0; i<fNHisto; i++) {
        fHisto->ScaleH1(i,x);
    }
    // will work only for pure material - 1 element
    //  G4cout << fMaterial << G4endl;
    G4double F= 1000/(fLength*barn*fMaterial->GetTotNbOfAtomsPerVolume());
    if (DoPrints) {std::cout << "F: " << F << std::endl;}
    if(F > 0.0) {
        fHisto->ScaleH1(16, F);
        fHisto->ScaleH1(20, F);
    }
    // std::cout << "fHisto->Save();" << std::endl;
    fHisto->Save();
    // std::cout << "done HistoManager::EndOfRun();" << std::endl;
    
    //    G4cout << std::setprecision(4) << "Average number of gamma              "
    //    << xg << G4endl;
    //    G4cout << std::setprecision(4) << "Average number of e-                 "
    //    << xe << G4endl;
    //    G4cout << std::setprecision(4) << "Average number of e+                 "
    //    << xp << G4endl;
    //    G4cout << std::setprecision(4) << "Average number of neutrons           "
    //    << xn << G4endl;
    //    G4cout << std::setprecision(4) << "Average number of protons            "
    //    << xpn << G4endl;
    //    //    G4cout << std::setprecision(4) << "Average number of antiprotons        "
    //    //    << xap << G4endl;
    //    G4cout << std::setprecision(4) << "Average number of pi+ & pi-          "
    //    << xpc << G4endl;
    //    G4cout << std::setprecision(4) << "Average number of pi0                "
    //    << xp0 << G4endl;
    //    G4cout << std::setprecision(4) << "Average number of kaons              "
    //    << xpk << G4endl;
    //    G4cout << std::setprecision(4) << "Average number of muons              "
    //    << xpm << G4endl;
    //    G4cout << std::setprecision(4) << "Average number of deuterons+tritons  "
    //    << xid << G4endl;
    //    G4cout << std::setprecision(4) << "Average number of He3+alpha          "
    //    << xia << G4endl;
    //    G4cout << std::setprecision(4) << "Average number of ions               "
    //    << xio << G4endl;
    
}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void HistoManager::PrintAverageNumberOfIons( G4double Normalisation, G4double Nevents, G4String ionName, G4int precision){
    
    G4double fraction = Normalisation * Nevents;
    // Poisson uncertainty, assuming that the total number of events is large and so fluctuations in Normalisation is small
    G4double fraction_err = Normalisation * sqrt(Nevents);
    
    std::cout << std::setprecision(precision)
    << "Average number of "
    << ionName << "     "
    << fraction
    << "+/-"
    << fraction_err
    << std::endl;
}





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
void HistoManager::BeginOfEvent(){
    ++fNevt;
    fEdepEvt = 0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HistoManager::EndOfEvent(){
    fEdepSum  += fEdepEvt;
    fEdepSum2 += fEdepEvt*fEdepEvt;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HistoManager::ScoreNewTrack(const G4Track* track){
    int fdebug = 0;
    if (fdebug) std::cout << "HistoManager::ScoreNewTrack(const G4Track* track)" << std::endl;
    const G4ParticleDefinition* pd = track->GetDefinition();
    G4String name = pd->GetParticleName();
    G4double e = track->GetKineticEnergy();
    
    
    
    
    // Primary track
    if(0 == track->GetParentID()) {
        
        fPrimaryKineticEnergy = e;
        fPrimaryDef = pd;
        
        
        G4ThreeVector dir = track->GetMomentumDirection();
        if(1 < fVerbose)
            G4cout << "### Primary " << name
            << " kinE(GeV)= " << e/GeV
            << "; m(GeV)= " << pd->GetPDGMass()/GeV
            << "; pos(mm)= " << track->GetPosition()/mm
            << ";  dir= " << track->GetMomentumDirection()
            << G4endl;
        
        
    }
    
    // Secondary tracks
    if(0 != track->GetParentID()) {
        
        G4int PDG = pd->GetPDGEncoding();
        // fHisto->Fill(PDG,e,1.0);
        
        if (fdebug>1){
            std::cout
            << "HistoManager::ScoreNewTrack(const G4Track* track): secondary"
            << std::endl
            << "track id: "<< track->GetTrackID() << ", "
            << "parent id: " << track->GetParentID() << ", "
            << "particle: "<< pd->GetParticleName() << ", (" << pd ->GetPDGEncoding() << "), "
            << std::endl;
        }
        
        if(1 < fVerbose) {
            
            G4cout << "=== Secondary " << name
            << " kinE(GeV)= " << e/GeV
            << "; m(GeV)= " << pd->GetPDGMass()/GeV
            << "; pos(mm)= " << track->GetPosition()/mm
            << ";  dir= " << track->GetMomentumDirection()
            << G4endl;
        }
        e = std::log10(e/GeV);
        if(pd == G4Gamma::Gamma()) {
            fNgam++;
            fHisto->Fill(1,e,1.0);
        } else if ( pd == G4Electron::Electron()) {
            fNelec++;
            fHisto->Fill(2,e,1.0);
        } else if ( pd == G4Positron::Positron()) {
            fNposit++;
            fHisto->Fill(3,e,1.0);
        } else if ( pd == G4Proton::Proton()) {
            fNproton++;
            fHisto->Fill(4,e,1.0);
        } else if ( pd == fNeutron) {
            fNneutron++;
            fHisto->Fill(5,e,1.0);
        } else if ( pd == G4AntiProton::AntiProton()) {
            fNaproton++;
        } else if ( pd == G4PionPlus::PionPlus() ) {
            fNcpions++;
            fHisto->Fill(6,e,1.0);
            fHisto->Fill(14,e,1.0);
            
        } else if ( pd == G4PionMinus::PionMinus()) {
            fNcpions++;
            fHisto->Fill(6,e,1.0);
            fHisto->Fill(15,e,1.0);
            
        } else if ( pd == G4PionZero::PionZero()) {
            fNpi0++;
            fHisto->Fill(7,e,1.0);
        } else if ( pd == G4KaonPlus::KaonPlus() ||
                   pd == G4KaonMinus::KaonMinus()) {
            fNkaons++;
            fHisto->Fill(8,e,1.0);
        } else if ( pd == G4KaonZeroShort::KaonZeroShort() ||
                   pd == G4KaonZeroLong::KaonZeroLong()) {
            fNkaons++;
            fHisto->Fill(9,e,1.0);
        } else if ( pd == G4Deuteron::Deuteron() || pd == G4Triton::Triton()) {
            fNdeut++;
            fHisto->Fill(10,e,1.0);
            
            // Erez
            if (pd == G4Deuteron::Deuteron()){
                fN_2H ++;
            }
            if (pd == G4Triton::Triton()){
                fN_3H ++;
            }
            
        } else if ( pd == G4He3::He3() || pd == G4Alpha::Alpha()) {
            fNalpha++;
            fHisto->Fill(11,e,1.0);
            
            
            //Erez
            if (pd == G4He3::He3()){
                fN_3He ++;
            }
            if (pd == G4Alpha::Alpha()){
                fN_4He ++;
            }
            
        } else if ( pd->GetParticleType() == "nucleus") {
            
            fNions++;
            fHisto->Fill(12,e,1.0);
            G4double Z = pd->GetPDGCharge()/eplus;
            G4double A = (G4double)pd->GetBaryonNumber();
            if(e > 0.0) {
                fHisto->Fill(16,Z,1.0);
                fHisto->Fill(17,A,1.0);
            } else {
                fHisto->Fill(18,Z,1.0);
                fHisto->Fill(19,A,1.0);
            }
            fHisto->Fill(20,Z,1.0);
            fHisto->Fill(21,A,1.0);
            
            
            
            //Erez
            if (name == "He5"){
                fN_5He ++;
            } else if (name == "Li6"){
                fN_6Li ++;
            } else if (name == "Li7"){
                fN_7Li ++;
            } else if (name == "Be8"){
                fN_8Be ++;
            } else if (name == "Be9"){
                fN_9Be ++;
            } else if (name == "B10"){
                fN_10B ++;
            } else if (name == "Be10"){
                fN_10Be ++;
            } else if (name == "B11"){
                fN_11Be ++;
            } else if (name == "Be11"){
                fN_11Be ++;
            }
            
            
            
        } else if ( pd == G4MuonPlus::MuonPlus() ||
                   pd == G4MuonMinus::MuonMinus()) {
            fNmuons++;
            fHisto->Fill(13,e,1.0);
        }
    }
    if (fdebug) std::cout << "done HistoManager::ScoreNewTrack(const G4Track* track)" << std::endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::AddTargetStep(const G4Step* step)
{
    ++fNstep;
    G4double edep = step->GetTotalEnergyDeposit();
    
    if(edep > 0.0) {
        G4ThreeVector pos =
        (step->GetPreStepPoint()->GetPosition() +
         step->GetPostStepPoint()->GetPosition())*0.5;
        
        G4double z = pos.z() - fAbsZ0;
        
        // scoring
        fEdepEvt += edep;
        fHisto->Fill(0,z,edep);
        
        if(1 < fVerbose) {
            const G4Track* track = step->GetTrack();
            G4cout << "HistoManager::AddEnergy: e(keV)= " << edep/keV
            << "; z(mm)= " << z/mm
            << "; step(mm)= " << step->GetStepLength()/mm
            << " by " << track->GetDefinition()->GetParticleName()
            << " E(GeV)= " << track->GetKineticEnergy()/GeV
            << G4endl;
        }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HistoManager::SetVerbose(G4int val)        
{
    fVerbose = val;
    fHisto->SetVerbose(val);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HistoManager::Fill(G4int id, G4double x, G4double w)
{
    fHisto->Fill(id, x, w);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HistoManager::SetIonPhysics(const G4String& nam)
{
    if(fIonPhysics) {
        G4cout << "### HistoManager WARNING: Ion Physics is already defined: <"
        << nam << "> is ignored!" << G4endl;
    } else if(nam == "HIJING") {
#ifdef G4_USE_HIJING
        fIonPhysics = new IonHIJINGPhysics();
        fPhysList->ReplacePhysics(fIonPhysics);
        G4RunManager::GetRunManager()->PhysicsHasBeenModified();
        G4cout << "### SetIonPhysics: Ion Physics FTFP/Binary is added"
        << G4endl;
#else
        G4cout << "Error: Ion Physics HIJING is requested but is not available"
        <<G4endl;
#endif
    } else if(nam == "UrQMD") {
#ifdef G4_USE_URQMD
        fIonPhysics = new IonUrQMDPhysics(1);
        fPhysList->ReplacePhysics(fIonPhysics);
        G4RunManager::GetRunManager()->PhysicsHasBeenModified();
        G4cout << "### SetIonPhysics: Ion Physics UrQMD is added"
        << G4endl;
#else
        G4cout << "Error: Ion Physics UrQMD is requested but is not available"
        <<G4endl;
#endif
    } else {
        G4cout << "### HistoManager WARNING: Ion Physics <"
        << nam << "> is unknown!" << G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

