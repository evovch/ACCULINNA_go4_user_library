/**

	@class DetEventDetector

*/

#ifndef DETEVENTDETECTOR_H
#define DETEVENTDETECTOR_H

#include <TGo4CompositeEvent.h> // mother class

// STD
#include <set>

// ROOT
#include <TString.h>

class DetEventDetector : public TGo4CompositeEvent
{
public:
	DetEventDetector(const char* name, Short_t id, const std::set<TString> stationList);
	virtual ~DetEventDetector();

	void Clear(Option_t* t = "");

	void Dump(void) const;

private:

	ClassDef(DetEventDetector, 1);
};

#endif // DETEVENTDETECTOR_H
