/**

	@class UserProcAdvMonitoring

*/

#ifndef USERPROCADVMONITORING_H
#define USERPROCADVMONITORING_H

#include <TGo4EventProcessor.h> // mother class

class TGo4EventElement;

class UserHistosAdvMonitoring;
class SetupConfiguration;
class DetMessage;

class UserProcAdvMonitoring : public TGo4EventProcessor
{
public: // methods

	UserProcAdvMonitoring(const char* name = "UserProcAdvMonitoring");
	virtual ~UserProcAdvMonitoring();

	virtual Bool_t BuildEvent(TGo4EventElement* p_dest);

	/** Method called when analysis is created */
	virtual void UserPreLoop();

	/** Method called when analysis is stopping */
	virtual void UserPostLoop();

	// reading input parameters
	void readParFile(TString parFile);

private: // methods
	void ProcessMessage(DetMessage* p_message, TString stName);

private: // data members
	/**
	 * Counter or processed events
	 */
	unsigned long int fEventCounter;

	/**
	 * Put all your output histograms inside this object.
	 * See UserHistosAdvMonitoring class.
	 */
	UserHistosAdvMonitoring* fHistoMan;

	/**
	 * 'this' object of the UserProcRawMonitoring class does not own this pointer
	 */
	const SetupConfiguration* fSetupConfig;

	/**
	 * Summary stream
	 */
	FILE* fFileSummary;

	Double_t parCsI_R_1[16],parCsI_R_2[16];

	ClassDef(UserProcAdvMonitoring, 1);
};

#endif // USERPROCADVMONITORING_H
