
#ifndef mySteppingAction_H
#define mySteppingAction_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

class mySteppingAction : public G4UserSteppingAction
{
  public:
    mySteppingAction();
    virtual ~mySteppingAction();

    virtual void UserSteppingAction(const G4Step*);
};

#endif
