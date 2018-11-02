#include "UserEventBeamDetMonitoring.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

UserEventBeamDetMonitoring::UserEventBeamDetMonitoring(const char* name) :
	TGo4EventElement(name)
{
	//cerr << "UserEventBeamDetMonitoring::UserEventBeamDetMonitoring" << endl;

	//cerr << "UserEventBeamDetMonitoring::UserEventBeamDetMonitoring() -> ";
	this->Clear();
}

UserEventBeamDetMonitoring::~UserEventBeamDetMonitoring()
{
}

/*void UserEventBeamDetMonitoring::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "UserEventBeamDetMonitoring::Clear()" << endl;
}*/

ClassImp(UserEventBeamDetMonitoring)
