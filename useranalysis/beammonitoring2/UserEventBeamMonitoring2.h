/**

	@class UserEventBeamMonitoring

	Event-class for the BeamMonitoring step of the analysis.

*/

#ifndef USEREVENTBEAMMONITORING2_H
#define USEREVENTBEAMMONITORING2_H

#include <TGo4EventElement.h> // mother class

class UserEventBeamMonitoring2 : public TGo4EventElement
{
public:
	UserEventBeamMonitoring2(const char* name = "UserEventBeamMonitoring2");
	virtual ~UserEventBeamMonitoring2();

	//void Clear(Option_t* t = "");

public:

	ClassDef(UserEventBeamMonitoring2, 1);
};

#endif // USEREVENTBEAMMONITORING2_H
