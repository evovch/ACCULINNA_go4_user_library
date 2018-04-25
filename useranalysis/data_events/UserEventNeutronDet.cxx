#include "UserEventNeutronDet.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

UserEventNeutronDet::UserEventNeutronDet(const char* name) :
	TGo4EventElement(name, name, 6)
{
	//cerr << "UserEventNeutronDet::UserEventNeutronDet() -> ";
	this->Clear();
}

UserEventNeutronDet::~UserEventNeutronDet()
{
}

void UserEventNeutronDet::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "UserEventNeutronDet::Clear()" << endl;

	// Neutron detector (stilbene)
	for (UInt_t i=0; i<32; i++) {
		neutAmp[i] = 0;
		neutTAC[i] = 0;
		neutTDC[i] = 0;
	}
}

void UserEventNeutronDet::Dump(void) const
{
	//TODO dump all data members!

	// Neutron detector (stilbene)
	cerr <<"neutAmp:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<neutAmp[i]; } cerr << endl;
	cerr <<"       :"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<neutAmp[i+16]; } cerr << endl;
	cerr <<"neutTAC:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<neutTAC[i]; } cerr << endl;
	cerr <<"       :"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<neutTAC[i+16]; } cerr << endl;
	cerr <<"neutTDC:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<neutTDC[i]; } cerr << endl;
	cerr <<"       :"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<neutTDC[i+16]; } cerr << endl;
}

UShort_t* UserEventNeutronDet::GetFieldByName(TString p_name)
{
	// Neutron detector (stilbene)
	if      (p_name ==  "neutAmp")  { return  neutAmp; }
	else if (p_name ==  "neutTAC")  { return  neutTAC; }
	else if (p_name ==  "neutTDC")  { return  neutTDC; }

	/*else if (p_name == "Unknown") {
		cerr << "[WARN  ] Acquiring en event data field '" << p_name << "' which does not exist." << endl;
		return NULL; }*/
	else {
		cerr << "[ERROR ] Acquiring an event data field '" << p_name << "' which does not exist." << endl;
		return NULL;
	}
}

ClassImp(UserEventNeutronDet)
