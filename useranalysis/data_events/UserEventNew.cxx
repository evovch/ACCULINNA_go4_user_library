#include "UserEventNew.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// Project
#include "UserEventLeftTele.h"
#include "UserEventRightTele.h"
//#include "UserEventTele3.h"
//#include "UserEventTele4.h"
#include "UserEventAnnularTele.h"
#include "UserEventNeutronDet.h"
#include "UserEventBeamDetTof.h"
#include "UserEventBeamDetMwpc.h"

UserEventNew::UserEventNew(const char* name) :
	TGo4CompositeEvent(name, name)
{
	// Detector ID is in the comment. It is hardcoded in each class.
	// It is also listed in the XML. Only for reference by now.
	addEventElement(new UserEventLeftTele()); // 1
	addEventElement(new UserEventRightTele()); // 2
	//addEventElement(new UserEventTele3()); // 3
	//addEventElement(new UserEventTele4()); // 4
	addEventElement(new UserEventAnnularTele()); // 5
	addEventElement(new UserEventNeutronDet()); // 6
	addEventElement(new UserEventBeamDetTof()); // 7
	addEventElement(new UserEventBeamDetMwpc()); // 8

	//cerr << "UserEventNew::UserEventNew() -> ";
	this->Clear();
}

UserEventNew::~UserEventNew()
{
}

void UserEventNew::Clear(Option_t* t)
{
	//TODO zero all data members!
	//cerr << "UserEventNew::Clear()" << endl;

}

void UserEventNew::Dump(void) const
{
	//TODO dump all data members!

}

ClassImp(UserEventNew)
