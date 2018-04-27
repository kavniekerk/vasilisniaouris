
#include "myRunAction.hh"

#include "G4Run.hh"
#include "G4ios.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "myAnalysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myRunAction::myRunAction(myEventAction *eventAction, myTrackingAction *trackingAction, myDetectorConstruction *detectorConstruction, myPrimaryGeneratorAction *primaryGenerationAction)
: G4UserRunAction(), fEventAction(eventAction), fTrackingAction(trackingAction), fDetectorConstruction(detectorConstruction), fPrimaryGeneratorAction(primaryGenerationAction)
{
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetNtupleMerging(true);
    analysisManager->SetFirstNtupleId(0);
    analysisManager->SetVerboseLevel(0);
    //analysisManager->SetFirstHistoId(1);
    //analysisManager->OpenFile("myPICOSEC");

    char stringLenMax [100];
    std::sprintf(stringLenMax, "%.2f", fDetectorConstruction->fLenMax);

    char stringRadius [100];
    std::sprintf(stringRadius, "%.3f", fDetectorConstruction->fRadius/cm);

    std::string stringRef = "";
    if (!fDetectorConstruction->fCoat) {
        stringRef = "no coating ";
    } else {
        if (fDetectorConstruction->fCoatReflectivity) {
            stringRef = "reflective coating ";
        } else {
            stringRef = "non-reflective coating ";
        }
    }

    char stringPE [100];
    std::sprintf(stringPE, "%.2f", fPrimaryGeneratorAction->fParticleEnergy/GeV);
    std::string stringPN = fPrimaryGeneratorAction->fParticleName;

    std::string myPICOSECDescription = "\n\nPos(cm), En&P(eV), Time(ns), Dose(gray)\n"
        +std::string(stringLenMax)+"*Xo Calorimeter in front of PICOSEC\n"
        +std::string(stringRadius)+"cm radius and "+stringRef +"on PICOSEC\n"
        +stringPN+stringPE+"GeV incoming particle\n";
    analysisManager->CreateNtuple("myPICOSEC", myPICOSECDescription);

    analysisManager->CreateNtupleDColumn("BeamX");
    analysisManager->CreateNtupleDColumn("BeamY");

    analysisManager->CreateNtupleDColumn("TotEnDepRad");
    analysisManager->CreateNtupleDColumn("TotDoseDepRad");
    analysisManager->CreateNtupleDColumn("TotEnDepCal");

    //analysisManager->CreateNtupleIColumn("ParentID");
    analysisManager->CreateNtupleDColumn("Time", fEventAction->GetTimeVector());
    analysisManager->CreateNtupleDColumn("PosX", fEventAction->GetPosXVector());
    analysisManager->CreateNtupleDColumn("PosY", fEventAction->GetPosYVector());
    analysisManager->CreateNtupleDColumn("PosZ", fEventAction->GetPosZVector());
    analysisManager->CreateNtupleDColumn("Edep", fEventAction->GetEdepVector());
    analysisManager->CreateNtupleIColumn("ParID", fEventAction->GetParIDVector());
    analysisManager->CreateNtupleIColumn("TrID", fEventAction->GetTrIDVector());
    analysisManager->CreateNtupleDColumn("TrLen", fEventAction->GetTrLenVector());
    analysisManager->CreateNtupleDColumn("LocalTime", fEventAction->GetLocalTimeVector());

    analysisManager->CreateNtupleDColumn("TimePhot", fEventAction->GetTimeVectorPhot());
    analysisManager->CreateNtupleDColumn("PosXPhot", fEventAction->GetPosXVectorPhot());
    analysisManager->CreateNtupleDColumn("PosYPhot", fEventAction->GetPosYVectorPhot());
    analysisManager->CreateNtupleDColumn("PosZPhot", fEventAction->GetPosZVectorPhot());
    analysisManager->CreateNtupleDColumn("EnPhot", fEventAction->GetEnVectorPhot());
    analysisManager->CreateNtupleIColumn("PDGPhot", fEventAction->GetPDGVectorPhot());
    analysisManager->CreateNtupleIColumn("ParIDPhot", fEventAction->GetParIDVectorPhot());
    analysisManager->CreateNtupleIColumn("TrIDPhot", fEventAction->GetTrIDVectorPhot());
    analysisManager->CreateNtupleDColumn("PXPhot", fEventAction->GetPXVectorPhot());
    analysisManager->CreateNtupleDColumn("PYPhot", fEventAction->GetPYVectorPhot());
    analysisManager->CreateNtupleDColumn("PZPhot", fEventAction->GetPZVectorPhot());

    analysisManager->CreateNtupleDColumn("TimeRad", fEventAction->GetTimeVectorRad());
    analysisManager->CreateNtupleDColumn("PosXRad", fEventAction->GetPosXVectorRad());
    analysisManager->CreateNtupleDColumn("PosYRad", fEventAction->GetPosYVectorRad());
    analysisManager->CreateNtupleDColumn("PosZRad", fEventAction->GetPosZVectorRad());
    analysisManager->CreateNtupleDColumn("EnRad", fEventAction->GetEnVectorRad());
    analysisManager->CreateNtupleIColumn("PDGRad", fEventAction->GetPDGVectorRad());
    analysisManager->CreateNtupleIColumn("ParIDRad", fEventAction->GetParIDVectorRad());
    analysisManager->CreateNtupleIColumn("TrIDRad", fEventAction->GetTrIDVectorRad());

    analysisManager->CreateNtupleDColumn("TimeCal", fEventAction->GetTimeVectorCal());
    analysisManager->CreateNtupleDColumn("PosXCal", fEventAction->GetPosXVectorCal());
    analysisManager->CreateNtupleDColumn("PosYCal", fEventAction->GetPosYVectorCal());
    analysisManager->CreateNtupleDColumn("PosZCal", fEventAction->GetPosZVectorCal());
    analysisManager->CreateNtupleDColumn("EnCal", fEventAction->GetEnVectorCal());
    analysisManager->CreateNtupleIColumn("PDGCal", fEventAction->GetPDGVectorCal());
    analysisManager->CreateNtupleIColumn("ParIDCal", fEventAction->GetParIDVectorCal());
    analysisManager->CreateNtupleIColumn("TrIDCal", fEventAction->GetTrIDVectorCal());

    //Trajectories
    analysisManager->CreateNtupleIColumn("TrajTrackID", fTrackingAction->GetTrajTrackIDVector());
    analysisManager->CreateNtupleIColumn("TrajParentID", fTrackingAction->GetTrajParentIDVector());
    analysisManager->CreateNtupleIColumn("TrajPdgID", fTrackingAction->GetTrajPdgIDVector());
    analysisManager->CreateNtupleIColumn("TrajProcessID", fTrackingAction->GetTrajProcessIDVector());

    analysisManager->FinishNtuple();

    G4Ntuple* ntuple = analysisManager->GetNtuple(0);
    G4cout<< ntuple<< " geiaaa"<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myRunAction::~myRunAction()
{
    delete G4AnalysisManager::Instance();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myRunAction::BeginOfRunAction(const G4Run* aRun)
{
    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

    auto analysisManager = G4AnalysisManager::Instance();
    G4String fileName = "myPICOSECrun001";
    analysisManager->OpenFile(fileName);

  // Savefile.open("../myPICOSEC/example.txt",ios::app);
  // Savefile<<5<<endl<<"salala"<<endl;
  //Savefile->close();

  //in_file->open("example.txt");

  //myFile mf;
  //mf.SetFile(*in_file);

//  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
}
void myRunAction::EndOfRunAction(const G4Run* aRun)
{

  auto analysisManager = G4AnalysisManager::Instance();

  G4int n_run = aRun->GetRunID();
  G4cout<<"We did it! "<< n_run <<G4endl;

  analysisManager->Write();
  analysisManager->CloseFile();

  //myFile mf;
  //ofstream *Savefile = mf.GetFile();
  //Savefile.close();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
