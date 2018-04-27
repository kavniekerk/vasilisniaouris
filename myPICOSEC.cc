
#include "myDetectorConstruction.hh"
#include "myActionInitialization.hh"

// #ifdef G4MULTITHREADED
// #include "G4MTRunManager.hh"
// #else
#include "G4RunManager.hh"
// #endif


#include "G4UImanager.hh"
#include "QBBC.hh"
#include "FTFP_BERT.hh"
#include "QGSP_BIC.hh"


#include "G4PhysListFactory.hh"
#include "G4OpticalPhysics.hh"


#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

#include "G4Cerenkov.hh"

#include "G4TrackingManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{

  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  // Choose the Random engine and seed
  // G4long seed = 1000;
  G4long seed = 1000; //apo 1000 me vhma 100 (ews 2300)
  G4Random::setTheSeed(seed);
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  // Construct the default run manager
  //
// #ifdef G4MULTITHREADED
//   G4MTRunManager* runManager = new G4MTRunManager;
// #else
   G4RunManager* runManager = new G4RunManager;
// #endif

  // Set mandatory initialization classes
  //
  // Detector construction
  myDetectorConstruction* myDC = new myDetectorConstruction();
  runManager->SetUserInitialization(myDC);

  // Physics list
  // G4VModularPhysicsList* physicsList = new FTFP_BERT;
  // physicsList->SetVerboseLevel(1);
  // runManager->SetUserInitialization(physicsList);
  // //my process
  // G4Cerenkov* theCerenkovProcess = new G4Cerenkov("Cerenkov");
  // theCerenkovProcess -> SetTrackSecondariesFirst(true);
  // G4int MaxNumPhotons = 300;
  // theCerenkovProcess->SetMaxNumPhotonsPerStep(MaxNumPhotons);
  // //end my process
  //

  //second Cerenkov initialization
  G4PhysListFactory factory;
  G4VModularPhysicsList* physicsList = NULL;
  G4String physName = "";
  char* path = getenv("PHYSLIST");
  if (path) {
      physName = G4String(path);
  } else {
      physName = "QGSP_BIC_EMZ"; // default
      //physName = "FTFP_BERT_EMZ"; // default
  }
  // reference PhysicsList via its name
  if (factory.IsReferencePhysList(physName)) {
      physicsList = factory.GetReferencePhysList(physName);
  }
 // processManager->AddDiscreteProcess(new G4LowEnergyPhotoElectric);

  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  physicsList->RegisterPhysics(opticalPhysics);
  opticalPhysics->Configure(kCerenkov, true);
  opticalPhysics->SetCerenkovStackPhotons(true);
  opticalPhysics->Configure(kScintillation, false);
  physicsList->DumpList();
  //physicsList->SetVerboseLevel(4);
  runManager->SetUserInitialization(physicsList);

  //end of second Cerenkov


  // User action initialization
  runManager->SetUserInitialization(new myActionInitialization(myDC));
  runManager->Initialize();//it was not here, but now batch works!

  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if ( ! ui ) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
