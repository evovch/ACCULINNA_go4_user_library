/**

	@class UserEventAnnularTele

*/

#ifndef USEREVENTANNULARTELE_H
#define USEREVENTANNULARTELE_H

#include <TGo4EventElement.h> // mother class

class UserEventAnnularTele : public TGo4EventElement
{
public:
	UserEventAnnularTele(const char* name = "UserEventAnnularTele");
	virtual ~UserEventAnnularTele();

	void Clear(Option_t* t = "");

	void Dump(void) const;

	UShort_t* GetFieldByName(TString p_name);

public: //TODO should be private. Made public for ER

	// Annular telescope
	UShort_t ANR[16];
	UShort_t ANS[16];


	ClassDef(UserEventAnnularTele, 1);
};

#endif // USEREVENTANNULARTELE_H
