/**

	@class DetEventStation

*/

#ifndef DETEVENTSTATION_H
#define DETEVENTSTATION_H

#include <TGo4EventElement.h> // mother class

class TClonesArray;

class DetEventStation : public TGo4EventElement
{
public:
	/**
	 * Default constructor is required for correct ROOT IO.
	 */
	DetEventStation();
	DetEventStation(const char* name, const char* title, Short_t id);
	virtual ~DetEventStation();

	void AddDetMessage(Int_t p_statch, Int_t p_val);

	void Clear(Option_t* t = "");

	void Dump(void) const;

private:

	TClonesArray* fDetMessages;

	ClassDef(DetEventStation, 1);
};

#endif // DETEVENTSTATION_H
