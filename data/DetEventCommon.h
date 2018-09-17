/**

	@class DetEventCommon

*/

#ifndef DETEVENTCOMMON_H
#define DETEVENTCOMMON_H

#include "go4pieces/TGo4EventElement.h" // mother class

class DetEventCommon : public TGo4EventElement
{
//	friend class UserProcRepacking; // provider

public:
	/**
	 * We don't need a constructor without arguments as the only
	 * available constructor has the default values for all the arguments.
	 */
	//TODO testing

	/**
	 * This default constructor will interfere with the second constructor with all
	 * arguments with default values. This in trunleads to an error:
	 * Error in <TClass::New>: cannot create object of class DetEventCommon
	 * when trying to browse this branch in the ROOT interactive browser.
	 * Initially this default constructor was introduced with the commit
	 * ae0870d26d54fa37fe160d57c3dcd8d88e0b0942
	 * and probably has something to do with the Reader module.
	 * Still to be tested...
	 */
	//DetEventCommon();

	DetEventCommon(const char* name = "DetEventCommon", Short_t id = 0);
	virtual ~DetEventCommon();

	void Clear(Option_t* t = "");

	virtual void Print(Option_t* option = "") const;

	Int_t GetFullTime(void) const
	{
		Int_t v_time = mtime[0] & 0xffff; // Overkill
		v_time = ((v_time << 16) & 0xffff0000) | (mtime[1] & 0x0000ffff);
		return v_time;
	}

public: //TODO should be private. Made public for ER

	//FIXME This has to reform into normal DetMessages in the corresponding place, not here
	// Beam detector - MWPC
	// MWPC[]
	//TODO temporary solution
/*	UChar_t nx1; // number of fired wires
	UChar_t ny1; // number of fired wires
	UChar_t nx2; // number of fired wires
	UChar_t ny2; // number of fired wires
	UChar_t x1[32]; // [nx1] indices of the fired wires
	UChar_t y1[32]; // [ny1] indices of the fired wires
	UChar_t x2[32]; // [nx2] indices of the fired wires
	UChar_t y2[32]; // [ny2] indices of the fired wires
	UInt_t rx1; // register (raw) as is has been read
	UInt_t ry1; // register (raw) as is has been read
	UInt_t rx2; // register (raw) as is has been read
	UInt_t ry2; // register (raw) as is has been read
	//TODO end of temporary solution
	UShort_t tMWPC[4];
*/
	// Trigger
	UInt_t trigger;

	/**
	 * Data from V560 scalers
	 * These data do not correspond to any specific detector
	 * There are 16 words, each 32 bits
	 */
	UInt_t scaler[16];

	// Machine time
	//TODO temporary solution. Think better. Currently - split 32 bits into two 16-bits half-words
	// Full time fill look like mtime[0]mtime[1]
	UShort_t mtime[2];

	// VULOM? - 96 ch. scaler?

	ClassDef(DetEventCommon, 1);
};

#endif // DETEVENTCOMMON_H
