
#include "myTrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include <G4VProcess.hh>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{

        G4int TrajCurrentStepNumber = aTrack->GetCurrentStepNumber();
        if (TrajCurrentStepNumber) return; //check if step is zero to process, else exit

        G4int TrajParentID = aTrack->GetParentID();
        if (!TrajParentID) return; //check if parID is NOT zero to process, else exit

        G4String TrajProcess = aTrack->GetCreatorProcess()->GetProcessName();
        G4int TrajTrackID = aTrack->GetTrackID();
        G4int TrajPDGEncoding = aTrack->GetParticleDefinition()->GetPDGEncoding();

        if (TrajProcess!="Cerenkov") {
            trajTrackid.push_back(TrajTrackID);
            trajParentid.push_back(TrajParentID);
            trajPdgid.push_back(TrajPDGEncoding);
            //if (TrajPDGEncoding>25) G4cout<< aTrack->GetParticleDefinition()->GetParticleName() <<G4endl;
            G4int TrajProcessID = - 1000 - aTrack->GetCreatorProcess()->GetProcessSubType();
            GetProcessID(TrajProcess, TrajProcessID);
            trajProcessid.push_back(TrajProcessID);
        }
        return;
    //if((aTrack->GetParentID())>0 && (aTrack->GetCreatorProcess()->GetProcessName())!="Cerenkov" && (aTrack->GetCurrentStepNumber()==0))
    //G4cout<<aTrack->GetParentID()<< "\t"<< aTrack->GetTrackID()<<"\t"<<aTrack->GetCreatorProcess()->GetProcessName()<<"\t"<<aTrack->GetParticleDefinition()->GetParticleName()<<G4endl;

    // if (aTrack->GetTrackID()<1) {
    //     G4cout<<aTrack->GetParentID()<< "\t"<< aTrack->GetTrackID()<<G4endl;
    // }
    //No Track processed with ID==0. Hence if TrackID ==1 -> Store ProccessName as ParticleGun
  // // Create trajectory only for primaries
  // if(aTrack->GetParentID()==0)
  // { fpTrackingManager->SetStoreTrajectory(true); }
  // else
  // { fpTrackingManager->SetStoreTrajectory(false); }
}

// void myTrackingAction::PostUserTrackingAction(const G4Track* aTrack) {
//
//     //G4cout<<aTrack->GetParentID()<< "\t"<< aTrack->GetTrackID()<<"\t"<<aTrack->GetCreatorProcess()->GetProcessName()<<G4endl;
// }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
