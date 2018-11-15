// $Id: TXXXParameter.h 1318 2015-01-08 13:28:41Z linev $
//-----------------------------------------------------------------------
//       The GSI Online Offline Object Oriented (Go4) Project
//         Experiment Data Processing at EE department, GSI
//-----------------------------------------------------------------------
// Copyright (C) 2000- GSI Helmholtzzentrum f�r Schwerionenforschung GmbH
//                     Planckstr. 1, 64291 Darmstadt, Germany
// Contact:            http://go4.gsi.de
//-----------------------------------------------------------------------
// This software can be used under the license agreements as stated
// in Go4License.txt file which is part of the distribution.
//-----------------------------------------------------------------------

#ifndef UserParameterTest_H
#define UserParameterTest_H

#include "TGo4Parameter.h"

//#include "TArrayI.h"

class UserParameterTest : public TGo4Parameter {
public:
	UserParameterTest(const char* name = "TestPar");
	virtual ~UserParameterTest();

  TString f20_L_Name;
  TString f1_L_Name;

  TString f20_R_Name;  
  TString f1_R_Name;

  TString fX_C_Name;
  TString fY_C_Name;
  TString fCsI_Name;

  // offline Thresholds [Channels]
  Double_t fThreshSSD20_L;
  Double_t fThreshDSDX_L;
  Double_t fThreshDSDY_L;
  Double_t fThreshSSD_L;  

  Double_t fThreshSSD20_R;
  Double_t fThreshSSDY_R; 
  Double_t fThreshSSD_R;
   
  Double_t fThreshSSD_C; 

	ClassDef(UserParameterTest,1)
};

#endif
