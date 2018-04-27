
#ifndef myPhotocathodeOPhotonHit_h
#define myPhotocathodeOPhotonHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4Types.hh"
#include "G4ThreeVector.hh"

class G4AttDef;

class myPhotocathodeOPhotonHit : public G4VHit
{
  public:

      myPhotocathodeOPhotonHit();
      virtual ~myPhotocathodeOPhotonHit();
      myPhotocathodeOPhotonHit(const myPhotocathodeOPhotonHit &right);
      const myPhotocathodeOPhotonHit& operator=(const myPhotocathodeOPhotonHit &right);
      G4int operator==(const myPhotocathodeOPhotonHit &right) const;

      inline void *operator new(size_t);
      inline void operator delete(void *aHit);

      virtual void Draw();
      virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
      virtual std::vector<G4AttValue>* CreateAttValues() const;
      virtual void Print();

  private:
      G4double fEdep;
      G4ThreeVector fPos;
      G4double fTime;
      G4int fParID;
      G4int fTrID;
      G4double fTrLen;
      G4double fLocalTime;

      static std::map<G4String,G4AttDef> fAttDefs;

  public:
      inline void SetEdep(G4double de)
      { fEdep = de; }
      inline G4double GetEdep()
      { return fEdep; }

      inline void SetPos(G4ThreeVector xyz)
      { fPos = xyz; }
      inline G4ThreeVector GetPos()
      { return fPos; }

      inline void SetTime(G4double tt)
      { fTime = tt; }
      inline G4double GetTime()
      { return fTime; }

      inline void SetParID(G4int ParID)
      { fParID = ParID; }
      inline G4int GetParID()
      { return fParID; }

      inline void SetTrID(G4int TrID)
      { fTrID = TrID; }
      inline G4int GetTrID()
      { return fTrID; }

      inline void SetTrLen(G4double TrLen)
      { fTrLen = TrLen; }
      inline G4double GetTrLen()
      { return fTrLen; }

      inline void SetLocalTime(G4double LocalTime)
      { fLocalTime = LocalTime; }
      inline G4double GetLocalTime()
      { return fLocalTime; }


};

typedef G4THitsCollection<myPhotocathodeOPhotonHit> myPhotocathodeOPhotonHitsCollection;

extern G4ThreadLocal G4Allocator<myPhotocathodeOPhotonHit>* myPhotocathodeOPhotonHitAllocator;

inline void* myPhotocathodeOPhotonHit::operator new(size_t)
{
  if(!myPhotocathodeOPhotonHitAllocator) myPhotocathodeOPhotonHitAllocator = new G4Allocator<myPhotocathodeOPhotonHit>;
  return (void *) myPhotocathodeOPhotonHitAllocator->MallocSingle();
}

inline void myPhotocathodeOPhotonHit::operator delete(void *aHit)
{
  myPhotocathodeOPhotonHitAllocator->FreeSingle((myPhotocathodeOPhotonHit*) aHit);
}

#endif
