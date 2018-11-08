#include "SiCalibPars.h"

#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

using namespace std;

SiCalibPars::SiCalibPars(const char* name) :
TGo4Parameter(name)
{

} //----------------------------------------------------------------

SiCalibPars::~SiCalibPars()
{
  cout << "Destructor SiCalibPars called " << endl;
} //-----------------------------------------------------------------

void SiCalibPars::Init(Int_t nChannels,TString stName)
{
// fill all paramteres here
  Double_t par1,par2;
  ifstream myfile;
  TString line,fileName;
  Int_t count=-2;// cause first two lines in .cal files are not parameters

  fileName = "/media/user/work/software/fork/useranalysis/calibration/parameters/" + stName + ".cal";
  myfile.open(fileName.Data());
  if(!myfile.is_open()) {
    cerr << fileName.Data() << " does not exist! " << endl;
    return;
  }

  fName = stName;
  fPar1.Set(nChannels);
  fPar2.Set(nChannels);

  while (! myfile.eof() ) { 
    line.ReadLine(myfile);
    if (count < 0) { // TODO may be we can use first two numbers from the .cal file 
      count++;
      continue;
    }
    if (line.IsNull() || count >= nChannels) break;
    sscanf(line.Data(),"%lf %lf", &par1,&par2);   
    fPar1.SetAt(par1,count);
    fPar2.SetAt(par2,count);    
    count++;
  }  
  cout << fName << endl;
  for(Int_t i=0;i<nChannels;i++) {
    cout << fPar1.GetAt(i) << " " << fPar2.GetAt(i) << endl; 
  } 

} 
//-----------------------------------------------------------------
TString SiCalibPars::getName()
{
  return fName;
}
//-----------------------------------------------------------------
Double_t SiCalibPars::getPar1(Int_t i)
{
  return fPar1.GetAt(i);
}
//-----------------------------------------------------------------
Double_t SiCalibPars::getPar2(Int_t i)
{
  return fPar2.GetAt(i);
}
//-----------------------------------------------------------------