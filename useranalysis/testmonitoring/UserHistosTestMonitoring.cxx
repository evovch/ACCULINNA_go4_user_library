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
		TString hName = "calibration/1D/" + pairs[i].first;
		detSi[i] = a->MakeTH1('D', hName.Data(), "Calibrated spectra from Si station", 1000, 0., 150.);
	}

	dE_E_Right = a->MakeTH2('D',"calibration/2D/dE-E_right","dE-E plot for right tel",300,0.,100,300,0,10,"Etotal","dE");
	dE_E_Left = a->MakeTH2('D',"calibration/2D/dE-E_left","dE-E plot for left tel",300,0.,100,300,0,10,"Etotal","dE");	

	dE_E_Central = new TH2*[16];
	for(Int_t i=0; i<16; i++) {
		TString hName;
		hName.Form("calibration/2D/dE-E_CsI_%d",i+1);
		dE_E_Central[i] = a->MakeTH2('D',hName.Data(),"dE-E plot for central tel",300,0.,4000,500,0,70,"E_CsI","dE_Si");
	}
	mult_Central = a->MakeTH1('I', "calibration/mult_central", "Multiplicity in the central Si detector", 32, 0, 32);
}

UserHistosTestMonitoring::~UserHistosTestMonitoring()
{
	/**
	 * Not 100% sure, but it looks as if you do not need to delete your histograms yourself.
	 * TGo4AnalysisObjectManager, as part of the singleton TGo4Analysis, will take care.
	 */
	 cout << "UserHistosTestMonitoring destructor called " << endl;
	 if (detSi) delete [] detSi;	
	 if (dE_E_Central) delete [] dE_E_Central; 

}

ClassImp(UserHistosTestMonitoring)
