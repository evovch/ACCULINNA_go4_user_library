/**

	@class UserEventBeamMonitoring

	Event-class for the BeamMonitoring step of the analysis.

*/

#ifndef USEREVENTBEAMMONITORING_H
#define USEREVENTBEAMMONITORING_H

#include <TGo4EventElement.h> // mother class

class UserEventBeamMonitoring : public TGo4EventElement
{
public:
	UserEventBeamMonitoring(const char* name = "UserEventBeamMonitoring");
	virtual ~UserEventBeamMonitoring();

	//void Clear(Option_t* t = "");

public:

	ClassDef(UserEventBeamMonitoring, 1);
};

#endif // USEREVENTBEAMMONITORING_H
