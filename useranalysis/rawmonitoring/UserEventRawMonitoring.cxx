#include "UserEventRawMonitoring.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

UserEventRawMonitoring::UserEventRawMonitoring(const char* name) :
	TGo4EventElement(name)
{
	//cerr << "UserEventRawMonitoring::UserEventRawMonitoring" << endl;

	//cerr << "UserEventRawMonitoring::UserEventRawMonitoring() -> ";
	this->Clear();
}

UserEventRawMonitoring::~UserEventRawMonitoring()
{
}

/*void UserEventRawMonitoring::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "UserEventRawMonitoring::Clear()" << endl;
}*/

ClassImp(UserEventRawMonitoring)
