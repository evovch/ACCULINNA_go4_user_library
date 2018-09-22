#include "DetEventStation.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// ROOT
#include <TClonesArray.h>

// Project
#include "DetMessage.h"

DetEventStation::DetEventStation() :
	TGo4EventElement(),
	fData(NULL)
{
	//TODO how do we avoid using default constructors when browsing the
	// analysis result root file?
	cerr << "DEFAULT DetEventStation CONSTRUCTOR" << endl;
}

DetEventStation::DetEventStation(const char* name, const char* title, Short_t id) :
	TGo4EventElement(name, title, id) // Unique ID as third argument
{
	cerr << "DetEventStation::DetEventStation: name=" << name << " id=" << id << endl;

	fData = new TClonesArray("DetMessage");

	//this->Clear(); // Not really needed
}

DetEventStation::~DetEventStation()
{
	if (fData) {
		fData->Clear();
		delete fData;
	}
}

void DetEventStation::AddDetMessage(Int_t p_statch, Int_t p_val)
{
	TClonesArray& clref = *fData;
	Int_t size = clref.GetEntriesFast();

	Short_t v_detID = this->GetParent()->getId();
	Short_t v_stationID = this->getId();

	////fprintf(stderr, "%d - %d\n", v_detID, v_stationID);

	// Write a new detector-message into the output TClonesArray
	///////////////////////////////////////////////////////////////////
	new(clref[size]) DetMessage(v_detID, v_stationID, p_statch, p_val);
	///////////////////////////////////////////////////////////////////
}

void DetEventStation::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "DetEventStation::Clear()" << endl;

	fData->Clear();
}

void DetEventStation::Print(Option_t* option) const
{
	//TODO dump all data members!
	cerr << "DetEventStation::Print() "
	     << "name = '" << this->GetName() << "'\t"
	     << "size = " << fData->GetEntries() << endl;

	TIter myiter(fData);
	DetMessage* curDetM;
	while ((curDetM = (DetMessage*)myiter.Next())) {
		curDetM->Print();
	}
}

ClassImp(DetEventStation)
