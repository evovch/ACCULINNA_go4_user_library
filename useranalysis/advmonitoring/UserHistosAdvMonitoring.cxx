#include "UserHistosAdvMonitoring.h"

// ROOT
#include <TH1D.h>
#include <TH2D.h>

// Go4
#include <TGo4Analysis.h>

UserHistosAdvMonitoring::UserHistosAdvMonitoring()
{
	//TGo4Analysis* a = TGo4Analysis::Instance();

}

UserHistosAdvMonitoring::~UserHistosAdvMonitoring()
{
	/**
	 * Not 100% sure, but it looks as if you do not need to delete your histograms yourself.
	 * TGo4AnalysisObjectManager, as part of the singleton TGo4Analysis, will take care.
	 */
}

ClassImp(UserHistosAdvMonitoring)
