#include "UserEventLeftTele.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

UserEventLeftTele::UserEventLeftTele(const char* name) :
	TGo4EventElement(name, name, 1)
{
	//cerr << "UserEventLeftTele::UserEventLeftTele() -> ";
	this->Clear();
}

UserEventLeftTele::~UserEventLeftTele()
{
}

void UserEventLeftTele::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "UserEventLeftTele::Clear()" << endl;

	// Left telescope
	for (UInt_t i=0; i<16; i++) {
		 CsI_L[i] = 0;
		tCsI_L[i] = 0;
		 SQX_L[i] = 0;
		 SQX_L[i+16] = 0;
		tSQX_L[i] = 0;
		tSQX_L[i+16] = 0;
		 SQY_L[i] = 0;
		tSQY_L[i] = 0;
		 SQ300[i] = 0;
		tSQ300[i] = 0;
		  SQ20[i] = 0;
		 tSQ20[i] = 0;
	}
}

void UserEventLeftTele::Dump(void) const
{
	//TODO dump all data members!

	// Left telescope
	cerr << " CsI_L:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<  CsI_L[i]; } cerr << endl;
	cerr << "tCsI_L:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" << tCsI_L[i]; } cerr << endl;
	cerr << " SQX_L:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<  SQX_L[i]; } cerr << endl;
	cerr << "      :"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<  SQX_L[i+16]; } cerr << endl;
	cerr << "tSQX_L:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" << tSQX_L[i]; } cerr << endl;
	cerr << "      :"; for (UInt_t i=0; i<16; i++) { cerr << "\t" << tSQX_L[i+16]; } cerr << endl;
	cerr << " SQY_L:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<  SQY_L[i]; } cerr << endl;
	cerr << "tSQY_L:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" << tSQY_L[i]; } cerr << endl;
	cerr << " SQ300:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<  SQ300[i]; } cerr << endl;
	cerr << "tSQ300:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" << tSQ300[i]; } cerr << endl;
	cerr << "  SQ20:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<   SQ20[i]; } cerr << endl;
	cerr << " tSQ20:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<  tSQ20[i]; } cerr << endl;
}

UShort_t* UserEventLeftTele::GetFieldByName(TString p_name)
{
	// Left telescope
	if      (p_name ==  "CsI_L")    { return  CsI_L; }
	else if (p_name == "tCsI_L")    { return tCsI_L; }
	else if (p_name ==  "SQX_L")    { return  SQX_L; }
	else if (p_name == "tSQX_L")    { return tSQX_L; }
	else if (p_name ==  "SQY_L")    { return  SQY_L; }
	else if (p_name == "tSQY_L")    { return tSQY_L; }
	else if (p_name ==  "SQ300")    { return  SQ300; }
	else if (p_name == "tSQ300")    { return tSQ300; }
	else if (p_name ==  "SQ20")     { return  SQ20; }
	else if (p_name == "tSQ20")     { return tSQ20; }

	/*else if (p_name == "Unknown") {
		cerr << "[WARN  ] Acquiring en event data field '" << p_name << "' which does not exist." << endl;
		return NULL; }*/
	else {
		cerr << "[ERROR ] Acquiring an event data field '" << p_name << "' which does not exist." << endl;
		return NULL;
	}
}

ClassImp(UserEventLeftTele)
