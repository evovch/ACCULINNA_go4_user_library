/**

	@class UserEventRawMonitoring

	Event-class for the RawMonitoring step of the analysis.

*/

#ifndef USEREVENTRAWMONITORING_H
#define USEREVENTRAWMONITORING_H

#include <TGo4EventElement.h> // mother class

class UserEventRawMonitoring : public TGo4EventElement
{
public:
	UserEventRawMonitoring(const char* name = "UserEventRawMonitoring");
	virtual ~UserEventRawMonitoring();

	//void Clear(Option_t* t = "");

public:

	ClassDef(UserEventRawMonitoring, 1);
};

#endif // USEREVENTRAWMONITORING_H
