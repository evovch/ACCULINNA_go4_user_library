/**

	@class UserEventNeutronDet

*/

#ifndef USEREVENTNEUTRONDET_H
#define USEREVENTNEUTRONDET_H

#include <TGo4EventElement.h> // mother class

class UserEventNeutronDet : public TGo4EventElement
{
public:
	UserEventNeutronDet(const char* name = "UserEventNeutronDet");
	virtual ~UserEventNeutronDet();

	void Clear(Option_t* t = "");

	void Dump(void) const;

	UShort_t* GetFieldByName(TString p_name);

public: //TODO should be private. Made public for ER

	// Neutron detector (stilbene)
	UShort_t neutAmp[32];
	UShort_t neutTAC[32];
	UShort_t neutTDC[32];

	ClassDef(UserEventNeutronDet, 1);
};

#endif // USEREVENTNEUTRONDET_H
