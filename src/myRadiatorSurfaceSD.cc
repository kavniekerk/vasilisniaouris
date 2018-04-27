
#include "myRadiatorSurfaceSD.hh"
#include "myRadiatorSurfaceHit.hh" //create this
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"

#include "G4Track.hh"
#include "myAnalysis.hh"
#include "G4VProcess.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myRadiatorSurfaceSD::myRadiatorSurfaceSD(G4String name)
: G4VSensitiveDetector(name),
  fRadiatorSurfaceCollection(0)
{
  G4String HCname;
  collectionName.insert(HCname="radiatorSurfaceCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myRadiatorSurfaceSD::~myRadiatorSurfaceSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myRadiatorSurfaceSD::Initialize(G4HCofThisEvent* HCE)
{
  static int HCID = -1;
  fRadiatorSurfaceCollection = new myRadiatorSurfaceHitsCollection
                      (SensitiveDetectorName, collectionName[0]);
  if(HCID<0)
    HCID = GetCollectionID(0);
  HCE->AddHitsCollection(HCID,fRadiatorSurfaceCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool myRadiatorSurfaceSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    G4Track * theTrack = aStep->GetTrack();
    if (theTrack->GetParticleDefinition()->GetParticleName()=="opticalphoton") return false;
    if (!(aStep->IsFirstStepInVolume())) return false;

    G4int ParticlePDG = theTrack->GetParticleDefinition()->GetPDGEncoding();
    myRadiatorSurfaceHit* newHit = new myRadiatorSurfaceHit();
    newHit->SetEn( aStep->GetPreStepPoint()->GetTotalEnergy() );//theTrack->GetDynamicParticle()->GetTotalEnergy() - aStep->GetPreStepPoint()->GetTotalEnergyDeposit() );
    newHit->SetPos( aStep->GetPreStepPoint()->GetPosition() );
    newHit->SetTime( aStep->GetPreStepPoint()->GetGlobalTime() );
    newHit->SetPDG(ParticlePDG);
    G4int ParentID = theTrack->GetParentID();
    G4int TrackID = theTrack->GetTrackID();
    newHit->SetParID(ParentID);
    newHit->SetTrID(TrackID);
    //   G4cout<<theTrack->GetParticleDefinition()->GetParticleName()<<G4endl;


    fRadiatorSurfaceCollection->insert( newHit );
    // delete newHit;
    return true;


//return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myRadiatorSurfaceSD::EndOfEvent(G4HCofThisEvent*)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myRadiatorSurfaceSD::clear()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myRadiatorSurfaceSD::DrawAll()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myRadiatorSurfaceSD::PrintAll()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
