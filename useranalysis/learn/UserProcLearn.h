/**

	@class UserProcLearn

*/

#ifndef USERPROCLEARN_H
#define USERPROCLEARN_H

#include <TGo4EventProcessor.h> // mother class

// STD
#include <set>

class TH2;

class TGo4EventElement;

class UserEventLearn;
class RawMessage;

class UserProcLearn : public TGo4EventProcessor
{
public: // methods

	UserProcLearn(const char* name = "UserProcLearn");
	virtual ~UserProcLearn();

	virtual Bool_t BuildEvent(TGo4EventElement* p_dest);

	/** Method called when analysis is created */
	virtual void UserPreLoop();

	/** Method called when analysis is stopping */
	virtual void UserPostLoop();

	void ProcessMessage(const RawMessage* p_message);

private: // methods

private: // data members
	/**
	 *
	 */
	unsigned long int fEventCounter;

	/**
	 *
	 */
	UserEventLearn* fCurrentOutputEvent;

	std::set<unsigned long int> fUsedChUIDs;

	TH2* fHistoAddrVsProcid;

	/**
	 * Summary stream
	 */
	FILE* fFileSummary;

	ClassDef(UserProcLearn, 1);
};

#endif // USERPROCLEARN_H
