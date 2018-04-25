/**

	@class UserEventBeamDetTof

*/

#ifndef USEREVENTBEAMDETTOF_H
#define USEREVENTBEAMDETTOF_H

#include <TGo4EventElement.h> // mother class

class UserEventBeamDetTof : public TGo4EventElement
{
public:
	UserEventBeamDetTof(const char* name = "UserEventBeamDetTof");
	virtual ~UserEventBeamDetTof();

	void Clear(Option_t* t = "");

	void Dump(void) const;

	UShort_t* GetFieldByName(TString p_name);

public: //TODO should be private. Made public for ER

	// Beam detector - ToF
	UShort_t  F3[4];
	UShort_t tF3[4];
	UShort_t  F5[4];
	UShort_t tF5[4];
	UShort_t  F6[4];
	UShort_t tF6[4];

	ClassDef(UserEventBeamDetTof, 1);
};

#endif // USEREVENTBEAMDETTOF_H
