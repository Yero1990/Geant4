/// \file hadronic/Hadr02/include/HistoManager.hh
#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"
#include "G4Material.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class Histo;
class G4Track;
class G4Step;
class G4ParticleDefinition;
class G4VModularPhysicsList;
class G4VPhysicsConstructor;

class HistoManager
{
public:
    int fdebug = 0;
    static HistoManager* GetPointer();
    
    ~HistoManager();
    
    void BookHisto();
    
    void BeginOfRun();
    void EndOfRun();
    void BeginOfEvent();
    void EndOfEvent();
    void Fill(G4int id, G4double x, G4double w);
    
    void ScoreNewTrack(const G4Track*);
    void AddTargetStep(const G4Step*);
    
    void SetVerbose(G4int val);
    void SetIonPhysics(const G4String&);
    
    inline void SetTargetMaterial(const G4Material* mat) {fMaterial = mat;};
    inline void SetTargetLength(G4double val)            {fLength  = val;};
    inline void SetNumberOfSlices(G4int val)             {fNSlices = val;};
    inline void SetNumberOfBinsE(G4int val)              {fNBinsE  = val;};
    
    inline G4double Length()         const               {return fLength;};
    inline G4int    NumberOfSlices() const               {return fNSlices;};
    
    inline G4int GetVerbose() const                      {return fVerbose;};
    
    inline void SetDefaultBeamPositionFlag(G4bool f)     {fBeamFlag = f;};
    inline G4bool DefaultBeamPosition() const            {return fBeamFlag;};
    
    inline void SetMaxEnergyDeposit(G4double val)        {fEdepMax = val;};
    
    inline void SetPhysicsList(G4VModularPhysicsList* p) {fPhysList = p;};
    
    
    void PrintAverageNumberOfIons(G4double Normalisation, G4double Nevents, G4String ionName, G4int precision=5);
private:
    HistoManager();
    
    void Initialise();
    
    static HistoManager* fManager;
    
    G4VModularPhysicsList*      fPhysList;
    G4VPhysicsConstructor*      fIonPhysics;
    const G4ParticleDefinition* fPrimaryDef;
    const G4ParticleDefinition* fNeutron;
    const G4Material*           fMaterial;
    
    G4double fEdepMax;
    G4double fEdepEvt;
    G4double fEdepSum;
    G4double fEdepSum2;
    G4double fLength;
    G4double fAbsZ0;
    G4double fPrimaryKineticEnergy;
    
    G4int fVerbose;
    G4int fNBinsE;
    G4int fNSlices;
    
    G4int fNevt;
    G4int fNelec;
    G4int fNposit;
    G4int fNgam;
    G4int fNcpions;
    G4int fNpi0;
    G4int fNkaons;
    G4int fNmuons;
    G4int fNions;
    G4int fNdeut;
    G4int fNalpha;
    G4int fNneutron;
    G4int fNproton;
    G4int fNaproton;
    G4int fNstep;
    G4int fNHisto;
    
    
    G4int fN_2H;
    G4int fN_3H;
    G4int fN_3He;
    G4int fN_4He;
    G4int fN_5He;
    G4int fN_6Li;
    G4int fN_7Li;
    G4int fN_8Be;
    G4int fN_9Be;
    G4int fN_10B;
    G4int fN_10Be;
    G4int fN_11B;
    G4int fN_11Be;
    
    G4bool fBeamFlag;
    
    Histo* fHisto;
};

#endif
