#include "UserHistosRawMonitoring.h"

// ROOT
#include <TH1D.h>
#include <TH2D.h>

// Go4
#include <TGo4Analysis.h>

UserHistosRawMonitoring::UserHistosRawMonitoring()
{
	TGo4Analysis* a = TGo4Analysis::Instance();

	//TODO define correct ranges

	mhEvType             = a->MakeTH1('D', "mhEvType            ", "mhEvType            ", 300, 0., 300.);
	mhEvSubType          = a->MakeTH1('D', "mhEvSubType         ", "mhEvSubType         ", 300, 0., 300.);
	mhEvDummy            = a->MakeTH1('D', "mhEvDummy           ", "mhEvDummy           ", 300, 0., 300.);
	mhEvTrigger          = a->MakeTH1('D', "mhEvTrigger         ", "mhEvTrigger         ", 300, 0., 300.);
	mhEvCount            = a->MakeTH1('D', "mhEvCount           ", "mhEvCount           ", 300, 0., 300.);
	mhSubEvDlen          = a->MakeTH1('D', "mhSubEvDlen         ", "mhSubEvDlen         ", 300, 0., 300.);
	mhSubEvType          = a->MakeTH1('D', "mhSubEvType         ", "mhSubEvType         ", 300, 0., 300.);
	mhSubEvSubcrate      = a->MakeTH1('D', "mhSubEvSubcrate     ", "mhSubEvSubcrate     ", 300, 0., 300.);
	mhSubEvControl       = a->MakeTH1('D', "mhSubEvControl      ", "mhSubEvControl      ", 300, 0., 300.);
	mhSubEvFullID        = a->MakeTH1('D', "mhSubEvFullID       ", "mhSubEvFullID       ", 300, 0., 300.);
	mhSubEvProcID        = a->MakeTH1('D', "mhSubEvProcID       ", "mhSubEvProcID       ", 300, 0., 300.);
	mhSubsubEvVendor     = a->MakeTH1('D', "mhSubsubEvVendor    ", "mhSubsubEvVendor    ", 4, 0., 4.);
	mhSubsubEvGeo        = a->MakeTH1('D', "mhSubsubEvGeo       ", "mhSubsubEvGeo       ", 65, -1., 64.);
	mhSubsubEvModule     = a->MakeTH1('D', "mhSubsubEvModule    ", "mhSubsubEvModule    ", 65, -1., 64.);
	mhChannel            = a->MakeTH1('D', "mhChannel           ", "mhChannel           ", 64, 0., 64.);
	mhValueQA            = a->MakeTH1('D', "mhValueQA           ", "mhValueQA           ", 2048, 0., 4. *2048.);
	mhValueT             = a->MakeTH1('D', "mhValueT            ", "mhValueT            ", 2048, 0., 16.*2048.);
	mhMessageIndex       = a->MakeTH1('D', "mhMessageIndex      ", "mhMessageIndex      ", 300, 0., 300.);
	mhSubsubEvFooterCntr = a->MakeTH1('D', "mhSubsubEvFooterCntr", "mhSubsubEvFooterCntr", 300, 0., 300.);
}

UserHistosRawMonitoring::~UserHistosRawMonitoring()
{
	/**
	 * Not 100% sure, but it looks as if you do not need to delete your histograms yourself.
	 * TGo4AnalysisObjectManager, as part of the singleton TGo4Analysis, will take care.
	 */
}

ClassImp(UserHistosRawMonitoring)
