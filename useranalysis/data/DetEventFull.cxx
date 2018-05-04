#include "DetEventFull.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

#include <set>

// Go4
#include <TGo4Analysis.h>

// Project
#include "UserParameter.h"
#include "setupconfigcppwrapper/SetupConfiguration.h"
#include "data/DetEventCommon.h"
#include "data/DetEventDetector.h"

DetEventFull::DetEventFull(const char* name) :
	TGo4CompositeEvent(name, name)
{
	addEventElement(new DetEventCommon("DetEventCommon", 100));

	//TODO check
	TGo4Analysis* a = TGo4Analysis::Instance();
	// Get the all-accessible parameter-set object
	UserParameter* v_params = (UserParameter*)a->GetParameter("UserParameter");
	const SetupConfiguration* v_setupConfig = v_params->GetSetupConfig();

	const std::set<TString> v_detectorList = v_setupConfig->GetDetectorList();

	UShort_t v_index = 0;
	std::set<TString>::const_iterator iter;
	for (iter = v_detectorList.begin(); iter != v_detectorList.end(); ++iter) {
		cerr << *iter << endl;

		const std::set<TString> v_stationList = v_setupConfig->GetStationList(*iter);

		for (std::set<TString>::const_iterator iter2 = v_stationList.begin();
			iter2 != v_stationList.end(); ++iter2) {
			cerr << "\t" << *iter2 << endl;
		}

		addEventElement(new DetEventDetector(*iter, v_index, v_stationList));

		v_index++;
	}

	//cerr << "DetEventFull::DetEventFull() -> ";
	this->Clear();
}

DetEventFull::~DetEventFull()
{
}

void DetEventFull::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "DetEventFull::Clear()" << endl;

}

void DetEventFull::Dump(void) const
{
	//TODO dump all data members!

}

ClassImp(DetEventFull)
