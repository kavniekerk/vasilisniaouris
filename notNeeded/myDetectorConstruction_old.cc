
#include "myDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "G4PhysicalConstants.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4MaterialPropertiesTable.hh"

#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4OpticalSurface.hh"

#include "myPhotocathodeSD.hh"
#include "G4SDManager.hh"

//#include "CLHEP/Units/PhysicalConstants.h"
#include <cmath>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myDetectorConstruction::myDetectorConstruction()
: G4VUserDetectorConstruction()//,
  //fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

myDetectorConstruction::~myDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myDetectorConstruction::AddOpticalPropertiesMgF2(G4Material* matMgF2, G4double thickness)
{
    //from max to min
    G4double photonWavelength[] = {1.61*micrometer, 1.572*micrometer, 1.518*micrometer, 1.465*micrometer, 1.413*micrometer, 1.364*micrometer, 1.316*micrometer, 1.27*micrometer, 1.226*micrometer, 1.183*micrometer, 1.142*micrometer, 1.102*micrometer, 1.064*micrometer, 1.026*micrometer, 0.9905*micrometer, 0.9559*micrometer, 0.9225*micrometer, 0.8903*micrometer, 0.8592*micrometer, 0.8292*micrometer, 0.8002*micrometer, 0.7723*micrometer, 0.7453*micrometer, 0.7193*micrometer, 0.6942*micrometer, 0.6699*micrometer, 0.6465*micrometer, 0.6239*micrometer, 0.6021*micrometer, 0.5811*micrometer, 0.5608*micrometer, 0.5412*micrometer, 0.5223*micrometer, 0.5041*micrometer, 0.4865*micrometer, 0.4695*micrometer, 0.4531*micrometer, 0.4372*micrometer, 0.422*micrometer, 0.4072*micrometer, 0.393*micrometer, 0.3793*micrometer, 0.366*micrometer, 0.3532*micrometer, 0.3409*micrometer, 0.329*micrometer, 0.3175*micrometer, 0.3064*micrometer, 0.2957*micrometer, 0.2854*micrometer, 0.2754*micrometer, 0.2658*micrometer, 0.2565*micrometer, 0.2476*micrometer, 0.2389*micrometer, 0.2306*micrometer, 0.22*micrometer};
    const G4int nEntries = sizeof(photonWavelength)/sizeof(G4double);

    G4double photonEnergy[nEntries];
    for (int i = nEntries; i--;)
        photonEnergy[i] = h_Planck*c_light/photonWavelength[i];

    G4double refractiveIndex[nEntries] = {1.37018299008657, 1.3703964599229, 1.3706954434578, 1.3709854791133, 1.3712675594927, 1.3715319527736, 1.3717905296533, 1.3720389088426, 1.3722780363801, 1.3725142998158, 1.3727431138139, 1.3729709541023, 1.3731929568741, 1.3734218941111, 1.373643616818, 1.3738687190303, 1.3740962747754, 1.3743271722957, 1.37456304318, 1.3748048358874, 1.3750543571762, 1.3753117156635, 1.3755797952111, 1.3758587136554, 1.3761506312517, 1.3764580520954, 1.3767810397294, 1.3771222877017, 1.3774832703664, 1.3778654290185, 1.3782722423115, 1.3787056326739, 1.3791675195011, 1.379659783282, 1.380187376075, 1.3807529936357, 1.381359367534, 1.3820135622524, 1.3827100519167, 1.3834660511646, 1.3842755731536, 1.385147751321, 1.386094468574, 1.387114337156, 1.3882119238027, 1.3894021962629, 1.3906930636314, 1.3920928837393, 1.3936104172929, 1.3952547546048, 1.3970543081151, 1.3990042606259, 1.4011392298917, 1.403451178233, 1.406013423153, 1.4087899348008, 1.41295989755522};

/*
    //add extraordinary refractive index
    G4double refractiveIndexExtraOrdinary[] = {1.2969430583497, 1.3037479423816, 1.3099385614219, 1.3156431482726, 1.3208770427562, 1.3256784790354, 1.3301031329507, 1.3341594176553, 1.3379145242959, 1.3413712582434, 1.3445515362219, 1.3475091598939, 1.3502262303477, 1.352751020249, 1.355080984337, 1.357229393055, 1.3592358994321, 1.3610825552967, 1.3628054583256, 1.3643997415401, 1.3658736519108, 1.3672461525669, 1.3685232689664, 1.3697107083997, 1.370813877063, 1.3718378950641, 1.3727969355027, 1.3736856119868, 1.3745083126953, 1.375285985298, 1.376004636983, 1.3766759093144, 1.3773026200811, 1.3778948388013, 1.3784401473161, 1.3789555523285, 1.3794427941165, 1.3798969800832, 1.3803202527938, 1.3807271982956, 1.3811066013527, 1.3814663380258, 1.38180780586, 1.3821264910598, 1.3824354770166, 1.3827358132747, 1.3830171405395, 1.3832920926928, 1.3835560089604, 1.3838098994703, 1.3840605451551, 1.3843030814412, 1.3845443727534, 1.3847792695885, 1.3850212788156, 1.3852554420131, 1.3854929546822, 1.385732835897, 1.3859760214917, 1.3862242290515, 1.3864784554652, 1.3867405987683, 1.3870107720679, 1.3872920012591, 1.3875844078715, 1.3878902549374, 1.3882121625293, 1.3885501939419, 1.3889071650812, 1.3892846141659, 1.3896840441597, 1.3901090855872, 1.3905617402294, 1.3910440054677, 1.3915578367426, 1.3921083928924, 1.3926984742538, 1.3933309160118, 1.3940130680671, 1.3947391490921, 1.3955270805632, 1.3963705924704, 1.3972791680293, 1.3982651410253, 1.3993270125129, 1.4004694766888, 1.4017080367113, 1.4030508253012, 1.4045064180542, 1.4060837805639, 1.407792186175, 1.4096609264178, 1.4116847188909, 1.4138991422766, 1.4162954303493, 1.4189489987686, 1.421821816895, 1.4250091623322, 1.4285076737772, 1.4323491847163, 1.4365683618225};

    G4double refractiveIndexEffective[nEntries];
    for(int i = nEntries; i--;)
        refractiveIndexEffective[i] = sqrt(refractiveIndexOrdinary[i]*refractiveIndexExtraOrdinary[i]);

*/

    //add absorption length table

    // G4double abslength[nEntries];
    // for(int i = nEntries; i--;)
    //     abslength[i] = 1.*cm/(0.04);

    G4double ti = 0.95; //transmitance
    G4double mu = -log10(ti)/thickness; //absorbance coefficient
    G4double lamdba = 1/(mu*log(10)); //absorbtion length
    //lamdba = 0.;

    G4double abslength[nEntries];
     for(int i = nEntries; i--;)
        abslength[i] = lamdba;

    //my material property table
    G4MaterialPropertiesTable* myMPT = new G4MaterialPropertiesTable();
    myMPT->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries)
    ->SetSpline(true);
    myMPT->AddProperty("ABSLENGTH",photonEnergy, abslength, nEntries)
    ->SetSpline(true);
    matMgF2->SetMaterialPropertiesTable(myMPT);
    //-------------end of optical properties------------------

}


