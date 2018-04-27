
#ifndef myFile_h
#define myFile_h 1

#include <iostream>
#include <fstream>
using namespace std;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class myFile
{
  public:
    myFile(){;}
    virtual ~myFile(){;}

  public:
    ofstream* Savefile;
    void SetFile(ofstream &file) {Savefile = &file; return;}
    ofstream* GetFile() {return Savefile;}

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
