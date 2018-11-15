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
class UserParameterTest;
class TClonesArray;
class DetEventFull;

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

	void FillAutoHistosCal(DetEventFull *v_input);
	// reading .cal files into TGo4Parameter objects
	void InitPars();
	// reading thin detectors thickness map
  void readThickness();

	void calibSi(TGo4CompositeEvent* p_message,std::pair <TString,Int_t> pair);

	void filldE_E_Right(TGo4CompositeEvent* d_Event);
	void filldE_E_Left(TGo4CompositeEvent* d_Event);
	void filldE_E_Central(TGo4CompositeEvent* d_Event);

	SiCalibPars* getSiPar(TString st_Name);

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
	* local User parameter
	*/
	UserParameterTest* fParTest;
	/**
	* Si-stations calibration parameters
	*/
	SiCalibPars** fParSi;
	std::pair <TString,Int_t>* fstPair;
	const Int_t fnPars = 9;

  // Thickness distribution for thin detectors
  Double_t fThicknessRight[16][16];
  Double_t fThicknessLeft[16][16];
	/**
	 * Summary stream
	 */
	FILE* fFileSummary;
	

	ClassDef(UserProcTestMonitoring, 1);
};

#endif // USERPROCTESTMONITORING_H
