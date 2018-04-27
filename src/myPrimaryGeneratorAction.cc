
#include "myPrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"

#include "myAnalysis.hh"
#include <cmath>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myPrimaryGeneratorAction::myPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(fParticleName);
    // = particleTable->FindParticle(particleName="mu-");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(fParticleEnergy);
  // fParticleGun->SetParticleEnergy(150.*GeV);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myPrimaryGeneratorAction::~myPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myPrimaryGeneratorAction::GeneratePrimaries(G4Event* evt)
{

    // G4double radius = 0.5;
    // BeamX = radius*(2.*G4UniformRand()-1.)*cm;
    // BeamY = radius*(2.*G4UniformRand()-1.)*cm;
    //
    // while(sqrt(BeamX*BeamX+BeamY*BeamY)>= radius*cm) {
    //         BeamX = radius*(2.*G4UniformRand()-1.)*cm;
    //         BeamY = radius*(2.*G4UniformRand()-1.)*cm;
    // }

    BeamX = 0;
    BeamY = 0;

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(0, BeamX/cm);
    analysisManager->FillNtupleDColumn(1, BeamY/cm);

    fParticleGun->SetParticlePosition(G4ThreeVector(BeamX, BeamY,-3.5*cm));
    // fParticleGun->SetParticlePosition(G4ThreeVector(BeamX, BeamY,0.*cm));

    fParticleGun->GeneratePrimaryVertex(evt);
    //G4cout<<BeamX/cm<<"\t"<<BeamY/cm<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
