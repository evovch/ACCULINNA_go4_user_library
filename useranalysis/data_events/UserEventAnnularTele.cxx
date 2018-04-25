#include "UserEventAnnularTele.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

UserEventAnnularTele::UserEventAnnularTele(const char* name) :
	TGo4EventElement(name, name, 5)
{
	//cerr << "UserEventAnnularTele::UserEventAnnularTele() -> ";
	this->Clear();
}

UserEventAnnularTele::~UserEventAnnularTele()
{
}

void UserEventAnnularTele::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "UserEventAnnularTele::Clear()" << endl;

	// Annular telescope
	for (UInt_t i=0; i<16; i++) {
		ANR[i] = 0;
		ANS[i] = 0;
	}

}

void UserEventAnnularTele::Dump(void) const
{
	//TODO dump all data members!

	// Annular telescope
	cerr << "   ANR:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<    ANR[i]; } cerr << endl;
	cerr << "   ANS:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<    ANS[i]; } cerr << endl;
}

UShort_t* UserEventAnnularTele::GetFieldByName(TString p_name)
{
	if      (p_name ==  "ANR")      { return  ANR; }
	else if (p_name ==  "ANS")      { return  ANS; }

	/*else if (p_name == "Unknown") {
		cerr << "[WARN  ] Acquiring en event data field '" << p_name << "' which does not exist." << endl;
		return NULL; }*/
	else {
		cerr << "[ERROR ] Acquiring an event data field '" << p_name << "' which does not exist." << endl;
		return NULL;
	}
}

ClassImp(UserEventAnnularTele)
