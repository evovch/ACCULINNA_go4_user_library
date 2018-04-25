/**

	@class UserEventLeftTele

*/

#ifndef USEREVENTLEFTTELE_H
#define USEREVENTLEFTTELE_H

#include <TGo4EventElement.h> // mother class

class UserEventLeftTele : public TGo4EventElement
{
public:
	UserEventLeftTele(const char* name = "UserEventLeftTele");
	virtual ~UserEventLeftTele();

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

	ClassDef(UserEventLeftTele, 1);
};

#endif // USEREVENTLEFTTELE_H
