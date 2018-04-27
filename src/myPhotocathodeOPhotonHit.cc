
#include "myPhotocathodeOPhotonHit.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4UnitsTable.hh"
#include "G4AttValue.hh"
#include "G4AttDef.hh"
#include "G4AttCheck.hh"

G4ThreadLocal G4Allocator<myPhotocathodeOPhotonHit>* myPhotocathodeOPhotonHitAllocator=0;
std::map<G4String,G4AttDef> myPhotocathodeOPhotonHit::fAttDefs;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myPhotocathodeOPhotonHit::myPhotocathodeOPhotonHit()
: G4VHit(),
  fEdep(0.),fPos(), fTime(),fParID(), fTrID(), fTrLen(), fLocalTime()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myPhotocathodeOPhotonHit::~myPhotocathodeOPhotonHit()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myPhotocathodeOPhotonHit::myPhotocathodeOPhotonHit(const myPhotocathodeOPhotonHit &right)
  : G4VHit()
{
  fEdep = right.fEdep;
  fPos = right.fPos;
  fTime = right.fTime;
  fParID = right.fParID;
  fTrID = right.fTrID;
  fTrLen = right.fTrLen;
  fLocalTime = right.fLocalTime;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const myPhotocathodeOPhotonHit& myPhotocathodeOPhotonHit::operator=(const myPhotocathodeOPhotonHit &right)
{
  fEdep = right.fEdep;
  fPos = right.fPos;
  fTime = right.fTime;
  fParID = right.fParID;
  fTrID = right.fTrID;
  fTrLen = right.fTrLen;
  fLocalTime = right.fLocalTime;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int myPhotocathodeOPhotonHit::operator==(const myPhotocathodeOPhotonHit &right) const
{
  return (this==&right) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myPhotocathodeOPhotonHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(fPos);
    circle.SetScreenSize(0.04);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const std::map<G4String,G4AttDef>* myPhotocathodeOPhotonHit::GetAttDefs() const
{
  // G4AttDefs have to have long life.  Use static member...
  if (fAttDefs.empty()) {
    fAttDefs["HitType"] =
      G4AttDef("HitType","Type of hit","Physics","","G4String");
  }
  return &fAttDefs;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<G4AttValue>* myPhotocathodeOPhotonHit::CreateAttValues() const
{
  // Create expendable G4AttsValues for picking...
  std::vector<G4AttValue>* attValues = new std::vector<G4AttValue>;
  attValues->push_back
    (G4AttValue("HitType","myPhotocathodeOPhotonHit",""));
  //G4cout << "Checking...\n" << G4AttCheck(attValues, GetAttDefs());
  return attValues;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myPhotocathodeOPhotonHit::Print()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
