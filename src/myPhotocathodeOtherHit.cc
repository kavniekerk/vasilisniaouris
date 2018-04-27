
#include "myPhotocathodeOtherHit.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4UnitsTable.hh"
#include "G4AttValue.hh"
#include "G4AttDef.hh"
#include "G4AttCheck.hh"

G4ThreadLocal G4Allocator<myPhotocathodeOtherHit>* myPhotocathodeOtherHitAllocator=0;
std::map<G4String,G4AttDef> myPhotocathodeOtherHit::fAttDefs;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myPhotocathodeOtherHit::myPhotocathodeOtherHit()
: G4VHit(),
  fEn(0.),fPos(), fTime(),fPDG(), fParID(), fTrID(), fPxyz()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myPhotocathodeOtherHit::~myPhotocathodeOtherHit()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myPhotocathodeOtherHit::myPhotocathodeOtherHit(const myPhotocathodeOtherHit &right)
  : G4VHit()
{
  fEn = right.fEn;
  fPos = right.fPos;
  fTime = right.fTime;
  fPDG = right.fPDG;
  fParID = right.fParID;
  fTrID = right.fTrID;
  fPxyz = right.fPxyz;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const myPhotocathodeOtherHit& myPhotocathodeOtherHit::operator=(const myPhotocathodeOtherHit &right)
{
  fEn = right.fEn;
  fPos = right.fPos;
  fTime = right.fTime;
  fPDG = right.fPDG;
  fParID = right.fParID;
  fTrID = right.fTrID;
  fPxyz = right.fPxyz;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int myPhotocathodeOtherHit::operator==(const myPhotocathodeOtherHit &right) const
{
  return (this==&right) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myPhotocathodeOtherHit::Draw()
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

const std::map<G4String,G4AttDef>* myPhotocathodeOtherHit::GetAttDefs() const
{
  // G4AttDefs have to have long life.  Use static member...
  if (fAttDefs.empty()) {
    fAttDefs["HitType"] =
      G4AttDef("HitType","Type of hit","Physics","","G4String");
  }
  return &fAttDefs;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<G4AttValue>* myPhotocathodeOtherHit::CreateAttValues() const
{
  // Create expendable G4AttsValues for picking...
  std::vector<G4AttValue>* attValues = new std::vector<G4AttValue>;
  attValues->push_back
    (G4AttValue("HitType","myPhotocathodeOtherHit",""));
  //G4cout << "Checking...\n" << G4AttCheck(attValues, GetAttDefs());
  return attValues;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myPhotocathodeOtherHit::Print()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
