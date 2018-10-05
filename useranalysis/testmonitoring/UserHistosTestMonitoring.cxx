#include "UserHistosTestMonitoring.h"

// ROOT
#include <TH1D.h>
#include <TH2D.h>

// Go4
#include <TGo4Analysis.h>

// Project
#include "setupconfigcppwrapper/SetupConfiguration.h"

UserHistosTestMonitoring::UserHistosTestMonitoring()
{
	TGo4Analysis* a = TGo4Analysis::Instance();
	fTrigger_test = a->MakeTH1('I', "Trigger", "Values of trigger_test",   5, 0., 4.);
}

UserHistosTestMonitoring::~UserHistosTestMonitoring()
{
	/**
	 * Not 100% sure, but it looks as if you do not need to delete your histograms yourself.
	 * TGo4AnalysisObjectManager, as part of the singleton TGo4Analysis, will take care.
	 */
}

void UserHistosTestMonitoring::GenerateAutoHistos(void)
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
		// newHistoName.Form("TestMon/AutoHistos/%d_%s_%s_%d_test", v_statid*100+v_det_ch, v_detector.Data(), v_station.Data(), v_det_ch);


		Int_t nBins,nLow,nUp;
		TString stationNameFull;
		stationNameFull = v_detector + "_" + v_station;
		if(stationNameFull.Contains("DAQ")) continue;
		if(stationNameFull.Contains("Beam_detector_MWPC")) {
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
		fAutoHistos_test.insert(std::pair<unsigned int, TH1*>(v_statid*100+v_det_ch, v_histo));
	}
}

ClassImp(UserHistosTestMonitoring)
