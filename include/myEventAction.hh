
#ifndef myEventAction_h
#define myEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "myAnalysis.hh"
#include "myTrackingAction.hh"

class myEventAction : public G4UserEventAction
{
  public:
    myEventAction(myTrackingAction* trackingAction);
    virtual ~myEventAction();

  public:
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

//photocathode optical photons
    std::vector<G4double>& GetTimeVector() {return ttime;}
    std::vector<G4double>& GetPosXVector() {return posx;}
    std::vector<G4double>& GetPosYVector() {return posy;}
    std::vector<G4double>& GetPosZVector() {return posz;}
    std::vector<G4double>& GetEdepVector() {return edep;}
    std::vector<G4int>& GetParIDVector() {return parid;}
    std::vector<G4int>& GetTrIDVector() {return trid;}
    std::vector<G4double>& GetTrLenVector() {return trlen;}
    std::vector<G4double>& GetLocalTimeVector() {return loctime;}

//photocathode other particles
    std::vector<G4double>& GetTimeVectorPhot() {return ttimePhot;}
    std::vector<G4double>& GetPosXVectorPhot() {return posxPhot;}
    std::vector<G4double>& GetPosYVectorPhot() {return posyPhot;}
    std::vector<G4double>& GetPosZVectorPhot() {return poszPhot;}
    std::vector<G4double>& GetEnVectorPhot() {return enPhot;}
    std::vector<G4int>& GetPDGVectorPhot() {return pdgPhot;}
    std::vector<G4int>& GetParIDVectorPhot() {return paridPhot;}
    std::vector<G4int>& GetTrIDVectorPhot() {return tridPhot;}
    std::vector<G4double>& GetPXVectorPhot() {return pxPhot;}
    std::vector<G4double>& GetPYVectorPhot() {return pyPhot;}
    std::vector<G4double>& GetPZVectorPhot() {return pzPhot;}


//radiatorSurface other particles
    std::vector<G4double>& GetTimeVectorRad() {return ttimeRad;}
    std::vector<G4double>& GetPosXVectorRad() {return posxRad;}
    std::vector<G4double>& GetPosYVectorRad() {return posyRad;}
    std::vector<G4double>& GetPosZVectorRad() {return poszRad;}
    std::vector<G4double>& GetEnVectorRad() {return enRad;}
    std::vector<G4int>& GetPDGVectorRad() {return pdgRad;}
    std::vector<G4int>& GetParIDVectorRad() {return paridRad;}
    std::vector<G4int>& GetTrIDVectorRad() {return tridRad;}

//calorimeterSurface other particles
    std::vector<G4double>& GetTimeVectorCal() {return ttimeCal;}
    std::vector<G4double>& GetPosXVectorCal() {return posxCal;}
    std::vector<G4double>& GetPosYVectorCal() {return posyCal;}
    std::vector<G4double>& GetPosZVectorCal() {return poszCal;}
    std::vector<G4double>& GetEnVectorCal() {return enCal;}
    std::vector<G4int>& GetPDGVectorCal() {return pdgCal;}
    std::vector<G4int>& GetParIDVectorCal() {return paridCal;}
    std::vector<G4int>& GetTrIDVectorCal() {return tridCal;}


  private:
    G4int fPhotocathodeOPhotonCollID;
    std::vector<G4double> ttime;
    std::vector<G4double> posx;
    std::vector<G4double> posy;
    std::vector<G4double> posz;
    std::vector<G4double> edep;
    std::vector<G4int> parid;
    std::vector<G4int> trid;
    std::vector<G4double> trlen;
    std::vector<G4double> loctime;

    G4int fPhotocathodeOtherCollID;
    std::vector<G4double> ttimePhot;
    std::vector<G4double> posxPhot;
    std::vector<G4double> posyPhot;
    std::vector<G4double> poszPhot;
    std::vector<G4double> enPhot;
    std::vector<G4int> pdgPhot;
    std::vector<G4int> paridPhot;
    std::vector<G4int> tridPhot;
    std::vector<G4double> pxPhot;
    std::vector<G4double> pyPhot;
    std::vector<G4double> pzPhot;

    G4int fRadiatorSurfaceCollID;
    std::vector<G4double> ttimeRad;
    std::vector<G4double> posxRad;
    std::vector<G4double> posyRad;
    std::vector<G4double> poszRad;
    std::vector<G4double> enRad;
    std::vector<G4int> pdgRad;
    std::vector<G4int> paridRad;
    std::vector<G4int> tridRad;

    G4int fCalorimeterSurfaceCollID;
    std::vector<G4double> ttimeCal;
    std::vector<G4double> posxCal;
    std::vector<G4double> posyCal;
    std::vector<G4double> poszCal;
    std::vector<G4double> enCal;
    std::vector<G4int> pdgCal;
    std::vector<G4int> paridCal;
    std::vector<G4int> tridCal;

    G4int fRadiatorTEDCollID;
    G4double totEnDepRad;
    G4int fRadiatorTDDCollID;
    G4double totDoseDepRad;

    G4int fCalorimeterTEDCollID;
    G4double totEnDepCal;

    myTrackingAction* fTrackingAction;

};



#endif
