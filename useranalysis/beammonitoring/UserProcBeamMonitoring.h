/**

	@class UserProcBeamMonitoring

*/

#ifndef USERPROCBEAMMONITORING_H
#define USERPROCBEAMMONITORING_H

#include <TGo4EventProcessor.h> // mother class

class TGo4EventElement;
class TGo4CompositeEvent;

class UserHistosBeamMonitoring;
class SetupConfiguration;
class DetMessage;

class UserProcBeamMonitoring : public TGo4EventProcessor
{
public: // methods

	UserProcBeamMonitoring(const char* name = "UserProcBeamMonitoring");
	virtual ~UserProcBeamMonitoring();

	virtual Bool_t BuildEvent(TGo4EventElement* p_dest);

	/** Method called when analysis is created */
	virtual void UserPreLoop();

	/** Method called when analysis is stopping */
	virtual void UserPostLoop();

private: // methods
	void ProcessMessage(DetMessage* p_message, TString stName);

	// example of 2-D histogram filling
	void fill2D(TGo4EventElement* v_subElement);

	void profileMWPC(TGo4CompositeEvent* dEvent,TString st_Name1,TString st_Name2,TH2* histo,TH2* histo_Cal,Float_t X0,Float_t dX,Float_t Y0,Float_t dY);
 // data members
	/**
	 * Counter or processed events
	 */
	unsigned long int fEventCounter;

	/**
	 * Put all your output histograms inside this object.
	 * See UserHistosBeamMonitoring class.
	 */
	UserHistosBeamMonitoring* fHistoMan;

	/**
	 * 'this' object of the UserProcRawMonitoring class does not own this pointer
	 */
	const SetupConfiguration* fSetupConfig;

	/**
	 * Summary stream
	 */
	FILE* fFileSummary;

	//parameters 
	UInt_t fTrigger;

  Float_t fMWPC1_X_displacement  = -1.0;
  Float_t fMWPC1_Y_displacement  = -2.1375;
  Float_t fMWPC2_X_displacement  = 0.2;
  Float_t fMWPC2_Y_displacement  = -1.125;
  Float_t fMWPC1_X_zero_position = -15.5*1.25;
  Float_t fMWPC1_Y_zero_position = -15.5*1.25;
  Float_t fMWPC2_X_zero_position = -15.5*1.25;
  Float_t fMWPC2_Y_zero_position = -15.5*1.25;


  TString fst_MWPC1;
  TString fst_MWPC2;
  TString fst_MWPC3;
  TString fst_MWPC4;


	ClassDef(UserProcBeamMonitoring, 1);
};

#endif // USERPROCBEAMMONITORING_H
