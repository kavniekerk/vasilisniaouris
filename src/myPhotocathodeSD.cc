
#include "myPhotocathodeSD.hh"
#include "myPhotocathodeOPhotonHit.hh" //create this
// #include "myPhotocathodeOtherHit.hh" //create this
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"

#include "G4Track.hh"
#include "myAnalysis.hh"
#include "G4VProcess.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myPhotocathodeSD::myPhotocathodeSD(G4String name)
: G4VSensitiveDetector(name),
  fPhotocathodeOPhotonCollection(0)//, fPhotocathodeOtherCollection(0)
{
    G4String HCname;
    collectionName.insert(HCname="photocathodeOPhotonCollection");
    // G4String HCname2;
    // collectionName.insert(HCname2="photocathodeOtherCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myPhotocathodeSD::~myPhotocathodeSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myPhotocathodeSD::Initialize(G4HCofThisEvent* HCE)
{
    static int HCID = -1;
    fPhotocathodeOPhotonCollection = new myPhotocathodeOPhotonHitsCollection
                      (SensitiveDetectorName,collectionName[0]);
    // if(HCID<0)
    HCID = GetCollectionID(0);
    HCE->AddHitsCollection(HCID,fPhotocathodeOPhotonCollection);

    // fPhotocathodeOtherCollection = new myPhotocathodeOtherHitsCollection
    //                     (SensitiveDetectorName,collectionName[1]);
    // // if(HCID<0)
    // HCID = GetCollectionID(1);
    // HCE->AddHitsCollection(HCID,fPhotocathodeOtherCollection);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool myPhotocathodeSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    // G4double charge = aStep->GetPreStepPoint()->GetCharge();
    G4Track * theTrack = aStep->GetTrack();
    // G4int ParticlePDG = theTrack->GetParticleDefinition()->GetPDGEncoding();

    if(theTrack->GetParticleDefinition()->GetParticleName()=="opticalphoton") {
        // G4cout<<theTrack->GetParticleDefinition()->GetParticleName()<<G4endl;
        G4double edep = aStep->GetTotalEnergyDeposit();
        if (!edep) return false;
        G4int ParentID = theTrack->GetParentID();
        G4int TrackID = theTrack->GetTrackID();
        G4double TrackLength = theTrack->GetTrackLength();
        //G4int PPGDid = theTrack->GetParticleDefinition()->GetPDGEncoding();

        //optical photons have 0 pdgencoding

        // G4Track * parTrack = aStep->GetTrack(ParentID);
        // if (ParentID>1) G4cout<<theTrack->GetParentID()<<G4endl;
        // G4cout<<theTrack->GetParticleDefinition()->GetParticleName()<<G4endl;
        // G4String salala = aStep->GetTrack()->GetCreatorProcess()->GetProcessName();

        //G4cout<<theTrack->GetCreatorProcess()->GetProcessName()<<G4endl;
        myPhotocathodeOPhotonHit* newHit = new myPhotocathodeOPhotonHit();
        newHit->SetEdep( edep );
        //newHit->SetPos( aStep->GetPreStepPoint()->GetPosition() );
        //newHit->SetTime(aStep->GetPreStepPoint()->GetGlobalTime());
        newHit->SetPos( aStep->GetPostStepPoint()->GetPosition() );
        newHit->SetTime(aStep->GetPostStepPoint()->GetGlobalTime());
        newHit->SetParID(ParentID);
        newHit->SetTrID(TrackID);
        newHit->SetTrLen(TrackLength+aStep->GetStepLength());
        newHit->SetLocalTime(aStep->GetPostStepPoint()->GetLocalTime());

        fPhotocathodeOPhotonCollection->insert( newHit );
        // delete newHit;
        return true;
    } //else {
    //     if (!(aStep->IsFirstStepInVolume())) return false;
    //     myPhotocathodeOtherHit* newHit = new myPhotocathodeOtherHit();
    //     newHit->SetEn( aStep->GetPreStepPoint()->GetTotalEnergy() );
    //     newHit->SetPos( aStep->GetPreStepPoint()->GetPosition() );
    //     newHit->SetTime( aStep->GetPreStepPoint()->GetGlobalTime() );
    //     newHit->SetPDG(ParticlePDG);
    //     G4int ParentID = theTrack->GetParentID();
    //     G4int TrackID = theTrack->GetTrackID();
    //     newHit->SetParID(ParentID);
    //     newHit->SetTrID(TrackID);
    //     newHit->SetPxyz( aStep->GetPreStepPoint()->GetMomentum() );
    //
    //   //   G4cout<<theTrack->GetParticleDefinition()->GetParticleName()<<G4endl;
    //     theTrack->SetTrackStatus(fStopAndKill);
    //
    //     fPhotocathodeOtherCollection->insert( newHit );
    //     return true;
    // }

    return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myPhotocathodeSD::EndOfEvent(G4HCofThisEvent*)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myPhotocathodeSD::clear()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myPhotocathodeSD::DrawAll()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myPhotocathodeSD::PrintAll()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
