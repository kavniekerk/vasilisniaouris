
#ifndef myRunAction_h
#define myRunAction_h 1

#include "G4UserRunAction.hh"
#include "myEventAction.hh"
#include "myTrackingAction.hh"
#include "myDetectorConstruction.hh"
#include "myPrimaryGeneratorAction.hh"

//#include "myFile.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;

class myRunAction : public G4UserRunAction
{
  public:
    myRunAction(myEventAction *eventAction, myTrackingAction *trackingAction, myDetectorConstruction *detectorConstruction, myPrimaryGeneratorAction *primaryGenerationAction);
    virtual ~myRunAction();

  public:
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

private:
    myEventAction* fEventAction;
    myTrackingAction* fTrackingAction;
    myDetectorConstruction* fDetectorConstruction;
    myPrimaryGeneratorAction* fPrimaryGeneratorAction;


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
