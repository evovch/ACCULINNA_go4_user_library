#include "UserHistosBeamMonitoring.h"

// ROOT
#include <TH1D.h>
#include <TH2D.h>

// Go4
#include <TGo4Analysis.h>

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
	fdEToF =  a->MakeTH2('F', "beam2D/dEToF", "dE-ToF identification plot", 200,0.,1000.,200,0.,6000.,"ToF","dE"); 
}

UserHistosBeamMonitoring::~UserHistosBeamMonitoring()
{
	/**
	 * Not 100% sure, but it looks as if you do not need to delete your histograms yourself.
	 * TGo4AnalysisObjectManager, as part of the singleton TGo4Analysis, will take care.
	 */
}

void UserHistosBeamMonitoring::GenerateAutoHistos(void)
{

}

ClassImp(UserHistosBeamMonitoring)
