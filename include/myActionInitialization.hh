
#ifndef myActionInitialization_H
#define myActionInitialization_H 1

#include "globals.hh"
#include "G4VUserActionInitialization.hh"
#include "myDetectorConstruction.hh"

class myActionInitialization : public G4VUserActionInitialization
{
  public:
    myActionInitialization(myDetectorConstruction *detectorConstruction);
    virtual ~myActionInitialization();

    virtual void Build() const;
    virtual void BuildForMaster() const;
  private:
    myDetectorConstruction* fDetectorConstruction;

};

#endif
