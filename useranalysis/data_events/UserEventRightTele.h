/**

	@class UserEventRightTele

*/

#ifndef USEREVENTRIGHTTELE_H
#define USEREVENTRIGHTTELE_H

#include <TGo4EventElement.h> // mother class

class UserEventRightTele : public TGo4EventElement
{
public:
	UserEventRightTele(const char* name = "UserEventRightTele");
	virtual ~UserEventRightTele();

	void Clear(Option_t* t = "");

	void Dump(void) const;

	UShort_t* GetFieldByName(TString p_name);

public: //TODO should be private. Made public for ER

	// Right telescope
	UShort_t  CsI_R[16];
	UShort_t tCsI_R[16];
	UShort_t  SQX_R[32];
	UShort_t tSQX_R[32];
	UShort_t  SQY_R[16];
	UShort_t tSQY_R[16];

	ClassDef(UserEventRightTele, 1);
};

#endif // USEREVENTRIGHTTELE_H
