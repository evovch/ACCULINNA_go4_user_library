/**

	@class UserProcLearn

	Processor-class for the Learn step of the analysis.

	Processor-class which analyses unpacked input stream and builds the summary.
	It's goal it to detect if there are:
	1) mapped channels without data or
	2) unmapped channels with data.

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
class SetupConfiguration;

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

	void ProcessUnmappedChannels(void) const;

private: // data members
	/**
	 *
	 */
	unsigned long int fEventCounter;

	/**
	 *
	 */
	UserEventLearn* fCurrentOutputEvent;

	std::set<unsigned int> fUsedChUIDs;

	TH2* fHistoAddrVsProcid;

	/**
	 * 'this' object of the UserProcLearn class does not own this pointer
	 */
	const SetupConfiguration* fSetupConfig;

	/**
	 * Summary stream
	 */
	FILE* fFileSummary;

	ClassDef(UserProcLearn, 1);
};

#endif // USERPROCLEARN_H
