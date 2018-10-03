#include "UserHistosAdvMonitoring.h"

// ROOT
#include <TH1D.h>
#include <TH2D.h>

// Go4
#include <TGo4Analysis.h>

// Project
#include "setupconfigcppwrapper/SetupConfiguration.h"

UserHistosAdvMonitoring::UserHistosAdvMonitoring()
{
	TGo4Analysis* a = TGo4Analysis::Instance();

	fTrigger = a->MakeTH1('I', "Trigger",    "Values of trigger",   16, 0., 15.);

	for (Int_t i = 0; i < 16; i++){
		fCsI_L[i] = a->MakeTH1('I', Form("Left_telescope/CsI_L/raw/CsI_L_%d",i)
									, Form("CsI left raw %d",i) , 8192, 0., 8191.);
		ftCsI_L[i] = a->MakeTH1('I', Form("Left_telescope/CsI_L/tdc/tCsI_L_%d",i)
									, Form("CsI left tdc %d",i) , 4095, 0., 4096.);
		fCsI_R[i] = a->MakeTH1('I', Form("Right_telescope/CsI_R/raw/CsI_R_%d",i)
									, Form("CsI right raw %d",i) , 8192, 0., 8191.);
		ftCsI_R[i] = a->MakeTH1('I', Form("Right_telescope/CsI_R/tdc/tCsI_R_%d",i)
									, Form("CsI right tdc %d",i) , 4095, 0., 4096.);

		fSQ20[i] = a->MakeTH1('I', Form("Left_telescope/SQ20/raw/SQ20_%2.2d",i)
				, Form("Square left 20mkm X stripes CH %2.2d",i), 4096, 0., 4095.);
		ftSQ20[i] = a->MakeTH1('I', Form("Left_telescope/SQ20/tdc/tSQ20_%d",i)
				, Form("Square left 20mkm X tdc %d",i), 4096, 0., 4095.);

		fSQY_L[i] = a->MakeTH1('I', Form("Left_telescope/SQY_L/raw/SQY_L_%d",i)
				, Form("Square left DSSD Y stripes CH %2.2d",i), 4096, 0., 4095.);
		ftSQY_L[i] = a->MakeTH1('I', Form("Left_telescope/SQY_L/tdc/tSQY_L_%d",i)
				, Form("Square left DSSD Y tdc %d",i), 4096, 0., 4095.);

		fSQY_R[i] = a->MakeTH1('I', Form("Right_telescope/SQY_R/raw/SQY_R_%d",i)
				, Form("Square right DSSD Y stripes CH %2.2d",i), 4096, 0., 4095.);
		ftSQY_R[i] = a->MakeTH1('I', Form("Right_telescope/SQY_R/tdc/tSQY_R_%d",i)
				, Form("Square right DSSD Y tdc %d",i), 4096, 0., 4095.);

		//CALIBRATED
		fCsI_R_C[i] = a->MakeTH1('D', Form("Right_telescope/CsI_R_C/CsI_R_C_%d",i)
				, Form("Square RIGHT CsI CALIBRATED crystal CH %d",i), 8000, 0., 240.);
	}

	for (Int_t i = 0; i < 32; i++){
		fSQX_L[i] = a->MakeTH1('I', Form("Left_telescope/SQX_L/raw/SQX_L_%d",i)
				, Form("Square left DSSD X stripes CH %2.2d",i), 4096, 0., 4095.);
		ftSQX_L[i] = a->MakeTH1('I', Form("Left_telescope/SQX_L/tdc/tSQX_L_%d",i)
				, Form("Square left DSSD X tdc %d",i), 4096, 0., 4095.);
		fSQX_R[i] = a->MakeTH1('I', Form("Right_telescope/SQX_R/raw/SQX_R_%d",i)
				, Form("Square right DSSD X stripes CH %2.2d",i), 4096, 0., 4095.);
		ftSQX_R[i] = a->MakeTH1('I', Form("Right_telescope/SQX_R/tdc/tSQX_R_%d",i)
				, Form("Square right DSSD X tdc %d",i), 4096, 0., 4095.);
	}

	fF3[0] = a->MakeTH1('I',"F3/QDC/F3_QDC_L","F3 QDC  left", 4096, 0., 4095.);
	fF3[1] = a->MakeTH1('I',"F3/QDC/F3_QDC_R","F3 QDC right", 4096, 0., 4095.);
	fF3[2] = a->MakeTH1('I',"F3/QDC/F3_QDC_U","F3 QDC    up", 4096, 0., 4095.);
	fF3[3] = a->MakeTH1('I',"F3/QDC/F3_QDC_D","F3 QDC  down", 4096, 0., 4095.);
	ftF3[0] = a->MakeTH1('I',"F3/TDC/tF3_TDC_L","F3 TDC  left", 6400, 0., 6399.);
	ftF3[1] = a->MakeTH1('I',"F3/TDC/tF3_TDC_R","F3 TDC right", 6400, 0., 6399.);
	ftF3[2] = a->MakeTH1('I',"F3/TDC/tF3_TDC_U","F3 TDC    up", 6400, 0., 6399.);
	ftF3[3] = a->MakeTH1('I',"F3/TDC/tF3_TDC_D","F3 TDC  down", 6400, 0., 6399.);

	fF5[0] = a->MakeTH1('I',"F5/QDC/F5_QDC_L","F5 QDC  left", 4096, 0., 4095.);
	fF5[1] = a->MakeTH1('I',"F5/QDC/F5_QDC_R","F5 QDC right", 4096, 0., 4095.);
	fF5[2] = a->MakeTH1('I',"F5/QDC/F5_QDC_U","F5 QDC    up", 4096, 0., 4095.);
	fF5[3] = a->MakeTH1('I',"F5/QDC/F5_QDC_D","F5 QDC  down", 4096, 0., 4095.);
	ftF5[0] = a->MakeTH1('I',"F5/TDC/tF5_TDC_L","F5 TDC  left", 6400, 0., 6399.);
	ftF5[1] = a->MakeTH1('I',"F5/TDC/tF5_TDC_R","F5 TDC right", 6400, 0., 6399.);
	ftF5[2] = a->MakeTH1('I',"F5/TDC/tF5_TDC_U","F5 TDC    up", 6400, 0., 6399.);
	ftF5[3] = a->MakeTH1('I',"F5/TDC/tF5_TDC_D","F5 TDC  down", 6400, 0., 6399.);

	ftMWPC[0] = a->MakeTH1('I',"tMWPC/tX1","MWPC TDC  X1", 6400, 0., 6399.);
	ftMWPC[1] = a->MakeTH1('I',"tMWPC/tY1","MWPC TDC  Y1", 6400, 0., 6399.);
	ftMWPC[2] = a->MakeTH1('I',"tMWPC/tX2","MWPC TDC  X2", 6400, 0., 6399.);
	ftMWPC[3] = a->MakeTH1('I',"tMWPC/tY2","MWPC TDC  Y2", 6400, 0., 6399.);

	//MWPC	
	fNX1 = a->MakeTH1('I', "WIRES/NX1","WIRES X1",32,0.,32.);
	fNY1 = a->MakeTH1('I', "WIRES/NY1","WIRES Y1",32,0.,32.);
	fNX2 = a->MakeTH1('I', "WIRES/NX2","WIRES X2",32,0.,32.);
	fNY2 = a->MakeTH1('I', "WIRES/NY2","WIRES Y2",32,0.,32.);

	//RECONSTRUCTED
	fY1_X1 = a->MakeTH2('I', "WIRES/XY1", "WIRES 1 Y1 vs X1", 32,0.,32.,32,0.,32.,"X1","Y1");
	fY1_X1_C = a->MakeTH2('F', "WIRES/XY1_C", "beam profile at the MWPC1 plane", 32,-20.,20.,32,-20.,20.,"X1_C","Y1_C");

}