void myDetectorConstruction::AddOpticalPropertiesVacuum(G4Material* matVacuum)
{

    G4double photonWavelength[] ={1.61*micrometer, 0.22*micrometer};
    const G4int nEntries = sizeof(photonWavelength)/sizeof(G4double);

    G4double photonEnergy[nEntries];
    for (int i = nEntries; i--;)
        photonEnergy[i] = h_Planck*c_light/photonWavelength[i];

    G4double refractiveIndex[nEntries];
    for(int i = nEntries; i--;)
        refractiveIndex[i] = 1.;

    //my material property table
    G4MaterialPropertiesTable* myMPT = new G4MaterialPropertiesTable();
    myMPT->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries)
    ->SetSpline(true);
    //myMPT->AddProperty("ABSLENGTH", photonEnergy, abslength, nEntries)
    //->SetSpline(true);
    matVacuum->SetMaterialPropertiesTable(myMPT);

}


void myDetectorConstruction::AddOpticalPropertiesCr(G4Material* matCr, G4double thickness)
{
    G4double photonWavelength[] = {1.61*micrometer, 1.393*micrometer, 1.216*micrometer, 1.088*micrometer, 0.984*micrometer, 0.892*micrometer, 0.821*micrometer, 0.756*micrometer, 0.704*micrometer, 0.659*micrometer, 0.617*micrometer, 0.582*micrometer, 0.549*micrometer, 0.521*micrometer, 0.496*micrometer, 0.471*micrometer, 0.451*micrometer, 0.431*micrometer, 0.413*micrometer, 0.397*micrometer, 0.381*micrometer, 0.368*micrometer, 0.354*micrometer, 0.342*micrometer, 0.332*micrometer, 0.32*micrometer, 0.311*micrometer, 0.301*micrometer, 0.292*micrometer, 0.284*micrometer, 0.276*micrometer, 0.269*micrometer, 0.262*micrometer, 0.255*micrometer, 0.249*micrometer, 0.243*micrometer, 0.237*micrometer, 0.231*micrometer, 0.226*micrometer, 0.22*micrometer};
    const G4int nEntries = sizeof(photonWavelength)/sizeof(G4double);

    G4double photonEnergy[nEntries];
    for (int i = nEntries; i--;)
        photonEnergy[i] = h_Planck*c_light/photonWavelength[i];

    G4double refractiveIndex[nEntries] = {3.66, 3.69, 3.67, 3.58, 3.41, 3.3, 3.2, 3.08, 3.05, 3.09, 3.17, 3.22, 3.18, 2.94, 2.75, 2.51, 2.33, 2.19, 2.08, 2, 1.92, 1.87, 1.84, 1.76, 1.69, 1.65, 1.58, 1.53, 1.48, 1.45, 1.43, 1.39, 1.38, 1.37, 1.36, 1.36, 1.38, 1.4, 1.43, 1.454};

    // G4double refractiveIndexImag[nEntries] = {4.31, 3.84, 3.6, 3.58, 3.57, 3.52, 3.48, 3.42, 3.39, 3.34, 3.3, 3.3, 3.33, 3.33, 3.3, 3.24, 3.14, 3.04, 2.93, 2.83, 2.74, 2.69, 2.64, 2.58, 2.53, 2.47, 2.4, 2.34, 2.28, 2.21, 2.15, 2.08, 2.03, 1.97, 1.91, 1.85, 1.8, 1.77, 1.74, 1.728};


    //-----------transmitance-----------------


    //G4double transmitance[nEntries] = {0.109722786876083, 0.133385964525309, 0.145280131109727, 0.143044405461597, 0.139702865502624, 0.13910160296018, 0.138103065088929, 0.138212708858361, 0.137132617328541, 0.138421983921369, 0.138922503880897, 0.136119691506984, 0.130422440131141, 0.125543028316201, 0.122702144906516, 0.120348942584936, 0.122332947004673, 0.124996220922419, 0.129813373348621, 0.135162919597748, 0.139564052765862, 0.141007322040886, 0.143115603914701, 0.144416006557204, 0.145116632975507, 0.148356636623192, 0.151986803233141, 0.155158919402013, 0.1586243432373, 0.165765468838244, 0.172316688024638, 0.179769410970641, 0.186277748603086, 0.195175185770797, 0.204978023690241, 0.216250211529313, 0.227351114923098, 0.233504021988139, 0.240850313849703, 0.242206988372671};

    //G4double lambda[nEntries]; //absorbtion length

    // for(int i = nEntries; i--;)
    //     lambda[i] = 1/(log(10)*(-log10(transmitance[i])/thickness));

    //my material property table
    G4MaterialPropertiesTable* myMPT = new G4MaterialPropertiesTable();
    myMPT->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries)
    ->SetSpline(true);
    // myMPT->AddProperty("REALRINDEX", photonEnergy, refractiveIndex, nEntries)
    // ->SetSpline(true);
    // myMPT->AddProperty("IMAGINARYRINDEX", photonEnergy, refractiveIndexImag, nEntries)
    // ->SetSpline(true);
    // myMPT->AddProperty("ABSLENGTH", photonEnergy, lambda, nEntries)
    // ->SetSpline(true);
    matCr->SetMaterialPropertiesTable(myMPT);
}

