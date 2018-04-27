
#include "myPhotocathodeSurfaceSD.hh"
#include "myPhotocathodeOtherHit.hh" //create this
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"

#include "G4Track.hh"
#include "myAnalysis.hh"
#include "G4VProcess.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myPhotocathodeSurfaceSD::myPhotocathodeSurfaceSD(G4String name)
: G4VSensitiveDetector(name),
  fPhotocathodeOtherCollection(0)
{
    G4String HCname2;
    collectionName.insert(HCname2="photocathodeOtherCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myPhotocathodeSurfaceSD::~myPhotocathodeSurfaceSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myPhotocathodeSurfaceSD::Initialize(G4HCofThisEvent* HCE)
{
    static int HCID = -1;
    fPhotocathodeOtherCollection = new myPhotocathodeOtherHitsCollection
                      (SensitiveDetectorName,collectionName[0]);
    // if(HCID<0)
    HCID = GetCollectionID(0);
    HCE->AddHitsCollection(HCID,fPhotocathodeOtherCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool myPhotocathodeSurfaceSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    // G4double charge = aStep->GetPreStepPoint()->GetCharge();
    G4Track * theTrack = aStep->GetTrack();
    G4int ParticlePDG = theTrack->GetParticleDefinition()->GetPDGEncoding();

    if(theTrack->GetParticleDefinition()->GetParticleName()!="opticalphoton") {

        if (!(aStep->IsFirstStepInVolume())) return false;
        myPhotocathodeOtherHit* newHit = new myPhotocathodeOtherHit();
        newHit->SetEn( aStep->GetPreStepPoint()->GetTotalEnergy() );
        newHit->SetPos( aStep->GetPreStepPoint()->GetPosition() );
        newHit->SetTime( aStep->GetPreStepPoint()->GetGlobalTime() );
        newHit->SetPDG(ParticlePDG);
        G4int ParentID = theTrack->GetParentID();
        G4int TrackID = theTrack->GetTrackID();
        newHit->SetParID(ParentID);
        newHit->SetTrID(TrackID);
        newHit->SetPxyz( aStep->GetPreStepPoint()->GetMomentum() );

        //   G4cout<<theTrack->GetParticleDefinition()->GetParticleName()<<G4endl;
        theTrack->SetTrackStatus(fStopAndKill);

        fPhotocathodeOtherCollection->insert( newHit );
        // delete[] newHit;
        return true;
    } else {
        theTrack->SetTrackStatus(fStopAndKill);
        return false;
    }


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myPhotocathodeSurfaceSD::EndOfEvent(G4HCofThisEvent*)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myPhotocathodeSurfaceSD::clear()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myPhotocathodeSurfaceSD::DrawAll()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myPhotocathodeSurfaceSD::PrintAll()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
