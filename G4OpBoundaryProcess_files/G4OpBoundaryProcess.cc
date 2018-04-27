//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
////////////////////////////////////////////////////////////////////////
// Optical Photon Boundary Process Class Implementation
////////////////////////////////////////////////////////////////////////
//
// File:        G4OpBoundaryProcess.cc
// Description: Discrete Process -- reflection/refraction at
//                                  optical interfaces
// Version:     1.1
// Created:     1997-06-18
// Modified:    1998-05-25 - Correct parallel component of polarization
//                           (thanks to: Stefano Magni + Giovanni Pieri)
//              1998-05-28 - NULL Rindex pointer before reuse
//                           (thanks to: Stefano Magni)
//              1998-06-11 - delete *sint1 in oblique reflection
//                           (thanks to: Giovanni Pieri)
//              1998-06-19 - move from GetLocalExitNormal() to the new
//                           method: GetLocalExitNormal(&valid) to get
//                           the surface normal in all cases
//              1998-11-07 - NULL OpticalSurface pointer before use
//                           comparison not sharp for: std::abs(cost1) < 1.0
//                           remove sin1, sin2 in lines 556,567
//                           (thanks to Stefano Magni)
//              1999-10-10 - Accommodate changes done in DoAbsorption by
//                           changing logic in DielectricMetal
//              2001-10-18 - avoid Linux (gcc-2.95.2) warning about variables
//                           might be used uninitialized in this function
//                           moved E2_perp, E2_parl and E2_total out of 'if'
//              2003-11-27 - Modified line 168-9 to reflect changes made to
//                           G4OpticalSurface class ( by Fan Lei)
//              2004-02-02 - Set theStatus = Undefined at start of DoIt
//              2005-07-28 - add G4ProcessType to constructor
//              2006-11-04 - add capability of calculating the reflectivity
//                           off a metal surface by way of a complex index
//                           of refraction - Thanks to Sehwook Lee and John
//                           Hauptman (Dept. of Physics - Iowa State Univ.)
//              2009-11-10 - add capability of simulating surface reflections
//                           with Look-Up-Tables (LUT) containing measured
//                           optical reflectance for a variety of surface
//                           treatments - Thanks to Martin Janecek and
//                           William Moses (Lawrence Berkeley National Lab.)
//              2013-06-01 - add the capability of simulating the transmission
//                           of a dichronic filter
//
// Author:      Peter Gumplinger
// 		adopted from work by Werner Keil - April 2/96
// mail:        gum@triumf.ca
//
////////////////////////////////////////////////////////////////////////

#include "G4ios.hh"
#include "G4PhysicalConstants.hh"
#include "G4OpProcessSubType.hh"

#include "G4OpBoundaryProcess.hh"
#include "G4GeometryTolerance.hh"

#include "G4VSensitiveDetector.hh"
#include "G4ParallelWorldProcess.hh"

#include "G4SystemOfUnits.hh"

#include "G4String.hh"
/////////////////////////
// Class Implementation
/////////////////////////

        //////////////
        // Operators
        //////////////

// G4OpBoundaryProcess::operator=(const G4OpBoundaryProcess &right)
// {
// }

        /////////////////
        // Constructors
        /////////////////

G4OpBoundaryProcess::G4OpBoundaryProcess(const G4String& processName,
                                               G4ProcessType type)
             : G4VDiscreteProcess(processName, type)
{
        if ( verboseLevel > 0) {
           G4cout << GetProcessName() << " is created " << G4endl;
        }

        SetProcessSubType(fOpBoundary);

	theStatus = Undefined;
	theModel = glisur;
	theFinish = polished;
        theReflectivity =  1.;
        theEfficiency   =  0.;
        theTransmittance = 0.;

        theSurfaceRoughness = 0.;

        prob_sl = 0.;
        prob_ss = 0.;
        prob_bs = 0.;

        PropertyPointer  = NULL;
        PropertyPointer1 = NULL;
        PropertyPointer2 = NULL;

        Material1 = NULL;
        Material2 = NULL;

        OpticalSurface = NULL;

        kCarTolerance = G4GeometryTolerance::GetInstance()
                        ->GetSurfaceTolerance();

        iTE = iTM = 0;
        thePhotonMomentum = 0.;
        Rindex1 = Rindex2 = 1.;
        cost1 = cost2 = sint1 = sint2 = 0.;

        idx = idy = 0;
        DichroicVector = NULL;

        fInvokeSD = true;
}

// G4OpBoundaryProcess::G4OpBoundaryProcess(const G4OpBoundaryProcess &right)
// {
// }

        ////////////////
        // Destructors
        ////////////////

G4OpBoundaryProcess::~G4OpBoundaryProcess(){}

        ////////////
        // Methods
        ////////////

// PostStepDoIt
// ------------
//

