#include "DetEventDetector.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// Project
#include "DetEventStation.h"

DetEventDetector::DetEventDetector(const char* name, Short_t id, const std::set<TString> stationList) :
	TGo4CompositeEvent(name, name, id) // Unique ID as third argument
{
	UShort_t v_index = 0;
	std::set<TString>::const_iterator iter;
	for (iter = stationList.begin(); iter != stationList.end(); ++iter) {
		cerr << *iter << endl;

		//TString brName;
		//brName.Form("%s/%s", name, (*iter).Data());
		addEventElement(new DetEventStation(*iter, name, id*100 + v_index));
		v_index++;
	}

	//cerr << "DetEventDetector::DetEventDetector() -> ";
	this->Clear();
}

DetEventDetector::~DetEventDetector()
{
}

void DetEventDetector::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "DetEventDetector::Clear()" << endl;

}

void DetEventDetector::Dump(void) const
{
	//TODO dump all data members!

}

ClassImp(DetEventDetector)
