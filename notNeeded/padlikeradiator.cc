
#include "myDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4Polyhedra.hh"
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

void myDetectorConstruction::AddOpticalPropertiesMgF2(G4Material* matMgF2)
{
    //from max to min
    G4double photonWavelength[] =
       {240.*nm, 230.*nm,220.*nm,
        210.*nm,200.*nm,190.*nm,
        180.*nm,170.*nm,160.*nm,
        150.*nm,140.*nm,130.*nm,
        120.*nm,113.7*nm, 100.*nm};

    const G4int nEntries = sizeof(photonWavelength)/sizeof(G4double);

    G4double photonEnergy[nEntries];
    for (int i = nEntries; i--;)
        photonEnergy[i] = h_Planck*c_light/photonWavelength[i];

    G4double refractiveIndex[nEntries] =
       {1.4057, 1.4092, 1.4131, 1.4177, 1.4232, 1.4299,
        1.4381, 1.4484, 1.4617, 1.4792, 1.5034, 1.5565,
        1.6510, 1.7805, 2.09934};


    G4double photonWavelength2[] =
        {240.176561547517*nm, 238.293355301992*nm, 236.424915134141*nm, 234.571125264265*nm, 232.731870820488*nm, 230.907037831632*nm, 229.096513220161*nm, 227.300184795171*nm, 227.300184795171*nm, 225.517941245437*nm, 223.749672132519*nm, 221.995267883915*nm, 221.995267883915*nm, 220.254619786272*nm, 220.254619786272*nm, 218.527619978653*nm, 218.527619978653*nm, 216.814161445847*nm, 216.814161445847*nm, 215.114138011744*nm, 213.427444332751*nm, 211.753975891267*nm, 211.753975891267*nm, 210.093628989206*nm, 210.093628989206*nm, 208.446300741569*nm, 208.446300741569*nm, 206.811889070073*nm, 206.811889070073*nm, 205.190292696821*nm, 205.190292696821*nm, 203.581411138029*nm, 203.581411138029*nm, 201.9851446978*nm, 201.9851446978*nm, 200.401394461943*nm, 200.401394461943*nm, 198.830062291847*nm, 198.830062291847*nm, 197.271050818398*nm, 195.724263435946*nm, 195.724263435946*nm, 194.189604296318*nm, 192.666978302879*nm, 191.156291104642*nm, 189.657449090417*nm, 188.170359383011*nm, 186.694929833477*nm, 186.694929833477*nm, 185.231069015398*nm, 183.778686219227*nm, 183.778686219227*nm, 182.337691446663*nm, 182.337691446663*nm, 180.907995405074*nm, 179.489509501965*nm, 179.489509501965*nm, 178.08214583949*nm, 178.08214583949*nm, 176.685817209*nm, 176.685817209*nm, 175.300437085644*nm, 175.300437085644*nm, 173.925919623008*nm, 173.925919623008*nm, 172.56217964779*nm, 172.56217964779*nm, 171.209132654528*nm, 171.209132654528*nm, 169.866694800357*nm, 169.866694800357*nm, 168.534782899822*nm, 168.534782899822*nm, 167.213314419716*nm, 167.213314419716*nm, 165.902207473971*nm, 164.601380818579*nm, 163.310753846562*nm, 163.310753846562*nm, 162.030246582974*nm, 162.030246582974*nm, 162.030246582974*nm, 160.759779679946*nm, 160.759779679946*nm, 160.759779679946*nm, 159.499274411773*nm, 159.499274411773*nm, 159.499274411773*nm, 158.248652670027*nm, 157.007836958728*nm, 157.007836958728*nm, 157.007836958728*nm, 157.007836958728*nm, 155.776750389531*nm, 155.776750389531*nm, 155.776750389531*nm, 154.55531667697*nm, 154.55531667697*nm, 154.55531667697*nm, 153.343460133726*nm, 153.343460133726*nm, 153.343460133726*nm, 153.343460133726*nm, 152.141105665939*nm, 152.141105665939*nm, 152.141105665939*nm, 152.141105665939*nm, 150.948178768555*nm, 150.948178768555*nm, 150.948178768555*nm, 150.948178768555*nm, 149.764605520708*nm, 149.764605520708*nm, 149.764605520708*nm, 149.764605520708*nm, 148.590312581139*nm, 148.590312581139*nm, 148.590312581139*nm, 148.590312581139*nm, 147.425227183654*nm, 147.425227183654*nm, 147.425227183654*nm, 147.425227183654*nm, 147.425227183654*nm, 147.425227183654*nm, 146.269277132611*nm, 146.269277132611*nm, 146.269277132611*nm, 146.269277132611*nm, 146.269277132611*nm, 145.12239079845*nm, 145.12239079845*nm, 145.12239079845*nm, 145.12239079845*nm, 143.984497113253*nm, 143.984497113253*nm, 143.984497113253*nm, 143.984497113253*nm, 143.984497113253*nm, 142.855525566341*nm, 142.855525566341*nm, 142.855525566341*nm, 141.735406199901*nm, 141.735406199901*nm, 141.735406199901*nm, 140.624069604657*nm, 140.624069604657*nm, 140.624069604657*nm, 139.521446915564*nm, 139.521446915564*nm, 138.427469807543*nm, 138.427469807543*nm, 138.427469807543*nm, 137.342070491248*nm, 137.342070491248*nm, 137.342070491248*nm, 136.265181708862*nm, 136.265181708862*nm, 136.265181708862*nm, 135.196736729934*nm, 135.196736729934*nm, 134.136669347239*nm, 134.136669347239*nm, 133.084913872679*nm, 133.084913872679*nm, 133.084913872679*nm, 133.084913872679*nm, 132.041405133211*nm, 132.041405133211*nm, 131.006078466809*nm, 131.006078466809*nm, 129.978869718458*nm, 129.978869718458*nm, 129.978869718458*nm, 128.959715236177*nm, 128.959715236177*nm, 128.959715236177*nm, 127.948551867074*nm, 127.948551867074*nm, 126.945316953436*nm, 126.945316953436*nm, 126.945316953436*nm, 125.949948328844*nm, 125.949948328844*nm, 125.949948328844*nm, 125.949948328844*nm, 124.96238431432*nm, 124.96238431432*nm, 124.96238431432*nm, 123.982563714507*nm, 123.982563714507*nm, 123.982563714507*nm, 123.010425813877*nm, 123.010425813877*nm, 123.010425813877*nm, 122.045910372967*nm, 122.045910372967*nm, 121.088957624646*nm, 121.088957624646*nm, 121.088957624646*nm, 120.139508270414*nm, 120.139508270414*nm, 119.197503476727*nm, 119.197503476727*nm, 119.197503476727*nm, 119.197503476727*nm, 118.262884871347*nm, 118.262884871347*nm, 118.262884871347*nm, 117.335594539732*nm, 117.335594539732*nm, 117.335594539732*nm, 117.335594539732*nm, 116.415575021442*nm, 116.415575021442*nm, 116.415575021442*nm, 116.415575021442*nm, 116.415575021442*nm, 115.502769306579*nm, 115.502769306579*nm, 115.502769306579*nm, 115.502769306579*nm, 115.502769306579*nm, 115.502769306579*nm, 115.502769306579*nm, 115.502769306579*nm, 115.502769306579*nm, 115.502769306579*nm, 115.502769306579*nm, 115.502769306579*nm, 114.597120832256*nm, 114.597120832256*nm, 114.597120832256*nm, 114.597120832256*nm, 114.597120832256*nm, 114.597120832256*nm, 114.597120832256*nm, 114.597120832256*nm, 114.597120832256*nm, 114.597120832256*nm, 114.597120832256*nm, 114.597120832256*nm, 114.597120832256*nm, 114.597120832256*nm, 114.597120832256*nm, 114.586672405933*nm, 114.586672405933*nm, 114.586672405933*nm, 114.586672405933*nm, 114.0308010764*nm, 114.0308010764*nm, 114.0308010764*nm, 113.477626333901*nm, 110.81461358550201*nm, 108.93418711969774*nm, 107.08566983607519*nm,
        105.71989619874096*nm, 104.37154167669356*nm, 103.48220612070234*nm, 101.7262041564033*nm,
        101.29187947249474*nm, 100.8594091577*nm, 100.42878529470527*nm, 100.*nm};

    const G4int nEntries2 = sizeof(photonWavelength2)/sizeof(G4double);

    G4double photonEnergy2[nEntries2];
    for (int i = nEntries2; i--;)
        photonEnergy2[i] = h_Planck*c_light/photonWavelength2[i];

    G4double transmitance[nEntries2] = //for 2mm
        {94.1892138939671, 93.9524680073126, 93.9524680073126, 93.9524680073126, 94.1892138939671, 94.1892138939671, 93.9524680073126, 93.9524680073126, 93.7157221206581, 93.9524680073126, 93.9524680073126, 93.9524680073126, 93.7157221206581, 93.9524680073126, 93.7157221206581, 93.9524680073126, 93.7157221206581, 93.9524680073126, 93.7157221206581, 93.7157221206581, 93.7157221206581, 93.7157221206581, 93.4789762340037, 93.7157221206581, 93.4789762340037, 93.7157221206581, 93.4789762340037, 93.7157221206581, 93.4789762340037, 93.4789762340037, 93.2422303473492, 93.4789762340037, 93.2422303473492, 93.4789762340037, 93.2422303473492, 93.2422303473492, 93.0054844606947, 93.2422303473492, 93.0054844606947, 93.0054844606947, 93.0054844606947, 92.7687385740402, 92.7687385740402, 92.7687385740402, 92.5319926873857, 92.5319926873857, 92.5319926873857, 92.5319926873857, 92.2952468007313, 92.2952468007313, 92.2952468007313, 92.0585009140768, 92.0585009140768, 91.8217550274223, 91.8217550274223, 91.8217550274223, 91.5850091407678, 91.5850091407678, 91.3482632541134, 91.3482632541134, 91.1115173674589, 91.1115173674589, 90.8747714808044, 90.6380255941499, 90.4012797074954, 90.4012797074954, 90.1645338208409, 90.1645338208409, 89.9277879341865, 89.9277879341865, 89.691042047532, 89.691042047532, 89.4542961608775, 89.4542961608775, 89.217550274223, 89.217550274223, 88.9808043875686, 88.9808043875686, 88.7440585009141, 88.9808043875686, 88.7440585009141, 88.5073126142596, 88.7440585009141, 88.5073126142596, 88.2705667276051, 88.5073126142596, 88.2705667276051, 88.0338208409506, 87.7970749542962, 87.7970749542962, 87.5603290676417, 87.3235831809872, 87.0868372943327, 87.0868372943327, 86.8500914076783, 86.6133455210238, 86.6133455210238, 86.3765996343693, 86.1398537477148, 85.9031078610603, 85.6663619744058, 85.4296160877514, 85.1928702010969, 84.9561243144424, 84.7193784277879, 84.4826325411335, 84.245886654479, 84.245886654479, 84.0091407678245, 83.77239488117, 83.5356489945155, 83.2989031078611, 83.0621572212066, 82.8254113345521, 82.5886654478976, 82.5886654478976, 82.3519195612431, 82.1151736745887, 81.8784277879342, 81.8784277879342, 81.6416819012797, 81.4049360146252, 81.1681901279708, 80.9314442413163, 80.6946983546618, 80.6946983546618, 80.4579524680073, 80.2212065813528, 79.9844606946984, 79.7477148080439, 80.2212065813528, 79.7477148080439, 79.5109689213894, 79.2742230347349, 79.0374771480804, 78.800731261426, 78.5639853747715, 78.327239488117, 78.0904936014625, 78.0904936014625, 77.853747714808, 77.6170018281536, 77.6170018281536, 77.3802559414991, 77.1435100548446, 76.9067641681901, 76.6700182815356, 76.4332723948812, 76.1965265082267, 75.9597806215722, 75.7230347349177, 75.4862888482633, 75.2495429616088, 75.2495429616088, 75.0127970749543, 74.7760511882998, 74.7760511882998, 74.5393053016453, 74.3025594149909, 74.0658135283364, 73.8290676416819, 73.5923217550274, 73.3555758683729, 73.3555758683729, 73.1188299817185, 72.882084095064, 72.6453382084095, 72.6453382084095, 72.408592321755, 72.1718464351005, 71.9351005484461, 71.9351005484461, 71.6983546617916, 71.4616087751371, 71.4616087751371, 71.2248628884826, 70.9881170018282, 70.7513711151737, 70.5146252285192, 70.5146252285192, 70.2778793418647, 69.8043875685558, 70.0411334552102, 69.8043875685558, 69.5676416819013, 69.3308957952468, 69.0941499085923, 68.8574040219378, 68.6206581352834, 68.3839122486289, 68.1471663619744, 67.9104204753199, 67.9104204753199, 67.6736745886655, 67.436928702011, 67.2001828153565, 66.963436928702, 66.7266910420475, 66.4899451553931, 66.2531992687386, 66.2531992687386, 66.0164533820841, 66.0164533820841, 65.7797074954296, 65.5429616087752, 65.3062157221207, 65.5429616087752, 65.0694698354662, 64.8327239488117, 64.5959780621572, 64.3592321755027, 64.1224862888483, 63.8857404021938, 63.4122486288848, 63.1755027422304, 62.9387568555759, 61.7550274223035, 61.518281535649, 61.518281535649, 61.2815356489945, 61.04478976234, 60.8080438756856, 60.5712979890311, 60.3345521023766, 60.0978062157221, 59.8610603290677, 59.6243144424132, 59.3875685557587, 59.1508226691042, 58.9140767824497, 58.2038391224863, 57.7303473491773, 57.4936014625229, 57.2568555758684, 57.0201096892139, 56.7833638025594, 56.5466179159049, 56.3098720292505, 56.073126142596, 55.8363802559415, 55.599634369287, 55.3628884826326, 55.1261425959781, 54.8893967093236, 54.6526508226691, 50.6581352833638, 51.3893967093236, 52.2669104204753, 53.290676416819, 44.0767824497258, 44.3692870201097, 44.6617915904936, 44.0767824497258,
        39.8352323, 38.4831618, 36.4791851,
        33.5752323, 30.9192089, 29.1227753, 25.7921744,
        22.6582144, 19.0805193, 15.903628, 0.};

    G4double thickness = 2.*mm;
    G4double abslength[nEntries2];
     for(int i = nEntries2; i--;) {
        abslength[i] = -thickness/(log10(0.01*transmitance[i])*log(10));
        //G4cout<<abslength[i]<<G4endl;
    }

    //my material property table
    G4MaterialPropertiesTable* myMPT = new G4MaterialPropertiesTable();
    myMPT->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries)
    ->SetSpline(true);
    myMPT->AddProperty("ABSLENGTH",photonEnergy2, abslength, nEntries2)
    ->SetSpline(true);
    matMgF2->SetMaterialPropertiesTable(myMPT);
    //-------------end of optical properties------------------

}


