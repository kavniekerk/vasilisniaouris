This is a masters thesis project.
It is a geant4 simulation of the picosec micromegas detector used for timing after an electromagnetic calorimer.
It is based on geant4 examples B1 and lAr-Calorimeter.
The code runs with geant4.10.03.p03.
However I changed the G4OpBoundaryProcess.hh and G4OpBoundaryProcess.cc in order to simulate reflectivity of thin film layers (based on the specific needs of the detector).

In order to run the code you have to re-build geant4 by replacing those two files prior to building. (they are in <geant4sourcefolder>/geant4.10.03.p03/source/processes/optical/include/)

After that you run ./myPICOSEC in order see the detector with visuals or ./myPICOSEC runBeam.mac in order to run many events and get back a root file.

In order to compile the code, write the bash script:

#!/bin/bash

G4HOME=" <geant4buildfolder>/geant4.10.03-install"
G4PROJ="<path_to_projects>"
EXNAME="<project>"

cmake -DGeant4_DIR= $G4HOME/lib/Geant4-10.3.3/ $G4PROJ/$EXNAME

cmake -DCMAKE_PREFIX_PATH=$G4HOME $G4PROJ/$EXNAME

make -j VERBOSE=1


