#include "UserEventUnpacking.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// ROOT
#include <TClonesArray.h>

// Project
#include "data/RawMessage.h"

UserEventUnpacking::UserEventUnpacking(const char* name) :
	TGo4EventElement(name)
{
	fRawMessages = new TClonesArray("RawMessage");

	//cerr << "UserEventUnpacking::UserEventUnpacking() -> ";
	this->Clear();
}

UserEventUnpacking::~UserEventUnpacking()
{
	//TODO delete fRawMessages?
}

void UserEventUnpacking::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "UserEventUnpacking::Clear()" << endl;

	fRawMessages->Clear();

	for (UInt_t i=0; i<8; i++) {
		fCAMAC[i] = 0;
	}

	fTrigger = 0;
}

void UserEventUnpacking::Dump(void) const
{
	cerr << "UserEventUnpacking contains " << fRawMessages->GetEntries() << " raw messages." << endl;
	//TODO implement
}

ClassImp(UserEventUnpacking)
