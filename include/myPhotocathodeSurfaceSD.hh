
#ifndef myPhotocathodeSurfaceSD_h
#define myPhotocathodeSurfaceSD_h 1

#include "G4VSensitiveDetector.hh"
#include "myPhotocathodeOtherHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class myPhotocathodeSurfaceSD : public G4VSensitiveDetector
{
  public:
      myPhotocathodeSurfaceSD(G4String name);
      virtual ~myPhotocathodeSurfaceSD();

      virtual void Initialize(G4HCofThisEvent* HCE);
      virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
      virtual void EndOfEvent(G4HCofThisEvent* HCE);
      virtual void clear();
      virtual void DrawAll();
      virtual void PrintAll();

  private:
      myPhotocathodeOtherHitsCollection* fPhotocathodeOtherCollection;
};

#endif
