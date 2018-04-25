/**

	@class UserProcStep2

*/

#ifndef USERPROCSTEP2_H
#define USERPROCSTEP2_H

#include <TGo4EventProcessor.h> // mother class

class TGo4EventElement;

class UserEventStep2;

class UserProcStep2 : public TGo4EventProcessor
{
public: // methods

	UserProcStep2(const char* name = "UserProcStep2");
	virtual ~UserProcStep2();

	virtual Bool_t BuildEvent(TGo4EventElement* p_dest);

private: // methods
	/**
	 *
	 */
	void PrintBits(size_t const size, void const * const ptr);

private: // data members
	/**
	 *
	 */
	unsigned long int fEventCounter;

	/**
	 *
	 */
	UserEventStep2* fCurrentOutputEvent;

	ClassDef(UserProcStep2, 1);
};

#endif // USERPROCSTEP2_H
