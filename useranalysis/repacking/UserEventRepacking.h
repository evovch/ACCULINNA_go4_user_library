/**

	@class UserEventRepacking

	Event-class for the Repacking step of the analysis. Version 1 - only for spring 2018 period,

*/

#ifndef USEREVENTREPACKING_H
#define USEREVENTREPACKING_H

#include <TGo4EventElement.h> // mother class

class UserEventRepacking : public TGo4EventElement
{
	friend class UserProcMonitoring; // provider

public:
	UserEventRepacking(const char* name = "UserEventRepacking");
	virtual ~UserEventRepacking();

	void Clear(Option_t* t = "");

	virtual void Print(Option_t* option = "") const;

	UShort_t* GetFieldByName(TString p_name);

	Int_t GetFullTime(void) const
	{
		Int_t v_time = mtime[0] & 0xffff; // Overkill
		v_time = ((v_time << 16) & 0xffff0000) | (mtime[1] & 0x0000ffff);
		return v_time;
	}

public: //TODO should be private. Made public for ER

	// Left telescope
	UShort_t  CsI_L[16];
	UShort_t tCsI_L[16];
	UShort_t  SQX_L[32];
	UShort_t tSQX_L[32];
	UShort_t  SQY_L[16];
	UShort_t tSQY_L[16];
	UShort_t  SQ300[16];
	UShort_t tSQ300[16];
	UShort_t  SQ20[16];
	UShort_t tSQ20[16];

	// Right telescope
	UShort_t  CsI_R[16];
	UShort_t tCsI_R[16];
	UShort_t  SQX_R[32];
	UShort_t tSQX_R[32];
	UShort_t  SQY_R[16];
	UShort_t tSQY_R[16];

	// Annular telescope
	UShort_t ANR[16];
	UShort_t ANS[16];

	// Neutron detector (stilbene)
	UShort_t neutAmp[32];
	UShort_t neutTAC[32];
	UShort_t neutTDC[32];

	// Beam detector - ToF
	UShort_t  F3[4];
	UShort_t tF3[4];
	UShort_t  F5[4];
	UShort_t tF5[4];
	UShort_t  F6[4];
	UShort_t tF6[4];

	// Beam detector - MWPC
	// MWPC[]
	//TODO temporary solution
	UChar_t nx1; // number of fired wires
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

	ClassDef(UserEventRepacking, 1);
};

#endif // USEREVENTREPACKING_H
