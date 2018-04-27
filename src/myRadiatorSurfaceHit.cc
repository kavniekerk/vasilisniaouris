
#include "myRadiatorSurfaceHit.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4UnitsTable.hh"
#include "G4AttValue.hh"
#include "G4AttDef.hh"
#include "G4AttCheck.hh"

G4ThreadLocal G4Allocator<myRadiatorSurfaceHit>* myRadiatorSurfaceHitAllocator=0;
std::map<G4String,G4AttDef> myRadiatorSurfaceHit::fAttDefs;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myRadiatorSurfaceHit::myRadiatorSurfaceHit()
: G4VHit(),
  fEn(0.),fPos(), fTime(),fPDG()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myRadiatorSurfaceHit::~myRadiatorSurfaceHit()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myRadiatorSurfaceHit::myRadiatorSurfaceHit(const myRadiatorSurfaceHit &right)
  : G4VHit()
{
  fEn = right.fEn;
  fPos = right.fPos;
  fTime = right.fTime;
  fPDG = right.fPDG;
  fParID = right.fParID;
  fTrID = right.fTrID;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const myRadiatorSurfaceHit& myRadiatorSurfaceHit::operator=(const myRadiatorSurfaceHit &right)
{
  fEn = right.fEn;
  fPos = right.fPos;
  fTime = right.fTime;
  fPDG = right.fPDG;
  fParID = right.fParID;
  fTrID = right.fTrID;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int myRadiatorSurfaceHit::operator==(const myRadiatorSurfaceHit &right) const
{
  return (this==&right) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myRadiatorSurfaceHit::Draw()
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

const std::map<G4String,G4AttDef>* myRadiatorSurfaceHit::GetAttDefs() const
{
  // G4AttDefs have to have long life.  Use static member...
  if (fAttDefs.empty()) {
    fAttDefs["HitType"] =
      G4AttDef("HitType","Type of hit","Physics","","G4String");
  }
  return &fAttDefs;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<G4AttValue>* myRadiatorSurfaceHit::CreateAttValues() const
{
  // Create expendable G4AttsValues for picking...
  std::vector<G4AttValue>* attValues = new std::vector<G4AttValue>;
  attValues->push_back
    (G4AttValue("HitType","myRadiatorSurfaceHit",""));
  //G4cout << "Checking...\n" << G4AttCheck(attValues, GetAttDefs());
  return attValues;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myRadiatorSurfaceHit::Print()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
