
#ifndef myCalorimeterSurfaceHit_h
#define myCalorimeterSurfaceHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4Types.hh"
#include "G4ThreeVector.hh"

class G4AttDef;

class myCalorimeterSurfaceHit : public G4VHit
{
  public:

      myCalorimeterSurfaceHit();
      virtual ~myCalorimeterSurfaceHit();
      myCalorimeterSurfaceHit(const myCalorimeterSurfaceHit &right);
      const myCalorimeterSurfaceHit& operator=(const myCalorimeterSurfaceHit &right);
      G4int operator==(const myCalorimeterSurfaceHit &right) const;

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

typedef G4THitsCollection<myCalorimeterSurfaceHit> myCalorimeterSurfaceHitsCollection;

extern G4ThreadLocal G4Allocator<myCalorimeterSurfaceHit>* myCalorimeterSurfaceHitAllocator;

inline void* myCalorimeterSurfaceHit::operator new(size_t)
{
  if(!myCalorimeterSurfaceHitAllocator) myCalorimeterSurfaceHitAllocator = new G4Allocator<myCalorimeterSurfaceHit>;
  return (void *) myCalorimeterSurfaceHitAllocator->MallocSingle();
}

inline void myCalorimeterSurfaceHit::operator delete(void *aHit)
{
  myCalorimeterSurfaceHitAllocator->FreeSingle((myCalorimeterSurfaceHit*) aHit);
}

#endif
