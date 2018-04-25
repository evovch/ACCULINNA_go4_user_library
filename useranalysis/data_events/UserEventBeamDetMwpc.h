/**

	@class UserEventBeamDetMwpc

*/

#ifndef USEREVENTBEAMDETMWPC_H
#define USEREVENTBEAMDETMWPC_H

#include <TGo4EventElement.h> // mother class

class UserEventBeamDetMwpc : public TGo4EventElement
{
public:
	UserEventBeamDetMwpc(const char* name = "UserEventBeamDetMwpc");
	virtual ~UserEventBeamDetMwpc();

	void Clear(Option_t* t = "");

	void Dump(void) const;

	UShort_t* GetFieldByName(TString p_name);

public: //TODO should be private. Made public for ER

	// Beam detector - MWPC
	// MWPC[]
	UShort_t tMWPC[4];

	ClassDef(UserEventBeamDetMwpc, 1);
};

#endif // USEREVENTBEAMDETMWPC_H
