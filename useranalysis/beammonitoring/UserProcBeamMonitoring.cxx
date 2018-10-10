
#include "UserProcBeamMonitoring.h"

// STD
#include <fstream>
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

// ROOT
#include <TClonesArray.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TVector3.h>

// Project
#include "data/DetEventFull.h" // input event
#include "data/DetEventCommon.h"
#include "data/DetEventStation.h"
#include "data/DetMessage.h"

#include "UserHistosBeamMonitoring.h"
#include "UserParameter.h"
#include "setupconfigcppwrapper/SetupConfiguration.h"

#include <stdlib.h>

using namespace std;
/**
  Uncomment this if you want to see all the debug information.
  This allows you to analyze the raw bytes and bits by your eyes.
  This option produces A LOT OF DATA - run your analysis with a
  small number of events (~10-100)
*/
//#define DEBUGBeamMON

UserProcBeamMonitoring::UserProcBeamMonitoring(const char* name) :
	TGo4EventProcessor(name)
	// fEventCounter(0),
	// fTrigger(1),
	// fMWPC1_X_displacement(-1.), 
	// fMWPC1_Y_displacement(-2.1375), 
	// fMWPC2_X_displacement(0.2), 
	// fMWPC2_Y_displacement(-1.125), 
	// fMWPC1_X_zero_position(-15.5*1.25),
	// fMWPC1_Y_zero_position(-15.5*1.25),
	// fMWPC2_X_zero_position(-15.5*1.25),
	// fMWPC2_Y_zero_position(-15.5*1.25),
	// fMWPC1z(-816.),
	// fMWPC2z(-270.),
	// fBeamPlaneZ(0.),
 //  fst_MWPC1("Beam_detector_MWPC1"),
	// fst_MWPC2("Beam_detector_MWPC2"),
	// fst_MWPC3("Beam_detector_MWPC3"),
	// fst_MWPC4("Beam_detector_MWPC4")
{
	fEventCounter = 0;
	fTrigger = 1;
	fMWPC1_X_displacement = -1.; 
	fMWPC1_Y_displacement = -2.1375; 
	fMWPC2_X_displacement = 0.2; 
	fMWPC2_Y_displacement = -1.125; 
	fMWPC1_X_zero_position = -15.5*1.25;
	fMWPC1_Y_zero_position = -15.5*1.25;
	fMWPC2_X_zero_position = -15.5*1.25;
	fMWPC2_Y_zero_position = -15.5*1.25;
	fMWPC1z = -816.;
	fMWPC2z = -270.;
	fBeamPlaneZ = 0.;
  fst_MWPC1 = "Beam_detector_MWPC1";
	fst_MWPC2 = "Beam_detector_MWPC2";
	fst_MWPC3 = "Beam_detector_MWPC3";
	fst_MWPC4 = "Beam_detector_MWPC4";
	fst_F3 = "Beam_detector_F3";
	fst_tF3 = "Beam_detector_tF3";
	fst_F5 = "Beam_detector_F5";
	fst_tF5 = "Beam_detector_tF5";	

	fHistoMan = new UserHistosBeamMonitoring();
	// cerr << " UserProcBeamMonitoring CALLED !!! ## &Y$@!UHNEFJNASJDf " << endl;
	fFileSummary = fopen("textoutput/summaryBeamMonitoring.txt", "w");
	if (fFileSummary == NULL) {
		//TODO error
		cerr << "[WARN  ] " << "Could not open output text summary file '" << "summaryBeamMonitoring.txt" << "'" << endl;
	}
}

UserProcBeamMonitoring::~UserProcBeamMonitoring()
{
	if (fHistoMan) delete fHistoMan;
	if (fFileSummary != NULL) {
		fclose(fFileSummary);
	}
}

Bool_t UserProcBeamMonitoring::BuildEvent(TGo4EventElement* p_dest)
{
	Bool_t v_isValid = kFALSE;

	DetEventFull* v_input = (DetEventFull*)GetInputEvent("stepRepackedProvider2");
	if (v_input == NULL)
	{
		cerr << "[WARN  ] " << "UserProcBeamMonitoring::BuildEvent(): no input event!" << endl;
		return v_isValid;
	}
	v_isValid = kTRUE;

	#ifdef DEBUGBeamMON
	cerr << "[DEBUG ] " << "UserProcBeamMonitoring: Event " << fEventCounter
	     << " ======================================================================================================"
	     << endl;
	#endif

	// --------------------------
	// Loop over sub-elements. There is one sub-element which is the 'DetEventCommon'
	// and all other are 'DetEventDetector's
	UInt_t trigger;
	TGo4EventElement* v_comElement = v_input->getEventElement("DetEventCommon",1);
	if(!v_comElement) {
		cout << "Detector DetEventCommon was not found " << endl;
		return kFALSE;
	}
	DetEventCommon* v_commSubEl = (DetEventCommon*)(v_comElement);
	trigger = v_commSubEl->trigger;
	if(trigger!=fTrigger){
		// cout << " trigger has come not from beamDet" << endl;
		return kFALSE;	
	}

	TGo4EventElement* v_subElement = v_input->getEventElement("Beam_detector",1);
	if(!v_subElement){
		cerr << " Detector Beam_detector was not found " << endl;
		return kFALSE;
	}

	TGo4CompositeEvent* dEvent = (TGo4CompositeEvent*)(v_subElement);
	fill2D(dEvent);

	return v_isValid;
}

