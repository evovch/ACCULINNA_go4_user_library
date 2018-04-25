#include "UserEventBeamDetTof.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

UserEventBeamDetTof::UserEventBeamDetTof(const char* name) :
	TGo4EventElement(name, name, 7)
{
	//cerr << "UserEventBeamDetTof::UserEventBeamDetTof() -> ";
	this->Clear();
}

UserEventBeamDetTof::~UserEventBeamDetTof()
{
}

void UserEventBeamDetTof::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "UserEventBeamDetTof::Clear()" << endl;

	// Beam detector - ToF
	for (UInt_t i=0; i<4; i++) {
		 F3[i] = 0;
		tF3[i] = 0;
		 F5[i] = 0;
		tF5[i] = 0;
		 F6[i] = 0;
		tF6[i] = 0;
	}
}

void UserEventBeamDetTof::Dump(void) const
{
	//TODO dump all data members!

	// Beam detector - ToF
	cerr << "    F3:"; for (UInt_t i=0; i<4; i++)  { cerr << "\t" <<      F3[i]; } cerr << endl;
	cerr << "   tF3:"; for (UInt_t i=0; i<4; i++)  { cerr << "\t" <<     tF3[i]; } cerr << endl;
	cerr << "    F5:"; for (UInt_t i=0; i<4; i++)  { cerr << "\t" <<      F5[i]; } cerr << endl;
	cerr << "   tF5:"; for (UInt_t i=0; i<4; i++)  { cerr << "\t" <<     tF5[i]; } cerr << endl;
	cerr << "    F6:"; for (UInt_t i=0; i<4; i++)  { cerr << "\t" <<      F6[i]; } cerr << endl;
	cerr << "   tF6:"; for (UInt_t i=0; i<4; i++)  { cerr << "\t" <<     tF6[i]; } cerr << endl;
}

UShort_t* UserEventBeamDetTof::GetFieldByName(TString p_name)
{
	// Beam detector - ToF
	if      (p_name ==  "F3")       { return  F3; }
	else if (p_name == "tF3")       { return tF3; }
	else if (p_name ==  "F5")       { return  F5; }
	else if (p_name == "tF5")       { return tF5; }
	else if (p_name ==  "F6")       { return  F6; }
	else if (p_name == "tF6")       { return tF6; }

	/*else if (p_name == "Unknown") {
		cerr << "[WARN  ] Acquiring en event data field '" << p_name << "' which does not exist." << endl;
		return NULL; }*/
	else {
		cerr << "[ERROR ] Acquiring an event data field '" << p_name << "' which does not exist." << endl;
		return NULL;
	}
}

ClassImp(UserEventBeamDetTof)
