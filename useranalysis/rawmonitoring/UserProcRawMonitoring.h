/**

	@class UserProcRawMonitoring

*/

#ifndef USERPROCRAWMONITORING_H
#define USERPROCRAWMONITORING_H

#include <TGo4EventProcessor.h> // mother class

class TGo4EventElement;

class UserHistosRawMonitoring;
class RawMessage;

class UserProcRawMonitoring : public TGo4EventProcessor
{
public: // methods

	UserProcRawMonitoring(const char* name = "UserProcRawMonitoring");
	virtual ~UserProcRawMonitoring();

	virtual Bool_t BuildEvent(TGo4EventElement* p_dest);

	/** Method called when analysis is created */
	virtual void UserPreLoop();

	/** Method called when analysis is stopping */
	virtual void UserPostLoop();

	void ProcessMessage(const RawMessage* p_message);

private: // data members
	/**
	 * Counter or processed events
	 */
	unsigned long int fEventCounter;

	/**
	 * Put all your output histograms inside this object.
	 * See UserHistosRawMonitoring class.
	 */
	UserHistosRawMonitoring* fHistoMan;

	/**
	 * Summary stream
	 */
	FILE* fFileSummary;

	ClassDef(UserProcRawMonitoring, 1);
};

#endif // USERPROCRAWMONITORING_H
