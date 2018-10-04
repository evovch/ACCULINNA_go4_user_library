/**

	@class UserEventTestMonitoring

	Event-class for the TestMonitoring step of the analysis.

*/

#ifndef USEREVENTTESTMONITORING_H
#define USEREVENTTESTMONITORING_H

#include <TGo4EventElement.h> // mother class

class UserEventTestMonitoring : public TGo4EventElement
{
public:
	UserEventTestMonitoring(const char* name = "UserEventTestMonitoring");
	virtual ~UserEventTestMonitoring();

	//void Clear(Option_t* t = "");

public:

	ClassDef(UserEventTestMonitoring, 1);
};

#endif // USEREVENTTESTMONITORING_H
