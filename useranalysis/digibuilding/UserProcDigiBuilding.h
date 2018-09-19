/**

	@class UserProcDigiBuilding

*/

#ifndef USERPROCDIGIBUILDING_H
#define USERPROCDIGIBUILDING_H

#include <TGo4EventProcessor.h> // mother class

class TGo4EventElement;

class UserEventDigiBuilding;

class UserProcDigiBuilding : public TGo4EventProcessor
{
public: // methods

	UserProcDigiBuilding(const char* name = "UserProcDigiBuilding");
	virtual ~UserProcDigiBuilding();

	virtual Bool_t BuildEvent(TGo4EventElement* p_dest);

private: // methods

private: // data members
	/**
	 * Counter or processed events
	 */
	unsigned long int fEventCounter;

	UserEventDigiBuilding* fCurrentOutputEvent;

	ClassDef(UserProcDigiBuilding, 1);
};

#endif // USERPROCDIGIBUILDING_H
