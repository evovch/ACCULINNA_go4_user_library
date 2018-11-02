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
class DetEventStation;
class DetMessage;
class SiCalibPars;

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

private: // methods
	void ProcessMessage(DetEventStation* p_message,Int_t parNum);

	// reading .cal files into TGo4Parameter objects
	void InitPars();

	void calibSi(TGo4CompositeEvent* p_message,std::pair <TString,Int_t> pair);

public:

	std::pair <TString,Int_t>* getPairs();	

private: // data members
	/**
	 * Counter or processed events
	 */
	unsigned long int fEventCounter;

	/**
	 * Put all your output histograms inside this object.
	 * See UserHistosTestMonitoring class.
	 */
	UserHistosTestMonitoring* fHistoMan;

	/**
	 * 'this' object of the UserProcRawMonitoring class does not own this pointer
	 */
	const SetupConfiguration* fSetupConfig;

	/**
	* Si-stations calibration parameters
	*/
	SiCalibPars** fParSi;
	std::pair <TString,Int_t>* fstPair;
	const Int_t fnPars = 8;
	/**
	 * Summary stream
	 */
	FILE* fFileSummary;
	
	ClassDef(UserProcTestMonitoring, 1);
};

#endif // USERPROCTESTMONITORING_H
