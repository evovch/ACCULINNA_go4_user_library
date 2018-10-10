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

		if(v_detector!="Beam_detector") continue; // loop over the map
		//cerr << v_detector << "(" << v_detid << "): " << v_station << "(" << v_statid << "): " << v_det_ch << endl;

		//TODO check duplicates
		TString newHistoName;
		// newHistoName.Form("BeamMon/AutoHistos/%d_%s_%s_%d_Beam", v_statid*100+v_det_ch, v_detector.Data(), v_station.Data(), v_det_ch);


		Int_t nBins,nLow,nUp;
		TString stationNameFull;
		stationNameFull = v_detector + "_" + v_station;
		if(v_station == "MWPC1" || v_station == "MWPC2" || v_station == "MWPC3" || v_station == "MWPC4") {
			// cout << stationNameFull << endl;
			nBins = 32;
			nLow = 0;
			nUp = 31;
			newHistoName.Form("%s/WIRES/%s", v_detector.Data(), v_station.Data());
		}
		else {
			nBins = 500;
			nLow = 0;
			nUp = 10000;
			newHistoName.Form("%s/%s/%s_%d", v_detector.Data(), v_station.Data(), v_station.Data(), v_det_ch);
		}
		TH1* v_histo = a->MakeTH1('D', newHistoName, newHistoName, nBins, nLow, nUp); //TODO ranges
		fAutoHistos_Beam.insert(std::pair<unsigned int, TH1*>(v_statid*100+v_det_ch, v_histo));
	}
}

ClassImp(UserHistosBeamMonitoring)