G4VParticleChange*
G4OpBoundaryProcess::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)
{
        theStatus = Undefined;

        aParticleChange.Initialize(aTrack);
        aParticleChange.ProposeVelocity(aTrack.GetVelocity());

        // Get hyperStep from  G4ParallelWorldProcess
        //  NOTE: PostSetpDoIt of this process should be
        //        invoked after G4ParallelWorldProcess!

        const G4Step* pStep = &aStep;

        const G4Step* hStep = G4ParallelWorldProcess::GetHyperStep();

        if (hStep) pStep = hStep;

        G4bool isOnBoundary =
                (pStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary);

        if (isOnBoundary) {
           Material1 = pStep->GetPreStepPoint()->GetMaterial();
           Material2 = pStep->GetPostStepPoint()->GetMaterial();
        } else {
           theStatus = NotAtBoundary;
           if ( verboseLevel > 0) BoundaryProcessVerbose();
           return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
        }

        G4VPhysicalVolume* thePrePV  =
                               pStep->GetPreStepPoint() ->GetPhysicalVolume();
        G4VPhysicalVolume* thePostPV =
                               pStep->GetPostStepPoint()->GetPhysicalVolume();

        if ( verboseLevel > 0 ) {
           G4cout << " Photon at Boundary! " << G4endl;
           if (thePrePV)  G4cout << " thePrePV:  " << thePrePV->GetName()  << G4endl;
           if (thePostPV) G4cout << " thePostPV: " << thePostPV->GetName() << G4endl;
        }

	if (aTrack.GetStepLength()<=kCarTolerance/2){
	        theStatus = StepTooSmall;
                if ( verboseLevel > 0) BoundaryProcessVerbose();
	        return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
	}

        const G4DynamicParticle* aParticle = aTrack.GetDynamicParticle();

	thePhotonMomentum = aParticle->GetTotalMomentum();
        OldMomentum       = aParticle->GetMomentumDirection();
	OldPolarization   = aParticle->GetPolarization();

        if ( verboseLevel > 0 ) {
           G4cout << " Old Momentum Direction: " << OldMomentum     << G4endl;
           G4cout << " Old Polarization:       " << OldPolarization << G4endl;
        }

        G4ThreeVector theGlobalPoint = pStep->GetPostStepPoint()->GetPosition();

        G4bool valid;
        //  Use the new method for Exit Normal in global coordinates,
        //    which provides the normal more reliably.

        // ID of Navigator which limits step

        G4int hNavId = G4ParallelWorldProcess::GetHypNavigatorID();
        std::vector<G4Navigator*>::iterator iNav =
                G4TransportationManager::GetTransportationManager()->
                                         GetActiveNavigatorsIterator();
        theGlobalNormal =
                   (iNav[hNavId])->GetGlobalExitNormal(theGlobalPoint,&valid);

        if (valid) {
          theGlobalNormal = -theGlobalNormal;
        }
        else
        {
          G4ExceptionDescription ed;
          ed << " G4OpBoundaryProcess/PostStepDoIt(): "
                 << " The Navigator reports that it returned an invalid normal"
                 << G4endl;
          G4Exception("G4OpBoundaryProcess::PostStepDoIt", "OpBoun01",
                      EventMustBeAborted,ed,
                      "Invalid Surface Normal - Geometry must return valid surface normal");
        }

        if (OldMomentum * theGlobalNormal > 0.0) {
#ifdef G4OPTICAL_DEBUG
           G4ExceptionDescription ed;
           ed << " G4OpBoundaryProcess/PostStepDoIt(): "
              << " theGlobalNormal points in a wrong direction. "
              << G4endl;
           ed << "    The momentum of the photon arriving at interface (oldMomentum)"
              << " must exit the volume cross in the step. " << G4endl;
           ed << "  So it MUST have dot < 0 with the normal that Exits the new volume (globalNormal)." << G4endl;
           ed << "  >> The dot product of oldMomentum and global Normal is " << OldMomentum*theGlobalNormal << G4endl;
           ed << "     Old Momentum  (during step)     = " << OldMomentum << G4endl;
           ed << "     Global Normal (Exiting New Vol) = " << theGlobalNormal << G4endl;
           ed << G4endl;
           G4Exception("G4OpBoundaryProcess::PostStepDoIt", "OpBoun02",
                       EventMustBeAborted,  // Or JustWarning to see if it happens repeatedbly on one ray
                       ed,
                      "Invalid Surface Normal - Geometry must return valid surface normal pointing in the right direction");
#else
           theGlobalNormal = -theGlobalNormal;
#endif
        }

	G4MaterialPropertiesTable* aMaterialPropertiesTable;
        G4MaterialPropertyVector* Rindex;

	aMaterialPropertiesTable = Material1->GetMaterialPropertiesTable();
        if (aMaterialPropertiesTable) {
		Rindex = aMaterialPropertiesTable->GetProperty("RINDEX");
	}
	else {
	        theStatus = NoRINDEX;
                if ( verboseLevel > 0) BoundaryProcessVerbose();
                aParticleChange.ProposeLocalEnergyDeposit(thePhotonMomentum);
		aParticleChange.ProposeTrackStatus(fStopAndKill);
		return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
	}

        if (Rindex) {
		Rindex1 = Rindex->Value(thePhotonMomentum);
	}
	else {
	        theStatus = NoRINDEX;
                if ( verboseLevel > 0) BoundaryProcessVerbose();
                aParticleChange.ProposeLocalEnergyDeposit(thePhotonMomentum);
		aParticleChange.ProposeTrackStatus(fStopAndKill);
		return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
	}

        theReflectivity =  1.;
        theEfficiency   =  0.;
        theTransmittance = 0.;

        theSurfaceRoughness = 0.;

        theModel = glisur;
        theFinish = polished;

        G4SurfaceType type = dielectric_dielectric;

        Rindex = NULL;
        OpticalSurface = NULL;

        G4LogicalSurface* Surface = NULL;

        Surface = G4LogicalBorderSurface::GetSurface(thePrePV, thePostPV);

        if (Surface == NULL){
          G4bool enteredDaughter= (thePostPV->GetMotherLogical() ==
                                   thePrePV ->GetLogicalVolume());
	  if(enteredDaughter){
	    Surface =
              G4LogicalSkinSurface::GetSurface(thePostPV->GetLogicalVolume());
	    if(Surface == NULL)
	      Surface =
                G4LogicalSkinSurface::GetSurface(thePrePV->GetLogicalVolume());
	  }
	  else {
	    Surface =
              G4LogicalSkinSurface::GetSurface(thePrePV->GetLogicalVolume());
	    if(Surface == NULL)
	      Surface =
                G4LogicalSkinSurface::GetSurface(thePostPV->GetLogicalVolume());
	  }
	}

	if (Surface) OpticalSurface =
           dynamic_cast <G4OpticalSurface*> (Surface->GetSurfaceProperty());

	if (OpticalSurface) {

           type      = OpticalSurface->GetType();
	   theModel  = OpticalSurface->GetModel();
	   theFinish = OpticalSurface->GetFinish();

	   aMaterialPropertiesTable = OpticalSurface->
					GetMaterialPropertiesTable();

           if (aMaterialPropertiesTable) {

              if (theFinish == polishedbackpainted ||
                  theFinish == groundbackpainted ) {
                  Rindex = aMaterialPropertiesTable->GetProperty("RINDEX");
	          if (Rindex) {
                     Rindex2 = Rindex->Value(thePhotonMomentum);
                  }
                  else {
		     theStatus = NoRINDEX;
                     if ( verboseLevel > 0) BoundaryProcessVerbose();
                     aParticleChange.ProposeLocalEnergyDeposit(thePhotonMomentum);
                     aParticleChange.ProposeTrackStatus(fStopAndKill);
                     return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
                  }
              }

              PropertyPointer =
                      aMaterialPropertiesTable->GetProperty("REFLECTIVITY");
              PropertyPointer1 =
                      aMaterialPropertiesTable->GetProperty("REALRINDEX");
              PropertyPointer2 =
                      aMaterialPropertiesTable->GetProperty("IMAGINARYRINDEX");

              iTE = 1;
              iTM = 1;

              if (PropertyPointer) {

                 theReflectivity =
                          PropertyPointer->Value(thePhotonMomentum);

              } else if (PropertyPointer1 && PropertyPointer2) {

                 CalculateReflectivity();

              }

              PropertyPointer =
              aMaterialPropertiesTable->GetProperty("EFFICIENCY");
              if (PropertyPointer) {
                      theEfficiency =
                      PropertyPointer->Value(thePhotonMomentum);
              }

              PropertyPointer =
              aMaterialPropertiesTable->GetProperty("TRANSMITTANCE");
              if (PropertyPointer) {
                      theTransmittance =
                      PropertyPointer->Value(thePhotonMomentum);
              }

              if (aMaterialPropertiesTable->
                                     ConstPropertyExists("SURFACEROUGHNESS"))
                 theSurfaceRoughness = aMaterialPropertiesTable->
                                         GetConstProperty("SURFACEROUGHNESS");

	      if ( theModel == unified ) {
	        PropertyPointer =
		aMaterialPropertiesTable->GetProperty("SPECULARLOBECONSTANT");
	        if (PropertyPointer) {
                         prob_sl =
			 PropertyPointer->Value(thePhotonMomentum);
                } else {
                         prob_sl = 0.0;
                }

	        PropertyPointer =
		aMaterialPropertiesTable->GetProperty("SPECULARSPIKECONSTANT");
	        if (PropertyPointer) {
                         prob_ss =
			 PropertyPointer->Value(thePhotonMomentum);
                } else {
                         prob_ss = 0.0;
                }

	        PropertyPointer =
		aMaterialPropertiesTable->GetProperty("BACKSCATTERCONSTANT");
	        if (PropertyPointer) {
                         prob_bs =
			 PropertyPointer->Value(thePhotonMomentum);
                } else {
                         prob_bs = 0.0;
                }
	      }
	   }
           else if (theFinish == polishedbackpainted ||
                    theFinish == groundbackpainted ) {
                      aParticleChange.ProposeLocalEnergyDeposit(thePhotonMomentum);
                      aParticleChange.ProposeTrackStatus(fStopAndKill);
                      return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
           }
        }

        if (type == dielectric_dielectric ) {
           if (theFinish == polished || theFinish == ground ) {

	      if (Material1 == Material2){
		 theStatus = SameMaterial;
                 if ( verboseLevel > 0) BoundaryProcessVerbose();
		 return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
	      }
              aMaterialPropertiesTable =
                     Material2->GetMaterialPropertiesTable();
              if (aMaterialPropertiesTable)
                 Rindex = aMaterialPropertiesTable->GetProperty("RINDEX");
              if (Rindex) {
                 Rindex2 = Rindex->Value(thePhotonMomentum);
              }
              else {
		 theStatus = NoRINDEX;
                 if ( verboseLevel > 0) BoundaryProcessVerbose();
                 aParticleChange.ProposeLocalEnergyDeposit(thePhotonMomentum);
                 aParticleChange.ProposeTrackStatus(fStopAndKill);
                 return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
	      }
           }
        }
//-------------------------------my-begin---------------------------------------
        if (OpticalSurface) { type = OpticalSurface->GetType();
        if (type == thin_films) {
            if (Material1 == Material2){
                theStatus = SameMaterial;
                if ( verboseLevel > 0) BoundaryProcessVerbose();
                return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  	        }
            aMaterialPropertiesTable =
                Material1->GetMaterialPropertiesTable();
            if (aMaterialPropertiesTable) {
                Rindex = aMaterialPropertiesTable->GetProperty("RINDEX");
                if (Rindex) {
                    RindexReal.clear();
                    RindexImag.clear();
                    theThickness.clear();

                   Rindex2 = Rindex->Value(thePhotonMomentum);
                   RindexReal.push_back(Rindex2);
                   RindexImag.push_back(0.);
                   theThickness.push_back(1.);
                }
                else {
  		           theStatus = NoRINDEX;
                   if ( verboseLevel > 0) BoundaryProcessVerbose();
                   aParticleChange.ProposeLocalEnergyDeposit(thePhotonMomentum);
                   aParticleChange.ProposeTrackStatus(fStopAndKill);
                   return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
  	            }
            }

            aMaterialPropertiesTable = OpticalSurface->
     					GetMaterialPropertiesTable();
            if (aMaterialPropertiesTable) {
                if (aMaterialPropertiesTable->
                                       ConstPropertyExists("THINLAYERS")) {
                   theLayers = aMaterialPropertiesTable->
                                           GetConstProperty("THINLAYERS");
                    theStatus = ThinLayers;
                }
                else {
                    theStatus = NoThinLayers;
                    if ( verboseLevel > 0) BoundaryProcessVerbose();
                    aParticleChange.ProposeLocalEnergyDeposit(thePhotonMomentum);
                    aParticleChange.ProposeTrackStatus(fStopAndKill);
                    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
                }

                if (theLayers<=0) {
                    theStatus = NoThinLayers;
                    if ( verboseLevel > 0) BoundaryProcessVerbose();
                    aParticleChange.ProposeLocalEnergyDeposit(thePhotonMomentum);
                    aParticleChange.ProposeTrackStatus(fStopAndKill);
                    return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
                }

                for (int jj = 1; jj <= theLayers; jj++) {
                    G4String PropertyName = "REALRINDEX" + std::to_string(jj);
                    PropertyPointer =
                    aMaterialPropertiesTable->GetProperty(PropertyName);
                    if (PropertyPointer) {
                        RindexReal.push_back(PropertyPointer->Value(thePhotonMomentum));
                    }
                    PropertyName = "IMAGINARYRINDEX" + std::to_string(jj);
                    PropertyPointer =
                    aMaterialPropertiesTable->GetProperty(PropertyName);
                    if (PropertyPointer) {
                        RindexImag.push_back(PropertyPointer->Value(thePhotonMomentum));
                    }
                    PropertyName = "THICKNESS" + std::to_string(jj);
                    if (aMaterialPropertiesTable->
                                           ConstPropertyExists(PropertyName)) {
                       theThickness.push_back(aMaterialPropertiesTable->
                                               GetConstProperty(PropertyName));
                    }
                }
                PropertyPointer =
                aMaterialPropertiesTable->GetProperty("EFFICIENCY");
                if (PropertyPointer) {
                    theEfficiency = PropertyPointer->Value(thePhotonMomentum);
                } else {
                    theEfficiency = 0.;
                }
            }
        }
        }

    if (type == thin_films) {

        ThinFilms();
    }
//-------------------------------my-end_----------------------------------------

	else if (type == dielectric_metal) {

	  DielectricMetal();

	}
        else if (type == dielectric_LUT) {

          DielectricLUT();

        }
        else if (type == dielectric_dichroic) {

          DielectricDichroic();

        }
        else if (type == dielectric_dielectric) {

          if ( theFinish == polishedbackpainted ||
               theFinish == groundbackpainted ) {
             DielectricDielectric();
          }
          else {
             G4double rand = G4UniformRand();
             if ( rand > theReflectivity ) {
                if (rand > theReflectivity + theTransmittance) {
                   DoAbsorption();
                } else {
                   theStatus = Transmission;
                   NewMomentum = OldMomentum;
                   NewPolarization = OldPolarization;
                }
             }
             else {
                if ( theFinish == polishedfrontpainted ) {
                   DoReflection();
                }
                else if ( theFinish == groundfrontpainted ) {
                   theStatus = LambertianReflection;
                   DoReflection();
                }
                else {
                   DielectricDielectric();
                }
             }
          }
        }
        else {

          G4cerr << " Error: G4BoundaryProcess: illegal boundary type " << G4endl;
          return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);

        }

        NewMomentum = NewMomentum.unit();
        NewPolarization = NewPolarization.unit();

        if ( verboseLevel > 0) {
           G4cout << " New Momentum Direction: " << NewMomentum     << G4endl;
           G4cout << " New Polarization:       " << NewPolarization << G4endl;
           BoundaryProcessVerbose();
        }

        aParticleChange.ProposeMomentumDirection(NewMomentum);
        aParticleChange.ProposePolarization(NewPolarization);

        if ( theStatus == FresnelRefraction || theStatus == Transmission ) {
           G4MaterialPropertyVector* groupvel =
           Material2->GetMaterialPropertiesTable()->GetProperty("GROUPVEL");
           G4double finalVelocity = groupvel->Value(thePhotonMomentum);
           aParticleChange.ProposeVelocity(finalVelocity);
        }

        if ( theStatus == Detection && fInvokeSD ) InvokeSD(pStep);

        return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
}

