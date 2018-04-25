/**

	@class UserEventStep2

*/

#ifndef USEREVENTSTEP2_H
#define USEREVENTSTEP2_H

#include <TGo4EventElement.h> // mother class

class TClonesArray;

class UserEventStep2 : public TGo4EventElement
{
public:
	UserEventStep2(const char* name = "UserEventStep2");
	virtual ~UserEventStep2();

	void Clear(Option_t* t = "");

public:
	TClonesArray* fMWPCdigi;

	ClassDef(UserEventStep2, 1);
};

#endif // USEREVENTSTEP2_H
