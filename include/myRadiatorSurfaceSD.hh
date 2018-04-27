
#ifndef myRadiatorSurfaceSD_h
#define myRadiatorSurfaceSD_h 1

#include "G4VSensitiveDetector.hh"
#include "myRadiatorSurfaceHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class myRadiatorSurfaceSD : public G4VSensitiveDetector
{
  public:
      myRadiatorSurfaceSD(G4String name);
      virtual ~myRadiatorSurfaceSD();

      virtual void Initialize(G4HCofThisEvent* HCE);
      virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
      virtual void EndOfEvent(G4HCofThisEvent* HCE);
      virtual void clear();
      virtual void DrawAll();
      virtual void PrintAll();

  private:
      myRadiatorSurfaceHitsCollection* fRadiatorSurfaceCollection;
};

#endif
