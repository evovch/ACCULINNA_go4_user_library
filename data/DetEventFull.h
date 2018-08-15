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

#include "go4pieces/TGo4CompositeEvent.h" // mother class

// ROOT
#include <TArrayS.h>

class DetEventFull : public TGo4CompositeEvent
{
public:
	/**
	 * Default constructor is required for correct ROOT IO.
	 */
	DetEventFull();
	DetEventFull(const char* name);
	virtual ~DetEventFull();

	//void MapToBranch(TTree* t);

	//void Clear(Option_t* t = "");

	virtual void Print(Option_t* option = "") const;

private:

	TArrayS mChildrenIndices;

	ClassDef(DetEventFull, 1);
};

#endif // DETEVENTFULL_H
