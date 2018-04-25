#include "UserEventMonitoring.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

UserEventMonitoring::UserEventMonitoring(const char* name) :
	TGo4EventElement(name)
{
	//cerr << "UserEventMonitoring::UserEventMonitoring() -> ";
	this->Clear();
}

UserEventMonitoring::~UserEventMonitoring()
{
}

void UserEventMonitoring::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "UserEventMonitoring::Clear()" << endl;

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

	// Annular telescope
	for (UInt_t i=0; i<16; i++) {
		ANR[i] = 0;
		ANS[i] = 0;
	}

	// Neutron detector (stilbene)
	for (UInt_t i=0; i<32; i++) {
		neutAmp[i] = 0;
		neutTAC[i] = 0;
		neutTDC[i] = 0;
	}

	// Beam detector - ToF
	for (UInt_t i=0; i<4; i++) {
		 F3[i] = 0;
		tF3[i] = 0;
		 F5[i] = 0;
		tF5[i] = 0;
		 F6[i] = 0;
		tF6[i] = 0;
	}

	// Beam detector - MWPC
	//TODO temporary solution
	nx1=0; ny1=0; nx2=0; ny2=0;
	for (UInt_t i=0; i<32; i++) {
		x1[i]=0; y1[i]=0; x2[i]=0; y2[i]=0;
	}
	rx1=0; ry1=0; rx2=0; ry2=0;
	//TODO end of temporary solution
	// MWPC[]
	for (UInt_t i=0; i<4; i++) {
		tMWPC[i] = 0;
	}

	// Trigger
	trigger = 0;

	// Scalers
	for (UInt_t i=0; i<16; i++) {
		scaler[i] = 0;
	}

	// Machine time

}

void UserEventMonitoring::Dump(void) const
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

	// Right telescope
	cerr << " CsI_R:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<  CsI_R[i]; } cerr << endl;
	cerr << "tCsI_R:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" << tCsI_R[i]; } cerr << endl;
	cerr << " SQX_R:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<  SQX_R[i]; } cerr << endl;
	cerr << "      :"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<  SQX_R[i+16]; } cerr << endl;
	cerr << "tSQX_R:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" << tSQX_R[i]; } cerr << endl;
	cerr << "      :"; for (UInt_t i=0; i<16; i++) { cerr << "\t" << tSQX_R[i+16]; } cerr << endl;
	cerr << " SQY_R:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<  SQY_R[i]; } cerr << endl;
	cerr << "tSQY_R:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" << tSQY_R[i]; } cerr << endl;

	// Annular telescope
	cerr << "   ANR:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<    ANR[i]; } cerr << endl;
	cerr << "   ANS:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<    ANS[i]; } cerr << endl;

	// Neutron detector (stilbene)
	cerr <<"neutAmp:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<neutAmp[i]; } cerr << endl;
	cerr <<"       :"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<neutAmp[i+16]; } cerr << endl;
	cerr <<"neutTAC:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<neutTAC[i]; } cerr << endl;
	cerr <<"       :"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<neutTAC[i+16]; } cerr << endl;
	cerr <<"neutTDC:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<neutTDC[i]; } cerr << endl;
	cerr <<"       :"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<neutTDC[i+16]; } cerr << endl;

	// Beam detector - ToF
	cerr << "    F3:"; for (UInt_t i=0; i<4; i++)  { cerr << "\t" <<     F3[i]; } cerr << endl;
	cerr << "   tF3:"; for (UInt_t i=0; i<4; i++)  { cerr << "\t" <<    tF3[i]; } cerr << endl;
	cerr << "    F5:"; for (UInt_t i=0; i<4; i++)  { cerr << "\t" <<     F5[i]; } cerr << endl;
	cerr << "   tF5:"; for (UInt_t i=0; i<4; i++)  { cerr << "\t" <<    tF5[i]; } cerr << endl;
	cerr << "    F6:"; for (UInt_t i=0; i<4; i++)  { cerr << "\t" <<     F6[i]; } cerr << endl;
	cerr << "   tF6:"; for (UInt_t i=0; i<4; i++)  { cerr << "\t" <<    tF6[i]; } cerr << endl;

	// Beam detector - MWPC
	//TODO temporary solution
	cerr << " nMWPC:"; cerr << "nx1=" << (UInt_t)nx1 << "\t" << "ny1=" << (UInt_t)ny1 << "\t"
	                        << "nx2=" << (UInt_t)nx2 << "\t" << "ny2=" << (UInt_t)ny2 << endl;
	//TODO end of temporary solution
	// MWPC[]
	cerr << " tMWPC:"; for (UInt_t i=0; i<4; i++)  { cerr << "\t" <<   tMWPC[i]; } cerr << endl;

	// Trigger
	cerr << "  trig:"; cerr << trigger << endl;

	// Scalers
	cerr << "scaler:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<  scaler[i]; } cerr << endl;

	// Machine time
}

UShort_t* UserEventMonitoring::GetFieldByName(TString p_name)
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

	// Right telescope
	else if (p_name ==  "CsI_R")    { return  CsI_R; }
	else if (p_name == "tCsI_R")    { return tCsI_R; }
	else if (p_name ==  "SQX_R")    { return  SQX_R; }
	else if (p_name == "tSQX_R")    { return tSQX_R; }
	else if (p_name ==  "SQY_R")    { return  SQY_R; }
	else if (p_name == "tSQY_R")    { return tSQY_R; }

	// Annular telescope
	else if (p_name ==  "ANR")      { return  ANR; }
	else if (p_name ==  "ANS")      { return  ANS; }

	// Neutron detector (stilbene)
	else if (p_name ==  "neutAmp")  { return  neutAmp; }
	else if (p_name ==  "neutTAC")  { return  neutTAC; }
	else if (p_name ==  "neutTDC")  { return  neutTDC; }

	// Beam detector - ToF
	else if (p_name ==  "F3")       { return  F3; }
	else if (p_name == "tF3")       { return tF3; }
	else if (p_name ==  "F5")       { return  F5; }
	else if (p_name == "tF5")       { return tF5; }
	else if (p_name ==  "F6")       { return  F6; }
	else if (p_name == "tF6")       { return tF6; }

	// Beam detector - MWPC
	//else if (p_name ==  "MWPC")     { return  MWPC; }
	else if (p_name == "tMWPC")     { return tMWPC; }

	// Trigger
	////// else if (p_name == "trigger")   { return &trigger; } // We dont' need this

	// Scalers
	////// else if (p_name == "scaler")    { return scaler; } // We dont' need this

	else if (p_name == "Ignore") {
		cerr << "[WARN  ] Acquiring en event data field '" << p_name << "' which does not exist." << endl;
		return NULL; }
	else {
		cerr << "[ERROR ] Acquiring an event data field '" << p_name << "' which does not exist." << endl;
		return NULL;
	}
}

ClassImp(UserEventMonitoring)
