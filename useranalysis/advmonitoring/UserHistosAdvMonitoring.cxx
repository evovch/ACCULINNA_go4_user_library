#include "UserHistosAdvMonitoring.h"

// ROOT
#include <TH1D.h>
#include <TH2D.h>

// Go4
#include <TGo4Analysis.h>

// Project
#include "setupconfigcppwrapper/SetupConfiguration.h"
#include "setupconfigcppwrapper/ElectronicsChars.h"
#include "UserParameter.h"

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
	UserParameter* v_params = (UserParameter*)(a->GetParameter("UserParameter"));

	const SetupConfiguration& v_SetupConfig = SetupConfiguration::GetInstance();
	const ElectronicsChars* v_ElectrChars = v_params->GetElectrChars();

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
		TString newHistoTitle;

		// option1
		//newHistoName.Form("AdvMon/AutoHistos/%s/%s/id_%d_ch_%d", v_detector.Data(), v_station.Data(), v_statid*100+v_det_ch, v_det_ch);
		//newHistoTitle.Form("%s/%s/id_%d_ch_%d", v_detector.Data(), v_station.Data(), v_statid*100+v_det_ch, v_det_ch);
		// option2
		newHistoName.Form("AdvMon/AutoHistos/%s/%s/%s_ch_%d", v_detector.Data(), v_station.Data(), v_station.Data(), v_det_ch);
		newHistoTitle.Form("%s/%s/%s_ch_%d", v_detector.Data(), v_station.Data(), v_station.Data(), v_det_ch);

		Double_t v_rangeLow = (Double_t)(v_ElectrChars->GetRangeLow(v_elblock));
		Double_t v_rangeHigh = (Double_t)(v_ElectrChars->GetRangeHigh(v_elblock));
		Int_t v_nBins = (Int_t)(v_ElectrChars->GetNbins(v_elblock));

		TH1* v_histo = a->MakeTH1('D', newHistoName, newHistoTitle, v_nBins, v_rangeLow, v_rangeHigh);
		fAutoHistos.insert(std::pair<unsigned int, TH1*>(v_statid*100+v_det_ch, v_histo));
	}
}

ClassImp(UserHistosAdvMonitoring)
