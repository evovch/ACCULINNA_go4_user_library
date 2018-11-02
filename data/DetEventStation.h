/**

	@class DetEventStation

*/

#ifndef DETEVENTSTATION_H
#define DETEVENTSTATION_H

#include "go4pieces/TGo4EventElement.h" // mother class

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

	virtual void Print(Option_t* option = "") const;

public:
	/**
	 * Accessor
	 */
	TClonesArray* GetDetMessages(void) const { return fData; }

private:
	/**
	 * The main component of the station-event -
	 * an array of detector-messages.
	 */
	TClonesArray* fData;

	ClassDef(DetEventStation, 1);
};

#endif // DETEVENTSTATION_H
