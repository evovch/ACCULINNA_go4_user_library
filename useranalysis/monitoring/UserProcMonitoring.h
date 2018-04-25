/**

	@class UserProcMonitoring

*/

#ifndef USERPROCMONITORING_H
#define USERPROCMONITORING_H

#include <TGo4EventProcessor.h> // mother class

class TGo4EventElement;

class UserEventMonitoring;
//class UserEventNew;
class UserHistosMonitoring;
class UserEventUnpacking;
class RawMessage;

class UserProcMonitoring : public TGo4EventProcessor
{
public: // methods

	UserProcMonitoring(const char* name = "UserProcMonitoring");
	virtual ~UserProcMonitoring();

	virtual Bool_t BuildEvent(TGo4EventElement* p_dest);

private: // methods
	/**
	 * Process raw messages from MESYTEC and CAEN electronics blocks
	 */
	void ProcessMessageUniversal(const RawMessage* p_message);

	/**
	 * Process scaler message
	 */
	void ProcessMessageScaler(const RawMessage* p_message);

	/**
	 * Process raw CAMAC words assuming they have been produced by MWPCs
	 */
	void ProcessCAMACmwpcWords(const UserEventUnpacking* p_inputEvent);

	/**
	 *
	 */
	void FillHistograms(void) const;

private: // data members
	/**
	 * Counter or processed events
	 */
	unsigned long int fEventCounter;

	/**
	 * Current output event object. This object is filled during BuildEvent
	 * and then pushed into the output tree by the framework.
	 */
	UserEventMonitoring* fCurrentOutputEvent;
	//UserEventNew* fCurrentOutputEvent;

	/**
	 * Put all your output histograms inside this object.
	 * See UserHistosMonitoring class.
	 */
	UserHistosMonitoring* fHistoMan;

	/**
	 * Summary stream
	 */
	FILE* fFileSummary;

	ClassDef(UserProcMonitoring, 1);
};

#endif // USERPROCMONITORING_H
