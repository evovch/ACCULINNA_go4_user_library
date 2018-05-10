/**

	@class DetEventFull

	Event-class for the Repacking step of the analysis. Version 2 - generic.

	The composite-event functionality of the Go4 framework is used here.

	One DetEventFull is a group of DetEventDetector's. Each DetEventDetector
	is a group of DetEventStation's. The terminal DetEventStation is not
	a composite-event but an event-element. DetEventStation holds a TClonesArray
	of detector-messages, each detector-messages being (in essence) a pair
	<detCh, val>

*/

#ifndef DETEVENTFULL_H
#define DETEVENTFULL_H

#include <TGo4CompositeEvent.h> // mother class

class DetEventFull : public TGo4CompositeEvent
{
public:
	DetEventFull(const char* name = "DetEventFull");
	virtual ~DetEventFull();

	//void Clear(Option_t* t = "");

	void Dump(void) const;

public:

	ClassDef(DetEventFull, 1);
};

#endif // DETEVENTFULL_H
