#include "UserEventTestMonitoring.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

UserEventTestMonitoring::UserEventTestMonitoring(const char* name) :
	TGo4EventElement(name)
{
	//cerr << "UserEventTestMonitoring::UserEventTestMonitoring" << endl;

	//cerr << "UserEventTestMonitoring::UserEventTestMonitoring() -> ";
	this->Clear();
}

UserEventTestMonitoring::~UserEventTestMonitoring()
{
}

/*void UserEventTestMonitoring::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "UserEventTestMonitoring::Clear()" << endl;
}*/

ClassImp(UserEventTestMonitoring)