void myDetectorConstruction::AddOpticalPropertiesVacuum(G4Material* matVacuum)
{
    G4double photonWavelength[] ={0.24*micrometer, 0.1*micrometer};
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

// new method of calculationg reflectance
void myDetectorConstruction::AddPhotocathodeSurfaceProperties(G4OpticalSurface *photocathode_opsurf) {

    G4double photonWavelength[] =
       {100*nm, 101.8*nm, 103.7*nm, 105.6*nm, 107.6*nm, 109.6*nm, 111.6*nm, 113.7*nm, 115.8*nm, 117.9*nm, 120.1*nm, 122.3*nm,
        124.5*nm, 126.8*nm, 129.2*nm, 131.6*nm, 134*nm, 136.5*nm, 139*nm, 141.5*nm, 144.2*nm, 146.8*nm, 149.5*nm, 152.3*nm,
        155.1*nm, 158*nm, 160.9*nm, 163.8*nm, 166.9*nm, 170*nm, 173.1*nm, 176.3*nm, 179.5*nm, 182.8*nm, 186.2*nm, 189.7*nm,
        193.2*nm, 196.7*nm, 200.4*nm, 204.1*nm, 207.8*nm, 211.7*nm, 215.6*nm, 219.5*nm, 223.6*nm, 227.7*nm, 231.9*nm, 236.2*nm,
        240.6*nm};

    const G4int nEntries = sizeof(photonWavelength)/sizeof(G4double);

    G4double photonEnergy[nEntries];
    for (int i = nEntries; i--;)
        photonEnergy[i] = h_Planck*c_light/photonWavelength[i];

    G4double refractiveIndexReal[nEntries] =
       {1.144951, 1.135839, 1.125407, 1.103702, 1.091193, 1.071493, 1.05098, 1.040938, 1.024156, 1.01113, 1.001322, 0.989403,
        0.974532, 0.951885, 0.927984, 0.902064, 0.860704, 0.835372, 0.810144, 0.78465, 0.758239, 0.734709,0.717417, 0.698299,
        0.68, 0.674903, 0.67, 0.664104, 0.684959, 0.699429, 0.715265, 0.736101, 0.757116, 0.755732, 0.788406, 0.811363, 0.843707,
        0.876288, 0.893894, 0.926121, 0.94533, 0.964185, 0.97, 0.957536, 0.943928, 0.9341, 0.915394, 0.883099, 0.865438};

    G4double refractiveIndexImag[nEntries] =
       {0.719903, 0.705839, 0.698469, 0.691234, 0.69, 0.69, 0.69, 0.694531, 0.705844, 0.71887, 0.72, 0.725299, 0.73, 0.73,
        0.737339, 0.748968, 0.769648, 0.802837, 0.829856, 0.8638, 0.892641, 0.933226, 0.977748, 1.023402, 1.07245, 1.130388,
        1.187375, 1.246415, 1.314877, 1.358288, 1.405794, 1.444152, 1.484231, 1.521465, 1.578406, 1.612727, 1.653707, 1.677525,
        1.693895, 1.72306, 1.73, 1.734185, 1.744778, 1.743768, 1.752144, 1.783601, 1.814606, 1.852535, 1.901936};

    //my material property table
    G4MaterialPropertiesTable* myMPT = new G4MaterialPropertiesTable();
    myMPT->AddProperty("REALRINDEX1", photonEnergy, refractiveIndexReal, nEntries)
    ->SetSpline(true);
    myMPT->AddProperty("IMAGINARYRINDEX1", photonEnergy, refractiveIndexImag, nEntries)
    ->SetSpline(true);

    G4double thickness = fthick1;
    myMPT->AddConstProperty("THICKNESS1", thickness);

    G4double refractiveIndexReal2[nEntries] =
       {0.86, 0.86, 0.86, 0.86, 0.86, 0.86, 0.907088, 0.93, 0.93, 0.913327, 0.927385, 0.968288, 1.066797, 1.175075,
        1.270957, 1.332843, 1.379368, 1.4, 1.4, 1.402244, 1.459916, 1.635187, 1.964627, 2.289999, 2.570836, 2.842137,
        3.136905, 2.964945, 2.491985, 2.124166, 2.046818, 2.28989, 2.785555, 2.880481, 3.10464, 3.042062, 2.82186,
        2.662907, 2.547633, 2.608693, 2.924363, 2.949368, 2.704261, 2.799137, 2.979076, 2.85431, 2.609546, 2.404099,
        2.275139};

    G4double refractiveIndexImag2[nEntries] =
       {0.445, 0.445, 0.445, 0.445, 0.445, 0.445, 0.471059, 0.478, 0.489573, 0.526841, 0.615141, 0.74094, 0.891409,
        1.023286, 1.028593, 1.031035, 1.066621, 1.102245, 1.139681, 1.230526, 1.419743, 1.617581, 1.824, 1.843345,
        1.757582, 1.623461, 1.361667, 0.766472, 0.623098, 0.699139, 1.016066, 1.391582, 1.380832, 1.131868, 1.026344,
        0.666187, 0.3978, 0.403262, 0.438084, 0.60047, 0.684407, 0.334021, 0.161257, 0.312568, 0.292204, 0.088022, 0,
        0, 0};

    myMPT->AddProperty("REALRINDEX2", photonEnergy, refractiveIndexReal2, nEntries)
    ->SetSpline(true);
    myMPT->AddProperty("IMAGINARYRINDEX2", photonEnergy, refractiveIndexImag2, nEntries)
    ->SetSpline(true);

    thickness = fthick2;
    myMPT->AddConstProperty("THICKNESS2", thickness);


    for (int i = 0; i < nEntries; i++) {
        refractiveIndexReal2[i] = 1.;
        refractiveIndexImag2[i] = 0.;
    }
    myMPT->AddProperty("REALRINDEX3", photonEnergy, refractiveIndexReal2, nEntries)
    ->SetSpline(true);
    myMPT->AddProperty("IMAGINARYRINDEX3", photonEnergy, refractiveIndexImag2, nEntries)
    ->SetSpline(true);
    myMPT->AddConstProperty("THICKNESS3", 10.);

    G4int layers = 3;
    myMPT->AddConstProperty("THINLAYERS", layers);

    G4double photonWavelength2[] =
       {240.022123894*nm, 220.022123894*nm,
        209.797400046*nm, 199.812554122*nm,
        190.057131392*nm, 180.029304919*nm,
        170.020064629*nm, 160.011985976*nm,
        150.01784696*nm, 140.218282045*nm,
        129.492892897*nm, 120.001742455*nm,
        110.023866348*nm, 100.777240374*nm};

    const G4int nEntries2 = sizeof(photonWavelength2)/sizeof(G4double);

    G4double photonEnergy2[nEntries2];
    for (int i = nEntries2; i--;)
        photonEnergy2[i] = h_Planck*c_light/photonWavelength2[i];

    G4double photocathode_EFF[nEntries2] = //Enables 'detection' of photons
       {0.000001, 0.0715990453461, 0.286396181384, 0.715990453461,
        2.00477326969, 5.08353221957, 7.01670644391, 8.87828162291,
        9.88066825776, 13.8902147971, 14.9642004773, 14.9642004773,
        14.9642004773, 14.892601432};

    // G4double photocathode_EFF[nEntries];
    for(int i = 0; i < nEntries2; i++)
        photocathode_EFF[i] *= 0.01;

    myMPT->AddProperty("EFFICIENCY", photonEnergy2, photocathode_EFF,nEntries2);
    photocathode_opsurf->SetMaterialPropertiesTable(myMPT);

}


void myDetectorConstruction::AddThinLayerSurfaceProperties(G4OpticalSurface *thinLayer_opsurf) {

    G4double photonWavelength[] =
       {100*nm, 101.8*nm, 103.7*nm, 105.6*nm, 107.6*nm, 109.6*nm, 111.6*nm, 113.7*nm, 115.8*nm, 117.9*nm, 120.1*nm, 122.3*nm,
        124.5*nm, 126.8*nm, 129.2*nm, 131.6*nm, 134*nm, 136.5*nm, 139*nm, 141.5*nm, 144.2*nm, 146.8*nm, 149.5*nm, 152.3*nm,
        155.1*nm, 158*nm, 160.9*nm, 163.8*nm, 166.9*nm, 170*nm, 173.1*nm, 176.3*nm, 179.5*nm, 182.8*nm, 186.2*nm, 189.7*nm,
        193.2*nm, 196.7*nm, 200.4*nm, 204.1*nm, 207.8*nm, 211.7*nm, 215.6*nm, 219.5*nm, 223.6*nm, 227.7*nm, 231.9*nm, 236.2*nm,
        240.6*nm};

    const G4int nEntries = sizeof(photonWavelength)/sizeof(G4double);

    G4double photonEnergy[nEntries];
    for (int i = nEntries; i--;)
        photonEnergy[i] = h_Planck*c_light/photonWavelength[i];

    G4double refractiveIndexReal[nEntries] =
       {1.144951, 1.135839, 1.125407, 1.103702, 1.091193, 1.071493, 1.05098, 1.040938, 1.024156, 1.01113, 1.001322, 0.989403,
        0.974532, 0.951885, 0.927984, 0.902064, 0.860704, 0.835372, 0.810144, 0.78465, 0.758239, 0.734709,0.717417, 0.698299,
        0.68, 0.674903, 0.67, 0.664104, 0.684959, 0.699429, 0.715265, 0.736101, 0.757116, 0.755732, 0.788406, 0.811363, 0.843707,
        0.876288, 0.893894, 0.926121, 0.94533, 0.964185, 0.97, 0.957536, 0.943928, 0.9341, 0.915394, 0.883099, 0.865438};

    G4double refractiveIndexImag[nEntries] =
       {0.719903, 0.705839, 0.698469, 0.691234, 0.69, 0.69, 0.69, 0.694531, 0.705844, 0.71887, 0.72, 0.725299, 0.73, 0.73,
        0.737339, 0.748968, 0.769648, 0.802837, 0.829856, 0.8638, 0.892641, 0.933226, 0.977748, 1.023402, 1.07245, 1.130388,
        1.187375, 1.246415, 1.314877, 1.358288, 1.405794, 1.444152, 1.484231, 1.521465, 1.578406, 1.612727, 1.653707, 1.677525,
        1.693895, 1.72306, 1.73, 1.734185, 1.744778, 1.743768, 1.752144, 1.783601, 1.814606, 1.852535, 1.901936};

    //my material property table
    G4MaterialPropertiesTable* myMPT = new G4MaterialPropertiesTable();
    myMPT->AddProperty("REALRINDEX1", photonEnergy, refractiveIndexReal, nEntries)
    ->SetSpline(true);
    myMPT->AddProperty("IMAGINARYRINDEX1", photonEnergy, refractiveIndexImag, nEntries)
    ->SetSpline(true);

    G4double thickness = fthick1;
    myMPT->AddConstProperty("THICKNESS1", thickness);

    for (int i = 0; i < nEntries; i++) {
        refractiveIndexReal[i] = 1.;
        refractiveIndexImag[i] = 0.;
    }
    myMPT->AddProperty("REALRINDEX2", photonEnergy, refractiveIndexReal, nEntries)
    ->SetSpline(true);
    myMPT->AddProperty("IMAGINARYRINDEX2", photonEnergy, refractiveIndexImag, nEntries)
    ->SetSpline(true);
    myMPT->AddConstProperty("THICKNESS2", 10.);

    G4int layers = 2;
    myMPT->AddConstProperty("THINLAYERS", layers);


    thinLayer_opsurf->SetMaterialPropertiesTable(myMPT);

}

void myDetectorConstruction::AddRefCoatSurfaceProperties(G4OpticalSurface *refCoat_opsurf) {
    //**Thin Layer surface properties

    G4double photonWavelength[] =
        {240.*nm, 230.*nm,220.*nm,
         210.*nm,200.*nm,190.*nm,
         180.*nm,170.*nm,160.*nm,
         150.*nm,140.*nm,130.*nm,
         120.*nm,113.7*nm,100.*nm};

    const G4int nEntries = sizeof(photonWavelength)/sizeof(G4double);

    G4double photonEnergy[nEntries];
    for (int i = nEntries; i--;)
        photonEnergy[i] = h_Planck*c_light/photonWavelength[i];

    G4double coating_EFF[nEntries];
    G4double coating_REF[nEntries];
    for(int i = nEntries; i--;) {
        coating_EFF[i] = 1.;
        coating_REF[i] = 1.;
    }

    G4MaterialPropertiesTable* coating_mt = new G4MaterialPropertiesTable();
    coating_mt->AddProperty("EFFICIENCY", photonEnergy, coating_EFF,nEntries);
    coating_mt->AddProperty("REFLECTIVITY", photonEnergy, coating_REF, nEntries);

    refCoat_opsurf->SetMaterialPropertiesTable(coating_mt);

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
    G4double world_sizeZ  = 25.*cm;

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


    //----------------Padlike Radiator--------------------------------------------------


//padlike Radiator
    G4double rmaxRadiator = 2.54*cm;
    const G4double kdistance = 0.05*cm;

    G4Material* matRadiator = nist->FindOrBuildMaterial("G4_MAGNESIUM_FLUORIDE");
    AddOpticalPropertiesMgF2(matRadiator);

    G4ThreeVector posRadiator = G4ThreeVector(0.*cm, 0.*cm, 0.*cm);
    G4double phiminRadiator = 30.*deg, phimaxRadiator = 390.*deg;
    const G4int numSides = 6;
    const G4int numZplanes = 2;
    G4double hzRadiator = 3*mm;
    G4double zplane[numZplanes] = {-0.5*hzRadiator, 0.5*hzRadiator};
    G4double rinner[numSides];
    G4double RtoCorner = 0.45*cm;
    G4double RtoSide = RtoCorner*std::sqrt(3.)/2.;
    G4double router[numSides]; //needs to be distance from 0 to SIDE!

    for (int i = 0; i <numSides; i++) {
        rinner[i] = 0.;
        router[i] = RtoSide;
    }

    G4Polyhedra* solidRadiator;
    G4LogicalVolume* LVRadiator;

    G4int counter = 0;
    G4int counterY = 0;


//
    G4Material* matRefCoat = nist->FindOrBuildMaterial("G4_Au");

    G4double rinnerRefCoat[numSides];
    G4double routerRefCoat[numSides]; //needs to be distance from 0 to SIDE!
    G4double thicknessRefCoat = kdistance/4.;

    for (int i = 0; i <numSides; i++) {
        rinnerRefCoat[i] = RtoSide;
        routerRefCoat[i] = RtoSide + thicknessRefCoat;
    }

    G4Polyhedra* solidRefCoat;
    G4LogicalVolume* LVRefCoat;

    G4OpticalSurface* RefCoat_opsurf;


//single Radiator pad

    G4String nameRadPad = "RadiatorPad" + std::to_string(counter) + "l";
    solidRadiator = new G4Polyhedra(nameRadPad, phiminRadiator,
        phimaxRadiator, numSides, numZplanes, zplane, rinner, router);

    LVRadiator = new G4LogicalVolume(solidRadiator, matRadiator, nameRadPad);
    new G4PVPlacement(0, posRadiator, LVRadiator, nameRadPad, logicWorld, false, 0, checkOverlaps);
    G4VisAttributes * VisAttRadiator = new G4VisAttributes(G4Colour(1. ,1. ,1.));
    VisAttRadiator->SetForceWireframe(true);
    LVRadiator->SetVisAttributes(VisAttRadiator);

    //refCoat pad

    G4String nameRefCoatPad = "RefCoatPad" + std::to_string(counter) + "l";
    solidRefCoat = new G4Polyhedra(nameRefCoatPad, phiminRadiator,
        phimaxRadiator, numSides, numZplanes, zplane, rinnerRefCoat, routerRefCoat);

    LVRefCoat = new G4LogicalVolume(solidRefCoat, matRefCoat, nameRefCoatPad);
    new G4PVPlacement(0, posRadiator, LVRefCoat, nameRefCoatPad, logicWorld, false, 0, checkOverlaps);

    //Optical surface
    G4String nameRefCoatOpSurf = "refCoat_opsurf" + std::to_string(counter) + "l";
    RefCoat_opsurf = new G4OpticalSurface(nameRefCoatOpSurf, glisur,polished, dielectric_metal);
    AddRefCoatSurfaceProperties(RefCoat_opsurf);
    G4String nameRefCoatSurf = "refCoat_surf" + std::to_string(counter) + "l";
    new G4LogicalSkinSurface(nameRefCoatSurf, LVRefCoat, RefCoat_opsurf);




//top
//     posRadiator.setY(RtoCorner + std::sqrt(3.)/4*kdistance);
//     while (posRadiator.getR() < rmaxRadiator + RtoSide) {
//         counterY +=1;
//         //along X
//         while (posRadiator.getR() < rmaxRadiator + RtoSide) {
//             //radiatorPad
//             G4String nameRadPad = "RadiatorPad" + std::to_string(counter) + "l";
//             solidRadiator = new G4Polyhedra(nameRadPad, phiminRadiator,
//                 phimaxRadiator, numSides, numZplanes, zplane, rinner, router);
//
//             LVRadiator = new G4LogicalVolume(solidRadiator, matRadiator, nameRadPad);
//             new G4PVPlacement(0, posRadiator, LVRadiator, nameRadPad, logicWorld, false, 0, checkOverlaps);
//             G4VisAttributes * VisAttRadiator = new G4VisAttributes(G4Colour(1. ,1. ,1.));
//             VisAttRadiator->SetForceWireframe(true);
//             LVRadiator->SetVisAttributes(VisAttRadiator);
//
//             //refCoat pad
//
//             G4String nameRefCoatPad = "RefCoatPad" + std::to_string(counter) + "l";
//             solidRefCoat = new G4Polyhedra(nameRefCoatPad, phiminRadiator,
//                 phimaxRadiator, numSides, numZplanes, zplane, rinnerRefCoat, routerRefCoat);
//
//             LVRefCoat = new G4LogicalVolume(solidRefCoat, matRefCoat, nameRefCoatPad);
//             new G4PVPlacement(0, posRadiator, LVRefCoat, nameRefCoatPad, logicWorld, false, 0, checkOverlaps);
//
//             //Optical surface
//             G4String nameRefCoatOpSurf = "refCoat_opsurf" + std::to_string(counter) + "l";
//             RefCoat_opsurf = new G4OpticalSurface(nameRefCoatOpSurf, glisur,polished, dielectric_metal);
//             AddRefCoatSurfaceProperties(RefCoat_opsurf);
//             G4String nameRefCoatSurf = "refCoat_surf" + std::to_string(counter) + "l";
//             new G4LogicalSkinSurface(nameRefCoatSurf, LVRefCoat, RefCoat_opsurf);
//
//
//             posRadiator.setX(-posRadiator.x());
//             //counter += 1;
//
//     //------------------------------------------------------------------------------
//
//             nameRadPad = "RadiatorPad" + std::to_string(counter) + "r";
//             solidRadiator = new G4Polyhedra(nameRadPad, phiminRadiator,
//                 phimaxRadiator, numSides, numZplanes, zplane, rinner, router);
//             //
//             LVRadiator = new G4LogicalVolume(solidRadiator, matRadiator, nameRadPad);
//             new G4PVPlacement(0, posRadiator, LVRadiator, nameRadPad, logicWorld, false, 0, checkOverlaps);
//             //         // Assignment of the visualization attributes to the logical volume
//             LVRadiator->SetVisAttributes(VisAttRadiator);
//
//             //refCoat pad
//
//             nameRefCoatPad = "RefCoatPad" + std::to_string(counter) + "r";
//             solidRefCoat = new G4Polyhedra(nameRefCoatPad, phiminRadiator,
//                 phimaxRadiator, numSides, numZplanes, zplane, rinnerRefCoat, routerRefCoat);
//
//             LVRefCoat = new G4LogicalVolume(solidRefCoat, matRefCoat, nameRefCoatPad);
//             new G4PVPlacement(0, posRadiator, LVRefCoat, nameRefCoatPad, logicWorld, false, 0, checkOverlaps);
//
//             //Optical surface
//             nameRefCoatOpSurf = "refCoat_opsurf" + std::to_string(counter) + "r";
//             RefCoat_opsurf = new G4OpticalSurface(nameRefCoatOpSurf, glisur,polished, dielectric_metal);
//             AddRefCoatSurfaceProperties(RefCoat_opsurf);
//             nameRefCoatSurf = "refCoat_surf" + std::to_string(counter) + "r";
//             new G4LogicalSkinSurface(nameRefCoatSurf, LVRefCoat, RefCoat_opsurf);
//
//             posRadiator.setX(-posRadiator.x() + std::sqrt(3)*RtoCorner + kdistance);
//             counter += 1;
//         }
//         posRadiator.setY(posRadiator.y() + 3./2.*RtoCorner + std::sqrt(3.)/2.*kdistance);
//         if (counterY%2)
//             posRadiator.setX(RtoCorner*std::sqrt(3)/2. + kdistance/2.);
//         else
//             posRadiator.setX(0);
//     }
//
// //bottom
//
//     counterY = 1;
//     posRadiator.setX(RtoCorner*std::sqrt(3)/2. + kdistance/2.);
//     posRadiator.setY(-RtoCorner/2. - std::sqrt(3.)/4*kdistance);
//     while (posRadiator.getR() < rmaxRadiator + RtoSide) {
//         counterY +=1;
//         //along X
//
//         while (posRadiator.getR() < rmaxRadiator + RtoSide) {
//             G4String nameRadPad = "RadiatorPad" + std::to_string(counter);
//             solidRadiator = new G4Polyhedra(nameRadPad, phiminRadiator,
//                 phimaxRadiator, numSides, numZplanes, zplane, rinner, router);
//
//             LVRadiator = new G4LogicalVolume(solidRadiator, matRadiator, nameRadPad);
//             new G4PVPlacement(0, posRadiator, LVRadiator, nameRadPad, logicWorld, false, 0, checkOverlaps);
//             G4VisAttributes * VisAttRadiator = new G4VisAttributes(G4Colour(1. ,1. ,1.));
//                     // Set the forced wireframe style
//             VisAttRadiator->SetForceWireframe(true);
//                     // Assignment of the visualization attributes to the logical volume
//             LVRadiator->SetVisAttributes(VisAttRadiator);
//
//             //refCoat pad
//
//             G4String nameRefCoatPad = "RefCoatPad" + std::to_string(counter) + "l";
//             solidRefCoat = new G4Polyhedra(nameRefCoatPad, phiminRadiator,
//                 phimaxRadiator, numSides, numZplanes, zplane, rinnerRefCoat, routerRefCoat);
//
//             LVRefCoat = new G4LogicalVolume(solidRefCoat, matRefCoat, nameRefCoatPad);
//             new G4PVPlacement(0, posRadiator, LVRefCoat, nameRefCoatPad, logicWorld, false, 0, checkOverlaps);
//
//             //Optical surface
//             G4String nameRefCoatOpSurf = "refCoat_opsurf" + std::to_string(counter) + "l";
//             RefCoat_opsurf = new G4OpticalSurface(nameRefCoatOpSurf, glisur,polished, dielectric_metal);
//             AddRefCoatSurfaceProperties(RefCoat_opsurf);
//             G4String nameRefCoatSurf = "refCoat_surf" + std::to_string(counter) + "l";
//             new G4LogicalSkinSurface(nameRefCoatSurf, LVRefCoat, RefCoat_opsurf);
//
//             posRadiator.setX(-posRadiator.x());
//
//     //------------------------------------------------------------------------------
//
//             nameRadPad = "RadiatorPad" + std::to_string(counter);
//             solidRadiator = new G4Polyhedra(nameRadPad, phiminRadiator,
//                 phimaxRadiator, numSides, numZplanes, zplane, rinner, router);
//
//             LVRadiator  = new G4LogicalVolume(solidRadiator, matRadiator, nameRadPad);
//             new G4PVPlacement(0, posRadiator, LVRadiator, nameRadPad, logicWorld, false, 0, checkOverlaps);
//                     // Assignment of the visualization attributes to the logical volume
//             LVRadiator->SetVisAttributes(VisAttRadiator);
//
//             //refCoat pad
//
//             nameRefCoatPad = "RefCoatPad" + std::to_string(counter) + "r";
//             solidRefCoat = new G4Polyhedra(nameRefCoatPad, phiminRadiator,
//                 phimaxRadiator, numSides, numZplanes, zplane, rinnerRefCoat, routerRefCoat);
//
//             LVRefCoat = new G4LogicalVolume(solidRefCoat, matRefCoat, nameRefCoatPad);
//             new G4PVPlacement(0, posRadiator, LVRefCoat, nameRefCoatPad, logicWorld, false, 0, checkOverlaps);
//
//             //Optical surface
//             nameRefCoatOpSurf = "refCoat_opsurf" + std::to_string(counter) + "r";
//             RefCoat_opsurf = new G4OpticalSurface(nameRefCoatOpSurf, glisur,polished, dielectric_metal);
//             AddRefCoatSurfaceProperties(RefCoat_opsurf);
//             nameRefCoatSurf = "refCoat_surf" + std::to_string(counter) + "r";
//             new G4LogicalSkinSurface(nameRefCoatSurf, LVRefCoat, RefCoat_opsurf);
//
//             posRadiator.setX(-posRadiator.x() + std::sqrt(3)*RtoCorner + kdistance);
//             counter += 1;
//         }
//         posRadiator.setY(posRadiator.y() - 3./2.*RtoCorner - std::sqrt(3.)/2.*kdistance);
//         if (counterY%2)
//             posRadiator.setX(RtoCorner*std::sqrt(3)/2. + kdistance/2.);
//         else
//             posRadiator.setX(0);
//    }






    // G4Polyhedra* solidRadiator = new G4Polyhedra("Radiator", phiminRadiator,
        // phimaxRadiator, numSides, numZplanes, zplane, rinner, router);
    //
    // G4LogicalVolume* LVRadiator = new G4LogicalVolume(solidRadiator, matRadiator, "Radiator");
    // new G4PVPlacement(0, posRadiator, LVRadiator, "Radiator", logicWorld, false, 0, checkOverlaps);
    // G4VisAttributes * VisAttRadiator = new G4VisAttributes(G4Colour(1. ,1. ,1.));
    //         // Set the forced wireframe style
    // VisAttRadiator->SetForceWireframe(true);
    //         // Assignment of the visualization attributes to the logical volume
    // LVRadiator->SetVisAttributes(VisAttRadiator);


    //----------------ThinLayer-------------------

    // G4double rminThinLayer =  0.5*cm;
    // G4double rmaxThinLayer =  2.54*cm;
    // // G4double rmaxThinLayer =  1.4*cm;
    // G4double hzThinLayer = 5.5*nm;//5.5*nm; //if you change this, change transmitance!!!!
    // G4double phiminThinLayer = 0.*deg, phimaxThinLayer = 360.*deg;
    //
    // G4Material* matThinLayer = nist->FindOrBuildMaterial("G4_Cr");
    // G4ThreeVector posThinLayer = G4ThreeVector(0.*cm, 0.*cm, posRadiator[2] + 0.5*hzThinLayer + 0.5*hzRadiator);
    // //AddOpticalPropertiesCr(matThinLayer);
    //
    // G4Tubs* solidThinLayer = new G4Tubs("ThinLayer", rminThinLayer, rmaxThinLayer, 0.5*hzThinLayer, phiminThinLayer, phimaxThinLayer);
    // G4LogicalVolume* LVThinLayer = new G4LogicalVolume(solidThinLayer, matThinLayer, "ThinLayer");
    //
    // new G4PVPlacement(0, posThinLayer, LVThinLayer, "ThinLayer", logicWorld, false, 0, checkOverlaps);
    //
    // G4VisAttributes * VisAttThinLayer = new G4VisAttributes(G4Colour(1., 0., 1.));
    //           // Set the forced wireframe style
    // //VisAttThinLayer->SetForceWireframe(true);
    //           // Assignment of the visualization attributes to the logical volume
    // LVThinLayer->SetVisAttributes(VisAttThinLayer);
    //
    // //Optical surface
    // G4OpticalSurface* thinLayer_opsurf = new G4OpticalSurface("thinLayer_opsurf", glisur,polished, thin_films);
    // AddThinLayerSurfaceProperties(thinLayer_opsurf);
    //
    // new G4LogicalSkinSurface("thinLayer_surf", LVThinLayer, thinLayer_opsurf);





    // //----------------Photocathode---------------------

    G4double rminPhotocathode =  0.*cm;
    G4double rmaxPhotocathode =  2.54*cm;
    G4double hzPhotocathode = 18*nm;
    G4double phiminPhotocathode = 0.*deg, phimaxPhotocathode = 360.*deg;

    G4Material* matPhotocathode = nist->FindOrBuildMaterial("G4_CESIUM_IODIDE");
    //G4Material* matPhotocathode = nist->FindOrBuildMaterial("Galactic");
    //G4ThreeVector posPhotocathode = G4ThreeVector(0.*cm, 0.*cm, posThinLayer[2] + 0.5*hzPhotocathode + 0.5*hzThinLayer);
     G4ThreeVector posPhotocathode = G4ThreeVector(0.*cm, 0.*cm, posRadiator[2] + 0.5*hzPhotocathode + 0.5*hzRadiator);

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
    G4OpticalSurface* photocathode_opsurf = new G4OpticalSurface("photocathode_opsurf", glisur,polished, thin_films);
    AddPhotocathodeSurfaceProperties(photocathode_opsurf);
    // G4OpticalSurface* photocathode_opsurf = new G4OpticalSurface("photocathode_opsurf", glisur,polished, dielectric_dielectric);
    // AddAirSurfaceProperties(photocathode_opsurf);
    new G4LogicalSkinSurface("photocath_surf", LVPhotocathode, photocathode_opsurf);



//calorimeter----------------------------------------------------
    G4double radiationlength = 1.265*cm;
    G4double asideCal = 1.5*cm;
    G4double thickofCal = 2*radiationlength;

    G4Material* matCalorimeter = nist->FindOrBuildMaterial("G4_GLASS_LEAD");
    G4ThreeVector posCalorimeter = G4ThreeVector(0.*cm, 0.*cm, - posRadiator[2] -0.5*hzRadiator - thickofCal - 6*radiationlength);
    G4Box* solidCalorimeter = new G4Box("Calorimeter", asideCal, asideCal, thickofCal);
    G4LogicalVolume* LVCalorimeter = new G4LogicalVolume(solidCalorimeter, matCalorimeter, "Calorimeter");

    new G4PVPlacement(0, posCalorimeter, LVCalorimeter, "Calorimeter", logicWorld, false, 0, checkOverlaps);

    G4VisAttributes * VisAttCalorimeter = new G4VisAttributes(G4Colour(1. ,1. ,1.));
    VisAttCalorimeter->SetForceWireframe(true);
    LVCalorimeter->SetVisAttributes(VisAttCalorimeter);






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
