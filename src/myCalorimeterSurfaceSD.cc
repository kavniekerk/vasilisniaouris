
#include "myCalorimeterSurfaceSD.hh"
#include "myCalorimeterSurfaceHit.hh" //create this
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"

#include "G4Track.hh"
#include "myAnalysis.hh"
#include "G4VProcess.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myCalorimeterSurfaceSD::myCalorimeterSurfaceSD(G4String name)
: G4VSensitiveDetector(name),
  fCalorimeterSurfaceCollection(0)
{
  G4String HCname;
  collectionName.insert(HCname="calorimeterSurfaceCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myCalorimeterSurfaceSD::~myCalorimeterSurfaceSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myCalorimeterSurfaceSD::Initialize(G4HCofThisEvent* HCE)
{
  static int HCID = -1;
  fCalorimeterSurfaceCollection = new myCalorimeterSurfaceHitsCollection
                      (SensitiveDetectorName, collectionName[0]);
  if(HCID<0)
    HCID = GetCollectionID(0);
  HCE->AddHitsCollection(HCID,fCalorimeterSurfaceCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool myCalorimeterSurfaceSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    G4Track * theTrack = aStep->GetTrack();
    if (theTrack->GetParticleDefinition()->GetParticleName()=="opticalphoton") return false;
    if (!(aStep->IsFirstStepInVolume())) return false;

    G4int ParticlePDG = theTrack->GetParticleDefinition()->GetPDGEncoding();
    myCalorimeterSurfaceHit* newHit = new myCalorimeterSurfaceHit();
    newHit->SetEn( aStep->GetPreStepPoint()->GetTotalEnergy() );//theTrack->GetDynamicParticle()->GetTotalEnergy() - aStep->GetPreStepPoint()->GetTotalEnergyDeposit() );
    newHit->SetPos( aStep->GetPreStepPoint()->GetPosition() );
    newHit->SetTime( aStep->GetPreStepPoint()->GetGlobalTime() );
    newHit->SetPDG(ParticlePDG);
    G4int ParentID = theTrack->GetParentID();
    G4int TrackID = theTrack->GetTrackID();
    newHit->SetParID(ParentID);
    newHit->SetTrID(TrackID);
    //   G4cout<<theTrack->GetParticleDefinition()->GetParticleName()<<G4endl;


    fCalorimeterSurfaceCollection->insert( newHit );
    // delete[] newHit;
    return true;


//return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myCalorimeterSurfaceSD::EndOfEvent(G4HCofThisEvent*)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myCalorimeterSurfaceSD::clear()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myCalorimeterSurfaceSD::DrawAll()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myCalorimeterSurfaceSD::PrintAll()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