void myDetectorConstruction::AddOpticalPropertiesCsI(G4Material* matCsI, G4double thickness)
{
    G4double photonWavelength[] ={1.61*micrometer, 1.6*micrometer, 1.58*micrometer, 1.56*micrometer, 1.54*micrometer, 1.52*micrometer, 1.5*micrometer, 1.48*micrometer, 1.46*micrometer, 1.44*micrometer, 1.42*micrometer, 1.4*micrometer, 1.38*micrometer, 1.36*micrometer, 1.34*micrometer, 1.32*micrometer, 1.3*micrometer, 1.28*micrometer, 1.26*micrometer, 1.24*micrometer, 1.22*micrometer, 1.2*micrometer, 1.18*micrometer, 1.16*micrometer, 1.14*micrometer, 1.12*micrometer, 1.1*micrometer, 1.08*micrometer, 1.06*micrometer, 1.04*micrometer, 1.02*micrometer, 1*micrometer, 0.98*micrometer, 0.96*micrometer, 0.94*micrometer, 0.92*micrometer, 0.9*micrometer, 0.88*micrometer, 0.86*micrometer, 0.84*micrometer, 0.82*micrometer, 0.8*micrometer, 0.78*micrometer, 0.76*micrometer, 0.74*micrometer, 0.72*micrometer, 0.7*micrometer, 0.68*micrometer, 0.66*micrometer, 0.64*micrometer, 0.62*micrometer, 0.6*micrometer, 0.58*micrometer, 0.56*micrometer, 0.54*micrometer, 0.52*micrometer, 0.5*micrometer, 0.48*micrometer, 0.46*micrometer, 0.44*micrometer, 0.42*micrometer, 0.4*micrometer, 0.38*micrometer, 0.36*micrometer, 0.34*micrometer, 0.32*micrometer, 0.3*micrometer, 0.28*micrometer, 0.26*micrometer, 0.24*micrometer, 0.22*micrometer};
    const G4int nEntries = sizeof(photonWavelength)/sizeof(G4double);

    G4double photonEnergy[nEntries];
    for (int i = nEntries; i--;)
        photonEnergy[i] = h_Planck*c_light/photonWavelength[i];

    G4double refractiveIndex[nEntries] = {1.7385, 1.738, 1.738, 1.738, 1.738, 1.738, 1.738, 1.738, 1.738, 1.738, 1.738, 1.739, 1.739, 1.739, 1.739, 1.739, 1.74, 1.74, 1.74, 1.74, 1.74, 1.74, 1.74, 1.74, 1.74, 1.74, 1.739, 1.739, 1.738, 1.738, 1.737, 1.736, 1.735, 1.735, 1.734, 1.734, 1.733, 1.733, 1.733, 1.732, 1.731, 1.73, 1.729, 1.729, 1.729, 1.729, 1.731, 1.731, 1.732, 1.734, 1.735, 1.737, 1.739, 1.74, 1.744, 1.748, 1.752, 1.757, 1.766, 1.774, 1.784, 1.794, 1.806, 1.822, 1.845, 1.867, 1.902, 1.955, 2.043, 2.227, 2.48};


    //-----------transmitance-----------------

    G4double transmitance[nEntries] = {0.864538118769033, 0.864538118769033, 0.864538118769033, 0.864538118769033, 0.864538118769033, 0.864538118769033, 0.864538118769033, 0.864538118769033, 0.864538118769033, 0.864538118769033, 0.864538118769033, 0.864288126525022, 0.864288126525022, 0.864288126525022, 0.864288126525022, 0.864288126525022, 0.864038136855696, 0.864038136855696, 0.864038136855696, 0.864038136855696, 0.864038136855696, 0.864038136855696, 0.864038136855696, 0.864038136855696, 0.864038136855696, 0.864038136855696, 0.864288126525022, 0.864288126525022, 0.864538118769033, 0.864538118769033, 0.864788113221027, 0.865038109513027, 0.865288107275776, 0.865288107275776, 0.865538106138732, 0.865538106138732, 0.865788105730063, 0.865788105730063, 0.865788105730063, 0.86603810567665, 0.866288105604073, 0.866538105136618, 0.866788103897263, 0.866788103897263, 0.866788103897263, 0.866788103897263, 0.866288105604073, 0.866288105604073, 0.86603810567665, 0.865538106138732, 0.865288107275776, 0.864788113221027, 0.864288126525022, 0.864038136855696, 0.863038211214746, 0.862038355775263, 0.861038593401063, 0.859789055624241, 0.857540480669406, 0.855542555767003, 0.853046485295467, 0.850552195173756, 0.84756182836826, 0.84358022484978, 0.837869903100005, 0.832425074498031, 0.823803860183766, 0.81086265624919, 0.789740867968895, 0.747374498890768, 7.4E-15};

    G4double lambda[nEntries]; //absorbtion length

    for(int i = nEntries; i--;)
        lambda[i] = 1/(log(10)*(-log10(transmitance[i])/thickness));

    //my materials table
    G4MaterialPropertiesTable* myMPT = new G4MaterialPropertiesTable();
    myMPT->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries)
    ->SetSpline(true);
    myMPT->AddProperty("ABSLENGTH", photonEnergy, lambda, nEntries)
    ->SetSpline(true);
    matCsI->SetMaterialPropertiesTable(myMPT);


}

