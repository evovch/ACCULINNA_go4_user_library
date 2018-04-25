/**

	@class UserEventMonitoring

*/

#ifndef USEREVENTMONITORING_H
#define USEREVENTMONITORING_H

#include <TGo4EventElement.h> // mother class

class UserEventMonitoring : public TGo4EventElement
{
	friend class UserProcMonitoring; // provider

public:
	UserEventMonitoring(const char* name = "UserEventMonitoring");
	virtual ~UserEventMonitoring();

	void Clear(Option_t* t = "");

	void Dump(void) const;

	UShort_t* GetFieldByName(TString p_name);

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

	// VULOM? - 96 ch. scaler?

	ClassDef(UserEventMonitoring, 1);
};

#endif // USEREVENTMONITORING_H
