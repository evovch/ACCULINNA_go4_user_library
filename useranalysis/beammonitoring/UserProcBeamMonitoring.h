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
class TVector3;
class TClonesArray;

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
	void fill2D(TGo4CompositeEvent* dEvent);

	TVector3 profileMWPC(TGo4CompositeEvent* dEvent,TString st_Name1,TString st_Name2,TH2* histo,TH2* histo_Cal,Float_t X0,Float_t dX,Float_t Y0,Float_t dY,TVector3 xyMWPC);
  void profileTarget(TVector3 beamVector,TVector3 xyMWPCclose,Float_t z,TH2* histo);

  Bool_t IsCluster (TClonesArray* v_MWPC);

  void IDdeToF(TGo4CompositeEvent* dEvent,TH2* histo);

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

  Float_t fMWPC1_X_displacement;
  Float_t fMWPC1_Y_displacement;
  Float_t fMWPC2_X_displacement;
  Float_t fMWPC2_Y_displacement;
  Float_t fMWPC1_X_zero_position;
  Float_t fMWPC1_Y_zero_position;
  Float_t fMWPC2_X_zero_position;
  Float_t fMWPC2_Y_zero_position;
  Float_t fMWPC1z;
  Float_t fMWPC2z;  

  TString fst_MWPC1;
  TString fst_MWPC2;
  TString fst_MWPC3;
  TString fst_MWPC4;

  TString fst_F3;
  TString fst_tF3; 
  TString fst_F5;
  TString fst_tF5;   

  Float_t fBeamPlaneZ;

	ClassDef(UserProcBeamMonitoring, 1);
};

#endif // USERPROCBEAMMONITORING_H