void G4OpBoundaryProcess::BoundaryProcessVerbose() const
{
        if ( theStatus == Undefined )
                G4cout << " *** Undefined *** " << G4endl;
        if ( theStatus == Transmission )
                G4cout << " *** Transmission *** " << G4endl;
        if ( theStatus == FresnelRefraction )
                G4cout << " *** FresnelRefraction *** " << G4endl;
        if ( theStatus == FresnelReflection )
                G4cout << " *** FresnelReflection *** " << G4endl;
        if ( theStatus == TotalInternalReflection )
                G4cout << " *** TotalInternalReflection *** " << G4endl;
        if ( theStatus == LambertianReflection )
                G4cout << " *** LambertianReflection *** " << G4endl;
        if ( theStatus == LobeReflection )
                G4cout << " *** LobeReflection *** " << G4endl;
        if ( theStatus == SpikeReflection )
                G4cout << " *** SpikeReflection *** " << G4endl;
        if ( theStatus == BackScattering )
                G4cout << " *** BackScattering *** " << G4endl;
        if ( theStatus == PolishedLumirrorAirReflection )
                G4cout << " *** PolishedLumirrorAirReflection *** " << G4endl;
        if ( theStatus == PolishedLumirrorGlueReflection )
                G4cout << " *** PolishedLumirrorGlueReflection *** " << G4endl;
        if ( theStatus == PolishedAirReflection )
                G4cout << " *** PolishedAirReflection *** " << G4endl;
        if ( theStatus == PolishedTeflonAirReflection )
                G4cout << " *** PolishedTeflonAirReflection *** " << G4endl;
        if ( theStatus == PolishedTiOAirReflection )
                G4cout << " *** PolishedTiOAirReflection *** " << G4endl;
        if ( theStatus == PolishedTyvekAirReflection )
                G4cout << " *** PolishedTyvekAirReflection *** " << G4endl;
        if ( theStatus == PolishedVM2000AirReflection )
                G4cout << " *** PolishedVM2000AirReflection *** " << G4endl;
        if ( theStatus == PolishedVM2000GlueReflection )
                G4cout << " *** PolishedVM2000GlueReflection *** " << G4endl;
        if ( theStatus == EtchedLumirrorAirReflection )
                G4cout << " *** EtchedLumirrorAirReflection *** " << G4endl;
        if ( theStatus == EtchedLumirrorGlueReflection )
                G4cout << " *** EtchedLumirrorGlueReflection *** " << G4endl;
        if ( theStatus == EtchedAirReflection )
                G4cout << " *** EtchedAirReflection *** " << G4endl;
        if ( theStatus == EtchedTeflonAirReflection )
                G4cout << " *** EtchedTeflonAirReflection *** " << G4endl;
        if ( theStatus == EtchedTiOAirReflection )
                G4cout << " *** EtchedTiOAirReflection *** " << G4endl;
        if ( theStatus == EtchedTyvekAirReflection )
                G4cout << " *** EtchedTyvekAirReflection *** " << G4endl;
        if ( theStatus == EtchedVM2000AirReflection )
                G4cout << " *** EtchedVM2000AirReflection *** " << G4endl;
        if ( theStatus == EtchedVM2000GlueReflection )
                G4cout << " *** EtchedVM2000GlueReflection *** " << G4endl;
        if ( theStatus == GroundLumirrorAirReflection )
                G4cout << " *** GroundLumirrorAirReflection *** " << G4endl;
        if ( theStatus == GroundLumirrorGlueReflection )
                G4cout << " *** GroundLumirrorGlueReflection *** " << G4endl;
        if ( theStatus == GroundAirReflection )
                G4cout << " *** GroundAirReflection *** " << G4endl;
        if ( theStatus == GroundTeflonAirReflection )
                G4cout << " *** GroundTeflonAirReflection *** " << G4endl;
        if ( theStatus == GroundTiOAirReflection )
                G4cout << " *** GroundTiOAirReflection *** " << G4endl;
        if ( theStatus == GroundTyvekAirReflection )
                G4cout << " *** GroundTyvekAirReflection *** " << G4endl;
        if ( theStatus == GroundVM2000AirReflection )
                G4cout << " *** GroundVM2000AirReflection *** " << G4endl;
        if ( theStatus == GroundVM2000GlueReflection )
                G4cout << " *** GroundVM2000GlueReflection *** " << G4endl;
        if ( theStatus == Absorption )
                G4cout << " *** Absorption *** " << G4endl;
        if ( theStatus == Detection )
                G4cout << " *** Detection *** " << G4endl;
        if ( theStatus == NotAtBoundary )
                G4cout << " *** NotAtBoundary *** " << G4endl;
        if ( theStatus == SameMaterial )
                G4cout << " *** SameMaterial *** " << G4endl;
        if ( theStatus == StepTooSmall )
                G4cout << " *** StepTooSmall *** " << G4endl;
        if ( theStatus == NoRINDEX )
                G4cout << " *** NoRINDEX *** " << G4endl;
        if ( theStatus == Dichroic )
                G4cout << " *** Dichroic Transmission *** " << G4endl;
}

G4ThreeVector
G4OpBoundaryProcess::GetFacetNormal(const G4ThreeVector& Momentum,
			            const G4ThreeVector&  Normal ) const
{
        G4ThreeVector FacetNormal;

	if (theModel == unified || theModel == LUT) {

	/* This function code alpha to a random value taken from the
           distribution p(alpha) = g(alpha; 0, sigma_alpha)*std::sin(alpha),
           for alpha > 0 and alpha < 90, where g(alpha; 0, sigma_alpha)
           is a gaussian distribution with mean 0 and standard deviation
           sigma_alpha.  */

	   G4double alpha;

	   G4double sigma_alpha = 0.0;
	   if (OpticalSurface) sigma_alpha = OpticalSurface->GetSigmaAlpha();

           if (sigma_alpha == 0.0) return FacetNormal = Normal;

	   G4double f_max = std::min(1.0,4.*sigma_alpha);

           G4double phi, SinAlpha, CosAlpha, SinPhi, CosPhi, unit_x, unit_y, unit_z;
           G4ThreeVector tmpNormal;

           do {
	      do {
	         alpha = G4RandGauss::shoot(0.0,sigma_alpha);
                // Loop checking, 13-Aug-2015, Peter Gumplinger
	      } while (G4UniformRand()*f_max > std::sin(alpha) || alpha >= halfpi );

	      phi = G4UniformRand()*twopi;

	      SinAlpha = std::sin(alpha);
	      CosAlpha = std::cos(alpha);
              SinPhi = std::sin(phi);
              CosPhi = std::cos(phi);

              unit_x = SinAlpha * CosPhi;
              unit_y = SinAlpha * SinPhi;
              unit_z = CosAlpha;

	      FacetNormal.setX(unit_x);
	      FacetNormal.setY(unit_y);
	      FacetNormal.setZ(unit_z);

              tmpNormal = Normal;

	      FacetNormal.rotateUz(tmpNormal);
             // Loop checking, 13-Aug-2015, Peter Gumplinger
	   } while (Momentum * FacetNormal >= 0.0);
	}
	else {

	   G4double  polish = 1.0;
	   if (OpticalSurface) polish = OpticalSurface->GetPolish();

           if (polish < 1.0) {
              do {
                 G4ThreeVector smear;
                 do {
                    smear.setX(2.*G4UniformRand()-1.0);
                    smear.setY(2.*G4UniformRand()-1.0);
                    smear.setZ(2.*G4UniformRand()-1.0);
                   // Loop checking, 13-Aug-2015, Peter Gumplinger
                 } while (smear.mag()>1.0);
                 smear = (1.-polish) * smear;
                 FacetNormal = Normal + smear;
                // Loop checking, 13-Aug-2015, Peter Gumplinger
              } while (Momentum * FacetNormal >= 0.0);
              FacetNormal = FacetNormal.unit();
	   }
           else {
              FacetNormal = Normal;
           }
	}
	return FacetNormal;
}


//-------------------------------my-begin---------------------------------------

