
#ifndef myRadiatorSurfaceHit_h
#define myRadiatorSurfaceHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4Types.hh"
#include "G4ThreeVector.hh"

class G4AttDef;

class myRadiatorSurfaceHit : public G4VHit
{
  public:

      myRadiatorSurfaceHit();
      virtual ~myRadiatorSurfaceHit();
      myRadiatorSurfaceHit(const myRadiatorSurfaceHit &right);
      const myRadiatorSurfaceHit& operator=(const myRadiatorSurfaceHit &right);
      G4int operator==(const myRadiatorSurfaceHit &right) const;

      inline void *operator new(size_t);
      inline void operator delete(void *aHit);

      virtual void Draw();
      virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
      virtual std::vector<G4AttValue>* CreateAttValues() const;
      virtual void Print();

  private:
      G4double fEn;
      G4ThreeVector fPos;
      G4double fTime;
      G4int fPDG;
      G4int fParID;
      G4int fTrID;

      static std::map<G4String,G4AttDef> fAttDefs;

  public:
      inline void SetEn(G4double en)
      { fEn = en; }
      inline G4double GetEn()
      { return fEn; }

      inline void SetPos(G4ThreeVector xyz)
      { fPos = xyz; }
      inline G4ThreeVector GetPos()
      { return fPos; }

      inline void SetTime(G4double tt)
      { fTime = tt; }
      inline G4double GetTime()
      { return fTime; }

      inline void SetPDG(G4int PDG)
      { fPDG = PDG; }
      inline G4int GetPDG()
      { return fPDG; }

      inline void SetParID(G4int ParID)
      { fParID = ParID; }
      inline G4int GetParID()
      { return fParID; }

      inline void SetTrID(G4int TrID)
      { fTrID = TrID; }
      inline G4int GetTrID()
      { return fTrID; }

};

typedef G4THitsCollection<myRadiatorSurfaceHit> myRadiatorSurfaceHitsCollection;

extern G4ThreadLocal G4Allocator<myRadiatorSurfaceHit>* myRadiatorSurfaceHitAllocator;

inline void* myRadiatorSurfaceHit::operator new(size_t)
{
  if(!myRadiatorSurfaceHitAllocator) myRadiatorSurfaceHitAllocator = new G4Allocator<myRadiatorSurfaceHit>;
  return (void *) myRadiatorSurfaceHitAllocator->MallocSingle();
}

inline void myRadiatorSurfaceHit::operator delete(void *aHit)
{
  myRadiatorSurfaceHitAllocator->FreeSingle((myRadiatorSurfaceHit*) aHit);
}

#endif
