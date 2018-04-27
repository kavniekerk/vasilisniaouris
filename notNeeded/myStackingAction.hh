//NO NEED YET
#ifndef RE05StackingAction_H
#define RE05StackingAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"
#include "G4ThreeVector.hh"

class G4Track;

#include "RE05TrackerHit.hh"
#include "RE05MuonHit.hh"
class RE05StackingActionMessenger;

class RE05StackingAction : public G4UserStackingAction
{
  public:
    RE05StackingAction();
    virtual ~RE05StackingAction();

  public:
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
    virtual void NewStage();
    virtual void PrepareNewEvent();

  private:
    G4bool InsideRoI(const G4Track * aTrack,G4double ang);
    G4VHitsCollection* GetCollection(G4String colName);

    RE05TrackerHitsCollection* fTrkHits;
    RE05MuonHitsCollection* fMuonHits;
    RE05StackingActionMessenger* fMessenger;

    G4int fStage;
    G4int fReqMuon;
    G4int fReqIsoMuon;
    G4int fReqIso;
    G4double fAngRoI;

  public:
    inline void SetNRequestMuon(G4int val) { fReqMuon = val; }
    inline G4int GetNRequestMuon() const { return fReqMuon; }
    inline void SetNRequestIsoMuon(G4int val) { fReqIsoMuon = val; }
    inline G4int GetNRequestIsoMuon() const { return fReqIsoMuon; }
    inline void SetNIsolation(G4int val) { fReqIso = val; }
    inline G4int GetNIsolation() const { return fReqIso; }
    inline void SetRoIAngle(G4double val) { fAngRoI = val; }
    inline G4double GetRoIAngle() const { return fAngRoI; }
};

#endif
