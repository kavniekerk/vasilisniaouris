
#ifndef myPhotocathodeOtherHit_h
#define myPhotocathodeOtherHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4Types.hh"
#include "G4ThreeVector.hh"

class G4AttDef;

class myPhotocathodeOtherHit : public G4VHit
{
  public:

      myPhotocathodeOtherHit();
      virtual ~myPhotocathodeOtherHit();
      myPhotocathodeOtherHit(const myPhotocathodeOtherHit &right);
      const myPhotocathodeOtherHit& operator=(const myPhotocathodeOtherHit &right);
      G4int operator==(const myPhotocathodeOtherHit &right) const;

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
      G4ThreeVector fPxyz;

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

      inline void SetPxyz(G4ThreeVector pxyz)
      { fPxyz = pxyz; }
      inline G4ThreeVector GetPxyz()
      { return fPxyz; }

};

typedef G4THitsCollection<myPhotocathodeOtherHit> myPhotocathodeOtherHitsCollection;

extern G4ThreadLocal G4Allocator<myPhotocathodeOtherHit>* myPhotocathodeOtherHitAllocator;

inline void* myPhotocathodeOtherHit::operator new(size_t)
{
  if(!myPhotocathodeOtherHitAllocator) myPhotocathodeOtherHitAllocator = new G4Allocator<myPhotocathodeOtherHit>;
  return (void *) myPhotocathodeOtherHitAllocator->MallocSingle();
}

inline void myPhotocathodeOtherHit::operator delete(void *aHit)
{
  myPhotocathodeOtherHitAllocator->FreeSingle((myPhotocathodeOtherHit*) aHit);
}

#endif
