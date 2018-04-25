/**

	@class UserEventNew

*/

#ifndef USEREVENTNEW_H
#define USEREVENTNEW_H

#include <TGo4CompositeEvent.h> // mother class

// Project
//class UserEventLeftTele;
//class UserEventRightTele;

class UserEventNew : public TGo4CompositeEvent
{
	friend class UserProcMonitoring; // provider

public:
	UserEventNew(const char* name = "UserEventNew");
	virtual ~UserEventNew();

	void Clear(Option_t* t = "");

	void Dump(void) const;

public: //TODO should be private. Made public for ER

//	UserEventLeftTele* fSQL;
//	UserEventRightTele* fSQR;

	ClassDef(UserEventNew, 1);
};

#endif // USEREVENTNEW_H
