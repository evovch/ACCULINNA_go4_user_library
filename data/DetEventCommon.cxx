#include "DetEventCommon.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

/*DetEventCommon::DetEventCommon() :
	TGo4EventElement()
{
	cerr << "DEFAULT DetEventCommon CONSTRUCTOR" << endl;
}*/

DetEventCommon::DetEventCommon(const char* name, Short_t id) :
	TGo4EventElement(name, name, id)
{
	cerr << "DetEventCommon::DetEventCommon: name=" << name << " id=" << id << endl;

	this->Clear();
}

DetEventCommon::~DetEventCommon()
{
}

void DetEventCommon::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "DetEventCommon::Clear()" << endl;
/*
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
*/
	// Trigger
	trigger = 0;

	// Scalers
	for (UInt_t i=0; i<16; i++) {
		scaler[i] = 0;
	}

	// Machine time
	mtime[0] = 0; mtime[1] = 0;
}

void DetEventCommon::Print(Option_t* option) const
{
	//TODO dump all data members!
/*
	// Beam detector - MWPC
	//TODO temporary solution
	cerr << " nMWPC:"; cerr << "nx1=" << (UInt_t)nx1 << "\t" << "ny1=" << (UInt_t)ny1 << "\t"
	                        << "nx2=" << (UInt_t)nx2 << "\t" << "ny2=" << (UInt_t)ny2 << endl;
	//TODO end of temporary solution
	// MWPC[]
	cerr << " tMWPC:"; for (UInt_t i=0; i<4; i++)  { cerr << "\t" <<   tMWPC[i]; } cerr << endl;
*/
	// Trigger
	cerr << "  trig:"; cerr << "\t" << trigger << endl;

	// Scalers
	cerr << "scaler:"; for (UInt_t i=0; i<16; i++) { cerr << "\t" <<  scaler[i]; } cerr << endl;

	// Machine time
	cerr << " mtime:"; cerr << "\t" << this->GetFullTime() << endl;
}

ClassImp(DetEventCommon)
