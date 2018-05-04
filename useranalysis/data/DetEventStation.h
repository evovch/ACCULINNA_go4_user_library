/**

	@class DetEventStation

*/

#ifndef DETEVENTSTATION_H
#define DETEVENTSTATION_H

#include <TGo4EventElement.h> // mother class

class DetEventStation : public TGo4EventElement
{
public:
	DetEventStation(const char* name, const char* folder, Short_t id);
	virtual ~DetEventStation();

	void Clear(Option_t* t = "");

	void Dump(void) const;

private:

	ClassDef(DetEventStation, 1);
};

#endif // DETEVENTSTATION_H
