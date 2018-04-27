
#ifndef myPhotocathodeSD_h
#define myPhotocathodeSD_h 1

#include "G4VSensitiveDetector.hh"
#include "myPhotocathodeOPhotonHit.hh"
// #include "myPhotocathodeOtherHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class myPhotocathodeSD : public G4VSensitiveDetector
{
  public:
      myPhotocathodeSD(G4String name);
      virtual ~myPhotocathodeSD();

      virtual void Initialize(G4HCofThisEvent* HCE);
      virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
      virtual void EndOfEvent(G4HCofThisEvent* HCE);
      virtual void clear();
      virtual void DrawAll();
      virtual void PrintAll();

  private:
      myPhotocathodeOPhotonHitsCollection* fPhotocathodeOPhotonCollection;
    //   myPhotocathodeOtherHitsCollection* fPhotocathodeOtherCollection;
};

#endif
