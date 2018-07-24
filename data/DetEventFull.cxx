#include "DetEventFull.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

#include <map>
#include <set>

// Go4
//#include <TGo4Analysis.h>

// Project
//#include "UserParameter.h"
#include "setupconfigcppwrapper/SetupConfiguration.h"
#include "data/DetEventCommon.h"
#include "data/DetEventDetector.h"

DetEventFull::DetEventFull() :
	TGo4CompositeEvent()
{
	cerr << "DEFAULT DetEventFull CONSTRUCTOR" << endl;
}

DetEventFull::DetEventFull(const char* name) :
	TGo4CompositeEvent(name, name)
{
	cerr << endl;

	UShort_t v_index = 0; // Reserve id=0 for DetEventCommon
	cerr << "DetEventFull::DetEventFull: id=" << v_index << endl;
	addEventElement(new DetEventCommon("DetEventCommon", v_index));
	cerr << endl;

	//TODO check
	//TODO old implementation
	/*
	TGo4Analysis* a = TGo4Analysis::Instance();
	// Get the all-accessible parameter-set object
	UserParameter* v_params = (UserParameter*)a->GetParameter("UserParameter");
	if (!v_params) {
		cerr << "No UserParameter global object. Aborting." << endl;
		exit(EXIT_FAILURE); 
	}
	const SetupConfiguration* v_setupConfig = v_params->GetSetupConfig();
	*/
	// new implementation
	SetupConfiguration* v_setupConfig = SetupConfiguration::GetInstance();

	const std::map<TString, unsigned short> v_detectorList = v_setupConfig->GetDetectorList();

	std::map<TString, unsigned short>::const_iterator iter;
	for (iter = v_detectorList.begin(); iter != v_detectorList.end(); ++iter) {

		v_index = iter->second;
		cerr << "index=" << v_index << " name=" << iter->first << endl;

		const std::map<TString, unsigned short> v_stationList = v_setupConfig->GetStationList(iter->first);

		/*for (std::set<TString>::const_iterator iter2 = v_stationList.begin();
			iter2 != v_stationList.end(); ++iter2) {
			cerr << "DetEventFull::DetEventFull: " << *iter2 << endl;
		}*/

		cerr << "DetEventFull::DetEventFull: id=" << v_index << " name=" << iter->first << endl;
		addEventElement(new DetEventDetector(iter->first, v_index, v_stationList));
		cerr << endl;

	}

	//cerr << "DetEventFull::DetEventFull() -> ";
	//this->Clear(); // Not really needed

	cerr << endl;
}

DetEventFull::~DetEventFull()
{
}

/*void DetEventFull::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "DetEventFull::Clear()" << endl;
}*/

void DetEventFull::Print(Option_t* option) const
{
	//TODO dump all data members!

	cerr << "DetEventFull::Print()\t";
	cerr << this->ClassName() << "\t";
	cerr << this->getNElements() << " elements:" << endl;

	for (Short_t iElem=0; iElem<this->getNElements(); iElem++)
	{
		TGo4EventElement* curElem = this->getEventElement(iElem);
		cerr << "Element " << iElem << ": "
		     << curElem->isComposed()
		     << "\t" << curElem->ClassName() << endl;

		curElem->Print();
	}

}

ClassImp(DetEventFull)
