/**

	@class UserProcBeamMonitoring

*/

#ifndef USERPROCBEAMMONITORING2_H
#define USERPROCBEAMMONITORING2_H

#include <TGo4EventProcessor.h> // mother class

class TGo4EventElement;
class TGo4CompositeEvent;

class UserHistosBeamMonitoring2;
class SetupConfiguration;
class DetMessage;

class UserProcBeamMonitoring2 : public TGo4EventProcessor
{
public: // methods

	UserProcBeamMonitoring2(const char* name = "UserProcBeamMonitoring2");
	virtual ~UserProcBeamMonitoring2();

	virtual Bool_t BuildEvent(TGo4EventElement* p_dest);

	/** Method called when analysis is created */
	virtual void UserPreLoop();

	/** Method called when analysis is stopping */
	virtual void UserPostLoop();

private: // methods
	void ProcessMessage(DetMessage* p_message, TString stName);

 // data members
	/**
	 * Counter or processed events
	 */
	unsigned long int fEventCounter;

	/**
	 * Put all your output histograms inside this object.
	 * See UserHistosBeamMonitoring class.
	 */
	UserHistosBeamMonitoring2* fHistoMan;

	/**
	 * 'this' object of the UserProcRawMonitoring class does not own this pointer
	 */
	const SetupConfiguration* fSetupConfig;

	/**
	 * Summary stream
	 */
	FILE* fFileSummary;

	ClassDef(UserProcBeamMonitoring2, 1);
};

#endif // USERPROCBEAMMONITORING2_H
