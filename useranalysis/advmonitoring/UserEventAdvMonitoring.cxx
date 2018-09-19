#include "UserEventAdvMonitoring.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

UserEventAdvMonitoring::UserEventAdvMonitoring(const char* name) :
	TGo4EventElement(name)
{
	//cerr << "UserEventAdvMonitoring::UserEventAdvMonitoring" << endl;

	//cerr << "UserEventAdvMonitoring::UserEventAdvMonitoring() -> ";
	this->Clear();
}

UserEventAdvMonitoring::~UserEventAdvMonitoring()
{
}

/*void UserEventAdvMonitoring::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "UserEventAdvMonitoring::Clear()" << endl;
}*/

ClassImp(UserEventAdvMonitoring)
