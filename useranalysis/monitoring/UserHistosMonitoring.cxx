#include "UserHistosMonitoring.h"

// ROOT
//#include <TH1D.h>
//#include <TH2D.h>

// Go4
#include <TGo4Analysis.h>

UserHistosMonitoring::UserHistosMonitoring()
{
	TGo4Analysis* a = TGo4Analysis::Instance();

	/*d1F3[0]    = a->MakeTH1('D', "F3/QDC/F3_QDC_L",  "F3 QDC  left", 4096, 0., 4096.);
	d1F3[1]    = a->MakeTH1('D', "F3/QDC/F3_QDC_R",  "F3 QDC right", 4096, 0., 4096.);
	d1F3[2]    = a->MakeTH1('D', "F3/QDC/F3_QDC_U",  "F3 QDC    up", 4096, 0., 4096.);
	d1F3[3]    = a->MakeTH1('D', "F3/QDC/F3_QDC_D",  "F3 QDC  down", 4096, 0., 4096.);
	d1tF3[0]   = a->MakeTH1('D', "F3/TDC/tF3_TDC_L", "F3 TDC  left", 6400, 0., 6400.);
	d1tF3[1]   = a->MakeTH1('D', "F3/TDC/tF3_TDC_R", "F3 TDC right", 6400, 0., 6400.);
	d1tF3[2]   = a->MakeTH1('D', "F3/TDC/tF3_TDC_U", "F3 TDC    up", 6400, 0., 6400.);
	d1tF3[3]   = a->MakeTH1('D', "F3/TDC/tF3_TDC_D", "F3 TDC  down", 6400, 0., 6400.);

	d1F5[0]    = a->MakeTH1('D', "F5/QDC/F5_QDC_L",  "F5 QDC  left", 4096, 0., 4096.);
	d1F5[1]    = a->MakeTH1('D', "F5/QDC/F5_QDC_R",  "F5 QDC right", 4096, 0., 4096.);
	d1F5[2]    = a->MakeTH1('D', "F5/QDC/F5_QDC_U",  "F5 QDC    up", 4096, 0., 4096.);
	d1F5[3]    = a->MakeTH1('D', "F5/QDC/F5_QDC_D",  "F5 QDC  down", 4096, 0., 4096.);
	d1tF5[0]   = a->MakeTH1('D', "F5/TDC/tF5_TDC_L", "F5 TDC  left", 6400, 0., 6400.);
	d1tF5[1]   = a->MakeTH1('D', "F5/TDC/tF5_TDC_R", "F5 TDC right", 6400, 0., 6400.);
	d1tF5[2]   = a->MakeTH1('D', "F5/TDC/tF5_TDC_U", "F5 TDC    up", 6400, 0., 6400.);
	d1tF5[3]   = a->MakeTH1('D', "F5/TDC/tF5_TDC_D", "F5 TDC  down", 6400, 0., 6400.);

	d1F6[0]    = a->MakeTH1('D', "F6/QDC/F6_QDC_L",  "F6 QDC  left", 4096, 0., 4096.);
	d1F6[1]    = a->MakeTH1('D', "F6/QDC/F6_QDC_R",  "F6 QDC right", 4096, 0., 4096.);
	d1F6[2]    = a->MakeTH1('D', "F6/QDC/F6_QDC_U",  "F6 QDC    up", 4096, 0., 4096.);
	d1F6[3]    = a->MakeTH1('D', "F6/QDC/F6_QDC_D",  "F6 QDC  down", 4096, 0., 4096.);
	d1tF6[0]   = a->MakeTH1('D', "F6/TDC/tF6_TDC_L", "F6 TDC  left", 6400, 0., 6400.);
	d1tF6[1]   = a->MakeTH1('D', "F6/TDC/tF6_TDC_R", "F6 TDC right", 6400, 0., 6400.);
	d1tF6[2]   = a->MakeTH1('D', "F6/TDC/tF6_TDC_U", "F6 TDC    up", 6400, 0., 6400.);
	d1tF6[3]   = a->MakeTH1('D', "F6/TDC/tF6_TDC_D", "F6 TDC  down", 6400, 0., 6400.);

	d1tMWPC[0] = a->MakeTH1('D', "tMWPC/tX1", "MWPC TDC  X1", 6400, 0., 6400.);
	d1tMWPC[1] = a->MakeTH1('D', "tMWPC/tY1", "MWPC TDC  Y1", 6400, 0., 6400.);
	d1tMWPC[2] = a->MakeTH1('D', "tMWPC/tX2", "MWPC TDC  X2", 6400, 0., 6400.);
	d1tMWPC[3] = a->MakeTH1('D', "tMWPC/tY2", "MWPC TDC  Y2", 6400, 0., 6400.);*/

	histoMWPCnx1 = a->MakeTH1('D', "MWPC/histoMWPCnx1", "histoMWPCnx1;number of fired wires", 33, 0., 33.);
	histoMWPCny1 = a->MakeTH1('D', "MWPC/histoMWPCny1", "histoMWPCny1;number of fired wires", 33, 0., 33.);
	histoMWPCnx2 = a->MakeTH1('D', "MWPC/histoMWPCnx2", "histoMWPCnx2;number of fired wires", 33, 0., 33.);
	histoMWPCny2 = a->MakeTH1('D', "MWPC/histoMWPCny2", "histoMWPCny2;number of fired wires", 33, 0., 33.);
	histoMWPCx1  = a->MakeTH1('D', "MWPC/histoMWPCx1",  "histoMWPCx1;index of the fired wire",  32, 0., 32.);
	histoMWPCy1  = a->MakeTH1('D', "MWPC/histoMWPCy1",  "histoMWPCy1;index of the fired wire",  32, 0., 32.);
	histoMWPCx2  = a->MakeTH1('D', "MWPC/histoMWPCx2",  "histoMWPCx2;index of the fired wire",  32, 0., 32.);
	histoMWPCy2  = a->MakeTH1('D', "MWPC/histoMWPCy2",  "histoMWPCy2;index of the fired wire",  32, 0., 32.);
	histoMWPCrx1 = a->MakeTH1('D', "MWPC/histoMWPCrx1", "histoMWPCrx1;index of the fired wire",  32, 0., 32.);
	histoMWPCry1 = a->MakeTH1('D', "MWPC/histoMWPCry1", "histoMWPCry1;index of the fired wire",  32, 0., 32.);
	histoMWPCrx2 = a->MakeTH1('D', "MWPC/histoMWPCrx2", "histoMWPCrx2;index of the fired wire",  32, 0., 32.);
	histoMWPCry2 = a->MakeTH1('D', "MWPC/histoMWPCry2", "histoMWPCry2;index of the fired wire",  32, 0., 32.);

}

UserHistosMonitoring::~UserHistosMonitoring()
{
	/**
	 * Not 100% sure, but it looks as if you do not need to delete your histograms yourself.
	 * TGo4AnalysisObjectManager, as part of the singleton TGo4Analysis, will take care.
	 */
}

ClassImp(UserHistosMonitoring)
