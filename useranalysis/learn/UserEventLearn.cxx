#include "UserEventLearn.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

UserEventLearn::UserEventLearn(const char* name) :
	TGo4EventElement(name)
{
	//cerr << "UserEventLearn::UserEventLearn" << endl;

	//cerr << "UserEventLearn::UserEventLearn() -> ";
	this->Clear();
}

UserEventLearn::~UserEventLearn()
{
}

void UserEventLearn::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "UserEventLearn::Clear()" << endl;

}

ClassImp(UserEventLearn)