void G4OpBoundaryProcess::ThinFilms()
{
    //verboseLevel = 4;

    if ( theFinish == ground ) {
       theFacetNormal =
                 GetFacetNormal(OldMomentum, theGlobalNormal);
    } else {
       theFacetNormal = theGlobalNormal;
    }

    G4double PdotN = OldMomentum * theFacetNormal;
    //G4double EdotN = OldPolarization * theFacetNormal;

    cost1 = - PdotN;
    sint1 = std::sqrt(1.-cost1*cost1);

    //G4double incang = GetIncidentAngle();
    //G4complex theta0 = {incang, 0.};
    //G4double pi = acos(-1);
    G4complex II = {0.,1.};
    const G4int NN = theLayers + 1;
    //G4cout<< asin(sint1)*180/pi << G4endl;

    std::vector<G4complex> n;
    std::vector<G4complex> thickness;
    G4double lambda = h_Planck*c_light/thePhotonMomentum;

    for (int i = 0; i < NN; i++) {
        G4complex qq = {RindexReal[i], RindexImag[i]};
        n.push_back(qq);
        thickness.push_back(theThickness[i]);
    }

    std::vector<G4complex> sinth(NN);
    std::vector<G4complex> costh(NN);
    std::vector<G4complex> delta(NN);

    G4complex sinth0 = sint1;//std::sin(theta0);
    sinth.push_back(0.);
    sinth[0] = sinth0;
    for (int i = 1; i < NN; i++) {
        sinth.push_back(0.);
        sinth[i] = sinth[i-1]*n[i-1]/n[i];
    }

    G4complex u = {1.,0};
    for (int i = 0; i < NN; i++) {
        costh.push_back(0.);
        costh[i] = std::sqrt(u - sinth[i]*sinth[i]);
        delta.push_back(0.);
        delta[i] = 2.*pi/lambda*n[i]*costh[i]*thickness[i];
    }

    // G4cout<<"------------"<<G4endl;
    // for(int i = 0; i< NN; i++)
    //     G4cout<<n[i]<<G4endl;
    // G4cout<<"------------"<<G4endl;

    std::vector<std::vector<G4complex>> rijs(NN);
    std::vector<std::vector<G4complex>> rijp(NN);
    std::vector<std::vector<G4complex>> tijs(NN);
    std::vector<std::vector<G4complex>> tijp(NN);

    for (int i = 0; i < NN; i++) {
        for (int j = 0; j < NN; j++) {
            rijs[i].push_back(0.);
            rijs[i][j] = n[i]*costh[i] - n[j]*costh[j] ;
            rijs[i][j] = rijs[i][j]/(n[i]*costh[i] + n[j]*costh[j]);

            rijp[i].push_back(0.);
            rijp[i][j]  = n[j]*costh[i] - n[i]*costh[j];
            rijp[i][j] = rijp[i][j]/( n[j]*costh[i] + n[i]*costh[j] );

            tijs[i].push_back(0.);
            tijs[i][j] = 2.*n[i]*costh[i];
            tijs[i][j] = tijs[i][j]/(n[i]*costh[i] + n[j]*costh[j]);

            tijp[i].push_back(0.);
            tijp[i][j] = 2.*n[i]*costh[i];
            tijp[i][j] = tijp[i][j]/( n[j]*costh[i] + n[i]*costh[j] );
        }
    }


    // //new way!!!!!!!!!!!11
    // //s polarization
    // G4complex Msbefore[2][2];
    // G4complex Mpbefore[2][2];
    //
    // G4complex M[2][2];
    // G4complex Mn[2][2];
    // M[0][0] = 1./tijs[0][1];
    // M[0][1] = rijs[0][1]/tijs[0][1];
    // M[1][0] = rijs[0][1]/tijs[0][1];
    // M[1][1] = 1./tijs[0][1];
    //
    // Msbefore[0][0] = M[0][0];
    // Msbefore[0][1] = M[0][1];
    // Msbefore[1][0] = M[1][0];
    // Msbefore[1][1] = M[1][1];
    //
    // for (int i = 1; i < NN-1; i++) {
    //     Mn[0][0] = std::exp(-II*delta[i]) /tijs[i][i+1];
    //     Mn[0][1] = rijs[i][i+1]*std::exp(-II*delta[i]) /tijs[i][i+1];
    //     Mn[1][0] = rijs[i][i+1]*std::exp(II*delta[i]) /tijs[i][i+1];
    //     Mn[1][1] = std::exp(II*delta[i]) /tijs[i][i+1];
    //     multiplyComplex(M, Mn, M);
    //
    //     if (i == NN-3) {
    //         Msbefore[0][0] = M[0][0];
    //         Msbefore[0][1] = M[0][1];
    //         Msbefore[1][0] = M[1][0];
    //         Msbefore[1][1] = M[1][1];
    //     }
    //
    // }
    //
    // G4complex rs = M[1][0]/M[0][0];
    // G4complex ts = 1./M[0][0];
    //
    // G4complex Rs = rs*conj(rs);
    // G4complex Ts = ts*conj(ts) * std::real(n[NN-1]*costh[NN-1])/std::real(n[0]*costh[0]);
    // G4complex As = u-Rs-Ts;
    //
    // //std::cout<<"Rs = " << Rs.real() << ", Ts = " << Ts.real()<<std::endl;
    // //std::cout<<"Rs = " << Rs.imag() << ", Ts = " << Ts.imag()<<std::endl;
    //
    // //p polarization
    // M[0][0] = 1./tijs[0][1];
    // M[0][1] = rijs[0][1]/tijs[0][1];
    // M[1][0] = rijs[0][1]/tijs[0][1];
    // M[1][1] = 1./tijs[0][1];
    //
    // Mpbefore[0][0] = M[0][0];
    // Mpbefore[0][1] = M[0][1];
    // Mpbefore[1][0] = M[1][0];
    // Mpbefore[1][1] = M[1][1];
    //
    // for (int i = 1; i < NN-1; i++) {
    //     Mn[0][0] = std::exp(-II*delta[i]) /tijp[i][i+1];
    //     Mn[0][1] = rijp[i][i+1]*std::exp(-II*delta[i]) /tijp[i][i+1];
    //     Mn[1][0] = rijp[i][i+1]*std::exp(II*delta[i]) /tijp[i][i+1];
    //     Mn[1][1] = std::exp(II*delta[i]) /tijp[i][i+1];
    //     multiplyComplex(M, Mn, M);
    //
    //     if (i == NN-3) {
    //         Mpbefore[0][0] = M[0][0];
    //         Mpbefore[0][1] = M[0][1];
    //         Mpbefore[1][0] = M[1][0];
    //         Mpbefore[1][1] = M[1][1];
    //     }
    //
    // }
    //
    // G4complex rp = M[1][0]/M[0][0];
    // G4complex tp = 1./M[0][0];
    //
    // G4complex Rp = rp*std::conj(rp);
    // G4complex Tp = tp*std::conj(tp) * std::real(n[NN-1]*conj(costh[NN-1]))/std::real(n[0]*costh[0]);
    // G4complex Ap = u-Rp-Tp;
    //
    // G4complex R = (Rs+Rp)/2.;
    // G4complex T = (Ts+Tp)/2.;
    // G4complex A = (As+Ap)/2.;



    //s polarization
    G4complex M[2][2];
    G4complex Mn[2][2];
    M[0][0] = {1.,0.};
    M[1][1] = {1.,0.};
    M[0][1] = M[1][0] = {0.,0.};
    for (int i = 1; i < NN-1; i++) {
        Mn[0][0] = std::exp(-II*delta[i]) /tijs[i-1][i];
        Mn[0][1] = rijs[i-1][i]*std::exp(II*delta[i]) /tijs[i-1][i];
        Mn[1][0] = rijs[i-1][i]*std::exp(-II*delta[i]) /tijs[i-1][i];
        Mn[1][1] = std::exp(II*delta[i]) /tijs[i-1][i];
        multiplyComplex(M, Mn, M);
    }

    G4complex Mend[2][2];
    Mend[0][0] = 1./tijs[NN-2][NN-1];
    Mend[0][1] = rijs[NN-2][NN-1] /tijs[NN-2][NN-1];
    Mend[1][0] = rijs[NN-2][NN-1] /tijs[NN-2][NN-1];
    Mend[1][1] = 1./tijs[NN-2][NN-1];

    multiplyComplex(M, Mend, M);

    G4complex rs = M[1][0]/M[0][0];
    G4complex ts = 1./M[0][0];

    G4complex Rs = rs*conj(rs);
    G4complex Ts = ts*conj(ts) * std::real(n[NN-1]*costh[NN-1])/std::real(n[0]*costh[0]);
    G4complex As = u-Rs-Ts;

    //std::cout<<"Rs = " << Rs.real() << ", Ts = " << Ts.real()<<std::endl;
    //std::cout<<"Rs = " << Rs.imag() << ", Ts = " << Ts.imag()<<std::endl;


    //p polarization
    M[0][0] = {1.,0.};
    M[1][1] = {1.,0.};
    M[0][1] = M[1][0] = {0.,0.};
    for (int i = 1; i < NN-1; i++) {
        Mn[0][0] = std::exp(-II*delta[i]) /tijp[i-1][i];
        Mn[0][1] = rijp[i-1][i]*std::exp(II*delta[i]) /tijp[i-1][i];
        Mn[1][0] = rijp[i-1][i]*std::exp(-II*delta[i]) /tijp[i-1][i];
        Mn[1][1] = std::exp(II*delta[i]) /tijp[i-1][i];
        multiplyComplex(M, Mn, M);
    }

    Mend[0][0] = 1./tijp[NN-2][NN-1];
    Mend[0][1] = rijp[NN-2][NN-1] /tijp[NN-2][NN-1];
    Mend[1][0] = rijp[NN-2][NN-1] /tijp[NN-2][NN-1];
    Mend[1][1] = 1./tijp[NN-2][NN-1];

    multiplyComplex(M, Mend, M);

    G4complex rp = M[1][0]/M[0][0];
    G4complex tp = 1./M[0][0];

    G4complex Rp = rp*std::conj(rp);
    G4complex Tp = tp*std::conj(tp) * std::real(std::conj(n[NN-1])*costh[NN-1])/std::real(n[0]*costh[0]);
    G4complex Ap = u-Rp-Tp;

    G4complex R = (Rs+Rp)/2.;
    G4complex T = (Ts+Tp)/2.;
    G4complex A = (As+Ap)/2.;
    // G4cout<< real(R) << "\t\t" << real(T) <<"\t\t"<< real(A) << G4endl;

    //G4cout<< R << "\t\t" << T <<"\t\t"<< A << G4endl;

    // G4cout<< "\t\t\t\t"<<asin(sint1)*180/pi<<G4endl;
    // G4cout<<"R = "<<R<<" and T = "<< T <<" and Rs = "<<Rs<<" and Rp = "<< Rp << G4endl;


    // G4double PdotN = OldMomentum * theFacetNormal;
    // cost1 = -PdotN;


    G4double tempcheck = asin(sint1)*180/pi;
    if (std::abs(cost1) < 1.0 - kCarTolerance) {
        sint1 = std::sqrt(1. - cost1*cost1);
    } else {
        sint1 = 0.0;
    }
    tempcheck -= asin(sint1)*180/pi;

    G4ThreeVector A_trans, A_paral, E1pp, E1pl;
    G4double E1_perp, E1_parl;

    if (sint1 > 0.0 ) {
        A_trans = OldMomentum.cross(theFacetNormal);
        A_trans = A_trans.unit();
        E1_perp = OldPolarization * A_trans;
        E1pp    = E1_perp * A_trans;
        E1pl    = OldPolarization - E1pp;
        E1_parl = E1pl.mag();
    }
    else {
        A_trans  = OldPolarization;
        // Here we Follow Jackson's conventions and we set the
        // parallel component = 1 in case of a ray perpendicular
        // to the surface
        E1_perp  = 0.0;
        E1_parl  = 1.0;
    }

    NewMomentum = OldMomentum - (2.*PdotN)*theFacetNormal;
    G4double EdotN = OldPolarization * theFacetNormal;

    A_paral   = NewMomentum.cross(A_trans);
    A_paral   = A_paral.unit();

    // This is my calculaton for reflectivity on a metalic surface
    // depending on the fraction of TE and TM polarization
    // when TE polarization, E1_parl=0 and E1_perp=1, R=abs(rTE)^2 and
    // when TM polarization, E1_parl=1 and E1_perp=0, R=abs(rTM)^2

    Rp =  Rp*(E1_perp*E1_perp)
                      / (E1_perp*E1_perp + E1_parl*E1_parl);
    Rs =  Rs*(E1_parl*E1_parl)
                      / (E1_perp*E1_perp + E1_parl*E1_parl);
    R = Rs + Rp;

    iTE = -1;
    iTM = -1;
    do {
       if(G4UniformRand()*real(R) > real(Rs))
         {iTE = -1;}else{iTE = 1;}
       if(G4UniformRand()*real(R) > real(Rp))
         {iTM = -1;}else{iTM = 1;}
      // Loop checking, 13-Aug-2015, Peter Gumplinger
    } while(iTE<0&&iTM<0);


    if(iTE>0&&iTM>0) {
      NewPolarization =
            -OldPolarization + (2.*EdotN)*theFacetNormal;
    } else if (iTE>0) {
      NewPolarization = -A_trans;
    } else if (iTM>0) {
      NewPolarization = -A_paral;
    }


    G4double Unirand = G4UniformRand();
    if (Unirand<=real(R)) {
        //tipota de xreiazetai
        //DoReflection();
    }
    else {


        //Cr layer
        // G4complex coefs = std::imag(n[NN-3]*costh[NN-3]*delta[NN-3])/std::real(n[0]*costh[0]);
        // G4complex A1s = coefs*rs*std::conj(rs);
        // G4complex A2s = coefs*ts*std::conj(ts);
        // G4complex A3s = coefs*ts*std::conj(rs);
        //
        // G4complex coefp = 2*std::imag(delta[NN-3])*std::real(n[NN-3]*std::conj(costh[NN-3]))/std::real(n[0]*costh[0]);
        // G4complex A1p = coefp*rp*std::conj(rp);
        // G4complex A2p = coefp*tp*std::conj(tp);
        //
        // coefp =  2*std::real(delta[NN-3])*std::imag(n[NN-3]*std::conj(costh[NN-3]))/std::real(n[0]*costh[0]);
        // G4complex A3p = coefp*tp*std::conj(rp);
        //
        // G4complex zz = {0.,0.};
        // for (int i= 1; i < NN-2; i++) {
        //     zz+= thickness[i];
        // }
        // G4complex AsLayer = A1s*std::exp(2.*zz*std::imag(delta[NN-3]));
        // AsLayer += A2s*std::exp(-2.*zz*std::imag(delta[NN-3]));
        // AsLayer += A3s*std::exp(2.*II*zz*std::real(delta[NN-3]));
        // AsLayer += std::conj(A3s)*std::exp(-2.*II*zz*std::real(delta[NN-3]));
        //
        // G4complex ApLayer = A1p*std::exp(2.*zz*std::imag(delta[NN-3]));
        // ApLayer += A2p*std::exp(-2.*zz*std::imag(delta[NN-3]));
        // ApLayer += A3p*std::exp(2.*II*zz*std::real(delta[NN-3]));
        // ApLayer += std::conj(A3p)*std::exp(-2.*II*zz*std::real(delta[NN-3]));

        // for (int i = 0; i < NN; i++) delta[i] /= thickness[i];
        //
        // rs = Msbefore[1][0]/Msbefore[0][0];
        // ts = 1./Msbefore[0][0];
        //
        // rp = Mpbefore[1][0]/Mpbefore[0][0];
        // tp = 1./Mpbefore[0][0];
        //
        // G4complex coefs = std::imag(n[NN-2]*costh[NN-2]*delta[NN-2])/std::real(n[0]*costh[0]);
        // G4complex A1s = coefs*rs*std::conj(rs);
        // G4complex A2s = coefs*ts*std::conj(ts);
        // G4complex A3s = coefs*ts*std::conj(rs);
        //
        // G4complex coefp = 2*std::imag(delta[NN-2])*std::real(n[NN-2]*std::conj(costh[NN-2]))/std::real(n[0]*costh[0]);
        // G4complex A1p = coefp*rp*std::conj(rp);
        // G4complex A2p = coefp*tp*std::conj(tp);
        //
        // coefp =  2*std::real(delta[NN-2])*std::imag(n[NN-2]*std::conj(costh[NN-2]))/std::real(n[0]*costh[0]);
        // G4complex A3p = coefp*tp*std::conj(rp);
        //
        // G4complex zz = thickness[NN-2];
        //
        // G4complex AsLayer = A1s*std::exp(2.*zz*std::imag(delta[NN-2]));
        // AsLayer += A2s*std::exp(-2.*zz*std::imag(delta[NN-2]));
        // AsLayer += A3s*std::exp(2.*II*zz*std::real(delta[NN-2]));
        // AsLayer += std::conj(A3s)*std::exp(-2.*II*zz*std::real(delta[NN-2]));
        //
        // G4complex ApLayer = A1p*std::exp(2.*zz*std::imag(delta[NN-2]));
        // ApLayer += A2p*std::exp(-2.*zz*std::imag(delta[NN-2]));
        // ApLayer += A3p*std::exp(2.*II*zz*std::real(delta[NN-2]));
        // ApLayer += std::conj(A3p)*std::exp(-2.*II*zz*std::real(delta[NN-2]));
        //
        // AsLayer = u - std::exp(-AsLayer*zz);
        // ApLayer = u - std::exp(-ApLayer*zz);

        // AsLayer = zz*AsLayer/std::log(10);
        // ApLayer = zz*ApLayer/std::log(10);

        // G4complex zz2 = {0.,0.};
        // for (int i= 1; i < NN-2; i++) {
        //     zz2+= thickness[i];
        // }
        //
        // G4complex AsLayer2 = A1s*std::exp(2.*zz2*std::imag(delta[NN-2]));
        // AsLayer2 += A2s*std::exp(-2.*zz2*std::imag(delta[NN-2]));
        // AsLayer2 += A3s*std::exp(2.*II*zz2*std::real(delta[NN-2]));
        // AsLayer2 += std::conj(A3s)*std::exp(-2.*II*zz2*std::real(delta[NN-2]));
        //
        // G4complex ApLayer2 = A1p*std::exp(2.*zz2*std::imag(delta[NN-2]));
        // ApLayer2 += A2p*std::exp(-2.*zz2*std::imag(delta[NN-2]));
        // ApLayer2 += A3p*std::exp(2.*II*zz2*std::real(delta[NN-2]));
        // ApLayer2 += std::conj(A3p)*std::exp(-2.*II*zz2*std::real(delta[NN-2]));
        //
        // // AsLayer2 = u - std::exp(-AsLayer*zz2);
        // // ApLayer2 = u - std::exp(-ApLayer*zz2);
        //
        // AsLayer2 = zz2*AsLayer2/std::log(10);
        // ApLayer2 = zz2*ApLayer2/std::log(10);
        //
        // G4complex AsLayer3 = A1s*std::exp(2.*zz2*std::imag(delta[NN-3]));
        // AsLayer3 += A2s*std::exp(-2.*zz2*std::imag(delta[NN-3]));
        // AsLayer3 += A3s*std::exp(2.*II*zz2*std::real(delta[NN-3]));
        // AsLayer3 += std::conj(A3s)*std::exp(-2.*II*zz2*std::real(delta[NN-3]));
        //
        // G4complex ApLayer3 = A1p*std::exp(2.*zz2*std::imag(delta[NN-3]));
        // ApLayer3 += A2p*std::exp(-2.*zz2*std::imag(delta[NN-3]));
        // ApLayer3 += A3p*std::exp(2.*II*zz2*std::real(delta[NN-3]));
        // ApLayer3 += std::conj(A3p)*std::exp(-2.*II*zz2*std::real(delta[NN-3]));
        //
        // // AsLayer2 = u - std::exp(-AsLayer*zz2);
        // // ApLayer2 = u - std::exp(-ApLayer*zz2);
        //
        // AsLayer3 = zz2*AsLayer3/std::log(10);
        // ApLayer3 = zz2*ApLayer3/std::log(10);



        // //cancer with integration
        //
        // // G4complex coefs = std::imag(n[NN-2]*costh[NN-2]*delta[NN-2])/std::real(n[0]*costh[0]);
        // // G4complex A1s = coefs*rs*std::conj(rs);
        // // G4complex A2s = coefs*ts*std::conj(ts);
        // // G4complex A3s = coefs*ts*std::conj(rs);
        // //
        // // G4complex coefp = 2*std::imag(delta[NN-2])*std::real(n[NN-2]*std::conj(costh[NN-2]))/std::real(n[0]*costh[0]);
        // // G4complex A1p = coefp*rp*std::conj(rp);
        // // G4complex A2p = coefp*tp*std::conj(tp);
        // //
        // // coefp =  2*std::real(delta[NN-2])*std::imag(n[NN-2]*std::conj(costh[NN-2]))/std::real(n[0]*costh[0]);
        // // G4complex A3p = coefp*tp*std::conj(rp);
        // //
        // // G4complex zzbeg = {0.,0.};
        // // for (int i= 1; i < NN-2; i++)
        // //     zzbeg+= thickness[i];
        // //
        // // G4complex zzfin = {0.,0.};
        // // for (int i= 1; i < NN-1; i++)
        // //     zzbeg+= thickness[i];
        // //
        // // zzbeg = {5.5*nm,0};
        // // zzfin = {23.5*nm,0};
        // G4complex AsLayer = 2.*std::imag(delta[NN-2])*A1s*std::exp(2.*zzfin*std::imag(delta[NN-2]));
        // AsLayer += -2.*std::imag(delta[NN-2])*A2s*std::exp(-2.*zzfin*std::imag(delta[NN-2]));
        // AsLayer += 2.*II*std::real(delta[NN-2])*A3s*std::exp(2.*II*zzfin*std::real(delta[NN-2]));
        // AsLayer += -2.*II*std::real(delta[NN-2])*std::conj(A3s)*std::exp(-2.*II*zzfin*std::real(delta[NN-2]));
        //
        // G4complex ApLayer = 2.*std::imag(delta[NN-2])*A1p*std::exp(2.*zzfin*std::imag(delta[NN-2]));
        // ApLayer += -2.*std::imag(delta[NN-2])*A2p*std::exp(-2.*zzfin*std::imag(delta[NN-2]));
        // ApLayer += 2.*II*std::real(delta[NN-2])*A3p*std::exp(2.*II*zzfin*std::real(delta[NN-2]));
        // ApLayer += -2.*II*std::real(delta[NN-2])*std::conj(A3p)*std::exp(-2.*II*zzfin*std::real(delta[NN-2]));
        //
        // AsLayer -= 2.*std::imag(delta[NN-2])*A1s*std::exp(2.*zzbeg*std::imag(delta[NN-2]));
        // AsLayer -= -2.*std::imag(delta[NN-2])*A2s*std::exp(-2.*zzbeg*std::imag(delta[NN-2]));
        // AsLayer -= 2.*II*std::real(delta[NN-2])*A3s*std::exp(2.*II*zzbeg*std::real(delta[NN-2]));
        // AsLayer -= -2.*II*std::real(delta[NN-2])*std::conj(A3s)*std::exp(-2.*II*zzbeg*std::real(delta[NN-2]));
        //
        // ApLayer -= 2.*std::imag(delta[NN-2])*A1p*std::exp(2.*zzbeg*std::imag(delta[NN-2]));
        // ApLayer -= -2.*std::imag(delta[NN-2])*A2p*std::exp(-2.*zzbeg*std::imag(delta[NN-2]));
        // ApLayer -= 2.*II*std::real(delta[NN-2])*A3p*std::exp(2.*II*zzbeg*std::real(delta[NN-2]));
        // ApLayer -= -2.*II*std::real(delta[NN-2])*std::conj(A3p)*std::exp(-2.*II*zzbeg*std::real(delta[NN-2]));


        // G4complex AsLayer = A1s*std::exp(2.*zz*std::imag(delta[NN-3]));
        // AsLayer += A2s*std::exp(-2.*zz*std::imag(delta[NN-3]));
        // AsLayer += A3s*std::exp(2.*II*zz*std::real(delta[NN-3]));
        // AsLayer += std::conj(A3s)*std::exp(-2.*II*zz*std::real(delta[NN-3]));
        //
        // G4complex ApLayer = A1p*std::exp(2.*zz*std::imag(delta[NN-3]));
        // ApLayer += A2p*std::exp(-2.*zz*std::imag(delta[NN-3]));
        // ApLayer += A3p*std::exp(2.*II*zz*std::real(delta[NN-3]));
        // ApLayer += std::conj(A3p)*std::exp(-2.*II*zz*std::real(delta[NN-3]));
        //

        // M[0][0] = {1.,0.};
        // M[1][1] = {1.,0.};
        // M[0][1] = M[1][0] = {0.,0.};
        // for (int i = 1; i < NN-2; i++) {
        //     Mn[0][0] = std::exp(-II*delta[i]) /tijs[i-1][i];
        //     Mn[0][1] = rijs[i-1][i]*std::exp(II*delta[i]) /tijs[i-1][i];
        //     Mn[1][0] = rijs[i-1][i]*std::exp(-II*delta[i]) /tijs[i-1][i];
        //     Mn[1][1] = std::exp(II*delta[i]) /tijs[i-1][i];
        //     multiplyComplex(M, Mn, M);
        // }
        //
        // Mend[0][0] = 1./tijs[NN-3][NN-2];
        // Mend[0][1] = rijs[NN-3][NN-2] /tijs[NN-3][NN-2];
        // Mend[1][0] = rijs[NN-3][NN-2] /tijs[NN-3][NN-2];
        // Mend[1][1] = 1./tijs[NN-3][NN-2];
        //
        // multiplyComplex(M, Mend, M);
        //
        // rs = M[1][0]/M[0][0];
        // ts = 1./M[0][0];
        //
        // G4complex RsB = rs*conj(rs);
        // G4complex TsB = ts*conj(ts) * std::real(n[NN-2]*costh[NN-2])/std::real(n[0]*costh[0]);
        // G4complex AsB = u-RsB-TsB;
        //
        //
        //
        // //p polarization
        // M[0][0] = {1.,0.};
        // M[1][1] = {1.,0.};
        // M[0][1] = M[1][0] = {0.,0.};
        // for (int i = 1; i < NN-1; i++) {
        //     Mn[0][0] = std::exp(-II*delta[i]) /tijp[i-1][i];
        //     Mn[0][1] = rijp[i-1][i]*std::exp(II*delta[i]) /tijp[i-1][i];
        //     Mn[1][0] = rijp[i-1][i]*std::exp(-II*delta[i]) /tijp[i-1][i];
        //     Mn[1][1] = std::exp(II*delta[i]) /tijp[i-1][i];
        //     multiplyComplex(M, Mn, M);
        // }
        //
        // Mend[0][0] = 1./tijp[NN-3][NN-2];
        // Mend[0][1] = rijp[NN-3][NN-2] /tijp[NN-3][NN-2];
        // Mend[1][0] = rijp[NN-3][NN-2] /tijp[NN-3][NN-2];
        // Mend[1][1] = 1./tijp[NN-3][NN-2];
        //
        // multiplyComplex(M, Mend, M);
        //
        // rp = M[1][0]/M[0][0];
        // tp = 1./M[0][0];
        //
        // G4complex RpB = rp*std::conj(rp);
        // G4complex TpB = tp*std::conj(tp) * std::real(n[NN-2]*costh[NN-2])/std::real(n[0]*costh[0]);
        // G4complex ApB = u-RpB-TpB;
        //
        // G4complex RB = (RsB+RpB)/2.;
        // G4complex TB = (TsB+TpB)/2.;
        // G4complex AB = (AsB+ApB)/2.;
        //
        //
        // RpB =  RpB*(E1_perp*E1_perp)
        //                   / (E1_perp*E1_perp + E1_parl*E1_parl);
        // RsB =  RsB*(E1_parl*E1_parl)
        //                   / (E1_perp*E1_perp + E1_parl*E1_parl);
        // RB = RsB + RpB;
        //
        // ApB =  ApB*(E1_perp*E1_perp)
        //                   / (E1_perp*E1_perp + E1_parl*E1_parl);
        // AsB =  AsB*(E1_parl*E1_parl)
        //                   / (E1_perp*E1_perp + E1_parl*E1_parl);
        // AB = AsB + ApB;
        // TB = u-RB-AB;

        //this is no layer
        // for (int i = 0; i < NN; i++) delta[i] /= thickness[i];
        // ApLayer = std::imag(n[NN-2]*std::conj(costh[NN-2])*( delta[NN-2] - std::conj(delta[NN-2])))/std::real(n[0]*costh[0]);
        // AsLayer = std::imag(n[NN-2]*costh[NN-2]*delta[NN-2])/std::real(n[0]*costh[0]);
        //
        // zz = {0.,0.};
        // for (int i = 2; i<NN-1; i++) zz += thickness[i];
        // ApLayer = zz*ApLayer;///std::log(10);
        // AsLayer = zz*AsLayer;///std::log(10);
        //
        //down is -ApLayer because I had to to 2-3 instead of 3-2
        // ApLayer =  ApLayer*(E1_perp*E1_perp)
        //                   / (E1_perp*E1_perp + E1_parl*E1_parl);
        // AsLayer =  AsLayer*(E1_parl*E1_parl)
        //                   / (E1_perp*E1_perp + E1_parl*E1_parl);
        // G4complex ALayer = AsLayer + ApLayer;
//
//         ApLayer2 =  ApLayer2*(E1_perp*E1_perp)
//                           / (E1_perp*E1_perp + E1_parl*E1_parl);
//         AsLayer2 =  AsLayer2*(E1_parl*E1_parl)
//                           / (E1_perp*E1_perp + E1_parl*E1_parl);
//         G4complex ALayer2 = AsLayer2 + ApLayer2;
// //
//         ApLayer3 =  ApLayer3*(E1_perp*E1_perp)
//                           / (E1_perp*E1_perp + E1_parl*E1_parl);
//         AsLayer3 =  AsLayer3*(E1_parl*E1_parl)
//                           / (E1_perp*E1_perp + E1_parl*E1_parl);
//         G4complex ALayer3 = AsLayer3 + ApLayer3;


        //Multilayer optical calculations //28.8.2016

        Ap =  Ap*(E1_perp*E1_perp)
                          / (E1_perp*E1_perp + E1_parl*E1_parl);
        As =  As*(E1_parl*E1_parl)
                          / (E1_perp*E1_perp + E1_parl*E1_parl);
        A = As + Ap;

        Tp =  Tp*(E1_perp*E1_perp)
                          / (E1_perp*E1_perp + E1_parl*E1_parl);
        Ts =  Ts*(E1_parl*E1_parl)
                          / (E1_perp*E1_perp + E1_parl*E1_parl);
        T = Ts + Tp;

        // if (NN>3)
        //     G4cout<<A<<ALayer<<G4endl;
        theEfficiency *= std::real(A)/(std::real(A)+std::real(T));
        // theEfficiency /= (std::real(A)+std::real(T));
        // theEfficiency *= std::real(ALayer)/(std::real(A)+std::real(T));
        // theEfficiency *= std::real(A);
        DoAbsorption();
    }

    if (tempcheck)
        G4cout<< "\t\t\t\t"<<tempcheck<<G4endl;
    // if (theLayers>2)
    // if (asin(sint1)*180/pi>65 || asin(sint1)*180/pi<25)
    //     G4cout<<asin(sint1)*180/pi<< " with R = " << std::real(R)<< " and T = "<< std::real(T) <<"->"<<G4endl;
    // G4cout<< "\t\t\t\t"<<asin(sint1)*180/pi<<G4endl;
    // G4cout<<"R = "<<R<<" and A = "<< A <<"\tstop"<<G4endl;

    n.clear();
    thickness.clear();

    sinth.clear();
    costh.clear();
    delta.clear();

    rijs.clear();
    rijp.clear();
    tijs.clear();
    tijp.clear();

    RindexImag.clear();
    RindexReal.clear();
    theThickness.clear();

}