void myDetectorConstruction::AddPhotocathodeSurfaceProperties(G4OpticalSurface *photocathode_opsurf) {
    //**Photocathode surface properties

    G4double photonWavelength[] ={1.61*micrometer, 1.6*micrometer, 1.58*micrometer, 1.56*micrometer, 1.54*micrometer, 1.52*micrometer, 1.5*micrometer, 1.48*micrometer, 1.46*micrometer, 1.44*micrometer, 1.42*micrometer, 1.4*micrometer, 1.38*micrometer, 1.36*micrometer, 1.34*micrometer, 1.32*micrometer, 1.3*micrometer, 1.28*micrometer, 1.26*micrometer, 1.24*micrometer, 1.22*micrometer, 1.2*micrometer, 1.18*micrometer, 1.16*micrometer, 1.14*micrometer, 1.12*micrometer, 1.1*micrometer, 1.08*micrometer, 1.06*micrometer, 1.04*micrometer, 1.02*micrometer, 1*micrometer, 0.98*micrometer, 0.96*micrometer, 0.94*micrometer, 0.92*micrometer, 0.9*micrometer, 0.88*micrometer, 0.86*micrometer, 0.84*micrometer, 0.82*micrometer, 0.8*micrometer, 0.78*micrometer, 0.76*micrometer, 0.74*micrometer, 0.72*micrometer, 0.7*micrometer, 0.68*micrometer, 0.66*micrometer, 0.64*micrometer, 0.62*micrometer, 0.6*micrometer, 0.58*micrometer, 0.56*micrometer, 0.54*micrometer, 0.52*micrometer, 0.5*micrometer, 0.48*micrometer, 0.46*micrometer, 0.44*micrometer, 0.42*micrometer, 0.4*micrometer, 0.38*micrometer, 0.36*micrometer, 0.34*micrometer, 0.32*micrometer, 0.3*micrometer, 0.28*micrometer, 0.26*micrometer, 0.24*micrometer, 0.22*micrometer};
    const G4int nEntries = sizeof(photonWavelength)/sizeof(G4double);

    G4double photonEnergy[nEntries];
    for (int i = nEntries; i--;)
        photonEnergy[i] = h_Planck*c_light/photonWavelength[i];

    G4double refractiveIndexReal[nEntries] = {1.7385, 1.738, 1.738, 1.738, 1.738, 1.738, 1.738, 1.738, 1.738, 1.738, 1.738, 1.739, 1.739, 1.739, 1.739, 1.739, 1.74, 1.74, 1.74, 1.74, 1.74, 1.74, 1.74, 1.74, 1.74, 1.74, 1.739, 1.739, 1.738, 1.738, 1.737, 1.736, 1.735, 1.735, 1.734, 1.734, 1.733, 1.733, 1.733, 1.732, 1.731, 1.73, 1.729, 1.729, 1.729, 1.729, 1.731, 1.731, 1.732, 1.734, 1.735, 1.737, 1.739, 1.74, 1.744, 1.748, 1.752, 1.757, 1.766, 1.774, 1.784, 1.794, 1.806, 1.822, 1.845, 1.867, 1.902, 1.955, 2.043, 2.227, 2.48};

    G4double refractiveIndexImag[nEntries];
    for(int i = nEntries-1; i--;)
        refractiveIndexImag[i] = 0.;
    refractiveIndexImag[nEntries-1] = 0.583;

    G4double photocathode_EFF[nEntries]; //Enables 'detection' of photons
    for(int i = nEntries; i--;)
        photocathode_EFF[i] = 1.;

    // G4double photocathode_REF[nEntries]; //Enables 'detection' of photons
    // for(int i = nEntries; i--;)
    //     photocathode_REF[i] = 0.5;


    G4MaterialPropertiesTable* photocathode_mt = new G4MaterialPropertiesTable();
    photocathode_mt->AddProperty("EFFICIENCY", photonEnergy, photocathode_EFF,nEntries);
    photocathode_mt->AddProperty("REALRINDEX", photonEnergy, refractiveIndexReal, nEntries);
    photocathode_mt->AddProperty("IMAGINARYRINDEX", photonEnergy, refractiveIndexImag, nEntries);
    // photocathode_mt->AddProperty("REFLECTIVITY", photonEnergy, photocathode_REF, nEntries);

    photocathode_opsurf->SetMaterialPropertiesTable(photocathode_mt);

}

