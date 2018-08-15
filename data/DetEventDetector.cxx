#include "DetEventDetector.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// ROOT
#include <TTree.h>

// Project
#include "DetEventStation.h"

DetEventDetector::DetEventDetector() :
	TGo4CompositeEvent()
{
	cerr << "DEFAULT DetEventDetector CONSTRUCTOR" << endl;
}

DetEventDetector::DetEventDetector(const char* name, Short_t id, const std::map<TString, unsigned short> stationList) :
	TGo4CompositeEvent(name, name, id) // Unique ID as third argument
{
	UShort_t v_childrenCounter = 0;

	mChildrenIndices.Set(stationList.size()); // set the size of the array

	std::map<TString, unsigned short>::const_iterator iter;
	for (iter = stationList.begin(); iter != stationList.end(); ++iter) {
		cerr << "DetEventDetector::DetEventDetector: name=" << iter->first << " id=" << iter->second << endl;

		TString brTitle;
		brTitle.Form("%s_%s", name, iter->first.Data());

		// Create a link between the 'detector' CompositeEvent object and the 'station' EventElement object
		TGo4EventElement* evSt = new DetEventStation(brTitle, brTitle, iter->second);
		evSt->SetParent(this);

		//////////////////////
		addEventElement(evSt);
		mChildrenIndices.AddAt(iter->second, v_childrenCounter);
		v_childrenCounter++;
		//////////////////////
	}

	//this->Clear(); // Not really needed //TODO ???
}

DetEventDetector::~DetEventDetector()
{
}

void DetEventDetector::MapToBranch(TTree* t, TString detName, const std::map<TString, unsigned short> stationList)
{
	cerr << "DetEventDetector::MapToBranch ================================================" << endl;

	std::map<TString, unsigned short>::const_iterator iter;
	for (iter = stationList.begin(); iter != stationList.end(); ++iter) {
		cerr << "DetEventDetector::MapToBranch: name=" << iter->first << " id=" << iter->second << endl;

		TString elName;
		elName.Form("%s_%s", detName.Data(), iter->first.Data());
		TString brName = elName + ".";

		TGo4EventElement* subSubEl = this->getEventElement(elName);
		if (subSubEl == NULL) {
			cerr << "Sub-sub-element '" << elName << "' not found. Aborting." << endl;
			exit(EXIT_FAILURE);
		} else {
			cerr << "Sub-sub-element '" << elName << "' found." << endl;
		}

		TBranch* curBranch = t->GetBranch(brName);
		if (curBranch == NULL) {
			cerr << "Station branch '" << brName << "' not found. Aborting." << endl;
			exit(EXIT_FAILURE);
		} else {
			cerr << "Station branch '" << brName << "' found." << endl;

			DetEventStation* subSubElDet = static_cast<DetEventStation*>(subSubEl);

			//cerr << "++++++++++++++++++++++++++++++++++++++++++" << endl;
			TClass* cl = subSubElDet->IsA();
			//cerr << "Class name: " << cl->GetName() << endl;
			t->SetBranchAddress(brName, &subSubElDet, 0, cl, kOther_t, true);
			//t->SetBranchAddress(brName, &subSubElDet);
			//cerr << "++++++++++++++++++++++++++++++++++++++++++" << endl;
		}

	}
}

/*void DetEventDetector::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "DetEventDetector::Clear()" << endl;
}*/

void DetEventDetector::Print(Option_t* option) const
{
	//TODO dump all data members!
	cerr << "DetEventDetector::Print()\t";
	cerr << "class = '" << this->ClassName() << "'\t";
	cerr << "name = '" << this->GetName() << "'\t";
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
		     << "\t" << curElem->ClassName() << "\t\t";

		curElem->Print();
	}

}

ClassImp(DetEventDetector)