//-------------------------------my-end_----------------------------------------


void G4OpBoundaryProcess::DielectricMetal()
{
        G4int n = 0;
        G4double rand, PdotN, EdotN;
        G4ThreeVector A_trans, A_paral;

	do {

           n++;

           rand = G4UniformRand();
           if ( rand > theReflectivity && n == 1 ) {
              if (rand > theReflectivity + theTransmittance) {
                DoAbsorption();
              } else {
                theStatus = Transmission;
                NewMomentum = OldMomentum;
                NewPolarization = OldPolarization;
              }
              break;
           }
           else {

             if (PropertyPointer1 && PropertyPointer2) {
                if ( n > 1 ) {
                   CalculateReflectivity();
                   if ( !G4BooleanRand(theReflectivity) ) {
                      DoAbsorption();
                      break;
                   }
                }
             }

             if ( theModel == glisur || theFinish == polished ) {

                DoReflection();

             } else {

                if ( n == 1 ) ChooseReflection();

                if ( theStatus == LambertianReflection ) {
                   DoReflection();
                }
                else if ( theStatus == BackScattering ) {
                   NewMomentum = -OldMomentum;
                   NewPolarization = -OldPolarization;
                }
                else {

                   if(theStatus==LobeReflection){
                     if ( PropertyPointer1 && PropertyPointer2 ){
                     } else {
                        theFacetNormal =
                            GetFacetNormal(OldMomentum,theGlobalNormal);
                     }
                   }

                   PdotN = OldMomentum * theFacetNormal;
                   NewMomentum = OldMomentum - (2.*PdotN)*theFacetNormal;
                   EdotN = OldPolarization * theFacetNormal;

                   if (sint1 > 0.0 ) {
                      A_trans = OldMomentum.cross(theFacetNormal);
                      A_trans = A_trans.unit();
                   } else {
                      A_trans  = OldPolarization;
                   }
                   A_paral   = NewMomentum.cross(A_trans);
                   A_paral   = A_paral.unit();

                   if(iTE>0&&iTM>0) {
                     NewPolarization =
                           -OldPolarization + (2.*EdotN)*theFacetNormal;
                   } else if (iTE>0) {
                     NewPolarization = -A_trans;
                   } else if (iTM>0) {
                     NewPolarization = -A_paral;
                   }

                }

             }

             OldMomentum = NewMomentum;
             OldPolarization = NewPolarization;

	   }

          // Loop checking, 13-Aug-2015, Peter Gumplinger
	} while (NewMomentum * theGlobalNormal < 0.0);
}