UserHistosAdvMonitoring::~UserHistosAdvMonitoring()
{
	/**
	 * Not 100% sure, but it looks as if you do not need to delete your histograms yourself.
	 * TGo4AnalysisObjectManager, as part of the singleton TGo4Analysis, will take care.
	 */
}

void UserHistosAdvMonitoring::GenerateAutoHistos(void)
{
	TGo4Analysis* a = TGo4Analysis::Instance();

	const SetupConfiguration& v_SetupConfig = SetupConfiguration::GetInstance();

	std::map<unsigned int, stc_mapping*> v_mappings = v_SetupConfig.GetMappings();
	std::map<unsigned int, stc_mapping*>::const_iterator v_mapIter = v_mappings.begin();

	for ( ; v_mapIter != v_mappings.end(); ++v_mapIter) {
		unsigned int v_uid = v_mapIter->first;
		//cerr << v_uid << endl;

		unsigned short v_procid = v_SetupConfig.GetProcIdFromUID(v_uid);
		unsigned short v_addr = v_SetupConfig.GetAddrFromUID(v_uid);
		unsigned short v_ch = v_SetupConfig.GetElChFromUID(v_uid);

		TString v_station;
		TString v_detector;
		TString v_elblock;
		unsigned short v_detid;
		unsigned short v_statid;
		unsigned short v_det_ch = v_SetupConfig.GetOutput(v_procid, v_addr, v_ch, &v_station,
		                                                  &v_detector, &v_elblock, &v_detid, &v_statid);

		//cerr << v_detector << "(" << v_detid << "): " << v_station << "(" << v_statid << "): " << v_det_ch << endl;

		//TODO check duplicates
		TString newHistoName;
		newHistoName.Form("AdvMon/AutoHistos/%d_%s_%s_%d", v_statid*100+v_det_ch, v_detector.Data(), v_station.Data(), v_det_ch);
		TH1* v_histo = a->MakeTH1('D', newHistoName, newHistoName, 500, 0., 10000.); //TODO ranges
		fAutoHistos.insert(std::pair<unsigned int, TH1*>(v_statid*100+v_det_ch, v_histo));

	}
}

ClassImp(UserHistosAdvMonitoring)
