/// \file Run.hh
/// \brief Definition of the Run class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef Run_h
#define Run_h 1

#include "G4Run.hh"
#include "G4VProcess.hh"
#include "globals.hh"
#include <map>
#include <stdio.h>

class DetectorConstruction;
class G4ParticleDefinition;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Run : public G4Run
{
public:
    
    Run(DetectorConstruction*);
    ~Run();
    
public:
    void CountProcesses(const G4VProcess* process);
    void ParticleCount(G4String, G4double);
    void SumTrackLength (G4int,G4int,G4double,G4double,G4double,G4double);
    
    void SetPrimary(G4ParticleDefinition* particle, G4double energy);
    void EndOfRun();
    
    virtual void Merge(const G4Run*);
    

private:
    struct ParticleData {
        ParticleData()
        : fCount(0), fEmean(0.), fEmin(0.), fEmax(0.) {}
        ParticleData(G4int count, G4double ekin, G4double emin, G4double emax)
        : fCount(count), fEmean(ekin), fEmin(emin), fEmax(emax) {}
        G4int     fCount;
        G4double  fEmean;
        G4double  fEmin;
        G4double  fEmax;
    };
    
    
    
    
    
private:
    DetectorConstruction* fDetector;
    G4ParticleDefinition* fParticle;
    G4double              fEkin;
    
    std::map<G4String,G4int>        fProcCounter;
    std::map<G4String,ParticleData> fParticleDataMap;
    
    G4int    fNbStep1, fNbStep2;
    G4double fTrackLen1, fTrackLen2;
    G4double fTime1, fTime2;
    
    

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