void G4OpBoundaryProcess::DielectricLUT()
{
        G4int thetaIndex, phiIndex;
        G4double AngularDistributionValue, thetaRad, phiRad, EdotN;
        G4ThreeVector PerpendicularVectorTheta, PerpendicularVectorPhi;

        theStatus = G4OpBoundaryProcessStatus(G4int(theFinish) +
                           (G4int(NoRINDEX)-G4int(groundbackpainted)));

        G4int thetaIndexMax = OpticalSurface->GetThetaIndexMax();
        G4int phiIndexMax   = OpticalSurface->GetPhiIndexMax();

        G4double rand;

        do {
           rand = G4UniformRand();
           if ( rand > theReflectivity ) {
              if (rand > theReflectivity + theTransmittance) {
                 DoAbsorption();
              } else {
                 theStatus = Transmission;
                 NewMomentum = OldMomentum;
                 NewPolarization = OldPolarization;
              }
              break;
           }
           else {
              // Calculate Angle between Normal and Photon Momentum
              G4double anglePhotonToNormal =
                                          OldMomentum.angle(-theGlobalNormal);
              // Round it to closest integer
              G4int angleIncident = G4int(std::floor(180/pi*anglePhotonToNormal+0.5));

              // Take random angles THETA and PHI,
              // and see if below Probability - if not - Redo
              do {
                 thetaIndex = G4RandFlat::shootInt(thetaIndexMax-1);
                 phiIndex = G4RandFlat::shootInt(phiIndexMax-1);
                 // Find probability with the new indeces from LUT
                 AngularDistributionValue = OpticalSurface ->
                   GetAngularDistributionValue(angleIncident,
                                               thetaIndex,
                                               phiIndex);
                // Loop checking, 13-Aug-2015, Peter Gumplinger
              } while ( !G4BooleanRand(AngularDistributionValue) );

              thetaRad = (-90 + 4*thetaIndex)*pi/180;
              phiRad = (-90 + 5*phiIndex)*pi/180;
              // Rotate Photon Momentum in Theta, then in Phi
              NewMomentum = -OldMomentum;

              PerpendicularVectorTheta = NewMomentum.cross(theGlobalNormal);
              if (PerpendicularVectorTheta.mag() < kCarTolerance )
                          PerpendicularVectorTheta = NewMomentum.orthogonal();
              NewMomentum =
                 NewMomentum.rotate(anglePhotonToNormal-thetaRad,
                                    PerpendicularVectorTheta);
              PerpendicularVectorPhi =
                                  PerpendicularVectorTheta.cross(NewMomentum);
              NewMomentum = NewMomentum.rotate(-phiRad,PerpendicularVectorPhi);

              // Rotate Polarization too:
              theFacetNormal = (NewMomentum - OldMomentum).unit();
              EdotN = OldPolarization * theFacetNormal;
              NewPolarization = -OldPolarization + (2.*EdotN)*theFacetNormal;
           }
          // Loop checking, 13-Aug-2015, Peter Gumplinger
        } while (NewMomentum * theGlobalNormal <= 0.0);
}

