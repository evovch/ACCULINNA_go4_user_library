/**

	@class UserEventBeamDetMonitoring

	Event-class for the BeamDetMonitoring step of the analysis.

*/

#ifndef USEREVENTBEAMDETMONITORING_H
#define USEREVENTBEAMDETMONITORING_H

#include <TGo4EventElement.h> // mother class

class UserEventBeamDetMonitoring : public TGo4EventElement
{
public:
	UserEventBeamDetMonitoring(const char* name = "UserEventBeamDetMonitoring");
	virtual ~UserEventBeamDetMonitoring();

	//void Clear(Option_t* t = "");

public:

	ClassDef(UserEventBeamDetMonitoring, 1);
};

#endif // USEREVENTBEAMDETMONITORING_H
