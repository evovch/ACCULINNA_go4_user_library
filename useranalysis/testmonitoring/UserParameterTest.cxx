// $Id: TXXXParameter.cxx 1318 2015-01-08 13:28:41Z linev $
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

#include "UserParameterTest.h"

using namespace std;

UserParameterTest::UserParameterTest(const char* name) :
	TGo4Parameter(name)
{
  f20_L_Name = "SSD20_L";
  f1_L_Name = "DSDX_L";

  f20_R_Name = "SSD20_R";  
  f1_R_Name = "DSDY_R";

  fX_C_Name = "DSDX_C";
  fY_C_Name = "DSDY_C";
  fCsI_Name = "CsI";  

  //offline thresholds [Channels]
  fThreshSSD20_L = 100;
  fThreshDSDX_L = 120;
  fThreshDSDY_L = 120;
  fThreshSSD_L = 120;  
  
  fThreshSSD20_R = 100;
  fThreshSSDY_R = 120; 
  fThreshSSD_R = 120;

  fThreshSSD_C = 120;

}
//-----------------------------------------------------------------------
UserParameterTest::~UserParameterTest() {
  cout << "Destructor UserParameterTest called " << endl;
}