void myDetectorConstruction::AddAirSurfaceProperties(G4OpticalSurface *air_opsurf) {
    //**Air surface properties

    G4double photonWavelength[] ={1.61*micrometer, 0.22*micrometer};
    const G4int nEntries = sizeof(photonWavelength)/sizeof(G4double);

    G4double photonEnergy[nEntries];
    for (int i = nEntries; i--;)
        photonEnergy[i] = h_Planck*c_light/photonWavelength[i];

    G4double refractiveIndexReal[nEntries] = {1., 1.};

    G4double refractiveIndexImag[nEntries];
    for(int i = nEntries; i--;)
        refractiveIndexImag[i] = 0.;

    G4double photocathode_EFF[nEntries]; //Enables 'detection' of photons
    for(int i = nEntries; i--;)
        photocathode_EFF[i] = 1.;

    // G4double photocathode_REF[nEntries]; //Enables 'detection' of photons
    // for(int i = nEntries; i--;)
    //     photocathode_REF[i] = 0.;


    G4MaterialPropertiesTable* air_mt = new G4MaterialPropertiesTable();
    air_mt->AddProperty("EFFICIENCY", photonEnergy, photocathode_EFF,nEntries);
    air_mt->AddProperty("REALRINDEX", photonEnergy, refractiveIndexReal, nEntries);
    air_mt->AddProperty("IMAGINARYRINDEX", photonEnergy, refractiveIndexImag, nEntries);
    //air_mt->AddProperty("REFLECTIVITY", photonEnergy, photocathode_REF, nEntries);

    air_opsurf->SetMaterialPropertiesTable(air_mt);

}

