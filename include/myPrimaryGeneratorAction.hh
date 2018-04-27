
#ifndef myPrimaryGeneratorAction_h
#define myPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

class G4ParticleGun;
class G4Event;
class G4Box;

class myPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    myPrimaryGeneratorAction();
    virtual ~myPrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);

    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }


    G4double GetBeamX() {return BeamX;}
    G4double GetBeamY() {return BeamY;}

    const std::string fParticleName="e-";
    const G4double fParticleEnergy = 30.*GeV;

  private:
    G4ParticleGun*  fParticleGun; // pointer a to G4 gun class

    G4double BeamX;
    G4double BeamY;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
