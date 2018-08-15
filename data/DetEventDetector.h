/**

	@class DetEventDetector

*/

#ifndef DETEVENTDETECTOR_H
#define DETEVENTDETECTOR_H

#include "go4pieces/TGo4CompositeEvent.h" // mother class

// STD
#include <map>

// ROOT
#include <TString.h>
#include <TArrayS.h>

class DetEventDetector : public TGo4CompositeEvent
{
public:
	/**
	 * Default constructor is required for correct ROOT IO.
	 */
	DetEventDetector();
	DetEventDetector(const char* name, Short_t id, const std::map<TString, unsigned short> stationList);
	virtual ~DetEventDetector();

	//void MapToBranch(TTree* t, TString detName, const std::map<TString, unsigned short> stationList);

	//void Clear(Option_t* t = "");

	virtual void Print(Option_t* option = "") const;

private:

	TArrayS mChildrenIndices;

	ClassDef(DetEventDetector, 1);
};

#endif // DETEVENTDETECTOR_H
