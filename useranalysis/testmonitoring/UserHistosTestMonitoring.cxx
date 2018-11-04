#include "UserHistosTestMonitoring.h"

// ROOT
#include <TH1D.h>
#include <TH2D.h>

// Go4
#include <TGo4Analysis.h>

// Project
#include "setupconfigcppwrapper/SetupConfiguration.h"

UserHistosTestMonitoring::UserHistosTestMonitoring(std::pair <TString,Int_t>* pairs,Int_t npairs)
{
	cout << "UserHistosTestMonitoring was called" << endl;
	TGo4Analysis* a = TGo4Analysis::Instance();
	/// TODO: be careful with new!
	detSi = new TH1*[npairs];
	for(Int_t i=0; i<npairs; i++) {
		TString hName = "calibration/" + pairs[i].first;
		detSi[i] = a->MakeTH1('D', hName.Data(), "Calibrated spectra from Si station", 1000, 0., 150.);
	}

	dE_E_Right = a->MakeTH2('D',"calibration/dE-E_right","dE-E plot for right tel",300,0.,100,300,0,30,"Etotal","dE");
	dE_E_Left = a->MakeTH2('D',"calibration/dE-E_left","dE-E plot for left tel",300,0.,100,300,0,30,"Etotal","dE");	

}

UserHistosTestMonitoring::~UserHistosTestMonitoring()
{
	/**
	 * Not 100% sure, but it looks as if you do not need to delete your histograms yourself.
	 * TGo4AnalysisObjectManager, as part of the singleton TGo4Analysis, will take care.
	 */
	 cout << "UserHistosTestMonitoring destructor called " << endl;
	 if (detSi) {
	 	 delete [] detSi;	
	 }

}

ClassImp(UserHistosTestMonitoring)
