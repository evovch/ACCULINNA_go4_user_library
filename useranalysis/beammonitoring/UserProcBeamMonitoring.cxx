
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
	TGo4EventProcessor(name),
	fEventCounter(0),
	fTrigger(1),
	fMWPC1_X_displacement(-1.), 
	fMWPC1_Y_displacement(-2.1375), 
	fMWPC2_X_displacement(0.2), 
	fMWPC2_Y_displacement(-1.125), 
	fMWPC1_X_zero_position(-15.5*1.25),
	fMWPC1_Y_zero_position(-15.5*1.25),
	fMWPC2_X_zero_position(-15.5*1.25),
	fMWPC2_Y_zero_position(-15.5*1.25),
  fst_MWPC1("Beam_detector_MWPC1"),
	fst_MWPC2("Beam_detector_MWPC2"),
	fst_MWPC3("Beam_detector_MWPC3"),
	fst_MWPC4("Beam_detector_MWPC4")
{
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
	// cerr << "\t ### UserProcBeamMonitoring::BuildEvent was called ### " <<  endl;

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

	// Short_t v_NsubElems = v_input->getNElements();
	//cerr << v_NsubElems << " subelements in the input full event." << endl;
	UInt_t trigger;
	// Loop over sub-elements. There is one sub-element which is the 'DetEventCommon'
	// and all other are 'DetEventDetector's

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

	Short_t curId = v_subElement->getId();
	// cerr << curId << ") " << curName << " this is it!! " << endl;
	TGo4CompositeEvent* dEvent = (TGo4CompositeEvent*)(v_subElement);
	fill2D(dEvent);

	Short_t v_NsubSubElems = dEvent->getNElements();

	// Loop over the stations of the current detector
	for (Short_t j=0; j<v_NsubSubElems; j++) {

		Short_t stId = curId*100 + j; //FIXME this is quite dangerous

		DetEventStation* v_stSubsubEl = (DetEventStation*)(dEvent->getEventElement(stId));
		TString stName = v_stSubsubEl->GetName();

		TClonesArray* v_data = v_stSubsubEl->GetDetMessages();

		TIter v_detMiter(v_data);
		DetMessage* v_curDetM;

		// Loop over the messages of the current station 
		while ((v_curDetM = (DetMessage*)v_detMiter.Next())) {
			//v_curDetM->Print();

			unsigned int chFullId = stId*100 + v_curDetM->GetStChannel();

			// Fill automatically generated histograms
			if(stName.Contains("Beam_detector_MWPC")){
				fHistoMan->fAutoHistos_Beam.at(chFullId)->Fill(v_curDetM->GetStChannel());
			}
			else {
				fHistoMan->fAutoHistos_Beam.at(chFullId)->Fill(v_curDetM->GetValue());
			}

			//TODO implement here your actions which require processing
			// of several messages simultaneously

			//TODO Look inside
			// this->ProcessMessage(v_curDetM,stName);
		} // end of loop over messages
	} // end of loop over the stations

	// --------------------------

	fEventCounter++;

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

	fHistoMan->GenerateAutoHistos();

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
	profileMWPC(dEvent,fst_MWPC1,fst_MWPC2,fHistoMan->fY1_X1,fHistoMan->fY1_X1_C,fMWPC1_X_zero_position,fMWPC1_X_displacement,fMWPC1_Y_zero_position,fMWPC1_Y_displacement);
	profileMWPC(dEvent,fst_MWPC3,fst_MWPC4,fHistoMan->fY2_X2,fHistoMan->fY2_X2_C,fMWPC2_X_zero_position,fMWPC2_X_displacement,fMWPC2_Y_zero_position,fMWPC2_Y_displacement);
}


void UserProcBeamMonitoring::profileMWPC(TGo4CompositeEvent* dEvent,TString st_Name1,TString st_Name2,TH2* histo,TH2* histo_Cal,Float_t X0,Float_t dX,Float_t Y0,Float_t dY){
	vector <Int_t> nx1;
	vector <Int_t> nx2;

	DetEventStation* st_MWPC1 = (DetEventStation*)(dEvent->getEventElement(st_Name1.Data(),1));
	if(!st_MWPC1) {
		cout << " station " << st_Name1.Data() <<  " was not found " << endl;
		return;
	}
	DetEventStation* st_MWPC2 = (DetEventStation*)(dEvent->getEventElement(st_Name2.Data(),1));
	if(!st_MWPC2) {
		cout << " station " << st_Name2.Data() <<  " was not found " << endl;
		return;
	}

	TClonesArray* v_MWPC1 = st_MWPC1->GetDetMessages();
	TClonesArray* v_MWPC2 = st_MWPC2->GetDetMessages();

	Int_t mx1 = v_MWPC1->GetEntriesFast();
	Int_t mx2 = v_MWPC2->GetEntriesFast();
	if(mx1<1 || mx2<1) {
		return;
	}
	for(Int_t i = 0; i < mx1; i++){
		DetMessage *mes_MWPC1 = (DetMessage*)v_MWPC1->At(i);
		nx1.push_back(mes_MWPC1->GetStChannel());
	}

	for(Int_t i = 0; i < mx2; i++){
		DetMessage *mes_MWPC2 = (DetMessage*)v_MWPC2->At(i);
		nx2.push_back(mes_MWPC2->GetStChannel());
	}

	histo->Fill(nx1.at(0),nx2.at(0));

  Float_t xMWPC1 = fMWPC1_X_zero_position + fMWPC1_X_displacement + nx1.at(0)*1.25;
  Float_t yMWPC1 = fMWPC1_Y_zero_position + fMWPC1_Y_displacement + nx2.at(0)*1.25;

	histo_Cal->Fill(xMWPC1,yMWPC1);
}

ClassImp(UserProcBeamMonitoring)