void myDetectorConstruction::AddThinLayerSurfaceProperties(G4OpticalSurface *thinLayer_opsurf) {
    //**Thin Layer surface properties

    G4double photonWavelength[] = {1.61*micrometer, 1.393*micrometer, 1.216*micrometer, 1.088*micrometer, 0.984*micrometer, 0.892*micrometer, 0.821*micrometer, 0.756*micrometer, 0.704*micrometer, 0.659*micrometer, 0.617*micrometer, 0.582*micrometer, 0.549*micrometer, 0.521*micrometer, 0.496*micrometer, 0.471*micrometer, 0.451*micrometer, 0.431*micrometer, 0.413*micrometer, 0.397*micrometer, 0.381*micrometer, 0.368*micrometer, 0.354*micrometer, 0.342*micrometer, 0.332*micrometer, 0.32*micrometer, 0.311*micrometer, 0.301*micrometer, 0.292*micrometer, 0.284*micrometer, 0.276*micrometer, 0.269*micrometer, 0.262*micrometer, 0.255*micrometer, 0.249*micrometer, 0.243*micrometer, 0.237*micrometer, 0.231*micrometer, 0.226*micrometer, 0.22*micrometer};
    const G4int nEntries = sizeof(photonWavelength)/sizeof(G4double);

    G4double photonEnergy[nEntries];
    for (int i = nEntries; i--;)
        photonEnergy[i] = h_Planck*c_light/photonWavelength[i];

    // G4double refractiveIndexReal[nEntries] = {1., 1.};
    //
    // G4double refractiveIndexImag[nEntries];
    // for(int i = nEntries; i--;)
    //     refractiveIndexImag[i] = 0.;

    G4double photocathode_EFF[nEntries]; //Enables 'detection' of photons
    for(int i = nEntries; i--;)
        photocathode_EFF[i] = 1.;

        //-----------transmitance-----------------


    G4double transmitance[nEntries] = {0.109722786876083, 0.133385964525309, 0.145280131109727, 0.143044405461597, 0.139702865502624, 0.13910160296018, 0.138103065088929, 0.138212708858361, 0.137132617328541, 0.138421983921369, 0.138922503880897, 0.136119691506984, 0.130422440131141, 0.125543028316201, 0.122702144906516, 0.120348942584936, 0.122332947004673, 0.124996220922419, 0.129813373348621, 0.135162919597748, 0.139564052765862, 0.141007322040886, 0.143115603914701, 0.144416006557204, 0.145116632975507, 0.148356636623192, 0.151986803233141, 0.155158919402013, 0.1586243432373, 0.165765468838244, 0.172316688024638, 0.179769410970641, 0.186277748603086, 0.195175185770797, 0.204978023690241, 0.216250211529313, 0.227351114923098, 0.233504021988139, 0.240850313849703, 0.242206988372671};


    G4double photocathode_REF[nEntries]; //Enables 'detection' of photons
    for(int i = nEntries; i--;)
        photocathode_REF[i] = 1 - transmitance[i];


    G4MaterialPropertiesTable* thinLayer_mt = new G4MaterialPropertiesTable();
    thinLayer_mt->AddProperty("EFFICIENCY", photonEnergy, photocathode_EFF,nEntries);
    // thinLayer_mt->AddProperty("REALRINDEX", photonEnergy, refractiveIndexReal, nEntries);
    // thinLayer_mt->AddProperty("IMAGINARYRINDEX", photonEnergy, refractiveIndexImag, nEntries);
    thinLayer_mt->AddProperty("REFLECTIVITY", photonEnergy, photocathode_REF, nEntries);

    thinLayer_opsurf->SetMaterialPropertiesTable(thinLayer_mt);

}








