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
   f1_R_Name = "SSDY_R";

}
//-----------------------------------------------------------------------
UserParameterTest::~UserParameterTest() {
  cout << "Destructor UserParameterTest called " << endl;
}