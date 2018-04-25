#include "UserEventRightTele.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

UserEventRightTele::UserEventRightTele(const char* name) :
	TGo4EventElement(name, name, 2)
{
	//cerr << "UserEventRightTele::UserEventRightTele() -> ";
	this->Clear();
}

UserEventRightTele::~UserEventRightTele()
{
}

void UserEventRightTele::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "UserEventRightTele::Clear()" << endl;

	// Right telescope
	for (UInt_t i=0; i<16; i++) {
		 CsI_R[i] = 0;
		tCsI_R[i] = 0;
		 SQX_R[i] = 0;
		 SQX_R[i+16] = 0;
		tSQX_R[i] = 0;
		tSQX_R[i+16] = 0;
		 SQY_R[i] = 0;
		tSQY_R[i] = 0;
	}
}

void UserEventRightTele::Dump(void) const
{
	//TODO dump all data members!

	// Right telescope
	cerr << " CsI_R:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<  CsI_R[i]; } cerr << endl;
	cerr << "tCsI_R:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" << tCsI_R[i]; } cerr << endl;
	cerr << " SQX_R:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<  SQX_R[i]; } cerr << endl;
	cerr << "      :"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<  SQX_R[i+16]; } cerr << endl;
	cerr << "tSQX_R:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" << tSQX_R[i]; } cerr << endl;
	cerr << "      :"; for (UInt_t i=0; i<16; i++) { cerr << "\t" << tSQX_R[i+16]; } cerr << endl;
	cerr << " SQY_R:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<  SQY_R[i]; } cerr << endl;
	cerr << "tSQY_R:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" << tSQY_R[i]; } cerr << endl;
}

UShort_t* UserEventRightTele::GetFieldByName(TString p_name)
{
	// Right telescope
	if      (p_name ==  "CsI_R")    { return  CsI_R; }
	else if (p_name == "tCsI_R")    { return tCsI_R; }
	else if (p_name ==  "SQX_R")    { return  SQX_R; }
	else if (p_name == "tSQX_R")    { return tSQX_R; }
	else if (p_name ==  "SQY_R")    { return  SQY_R; }
	else if (p_name == "tSQY_R")    { return tSQY_R; }

	/*else if (p_name == "Unknown") {
		cerr << "[WARN  ] Acquiring en event data field '" << p_name << "' which does not exist." << endl;
		return NULL; }*/
	else {
		cerr << "[ERROR ] Acquiring an event data field '" << p_name << "' which does not exist." << endl;
		return NULL;
	}
}

ClassImp(UserEventRightTele)
