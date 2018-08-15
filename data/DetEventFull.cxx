#include "DetEventFull.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

#include <map>
#include <set>

// ROOT
#include <TTree.h>

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
	SetupConfiguration& v_setupConfig = SetupConfiguration::GetInstance();

	cerr << endl;

	mChildrenIndices.Set(v_setupConfig.GetNdetectors() + 1); // set the size of the array

	UShort_t v_childrenCounter = 0;
	UShort_t v_index = 0; // Reserve id=0 for DetEventCommon
	cerr << "index=" << v_index << " name=DetEventCommon" << endl;
	addEventElement(new DetEventCommon("DetEventCommon", v_index));
	mChildrenIndices.AddAt(v_index, v_childrenCounter);
	v_childrenCounter++;
	cerr << endl;

	const std::map<TString, unsigned short> v_detectorList = v_setupConfig.GetDetectorList();

	std::map<TString, unsigned short>::const_iterator iter;
	for (iter = v_detectorList.begin(); iter != v_detectorList.end(); ++iter) {

		v_index = iter->second;
		cerr << "index=" << v_index << " name=" << iter->first << endl;

		const std::map<TString, unsigned short> v_stationList = v_setupConfig.GetStationList(iter->first);

		/*for (std::set<TString>::const_iterator iter2 = v_stationList.begin();
			iter2 != v_stationList.end(); ++iter2) {
			cerr << "DetEventFull::DetEventFull: " << *iter2 << endl;
		}*/

		cerr << "DetEventFull::DetEventFull: id=" << v_index << " name=" << iter->first << endl;
		addEventElement(new DetEventDetector(iter->first, v_index, v_stationList));
		mChildrenIndices.AddAt(v_index, v_childrenCounter);
		v_childrenCounter++;
		cerr << endl;

	}

	//cerr << "DetEventFull::DetEventFull() -> ";
	//this->Clear(); // Not really needed

	cerr << endl;
}

DetEventFull::~DetEventFull()
{
}

/*void DetEventFull::MapToBranch(TTree* t)
{
	cerr << "DetEventFull::MapToBranch ================================================" << endl;

	// new implementation
	SetupConfiguration& v_setupConfig = SetupConfiguration::GetInstance();

	// COMMON

	UShort_t v_index = 0; // Reserve id=0 for DetEventCommon
	cerr << "index=" << v_index << " name=DetEventCommon" << endl;

	TGo4EventElement* subEl0 = this->getEventElement("DetEventCommon");
	TBranch* curBranch0 = t->GetBranch("DetEventCommon.");

	if (subEl0 == NULL || curBranch0 == NULL) {
		exit(EXIT_FAILURE);
	}

	//cerr << "++++++++++++++++++++++++++++++++++++++++++" << endl;
	TClass* cl0 = subEl0->IsA();
	//cerr << "Class name: " << cl0->GetName() << endl;
	t->SetBranchAddress("DetEventCommon.", &subEl0, 0, cl0, kOther_t, true);
	//t->SetBranchAddress("DetEventCommon.", &subEl0);
	//cerr << "++++++++++++++++++++++++++++++++++++++++++" << endl;

	// DETECTORS

	const std::map<TString, unsigned short> v_detectorList = v_setupConfig.GetDetectorList();

	std::map<TString, unsigned short>::const_iterator iter;
	for (iter = v_detectorList.begin(); iter != v_detectorList.end(); ++iter) {

		TString detectorName(iter->first);

		v_index = iter->second;
		cerr << "index=" << v_index << " name=" << detectorName << endl;

		TGo4EventElement* subEl = this->getEventElement(iter->first);
		if (subEl == NULL) {
			cerr << "Sub-element '" << detectorName << "' not found. Aborting." << endl;
			exit(EXIT_FAILURE);
		} else {
			cerr << "Sub-element '" << detectorName << "' found." << endl;
		}

		TString brName = detectorName + ".";
		TBranch* curBranch = t->GetBranch(brName);
		if (curBranch == NULL) {
			cerr << "Detector branch '" << brName << "' not found. Aborting." << endl;
			exit(EXIT_FAILURE);
		} else {
			cerr << "Detector branch '" << brName << "' found." << endl;

			//cerr << "++++++++++++++++++++++++++++++++++++++++++" << endl;
			TClass* cl = subEl->IsA();
			//cerr << "Class name: " << cl->GetName() << endl;
			t->SetBranchAddress(brName, &subEl, 0, cl, kOther_t, true);
			//t->SetBranchAddress(brName, &subEl);
			////curBranch->SetAddress(&subEl);
			//cerr << "++++++++++++++++++++++++++++++++++++++++++" << endl;

			const std::map<TString, unsigned short> v_stationList = v_setupConfig.GetStationList(iter->first);

			DetEventDetector* subElDet = dynamic_cast<DetEventDetector*>(subEl);
			subElDet->MapToBranch(t, detectorName, v_stationList);
		}

	} // for loop over detectors
}*/

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
		TGo4EventElement* curElem = this->getEventElement(mChildrenIndices[iElem]);

		if (curElem == NULL) {
			cerr << "WTF" << endl;
			exit(EXIT_FAILURE);
		}

		cerr << "Element " << iElem << ": "
		     << curElem->isComposed()
		     << "\t" << curElem->ClassName() << endl;

		curElem->Print();
	}

}

ClassImp(DetEventFull)
