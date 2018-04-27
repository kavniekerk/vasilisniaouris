
#include "myEventAction.hh"

#include "myPhotocathodeOPhotonHit.hh"
#include "myPhotocathodeOtherHit.hh"
#include "myRadiatorSurfaceHit.hh"
#include "myCalorimeterSurfaceHit.hh"


#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4ios.hh"

#include "G4THitsMap.hh"
#include "myAnalysis.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myEventAction::myEventAction(myTrackingAction* trackingAction)
: G4UserEventAction()
{
    fPhotocathodeOPhotonCollID = -1;
    fPhotocathodeOtherCollID = -1;
    fRadiatorSurfaceCollID = -1;
    fCalorimeterSurfaceCollID = -1;
    fRadiatorTEDCollID = -1;
    fRadiatorTDDCollID = -1;
    fCalorimeterTEDCollID = -1;
    fTrackingAction = trackingAction;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myEventAction::~myEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myEventAction::BeginOfEventAction(const G4Event*)
{
    G4SDManager * SDman = G4SDManager::GetSDMpointer();
    if(fPhotocathodeOPhotonCollID<0) {
        G4String colNam;
        fPhotocathodeOPhotonCollID = SDman->GetCollectionID(colNam="photocathodeOPhotonCollection");
    }

    if(fPhotocathodeOtherCollID<0) {
        G4String colNam;
        fPhotocathodeOtherCollID = SDman->GetCollectionID(colNam="photocathodeOtherCollection");
    }

    if(fRadiatorSurfaceCollID<0) {
        G4String colNam;
        fRadiatorSurfaceCollID = SDman->GetCollectionID(colNam="radiatorSurfaceCollection");
    }

    if(fCalorimeterSurfaceCollID<0) {
        G4String colNam;
        fCalorimeterSurfaceCollID = SDman->GetCollectionID(colNam="calorimeterSurfaceCollection");
    }

    if(fRadiatorTEDCollID<0) {
        G4String colNam;
        fRadiatorTEDCollID = SDman->GetCollectionID(colNam="radiator/TotalEnergyDeposit");
    }

    if(fRadiatorTDDCollID<0) {
        G4String colNam;
        fRadiatorTDDCollID = SDman->GetCollectionID(colNam="radiator/TotalDoseDeposit");
    }

    if(fCalorimeterTEDCollID<0) {
        G4String colNam;
        fCalorimeterTEDCollID = SDman->GetCollectionID(colNam="calorimeter/TotalEnergyDeposit");
    }

    ttime.clear();
    posx.clear();
    posy.clear();
    posz.clear();
    edep.clear();
    parid.clear();
    trid.clear();
    trlen.clear();
    loctime.clear();

    ttimePhot.clear();
    posxPhot.clear();
    posyPhot.clear();
    poszPhot.clear();
    enPhot.clear();
    pdgPhot.clear();
    paridPhot.clear();
    tridPhot.clear();
    pxPhot.clear();
    pyPhot.clear();
    pzPhot.clear();

    ttimeRad.clear();
    posxRad.clear();
    posyRad.clear();
    poszRad.clear();
    enRad.clear();
    pdgRad.clear();
    paridRad.clear();
    tridRad.clear();

    ttimeCal.clear();
    posxCal.clear();
    posyCal.clear();
    poszCal.clear();
    enCal.clear();
    pdgCal.clear();
    paridCal.clear();
    tridCal.clear();

    //Trajectories
    fTrackingAction->ClearTrajTrackIDVector();
    fTrackingAction->ClearTrajParentIDVector();
    fTrackingAction->ClearTrajPdgIDVector();
    fTrackingAction->ClearTrajProcessIDVector();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myEventAction::EndOfEventAction(const G4Event* evt)
{
    if(!((evt->GetEventID()+1)%10))
    G4cout << ">>> Event " << evt->GetEventID()+1 << G4endl;

    bool checkIDs = fPhotocathodeOPhotonCollID<0 && fPhotocathodeOtherCollID<0;
    checkIDs = checkIDs && fRadiatorSurfaceCollID<0 && fCalorimeterSurfaceCollID<0;
    checkIDs = checkIDs && fRadiatorTEDCollID<0 && fRadiatorTDDCollID<0 && fCalorimeterTEDCollID<0;
    if(checkIDs) return;

    G4HCofThisEvent * HCE = evt->GetHCofThisEvent();
    myPhotocathodeOPhotonHitsCollection* PPHC = 0;
    myPhotocathodeOtherHitsCollection* POHC = 0;
    myRadiatorSurfaceHitsCollection* RSHC = 0;
    myCalorimeterSurfaceHitsCollection* CSHC = 0;

    if(HCE) PPHC = (myPhotocathodeOPhotonHitsCollection*)(HCE->GetHC(fPhotocathodeOPhotonCollID));
    if(HCE) POHC = (myPhotocathodeOtherHitsCollection*)(HCE->GetHC(fPhotocathodeOtherCollID));
    if(HCE) RSHC = (myRadiatorSurfaceHitsCollection*)(HCE->GetHC(fRadiatorSurfaceCollID));
    if(HCE) CSHC = (myCalorimeterSurfaceHitsCollection*)(HCE->GetHC(fCalorimeterSurfaceCollID));

    //G4RunManager* runMan = G4RunManager::GetRunManager();
    //runMan->GetUserPrimaryGeneratorAction();
    //G4double fff = runMan->GetUserPrimaryGeneratorAction()->GetBeamY();
    // const G4Run* aRun = runMan->GetCurrentRun();
    //
    // G4int n_run = aRun->GetRunID();
    // G4int n_event = evt->GetEventID();

    if (PPHC) {
        G4int n_hit = PPHC->entries();

        for(G4int i = 0; i < n_hit; i++) {
            ttime.push_back((*PPHC)[i]->GetTime()/ns);
            posx.push_back((*PPHC)[i]->GetPos().x()/cm);
            posy.push_back((*PPHC)[i]->GetPos().y()/cm);
            posz.push_back((*PPHC)[i]->GetPos().z()/cm);
            edep.push_back((*PPHC)[i]->GetEdep()/eV);
            parid.push_back((*PPHC)[i]->GetParID());
            trid.push_back((*PPHC)[i]->GetTrID());
            trlen.push_back((*PPHC)[i]->GetTrLen()/cm);
            loctime.push_back((*PPHC)[i]->GetLocalTime()/ns);
        }
    }

    if (POHC) {
        G4int n_hit = POHC->entries();

        for(G4int i = 0; i < n_hit; i++) {
            ttimePhot.push_back((*POHC)[i]->GetTime()/ns);
            posxPhot.push_back((*POHC)[i]->GetPos().x()/cm);
            posyPhot.push_back((*POHC)[i]->GetPos().y()/cm);
            poszPhot.push_back((*POHC)[i]->GetPos().z()/cm);
            enPhot.push_back((*POHC)[i]->GetEn()/eV);
            pdgPhot.push_back((*POHC)[i]->GetPDG());
            paridPhot.push_back((*POHC)[i]->GetParID());
            tridPhot.push_back((*POHC)[i]->GetTrID());
            pxPhot.push_back((*POHC)[i]->GetPxyz().x()/eV);
            pyPhot.push_back((*POHC)[i]->GetPxyz().y()/eV);
            pzPhot.push_back((*POHC)[i]->GetPxyz().z()/eV);

        }
    }

    if (RSHC) {
        G4int n_hit = RSHC->entries();

        for(G4int i = 0; i < n_hit; i++) {
            ttimeRad.push_back((*RSHC)[i]->GetTime()/ns);
            posxRad.push_back((*RSHC)[i]->GetPos().x()/cm);
            posyRad.push_back((*RSHC)[i]->GetPos().y()/cm);
            poszRad.push_back((*RSHC)[i]->GetPos().z()/cm);
            enRad.push_back((*RSHC)[i]->GetEn()/eV);
            pdgRad.push_back((*RSHC)[i]->GetPDG());
            paridRad.push_back((*RSHC)[i]->GetParID());
            tridRad.push_back((*RSHC)[i]->GetTrID());
        }
    }

    if (CSHC) {
        G4int n_hit = CSHC->entries();

        for(G4int i = 0; i < n_hit; i++) {
            ttimeCal.push_back((*CSHC)[i]->GetTime()/ns);
            posxCal.push_back((*CSHC)[i]->GetPos().x()/cm);
            posyCal.push_back((*CSHC)[i]->GetPos().y()/cm);
            poszCal.push_back((*CSHC)[i]->GetPos().z()/cm);
            enCal.push_back((*CSHC)[i]->GetEn()/eV);
            pdgCal.push_back((*CSHC)[i]->GetPDG());
            paridCal.push_back((*CSHC)[i]->GetParID());
            tridCal.push_back((*CSHC)[i]->GetTrID());

        }
    }

    G4THitsMap<G4double>* eventTotalEnergyDepositRadiator = (G4THitsMap<G4double>*)(HCE->GetHC(fRadiatorTEDCollID));
    std::map<G4int,G4double*>::iterator itrTEDRad = eventTotalEnergyDepositRadiator->GetMap()->begin();
    if (eventTotalEnergyDepositRadiator) totEnDepRad = *(itrTEDRad->second);

    G4THitsMap<G4double>* eventTotalDoseDepositRadiator = (G4THitsMap<G4double>*)(HCE->GetHC(fRadiatorTDDCollID));
    std::map<G4int,G4double*>::iterator itrTDDRad = eventTotalDoseDepositRadiator->GetMap()->begin();
    if (eventTotalDoseDepositRadiator) totDoseDepRad = *(itrTDDRad->second);

    G4THitsMap<G4double>* eventTotalEnergyDepositCalorimeter = (G4THitsMap<G4double>*)(HCE->GetHC(fCalorimeterTEDCollID));
    std::map<G4int,G4double*>::iterator itrTEDCal = eventTotalEnergyDepositCalorimeter->GetMap()->begin();
    if (eventTotalEnergyDepositCalorimeter) totEnDepCal = *(itrTEDCal->second);

    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(2, totEnDepRad/eV);
    analysisManager->FillNtupleDColumn(3, totDoseDepRad/gray);
    analysisManager->FillNtupleDColumn(4, totEnDepCal/eV);
    analysisManager->AddNtupleRow();

 // for (int i = 0; i<n_hit; i++) {
     //analysisManager->FillNtupleIColumn(0, 0, n_run);
    // analysisManager->FillNtupleIColumn(1, n_event);
    // analysisManager->FillNtupleIColumn(2, i);
    // analysisManager->FillNtupleDColumn(3, (*PHC)[i]->GetTime());
    // analysisManager->FillNtupleDColumn(4, (*PHC)[i]->GetPos().x());
    // analysisManager->FillNtupleDColumn(5, (*PHC)[i]->GetPos().y());
    // analysisManager->FillNtupleDColumn(6, (*PHC)[i]->GetPos().z());
    // analysisManager->FillNtupleDColumn(7, (*PHC)[i]->GetEdep());
    // analysisManager->AddNtupleRow();

 // }

  // if(THC)
  // {
  //   int n_hit = THC->entries();
  //   G4cout << "     " << n_hit
  //        << " hits are stored in myPhotocathodeOPhotonHitsCollection." << G4endl;
  // }

  // if(PHC)
  // {
  //   double n_hit = (*PHC)[1]->GetPos().x();
  //   G4cout << "     " << n_hit
  //        << " hits are stored in myPhotocathodeOPhotonHitsCollection." << G4endl;
  // }

  // myFile mf;
  // ofstream *Savefile = mf.GetFile();

  // G4double n_h = (*PHC)[1]->GetPos().x();
  // Savefile.open("../myPICOSEC/example.txt",ios::app);
  // Savefile<<n_h<<endl;
  // Savefile.close();

  // Savefile.close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