//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------

G4VPhysicalVolume* myDetectorConstruction::Construct()
{
    // Get nist material manager
    G4NistManager* nist = G4NistManager::Instance();

    G4bool checkOverlaps = true;

    G4double world_sizeXY = 10.*cm;
    G4double world_sizeZ  = 10.*cm;

    //creating vacuum
    G4double density = universe_mean_density; //from PhysicalConstants.h
    G4double pressure = 1.e-19*pascal;
    G4double temperature = 0.1*kelvin;
    G4double z, a;
    G4String name;
    new G4Material(name="Galactic", z=1., a=1.01*g/mole, density,
    kStateGas,temperature,pressure);

    G4Material* world_mat = nist->FindOrBuildMaterial("Galactic");

    G4Box* solidWorld =
      new G4Box("World",0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);
    //its name
    //its size

    AddOpticalPropertiesVacuum(world_mat);

    G4LogicalVolume* logicWorld =
      new G4LogicalVolume(solidWorld, world_mat, "World");
                    //its solid - its material - its name

    G4VPhysicalVolume* physWorld =
      new G4PVPlacement(0, G4ThreeVector(0,0,0), logicWorld, "World", 0, false, 0, checkOverlaps);
    //no rotation - at (0,0,0) - its logical volume - its name - its mother  volume - no boolean operation - copy number - overlaps checking


    //----------------Radiator-------------------------

    G4Material* matRadiator = nist->FindOrBuildMaterial("G4_MAGNESIUM_FLUORIDE");
    G4ThreeVector posRadiator = G4ThreeVector(0.*cm, 0.*cm, 0.*cm);

    G4double rminRadiator =  0.*cm;
    G4double rmaxRadiator =  2.54*cm;
    G4double hzRadiator = 3*mm;
    G4double phiminRadiator = 0.*deg, phimaxRadiator = 360.*deg;

    AddOpticalPropertiesMgF2(matRadiator, hzRadiator);

    G4Tubs* solidRadiator =
      new G4Tubs("Radiator", rminRadiator, rmaxRadiator, 0.5*hzRadiator, phiminRadiator, phimaxRadiator);

    G4LogicalVolume* LVRadiator = new G4LogicalVolume(solidRadiator, matRadiator, "Radiator");

    new G4PVPlacement(0, posRadiator, LVRadiator, "Radiator", logicWorld, false, 0, checkOverlaps);

    G4VisAttributes * VisAttRadiator = new G4VisAttributes(G4Colour(1. ,1. ,1.));
            // Set the forced wireframe style
    VisAttRadiator->SetForceWireframe(true);
            // Assignment of the visualization attributes to the logical volume
    LVRadiator->SetVisAttributes(VisAttRadiator);


    //----------------ThinLayer-------------------

    G4double rminThinLayer =  0.*cm;
    G4double rmaxThinLayer =  2.54*cm;
    G4double hzThinLayer = 5.5*nm;//5.5*nm; //if you change this, change transmitance!!!!
    G4double phiminThinLayer = 0.*deg, phimaxThinLayer = 360.*deg;

    G4Material* matThinLayer = nist->FindOrBuildMaterial("G4_Cr");
    G4ThreeVector posThinLayer = G4ThreeVector(0.*cm, 0.*cm, posRadiator[2] + 0.5*hzThinLayer + 0.5*hzRadiator);
    AddOpticalPropertiesCr(matThinLayer, hzThinLayer);

    G4Tubs* solidThinLayer = new G4Tubs("ThinLayer", rminThinLayer, rmaxThinLayer, 0.5*hzThinLayer, phiminThinLayer, phimaxThinLayer);
    G4LogicalVolume* LVThinLayer = new G4LogicalVolume(solidThinLayer, matThinLayer, "ThinLayer");

    new G4PVPlacement(0, posThinLayer, LVThinLayer, "ThinLayer", logicWorld, false, 0, checkOverlaps);

    G4VisAttributes * VisAttThinLayer = new G4VisAttributes(G4Colour(1., 0., 1.));
              // Set the forced wireframe style
    //VisAttThinLayer->SetForceWireframe(true);
              // Assignment of the visualization attributes to the logical volume
    LVThinLayer->SetVisAttributes(VisAttThinLayer);

    //Optical surface
    // G4OpticalSurface* thinLayer_opsurf = new G4OpticalSurface("thinLayer_opsurf", glisur,polished, dielectric_metal);
    // AddThinLayerSurfaceProperties(thinLayer_opsurf);
    // // G4OpticalSurface* photocathode_opsurf = new G4OpticalSurface("photocathode_opsurf", glisur,polished, dielectric_dielectric);
    // // AddAirSurfaceProperties(photocathode_opsurf);
    // new G4LogicalSkinSurface("thinLayer_surf", LVThinLayer, thinLayer_opsurf);





    // //----------------Photocathode---------------------

    G4double rminPhotocathode =  0.*cm;
    G4double rmaxPhotocathode =  0.5*cm;
    G4double hzPhotocathode = 18*nm;//5.5*nm;
    G4double phiminPhotocathode = 0.*deg, phimaxPhotocathode = 360.*deg;

    G4Material* matPhotocathode = nist->FindOrBuildMaterial("G4_CESIUM_IODIDE");
    //G4Material* matPhotocathode = nist->FindOrBuildMaterial("Galactic");
    G4ThreeVector posPhotocathode = G4ThreeVector(0.*cm, 0.*cm, posThinLayer[2] + 0.5*hzPhotocathode + 0.5*hzThinLayer);
    // G4ThreeVector posPhotocathode = G4ThreeVector(0.*cm, 0.*cm, posRadiator[2] + 0.5*hzPhotocathode + 0.5*hzRadiator);

    //AddOpticalPropertiesCsI(matPhotocathode, hzPhotocathode);

    G4Tubs* solidPhotocathode = new G4Tubs("Photocathode", rminPhotocathode, rmaxPhotocathode, 0.5*hzPhotocathode, phiminPhotocathode, phimaxPhotocathode);
    G4LogicalVolume* LVPhotocathode = new G4LogicalVolume(solidPhotocathode, matPhotocathode, "Photocathode");

    new G4PVPlacement(0, posPhotocathode, LVPhotocathode, "Photocathode", logicWorld, false, 0, checkOverlaps);

    G4VisAttributes * VisAttPhotocathode = new G4VisAttributes(G4Colour(1.,0.5,0.2));
              // Set the forced wireframe style
    //VisAttPhotocathode->SetForceWireframe(true);
              // Assignment of the visualization attributes to the logical volume
    LVPhotocathode->SetVisAttributes(VisAttPhotocathode);

    //Optical surface
    G4OpticalSurface* photocathode_opsurf = new G4OpticalSurface("photocathode_opsurf", glisur,polished, dielectric_metal);
    AddPhotocathodeSurfaceProperties(photocathode_opsurf);
    // G4OpticalSurface* photocathode_opsurf = new G4OpticalSurface("photocathode_opsurf", glisur,polished, dielectric_dielectric);
    // AddAirSurfaceProperties(photocathode_opsurf);
    new G4LogicalSkinSurface("photocath_surf", LVPhotocathode, photocathode_opsurf);


    // fScoringVolume = LVPhotocathode;
    //fScoringVolume = LVPhotocathode;
    return physWorld;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void myDetectorConstruction::ConstructSDandField()
{
  //------------------------------------------------------------------
  // Sensitive Detector
  //------------------------------------------------------------------
  G4String photocathodeSDname = "/mydet/photocathode";
  myPhotocathodeSD * photocathodeSD = new myPhotocathodeSD(photocathodeSDname);
  G4SDManager::GetSDMpointer()->AddNewDetector(photocathodeSD);
  SetSensitiveDetector("Photocathode",photocathodeSD);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
