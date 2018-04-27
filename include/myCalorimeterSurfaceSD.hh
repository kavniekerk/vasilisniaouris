
#ifndef myCalorimeterSurfaceSD_h
#define myCalorimeterSurfaceSD_h 1

#include "G4VSensitiveDetector.hh"
#include "myCalorimeterSurfaceHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class myCalorimeterSurfaceSD : public G4VSensitiveDetector
{
  public:
      myCalorimeterSurfaceSD(G4String name);
      virtual ~myCalorimeterSurfaceSD();

      virtual void Initialize(G4HCofThisEvent* HCE);
      virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
      virtual void EndOfEvent(G4HCofThisEvent* HCE);
      virtual void clear();
      virtual void DrawAll();
      virtual void PrintAll();

  private:
      myCalorimeterSurfaceHitsCollection* fCalorimeterSurfaceCollection;
};

#endif
