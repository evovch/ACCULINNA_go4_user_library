/**

	@class UserProcTestMonitoring

*/

#ifndef USERPROCTESTMONITORING_H
#define USERPROCTESTMONITORING_H

#include <TGo4EventProcessor.h> // mother class

class TGo4EventElement;
class TGo4CompositeEvent;

class UserHistosTestMonitoring;
class SetupConfiguration;
class DetMessage;

class UserProcTestMonitoring : public TGo4EventProcessor
{
public: // methods

	UserProcTestMonitoring(const char* name = "UserProcTestMonitoring");
	virtual ~UserProcTestMonitoring();

	virtual Bool_t BuildEvent(TGo4EventElement* p_dest);

	/** Method called when analysis is created */
	virtual void UserPreLoop();

	/** Method called when analysis is stopping */
	virtual void UserPostLoop();

	// reading input parameters
	void readParFile(TString parFile);

private: // methods
	void ProcessMessage(DetMessage* p_message, TString stName);

	// example of 2-D histogram filling
	void fill2D(TGo4CompositeEvent* dEvent);

private: // data members
	/**
	 * Counter or processed events
	 */
	unsigned long int fEventCounter;

	/**
	 * Put all your output histograms inside this object.
	 * See UserHistosTestMonitoring class.
	 */
	UserHistosTestMonitoring* fHistoMan_test;

	/**
	 * 'this' object of the UserProcRawMonitoring class does not own this pointer
	 */
	const SetupConfiguration* fSetupConfig;

	/**
	 * Summary stream
	 */
	FILE* fFileSummary;

	Double_t parCsI_R_1[16],parCsI_R_2[16];
	
	ClassDef(UserProcTestMonitoring, 1);
};

#endif // USERPROCTESTMONITORING_H
