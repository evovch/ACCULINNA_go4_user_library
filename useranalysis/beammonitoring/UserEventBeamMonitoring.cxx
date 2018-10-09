#include "UserEventBeamMonitoring.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

UserEventBeamMonitoring::UserEventBeamMonitoring(const char* name) :
	TGo4EventElement(name)
{
	//cerr << "UserEventBeamMonitoring::UserEventBeamMonitoring" << endl;

	//cerr << "UserEventBeamMonitoring::UserEventBeamMonitoring() -> ";
	this->Clear();
}

UserEventBeamMonitoring::~UserEventBeamMonitoring()
{
}

/*void UserEventBeamMonitoring::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "UserEventBeamMonitoring::Clear()" << endl;
}*/

ClassImp(UserEventBeamMonitoring)
