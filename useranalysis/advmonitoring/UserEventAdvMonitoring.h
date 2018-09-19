/**

	@class UserEventAdvMonitoring

	Event-class for the AdvMonitoring step of the analysis.

*/

#ifndef USEREVENTADVMONITORING_H
#define USEREVENTADVMONITORING_H

#include <TGo4EventElement.h> // mother class

class UserEventAdvMonitoring : public TGo4EventElement
{
public:
	UserEventAdvMonitoring(const char* name = "UserEventAdvMonitoring");
	virtual ~UserEventAdvMonitoring();

	//void Clear(Option_t* t = "");

public:

	ClassDef(UserEventAdvMonitoring, 1);
};

#endif // USEREVENTADVMONITORING_H