void UserProcBeamMonitoring::UserPreLoop()
{
	// Get the all-accessible parameter-set object
	UserParameter* v_params = (UserParameter*)GetParameter("UserParameter");
	fSetupConfig = v_params->GetSetupConfig();

	#ifdef DEBUGBeamMON
	cerr << "[DEBUG ] " << "UserProcBeamMonitoring::UserPreLoop ====================================" << endl;
	#endif

	// fHistoMan->GenerateAutoHistos();

	#ifdef DEBUGBeamMON
	cerr << "[DEBUG ] " << "=======================================================================" << endl;
	#endif
}

void UserProcBeamMonitoring::UserPostLoop()
{
}

void UserProcBeamMonitoring::ProcessMessage(DetMessage* p_message, TString stName)
{
	//TODO implement your processing of independent messages here

}

void UserProcBeamMonitoring::fill2D(TGo4CompositeEvent* dEvent){
	TVector3 hitFar(-1000.,-1000.,fMWPC1z); // KOSTYL!! if smth is wrong in clusterisation we gonna have
	TVector3 hitClose(1000.,1000.,fMWPC2z); // beamVector (2000,2000,z) which will not pass through the line 270
	TVector3 beamVector;

	hitFar = profileMWPC(dEvent,fst_MWPC1,fst_MWPC2,fHistoMan->fY1_X1,fHistoMan->fY1_X1_C,fMWPC1_X_zero_position,fMWPC1_X_displacement,fMWPC1_Y_zero_position,fMWPC1_Y_displacement,hitFar);
	hitClose = profileMWPC(dEvent,fst_MWPC3,fst_MWPC4,fHistoMan->fY2_X2,fHistoMan->fY2_X2_C,fMWPC2_X_zero_position,fMWPC2_X_displacement,fMWPC2_Y_zero_position,fMWPC2_Y_displacement,hitClose);
 
  beamVector = hitClose - hitFar;	
  profileTarget(beamVector,hitClose,fBeamPlaneZ,fHistoMan->fTarget);

	IDdeToF(dEvent,fHistoMan->fdEToF);


}

TVector3 UserProcBeamMonitoring::profileMWPC(TGo4CompositeEvent* dEvent,TString st_Name1,TString st_Name2,TH2* histo,TH2* histo_Cal,Float_t X0,Float_t dX,Float_t Y0,Float_t dY,TVector3 xyMWPC){
	vector <Int_t> nx1;
	vector <Int_t> nx2;

	DetEventStation* st_MWPC1 = (DetEventStation*)(dEvent->getEventElement(st_Name1.Data(),1));
	if(!st_MWPC1) {
		cout << " station " << st_Name1.Data() <<  " was not found " << endl;
		return xyMWPC;
	}
	DetEventStation* st_MWPC2 = (DetEventStation*)(dEvent->getEventElement(st_Name2.Data(),1));
	if(!st_MWPC2) {
		cout << " station " << st_Name2.Data() <<  " was not found " << endl;
		return xyMWPC;
	}

	TClonesArray* v_MWPC1 = st_MWPC1->GetDetMessages();
	TClonesArray* v_MWPC2 = st_MWPC2->GetDetMessages();

	Int_t mx1 = v_MWPC1->GetEntriesFast();
	Int_t mx2 = v_MWPC2->GetEntriesFast();

	for(Int_t i = 0; i < mx1; i++){
		DetMessage *mes_MWPC1 = (DetMessage*)v_MWPC1->At(i);
		nx1.push_back(mes_MWPC1->GetStChannel());
	}

	for(Int_t i = 0; i < mx2; i++){
		DetMessage *mes_MWPC2 = (DetMessage*)v_MWPC2->At(i);
		nx2.push_back(mes_MWPC2->GetStChannel());
	}

	if(mx1<1 || mx2<1) {
		return xyMWPC; // TODO make this EVENT unprocessed!!
	}

	Int_t nx,ny;
	nx = -1; ny=-1;

	Bool_t cluster=kTRUE;
	if(mx1>1) {
		cluster = IsCluster(v_MWPC1); // check that all wires in array are neigbours
		if(cluster){
			nx = (nx1.at(0) + nx1.at(mx1-1))/2;	//if the number of fired wires is even taking the lower middle
		}
		else{
			// cout << " found more than 1 cluster in " << st_Name1.Data() << endl;
			return xyMWPC; // TODO make this EVENT unprocessed!!
		}
	} // if mx1
	else{
		nx = nx1.at(0);
	}
	if(mx2>1) {
		cluster = IsCluster(v_MWPC2); // check that all wires in array are neigbours
		if(cluster){
			ny = (nx2.at(0) + nx2.at(mx2-1))/2;	//if the number of fired wires is even taking the lower middle		
		}
		else{
			// cout << " found more than 1 cluster in " << st_Name2.Data() << endl;
			return xyMWPC; // TODO make this EVENT unprocessed!!
		}
	} // if mx2
	else{
		ny = nx2.at(0);
	}
	
	histo->Fill(nx,ny);

  Float_t xMWPC = X0 + dX + nx*1.25;
  Float_t yMWPC = Y0 + dY + ny*1.25;

	histo_Cal->Fill(xMWPC,yMWPC);

 	xyMWPC.SetX(xMWPC);
  xyMWPC.SetY(yMWPC);

	return xyMWPC;
}

