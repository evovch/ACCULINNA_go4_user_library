#include "UserEventBeamDetMwpc.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

UserEventBeamDetMwpc::UserEventBeamDetMwpc(const char* name) :
	TGo4EventElement(name, name, 8)
{
	//cerr << "UserEventBeamDetMwpc::UserEventBeamDetMwpc() -> ";
	this->Clear();
}

UserEventBeamDetMwpc::~UserEventBeamDetMwpc()
{
}

void UserEventBeamDetMwpc::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "UserEventBeamDetMwpc::Clear()" << endl;

	// Beam detector - MWPC
	// MWPC[]
	for (UInt_t i=0; i<4; i++) {
		tMWPC[i] = 0;
	}
}

void UserEventBeamDetMwpc::Dump(void) const
{
	//TODO dump all data members!

	// Beam detector - MWPC
	// MWPC[]
	cerr << " tMWPC:"; for (UInt_t i=0; i<4; i++)  { cerr << "\t" <<   tMWPC[i]; } cerr << endl;
}

UShort_t* UserEventBeamDetMwpc::GetFieldByName(TString p_name)
{
	// Beam detector - MWPC
	//if (p_name ==  "MWPC")     { return  MWPC; }
	if (p_name == "tMWPC")     { return tMWPC; }

	/*else if (p_name == "Unknown") {
		cerr << "[WARN  ] Acquiring en event data field '" << p_name << "' which does not exist." << endl;
		return NULL; }*/
	else {
		cerr << "[ERROR ] Acquiring an event data field '" << p_name << "' which does not exist." << endl;
		return NULL;
	}
}

ClassImp(UserEventBeamDetMwpc)
