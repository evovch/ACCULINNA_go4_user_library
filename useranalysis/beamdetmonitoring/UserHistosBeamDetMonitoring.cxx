#include "UserHistosBeamDetMonitoring.h"

// ROOT
#include <TH1D.h>

// Go4
#include <TGo4Analysis.h>

// Project
////#include "setupconfigcppwrapper/SetupConfiguration.h"

UserHistosBeamDetMonitoring::UserHistosBeamDetMonitoring()
{
	TGo4Analysis* a = TGo4Analysis::Instance();

	char* titles[] = {"X1", "Y1", "X2", "Y2"}; //TODO make it nicer

	TString histoName;
	TString histoTitle;
	for (unsigned short i=0; i<4; i++) {
		histoName.Form("BeamDetMon/fhMWPC_%s", titles[i]);
		histoTitle.Form("fhMWPC_%s", titles[i]);
		fhMWPC[i] = a->MakeTH1('D', histoName, histoTitle, 32, 0., 32.);
	}
	for (unsigned short i=0; i<4; i++) {
		histoName.Form("BeamDetMon/fhMultW_%s", titles[i]);
		histoTitle.Form("fhMultW_%s", titles[i]);
		fhMultW[i] = a->MakeTH1('D', histoName, histoTitle, 32, 0., 32.);
	}
}

UserHistosBeamDetMonitoring::~UserHistosBeamDetMonitoring()
{
	/**
	 * Not 100% sure, but it looks as if you do not need to delete your histograms yourself.
	 * TGo4AnalysisObjectManager, as part of the singleton TGo4Analysis, will take care.
	 */
}

ClassImp(UserHistosBeamDetMonitoring)
