/**

	@class DetEventDetector

*/

#ifndef DETEVENTDETECTOR_H
#define DETEVENTDETECTOR_H

#include <TGo4CompositeEvent.h> // mother class

// STD
#include <map>

// ROOT
#include <TString.h>

class DetEventDetector : public TGo4CompositeEvent
{
public:
	/**
	 * Default constructor is required for correct ROOT IO.
	 */
	DetEventDetector();
	DetEventDetector(const char* name, Short_t id, const std::map<TString, unsigned short> stationList);
	virtual ~DetEventDetector();

	//void Clear(Option_t* t = "");

	void Dump(void) const;

private:

	ClassDef(DetEventDetector, 1);
};

#endif // DETEVENTDETECTOR_H
