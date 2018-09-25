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

	mhHisto1 = a->MakeTH1('D', "AdvMon/UserHistos/mhHisto1", "mhHisto1", 100, 0., 100.);
	mhHisto2 = a->MakeTH2('D', "AdvMon/UserHistos/mhHisto2", "mhHisto2", 100, 0., 100., 100, 0., 100.);
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