void UserProcBeamMonitoring::profileTarget(TVector3 beamVector,TVector3 xyMWPCclose,Float_t z,TH2* histo){
	Float_t xt,yt;

	xt = xyMWPCclose.X() + (z-xyMWPCclose.Z())*TMath::Tan(beamVector.Theta())*TMath::Sin(beamVector.Phi());
  yt = xyMWPCclose.Y() + (z-xyMWPCclose.Z())*TMath::Tan(beamVector.Theta())*TMath::Cos(beamVector.Phi());

  if(abs(xt)>100 || abs(yt)>100){ // checking if beam track is not crazy
  	// cout << " such beam track does not make sense " << endl;
  	return;
  }

 	histo->Fill(xt,yt);
}

Bool_t UserProcBeamMonitoring::IsCluster (TClonesArray* v_MWPC) {
  Bool_t isCluster = kTRUE;
  for(Int_t i = 0; i < v_MWPC->GetEntries() - 1; i++) {
  	DetMessage *mes_MWPC = (DetMessage*)v_MWPC->At(i);
  	DetMessage *mes_MWPC_next = (DetMessage*)v_MWPC->At(i+1);
    if(abs(mes_MWPC_next->GetStChannel() - mes_MWPC->GetStChannel()) != 1){
      isCluster = kFALSE;
      break;
    }
  }
  return isCluster;
}

void UserProcBeamMonitoring::IDdeToF(TGo4CompositeEvent* dEvent,TH2* histo){
	DetEventStation* st_F3 = (DetEventStation*)(dEvent->getEventElement(fst_F3.Data(),1));
	if(!st_F3) {
		cout << " station " << fst_F3.Data() <<  " was not found " << endl;
		return;
	}
	DetEventStation* st_F5 = (DetEventStation*)(dEvent->getEventElement(fst_F5.Data(),1));
	if(!st_F5) {
		cout << " station " << fst_F5.Data() <<  " was not found " << endl;
		return;
	}
	DetEventStation* st_tF3 = (DetEventStation*)(dEvent->getEventElement(fst_tF3.Data(),1));
	if(!st_tF3) {
		cout << " station " << fst_tF3.Data() <<  " was not found " << endl;
		return;
	}
	DetEventStation* st_tF5 = (DetEventStation*)(dEvent->getEventElement(fst_tF5.Data(),1));
	if(!st_tF5) {
		cout << " station " << fst_tF5.Data() <<  " was not found " << endl;
		return;
	}

	TClonesArray* v_F3 = st_F3->GetDetMessages();
	TClonesArray* v_F5 = st_F5->GetDetMessages();
	TClonesArray* v_tF3 = st_tF3->GetDetMessages();
	TClonesArray* v_tF5 = st_tF5->GetDetMessages();

	Int_t nF3 = v_F3->GetEntriesFast();
	Int_t ntF3 = v_tF3->GetEntriesFast();
	Int_t nF5 = v_F5->GetEntriesFast();
	Int_t ntF5 = v_tF5->GetEntriesFast();

	if(nF3<1 || ntF3<1 || nF5<1 || ntF5<1) {
		return;
	}

	Float_t av_F3 = 0.;
	for(Int_t i = 0; i < nF3; i++){
		DetMessage *mes_F3 = (DetMessage*)v_F3->At(i);
		av_F3 += mes_F3->GetValue();
	}
	av_F3 = av_F3/nF3;

	Float_t av_tF3 = 0.;
	for(Int_t i = 0; i < ntF3; i++){
		DetMessage *mes_tF3 = (DetMessage*)v_tF3->At(i);
		av_tF3 += mes_tF3->GetValue();
	}
	av_tF3 = av_tF3/ntF3;

	Float_t av_F5 = 0.;
	for(Int_t i = 0; i < nF5; i++){
		DetMessage *mes_F5 = (DetMessage*)v_F5->At(i);
		av_F5 += mes_F5->GetValue();
	}
	av_F5 = av_F5/nF5;

	Float_t av_tF5 = 0.;
	for(Int_t i = 0; i < ntF5; i++){
		DetMessage *mes_tF5 = (DetMessage*)v_tF5->At(i);
		av_tF5 += mes_tF5->GetValue();
	}
	av_tF5 = av_tF5/ntF5;

	histo->Fill(av_tF5-av_tF3, av_F3+av_F5);
}

ClassImp(UserProcBeamMonitoring)
