/**

	@class UserEventDigiBuilding
*/

#ifndef USEREVENTDIGIBUILDING_H
#define USEREVENTDIGIBUILDING_H

#include <TGo4EventElement.h> // mother class

class UserEventDigiBuilding : public TGo4EventElement
{
public:
	UserEventDigiBuilding(const char* name = "UserEventDigiBuilding");
	virtual ~UserEventDigiBuilding();

	void Clear(Option_t* t = "");

	virtual void Print(Option_t* option = "") const;

	ClassDef(UserEventDigiBuilding, 1);
};

#endif // USEREVENTDIGIBUILDING_H
