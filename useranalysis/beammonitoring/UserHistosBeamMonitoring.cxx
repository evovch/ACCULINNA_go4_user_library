#include "UserHistosBeamMonitoring.h"

// ROOT
#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>

// Go4
#include "TGo4Analysis.h"
#include "TGo4WinCond.h"
#include "TGo4PolyCond.h"
#include "TGo4ShapedCond.h"

// Project
#include "setupconfigcppwrapper/SetupConfiguration.h"

UserHistosBeamMonitoring::UserHistosBeamMonitoring()
{
	TGo4Analysis* a = TGo4Analysis::Instance();
	//TODO  MWPC displacements shoulb be taken into account in the MWPC_C histos!!
	fY1_X1 = a->MakeTH2('I', "beam2D/XY1", "WIRES 1 Y1 vs X1", 32,0.,32.,32,0.,32.,"X1","Y1");
	fY1_X1_C = a->MakeTH2('F', "beam2D/XY1_C", "beam profile at the MWPC1 plane", 32,-20.,20.,32,-20.,20.,"X1_C","Y1_C");
	fY2_X2 = a->MakeTH2('I', "beam2D/XY2", "WIRES 1 Y2 vs X2", 32,0.,32.,32,0.,32.,"X2","Y2");
	fY2_X2_C = a->MakeTH2('F', "beam2D/XY2_C", "beam profile at the MWPC2 plane", 32,-20.,20.,32,-20.,20.,"X2_C","Y2_C");
	fTarget = a->MakeTH2('F', "beam2D/target", "beam profile at the target plane", 60,-30.,30.,60,-30.,30.,"X_T","Y_T");
	//TODO ranges of deToF
	fdEToF = a->MakeTH2('F', "beam2D/dEToF", "dE-ToF identification plot", 200,0.,1000.,200,0.,6000.,"ToF","dE"); 

	// example of TGo4ShapedCond
	fBoxCond = new TGo4ShapedCond("boxcond");	
	fBoxCond->SetBox(685.,1650.,50.,340., 0);
	fBoxCond->SetHistogram("beam2D/dEToF");
	if(!fBoxCond->IsEnabled()) fBoxCond->Enable();
	a->AddAnalysisCondition(fBoxCond);

	//example of TGo4WinCond
	fWinCond = new TGo4WinCond("wincond");
	fWinCond->SetValues(630.,750.,1300.,2000.);
	fWinCond->SetHistogram("beam2D/dEToF");
	if(!fWinCond->IsEnabled()) fWinCond->Enable();
	a->AddAnalysisCondition(fWinCond);

	// example of TGo4PolyCond	
	fPolyCond = new TGo4PolyCond("polycond");
	fPolyCond->Disable(kFALSE);
	TString fileName("/media/user/work/useful/cut.root");
	TFile *f  = new TFile(fileName.Data(),"READ");
	if(f->IsOpen()) {
		TCutG *cut = (TCutG*)f->Get("mycut");
		fPolyCond->SetValues(cut);
		if(!fPolyCond->IsEnabled()) fPolyCond->Enable();
	}
	delete f;
	fPolyCond->SetHistogram("beam2D/dEToF");
	a->AddAnalysisCondition(fPolyCond);

}

UserHistosBeamMonitoring::~UserHistosBeamMonitoring()
{
	cout << " UserHistosBeamMonitoring destructor called! " << endl; 
	/**
	 * Not 100% sure, but it looks as if you do not need to delete your histograms yourself.
	 * TGo4AnalysisObjectManager, as part of the singleton TGo4Analysis, will take care.
	 */
}

void UserHistosBeamMonitoring::GenerateAutoHistos(void)
{

}

// ClassImp(UserHistosBeamMonitoring)
