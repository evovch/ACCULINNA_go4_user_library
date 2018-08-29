/**

	@class UserProcMonitoring

	Processor-class for the Repacking step of the analysis.

	The overall logic of this step is the following.
	The BuildEvent() method is called by the framework for each input event.
	Currently (and hopefully always) the input event is the result of the
	Unpacking step. In the BuildEvent() there are:
	a) loop over the input raw messages. For each input raw message the
	ProcessMessageUniversal() method is called;
	b) processing of the common event data such as 'tigger';
	c) call for the ProcessCAMACmwpcWords() method to process CAMAC words

	First ProcessMessageUniversal() identifies if the message comes from the scalers
	and calls for the ProcessMessageScaler() method, then return.

	Then ProcessMessageUniversal() identifies if the message contains machine time
	and calls for the ProcessMachineTime() method, then return.

	After that ProcessMessageUniversal() checks if the messages comes from a channel
	which is mapped to the 'Ignore' datactor-station. If so - return.

	In the end remaining messages are processed in a rather generic way.

*/

#ifndef USERPROCREPACKING_H
#define USERPROCREPACKING_H

#include <TGo4EventProcessor.h> // mother class

class TGo4EventElement;

//class UserEventRepacking;
class DetEventFull;
class UserHistosRepacking;
class UserEventUnpacking;
class RawMessage;

class UserProcRepacking : public TGo4EventProcessor
{
public: // methods

	UserProcRepacking(const char* name = "UserProcRepacking");
	virtual ~UserProcRepacking();

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
	 * Process machine time message
	 */
	void ProcessMachineTime(const RawMessage* p_message);

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
	//UserEventRepacking* fCurrentOutputEvent;
	DetEventFull* fCurrentOutputEvent;

	/**
	 * Put all your output histograms inside this object.
	 * See UserHistosRepacking class.
	 */
	UserHistosRepacking* fHistoMan;

	/**
	 * Summary stream
	 */
	FILE* fFileSummary;

	ClassDef(UserProcRepacking, 1);
};

#endif // USERPROCREPACKING_H
