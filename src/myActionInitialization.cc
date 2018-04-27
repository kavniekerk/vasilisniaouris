
#include "myActionInitialization.hh"
#include "myPrimaryGeneratorAction.hh"
#include "myRunAction.hh"
#include "myEventAction.hh"
// #include "myStackingAction.hh"
#include "myTrackingAction.hh"
// #include "mySteppingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myActionInitialization::myActionInitialization(myDetectorConstruction *detectorConstruction)
: G4VUserActionInitialization(), fDetectorConstruction(detectorConstruction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myActionInitialization::~myActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myActionInitialization::Build() const
{
  auto* gen_action = new myPrimaryGeneratorAction;
  SetUserAction(gen_action);
  //

  auto* tracking_action = new myTrackingAction;
  SetUserAction(tracking_action);

  auto* event_action = new myEventAction(tracking_action);
  SetUserAction(event_action);

  G4UserRunAction* run_action = new myRunAction(event_action, tracking_action, fDetectorConstruction, gen_action);
  SetUserAction(run_action);

  //
  // G4UserStackingAction* stacking_action = new myStackingAction;
  // SetUserAction(stacking_action);
  //

  // //
  // G4UserSteppingAction* stepping_action = new mySteppingAction;
  // SetUserAction(stepping_action);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
void myActionInitialization::BuildForMaster() const
{
  //
  auto* gen_action = new myPrimaryGeneratorAction;
  auto* tracking_action = new myTrackingAction;
  auto* event_action = new myEventAction(tracking_action);
  G4UserRunAction* run_action = new myRunAction(event_action, tracking_action, fDetectorConstruction, gen_action);
  SetUserAction(run_action);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