void G4OpBoundaryProcess::DielectricDichroic()
{
        // Calculate Angle between Normal and Photon Momentum
        G4double anglePhotonToNormal = OldMomentum.angle(-theGlobalNormal);

        // Round it to closest integer
        G4double angleIncident = std::floor(180/pi*anglePhotonToNormal+0.5);

        if (!DichroicVector) {
           if (OpticalSurface) DichroicVector = OpticalSurface->GetDichroicVector();
        }


        if (DichroicVector) {
           G4double wavelength = h_Planck*c_light/thePhotonMomentum;
           theTransmittance =
             DichroicVector->Value(wavelength/nm,angleIncident,idx,idy)*perCent;
//            G4cout << "wavelength: " << std::floor(wavelength/nm)
//                                     << "nm" << G4endl;
//            G4cout << "Incident angle: " << angleIncident << "deg" << G4endl;
//            G4cout << "Transmittance: "
//                   << std::floor(theTransmittance/perCent) << "%" << G4endl;
        } else {
           G4ExceptionDescription ed;
           ed << " G4OpBoundaryProcess/DielectricDichroic(): "
              << " The dichroic surface has no G4Physics2DVector"
              << G4endl;
           G4Exception("G4OpBoundaryProcess::DielectricDichroic", "OpBoun03",
                       FatalException,ed,
                       "A dichroic surface must have an associated G4Physics2DVector");
        }

        if ( !G4BooleanRand(theTransmittance) ) { // Not transmitted, so reflect

           if ( theModel == glisur || theFinish == polished ) {
              DoReflection();
           } else {
              ChooseReflection();
              if ( theStatus == LambertianReflection ) {
                 DoReflection();
              } else if ( theStatus == BackScattering ) {
                 NewMomentum = -OldMomentum;
                 NewPolarization = -OldPolarization;
              } else {
                G4double PdotN, EdotN;
                do {
                   if (theStatus==LobeReflection)
                      theFacetNormal = GetFacetNormal(OldMomentum,theGlobalNormal);
                   PdotN = OldMomentum * theFacetNormal;
                   NewMomentum = OldMomentum - (2.*PdotN)*theFacetNormal;
                  // Loop checking, 13-Aug-2015, Peter Gumplinger
                } while (NewMomentum * theGlobalNormal <= 0.0);
                EdotN = OldPolarization * theFacetNormal;
                NewPolarization = -OldPolarization + (2.*EdotN)*theFacetNormal;
              }
           }

        } else {

           theStatus = Dichroic;
           NewMomentum = OldMomentum;
           NewPolarization = OldPolarization;

        }
}

