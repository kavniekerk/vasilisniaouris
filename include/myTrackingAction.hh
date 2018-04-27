
#ifndef myTrackingAction_h
#define myTrackingAction_h 1

#include "G4UserTrackingAction.hh"

#include "globals.hh"
#include "myAnalysis.hh"

class myTrackingAction : public G4UserTrackingAction {

  public:
    myTrackingAction() : G4UserTrackingAction() {}
    virtual ~myTrackingAction(){}

    virtual void PreUserTrackingAction(const G4Track*);
    // virtual void PostUserTrackingAction(const G4Track*);

    //Trajectories
    std::vector<G4int>& GetTrajTrackIDVector() {return trajTrackid;}
    std::vector<G4int>& GetTrajParentIDVector() {return trajParentid;}
    std::vector<G4int>& GetTrajPdgIDVector() {return trajPdgid;}
    std::vector<G4int>& GetTrajProcessIDVector() {return trajProcessid;}

    virtual void ClearTrajTrackIDVector() {trajTrackid.clear();}
    virtual void ClearTrajParentIDVector() {trajParentid.clear();}
    virtual void ClearTrajPdgIDVector() {trajPdgid.clear();}
    virtual void ClearTrajProcessIDVector() {trajProcessid.clear();}


  private:
    //Trajectories
    std::vector<G4int> trajTrackid;
    std::vector<G4int> trajParentid;
    std::vector<G4int> trajPdgid;
    std::vector<G4int> trajProcessid;

    void GetProcessID(G4String TrajProcess, G4int& TrajProcessID) {

        if (TrajProcess == "muIoni") TrajProcessID = 1;
        else if (TrajProcess == "eBrem") TrajProcessID = 2;
        else if (TrajProcess == "phot") TrajProcessID = 3;
        else if (TrajProcess == "eIoni") TrajProcessID = 4;
        else if (TrajProcess == "muPairProd") TrajProcessID = 5;
        else if (TrajProcess == "compt") TrajProcessID = 6;
        else if (TrajProcess == "annihil") TrajProcessID = 7;
        else if (TrajProcess == "CoulombScat") TrajProcessID = 8;
        else if (TrajProcess == "muBrems") TrajProcessID = 9;
        else if (TrajProcess == "muonNuclear") TrajProcessID = 10;
        else if (TrajProcess == "conv") TrajProcessID = 11;
        else if (TrajProcess == "photonNuclear") TrajProcessID = 12;
        //else G4cout<<"WARNING! NEW INTERSCTION: " << TrajProcess << " with ProcessSubType = " << -TrajProcessID +1000<<G4endl;
        else {;}
        return;
    }

};


#endif
