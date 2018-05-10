/**

	@class UserEventLearn

	Event-class for the Learn step of the analysis.

*/

#ifndef USEREVENTLEARN_H
#define USEREVENTLEARN_H

#include <TGo4EventElement.h> // mother class

class UserEventLearn : public TGo4EventElement
{
public:
	UserEventLearn(const char* name = "UserEventLearn");
	virtual ~UserEventLearn();

	//void Clear(Option_t* t = "");

public:

	ClassDef(UserEventLearn, 1);
};

#endif // USEREVENTLEARN_H
