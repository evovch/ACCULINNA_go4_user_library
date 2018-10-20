#ifndef SICALIBPARS_H
#define SICALIBPARS_H

#include "TGo4Parameter.h"
#include <TArrayD.h>

class SiCalibPars : public TGo4Parameter {
public:
  SiCalibPars(const char* name = 0);
  virtual ~SiCalibPars();

  void Init(Int_t nChannels,TString stName);
  
  TString getName();
  Double_t getPar1(Int_t i);
  Double_t getPar2(Int_t i);  

private:
  // name of the station 
  TString fName;
  // calibration parameters
  TArrayD fPar1;
  TArrayD fPar2;
  
  ClassDef(SiCalibPars,1)
};

#endif //SICALIBPARS_H