void G4OpBoundaryProcess::DielectricDielectric()
{
	G4bool Inside = false;
	G4bool Swap = false;

        G4bool SurfaceRoughnessCriterionPass = 1;
        if (theSurfaceRoughness != 0. && Rindex1 > Rindex2) {
           G4double wavelength = h_Planck*c_light/thePhotonMomentum;
           G4double SurfaceRoughnessCriterion =
             std::exp(-std::pow((4*pi*theSurfaceRoughness*Rindex1*cost1/wavelength),2));
           SurfaceRoughnessCriterionPass =
                                     G4BooleanRand(SurfaceRoughnessCriterion);
        }

	leap:

        G4bool Through = false;
	G4bool Done = false;

        G4double PdotN, EdotN;

        G4ThreeVector A_trans, A_paral, E1pp, E1pl;
        G4double E1_perp, E1_parl;
        G4double s1, s2, E2_perp, E2_parl, E2_total, TransCoeff;
        G4double E2_abs, C_parl, C_perp;
        G4double alpha;

	do {

	   if (Through) {
	      Swap = !Swap;
	      Through = false;
	      theGlobalNormal = -theGlobalNormal;
	      G4SwapPtr(Material1,Material2);
	      G4SwapObj(&Rindex1,&Rindex2);
	   }

	   if ( theFinish == polished ) {
	        theFacetNormal = theGlobalNormal;
	   }
	   else {
	        theFacetNormal =
	                     GetFacetNormal(OldMomentum,theGlobalNormal);
	   }

	   PdotN = OldMomentum * theFacetNormal;
	   EdotN = OldPolarization * theFacetNormal;

	   cost1 = - PdotN;
	   if (std::abs(cost1) < 1.0-kCarTolerance){
	      sint1 = std::sqrt(1.-cost1*cost1);
	      sint2 = sint1*Rindex1/Rindex2;     // *** Snell's Law ***
	   }
	   else {
	      sint1 = 0.0;
	      sint2 = 0.0;
	   }

	   if (sint2 >= 1.0) {

	      // Simulate total internal reflection

	      if (Swap) Swap = !Swap;

              theStatus = TotalInternalReflection;

              if ( !SurfaceRoughnessCriterionPass ) theStatus =
                                                       LambertianReflection;

	      if ( theModel == unified && theFinish != polished )
						     ChooseReflection();

	      if ( theStatus == LambertianReflection ) {
		 DoReflection();
	      }
	      else if ( theStatus == BackScattering ) {
		 NewMomentum = -OldMomentum;
		 NewPolarization = -OldPolarization;
	      }
	      else {

                 PdotN = OldMomentum * theFacetNormal;
		 NewMomentum = OldMomentum - (2.*PdotN)*theFacetNormal;
		 EdotN = OldPolarization * theFacetNormal;
		 NewPolarization = -OldPolarization + (2.*EdotN)*theFacetNormal;

	      }
	   }
	   else if (sint2 < 1.0) {

	      // Calculate amplitude for transmission (Q = P x N)

	      if (cost1 > 0.0) {
	         cost2 =  std::sqrt(1.-sint2*sint2);
	      }
	      else {
	         cost2 = -std::sqrt(1.-sint2*sint2);
	      }

	      if (sint1 > 0.0) {
	         A_trans = OldMomentum.cross(theFacetNormal);
                 A_trans = A_trans.unit();
	         E1_perp = OldPolarization * A_trans;
                 E1pp    = E1_perp * A_trans;
                 E1pl    = OldPolarization - E1pp;
                 E1_parl = E1pl.mag();
              }
	      else {
	         A_trans  = OldPolarization;
	         // Here we Follow Jackson's conventions and we set the
	         // parallel component = 1 in case of a ray perpendicular
	         // to the surface
	         E1_perp  = 0.0;
	         E1_parl  = 1.0;
	      }

              s1 = Rindex1*cost1;
              E2_perp = 2.*s1*E1_perp/(Rindex1*cost1+Rindex2*cost2);
              E2_parl = 2.*s1*E1_parl/(Rindex2*cost1+Rindex1*cost2);
              E2_total = E2_perp*E2_perp + E2_parl*E2_parl;
              s2 = Rindex2*cost2*E2_total;

              if (theTransmittance > 0) TransCoeff = theTransmittance;
              else if (cost1 != 0.0) TransCoeff = s2/s1;
              else TransCoeff = 0.0;

	      if ( !G4BooleanRand(TransCoeff) ) {

	         // Simulate reflection

                 if (Swap) Swap = !Swap;

		 theStatus = FresnelReflection;

                 if ( !SurfaceRoughnessCriterionPass ) theStatus =
                                                          LambertianReflection;

		 if ( theModel == unified && theFinish != polished )
						     ChooseReflection();

		 if ( theStatus == LambertianReflection ) {
		    DoReflection();
		 }
		 else if ( theStatus == BackScattering ) {
		    NewMomentum = -OldMomentum;
		    NewPolarization = -OldPolarization;
		 }
		 else {

                    PdotN = OldMomentum * theFacetNormal;
	            NewMomentum = OldMomentum - (2.*PdotN)*theFacetNormal;

	            if (sint1 > 0.0) {   // incident ray oblique

		       E2_parl   = Rindex2*E2_parl/Rindex1 - E1_parl;
		       E2_perp   = E2_perp - E1_perp;
		       E2_total  = E2_perp*E2_perp + E2_parl*E2_parl;
                       A_paral   = NewMomentum.cross(A_trans);
                       A_paral   = A_paral.unit();
		       E2_abs    = std::sqrt(E2_total);
		       C_parl    = E2_parl/E2_abs;
		       C_perp    = E2_perp/E2_abs;

                       NewPolarization = C_parl*A_paral + C_perp*A_trans;

	            }

	            else {               // incident ray perpendicular

	               if (Rindex2 > Rindex1) {
		          NewPolarization = - OldPolarization;
	               }
	               else {
	                  NewPolarization =   OldPolarization;
	               }

	            }
	         }
	      }
	      else { // photon gets transmitted

	         // Simulate transmission/refraction

		 Inside = !Inside;
		 Through = true;
		 theStatus = FresnelRefraction;

	         if (sint1 > 0.0) {      // incident ray oblique

		    alpha = cost1 - cost2*(Rindex2/Rindex1);
		    NewMomentum = OldMomentum + alpha*theFacetNormal;
		    NewMomentum = NewMomentum.unit();
//                    PdotN = -cost2;
                    A_paral = NewMomentum.cross(A_trans);
                    A_paral = A_paral.unit();
		    E2_abs     = std::sqrt(E2_total);
		    C_parl     = E2_parl/E2_abs;
		    C_perp     = E2_perp/E2_abs;

                    NewPolarization = C_parl*A_paral + C_perp*A_trans;

	         }
	         else {                  // incident ray perpendicular

		    NewMomentum = OldMomentum;
		    NewPolarization = OldPolarization;

	         }
	      }
	   }

	   OldMomentum = NewMomentum.unit();
	   OldPolarization = NewPolarization.unit();

	   if (theStatus == FresnelRefraction) {
	      Done = (NewMomentum * theGlobalNormal <= 0.0);
	   }
	   else {
	      Done = (NewMomentum * theGlobalNormal >= -kCarTolerance);
	   }

          // Loop checking, 13-Aug-2015, Peter Gumplinger
	} while (!Done);

	if (Inside && !Swap) {
          if( theFinish == polishedbackpainted ||
              theFinish == groundbackpainted ) {

              G4double rand = G4UniformRand();
              if ( rand > theReflectivity ) {
                 if (rand > theReflectivity + theTransmittance) {
		    DoAbsorption();
                 } else {
                    theStatus = Transmission;
                    NewMomentum = OldMomentum;
                    NewPolarization = OldPolarization;
                 }
              }
	      else {
		if (theStatus != FresnelRefraction ) {
		   theGlobalNormal = -theGlobalNormal;
	        }
	        else {
		   Swap = !Swap;
		   G4SwapPtr(Material1,Material2);
		   G4SwapObj(&Rindex1,&Rindex2);
	        }
		if ( theFinish == groundbackpainted )
					theStatus = LambertianReflection;

	        DoReflection();

	        theGlobalNormal = -theGlobalNormal;
		OldMomentum = NewMomentum;

	        goto leap;
	      }
	  }
	}
}

// GetMeanFreePath
// ---------------
//
G4double G4OpBoundaryProcess::GetMeanFreePath(const G4Track& ,
                                              G4double ,
                                              G4ForceCondition* condition)
{
	*condition = Forced;

	return DBL_MAX;
}

G4double G4OpBoundaryProcess::GetIncidentAngle()
{
    G4double PdotN = OldMomentum * theFacetNormal;
    G4double magP= OldMomentum.mag();
    G4double magN= theFacetNormal.mag();
    G4double incidentangle = pi - std::acos(PdotN/(magP*magN));

    return incidentangle;
}

G4double G4OpBoundaryProcess::GetReflectivity(G4double E1_perp,
                                              G4double E1_parl,
                                              G4double incidentangle,
                                              G4double RealRindex,
                                              G4double ImaginaryRindex)
{
  G4complex Reflectivity, Reflectivity_TE, Reflectivity_TM;
  G4complex N1(Rindex1, 0), N2(RealRindex, ImaginaryRindex);
  G4complex CosPhi;

  G4complex u(1,0);           //unit number 1

  G4complex numeratorTE;      // E1_perp=1 E1_parl=0 -> TE polarization
  G4complex numeratorTM;      // E1_parl=1 E1_perp=0 -> TM polarization
  G4complex denominatorTE, denominatorTM;
  G4complex rTM, rTE;

  G4MaterialPropertiesTable* aMaterialPropertiesTable =
                                    Material1->GetMaterialPropertiesTable();
  G4MaterialPropertyVector* aPropertyPointerR =
                      aMaterialPropertiesTable->GetProperty("REALRINDEX");
  G4MaterialPropertyVector* aPropertyPointerI =
                      aMaterialPropertiesTable->GetProperty("IMAGINARYRINDEX");
  if (aPropertyPointerR && aPropertyPointerI) {
     G4double RRindex = aPropertyPointerR->Value(thePhotonMomentum);
     G4double IRindex = aPropertyPointerI->Value(thePhotonMomentum);
     N1 = G4complex(RRindex,IRindex);
  }

  // Following two equations, rTM and rTE, are from: "Introduction To Modern
  // Optics" written by Fowles

  CosPhi=std::sqrt(u-((std::sin(incidentangle)*std::sin(incidentangle))*(N1*N1)/(N2*N2)));

  numeratorTE   = N1*std::cos(incidentangle) - N2*CosPhi;
  denominatorTE = N1*std::cos(incidentangle) + N2*CosPhi;
  rTE = numeratorTE/denominatorTE;

  numeratorTM   = N2*std::cos(incidentangle) - N1*CosPhi;
  denominatorTM = N2*std::cos(incidentangle) + N1*CosPhi;
  rTM = numeratorTM/denominatorTM;

  // This is my calculaton for reflectivity on a metalic surface
  // depending on the fraction of TE and TM polarization
  // when TE polarization, E1_parl=0 and E1_perp=1, R=abs(rTE)^2 and
  // when TM polarization, E1_parl=1 and E1_perp=0, R=abs(rTM)^2

  Reflectivity_TE =  (rTE*conj(rTE))*(E1_perp*E1_perp)
                    / (E1_perp*E1_perp + E1_parl*E1_parl);
  Reflectivity_TM =  (rTM*conj(rTM))*(E1_parl*E1_parl)
                    / (E1_perp*E1_perp + E1_parl*E1_parl);
  Reflectivity    = Reflectivity_TE + Reflectivity_TM;

  do {
     if(G4UniformRand()*real(Reflectivity) > real(Reflectivity_TE))
       {iTE = -1;}else{iTE = 1;}
     if(G4UniformRand()*real(Reflectivity) > real(Reflectivity_TM))
       {iTM = -1;}else{iTM = 1;}
    // Loop checking, 13-Aug-2015, Peter Gumplinger
  } while(iTE<0&&iTM<0);

  return real(Reflectivity);

}

void G4OpBoundaryProcess::CalculateReflectivity()
{
  G4double RealRindex =
           PropertyPointer1->Value(thePhotonMomentum);
  G4double ImaginaryRindex =
           PropertyPointer2->Value(thePhotonMomentum);

  // calculate FacetNormal
  if ( theFinish == ground ) {
     theFacetNormal =
               GetFacetNormal(OldMomentum, theGlobalNormal);
  } else {
     theFacetNormal = theGlobalNormal;
  }

  G4double PdotN = OldMomentum * theFacetNormal;
  cost1 = -PdotN;

  if (std::abs(cost1) < 1.0 - kCarTolerance) {
     sint1 = std::sqrt(1. - cost1*cost1);
  } else {
     sint1 = 0.0;
  }

  G4ThreeVector A_trans, A_paral, E1pp, E1pl;
  G4double E1_perp, E1_parl;

  if (sint1 > 0.0 ) {
     A_trans = OldMomentum.cross(theFacetNormal);
     A_trans = A_trans.unit();
     E1_perp = OldPolarization * A_trans;
     E1pp    = E1_perp * A_trans;
     E1pl    = OldPolarization - E1pp;
     E1_parl = E1pl.mag();
  }
  else {
     A_trans  = OldPolarization;
     // Here we Follow Jackson's conventions and we set the
     // parallel component = 1 in case of a ray perpendicular
     // to the surface
     E1_perp  = 0.0;
     E1_parl  = 1.0;
  }

  //calculate incident angle
  G4double incidentangle = GetIncidentAngle();

  //calculate the reflectivity depending on incident angle,
  //polarization and complex refractive

  theReflectivity =
             GetReflectivity(E1_perp, E1_parl, incidentangle,
                                                 RealRindex, ImaginaryRindex);
}

G4bool G4OpBoundaryProcess::InvokeSD(const G4Step* pStep)
{
  G4Step aStep = *pStep;

  aStep.AddTotalEnergyDeposit(thePhotonMomentum);

  G4VSensitiveDetector* sd = aStep.GetPostStepPoint()->GetSensitiveDetector();
  if (sd) return sd->Hit(&aStep);
  else return false;
}
