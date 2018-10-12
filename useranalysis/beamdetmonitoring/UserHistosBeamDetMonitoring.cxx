#include "UserHistosBeamDetMonitoring.h"

// ROOT
////#include <TH1D.h>
////#include <TH2D.h>

// Go4
#include <TGo4Analysis.h>

// Project
////#include "setupconfigcppwrapper/SetupConfiguration.h"

UserHistosBeamDetMonitoring::UserHistosBeamDetMonitoring()
{
	////TGo4Analysis* a = TGo4Analysis::Instance();
}

UserHistosBeamDetMonitoring::~UserHistosBeamDetMonitoring()
{
	/**
	 * Not 100% sure, but it looks as if you do not need to delete your histograms yourself.
	 * TGo4AnalysisObjectManager, as part of the singleton TGo4Analysis, will take care.
	 */
}

ClassImp(UserHistosBeamDetMonitoring)
