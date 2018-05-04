/**

	@class DetEventFull

*/

#ifndef DETEVENTFULL_H
#define DETEVENTFULL_H

#include <TGo4CompositeEvent.h> // mother class

class DetEventFull : public TGo4CompositeEvent
{
public:
	DetEventFull(const char* name = "DetEventFull");
	virtual ~DetEventFull();

	void Clear(Option_t* t = "");

	void Dump(void) const;

public:

	ClassDef(DetEventFull, 1);
};

#endif // DETEVENTFULL_H
