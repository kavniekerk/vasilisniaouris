
#ifndef myDetectorConstruction_h
#define myDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4NistManager.hh"
#include "G4OpticalSurface.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

class myDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    myDetectorConstruction();
    virtual ~myDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    //G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

  private:

    void AddOpticalPropertiesMgF2(G4Material* matMgF2);
    void AddOpticalPropertiesVacuum(G4Material* matVacuum);

    void AddPhotocathodeSurfaceProperties(G4OpticalSurface *photocathode_opsurf);
    void AddThinLayerSurfaceProperties(G4OpticalSurface *thinLayer_opsurf);
    void AddRefCoatSurfaceProperties(G4OpticalSurface *refCoat_opsurf);

    //void AddAirSurfaceProperties(G4OpticalSurface *air_opsurf);

//----------------------------playing-------------------------------------------//
    const G4double radiationlength =  0.3504*cm;

   public:
    const G4double fThick1 = 3.5*nm; //taken from calibration simulation
    const G4double fThick1effective = 5.5*nm;
    const G4double fThick2 = 18.*nm;
    const G4double fRadius = 5.*mm;
    // const G4double fLenMax = 7.3; //from max at: Xo*(ln(E/epsilon)-1) with e = 550/Z MeV (for E=30GeV)
    const G4double fLenMax = 3.;
    const G4double fCoatReflectivity = 0.;
    const G4int fCoat = 0;
    const G4double fThicknessCost = 0.001*mm;

  protected:
    // G4LogicalVolume*  fScoringVolume;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
