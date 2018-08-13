#include "DetEventDetector.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

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
