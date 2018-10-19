#include "UserProcTestMonitoring.h"

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
#include "useranalysis/calibration/SiCalibPars.h"

#include "UserHistosTestMonitoring.h"
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
//#define DEBUGTestMON

UserProcTestMonitoring::UserProcTestMonitoring(const char* name) :
	TGo4EventProcessor(name),
	fEventCounter(0)
{	
	// Creating and filling TGo4Parameter objects
	this->InitPars();

	fHistoMan_test = new UserHistosTestMonitoring();
	fFileSummary = fopen("textoutput/summaryTestMonitoring.txt", "w");
	if (fFileSummary == NULL) {
		//TODO error
		cerr << "[WARN  ] " << "Could not open output text summary file '" << "summaryTestMonitoring.txt" << "'" << endl;
	}
}

UserProcTestMonitoring::~UserProcTestMonitoring()
{
	if (fHistoMan_test) delete fHistoMan_test;
	if (fFileSummary != NULL) {
		fclose(fFileSummary);
	}
}

Bool_t UserProcTestMonitoring::BuildEvent(TGo4EventElement* p_dest)
{
	// cerr << "\t ### UserProcTestMonitoring::BuildEvent was called ### " <<  endl;

	Bool_t v_isValid = kFALSE;

	DetEventFull* v_input = (DetEventFull*)GetInputEvent("stepRepackedProvider1");
	if (v_input == NULL)
	{
		cerr << "[WARN  ] " << "UserProcTestMonitoring::BuildEvent(): no input event!" << endl;
		return v_isValid;
	}
	v_isValid = kTRUE;

	#ifdef DEBUGTestMON
	cerr << "[DEBUG ] " << "UserProcTestMonitoring: Event " << fEventCounter
	     << " ======================================================================================================"
	     << endl;
	#endif

	// --------------------------

	Short_t v_NsubElems = v_input->getNElements();
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
	if(trigger>5) {
		cout << " Event wont be processed " << endl;
		return kFALSE;
	}
	fHistoMan->fTrigger->Fill(trigger);
	for (Short_t i=0; i<v_NsubElems; i++) {
		TGo4EventElement* v_subElement = v_input->getEventElement(i);

		TString curName = v_subElement->GetName();
		Short_t curId = v_subElement->getId();
		// cerr << curId << ") " << curName << " this is it!! " << endl;
		if (curName != "DetEventCommon") {
			TGo4CompositeEvent* v_detSubEl = (TGo4CompositeEvent*)(v_subElement);

			Short_t v_NsubSubElems = v_detSubEl->getNElements();

			// Loop over the stations of the current detector
			for (Short_t j=0; j<v_NsubSubElems; j++) {

				Short_t stId = curId*100 + j; //FIXME this is quite dangerous

				DetEventStation* v_stSubsubEl = (DetEventStation*)(v_detSubEl->getEventElement(stId));
				TString stName = v_stSubsubEl->GetName();

				TClonesArray* v_data = v_stSubsubEl->GetDetMessages();

				TIter v_detMiter(v_data);
				DetMessage* v_curDetM;

				// Loop over the messages of the current station 
				while ((v_curDetM = (DetMessage*)v_detMiter.Next())) {
					//v_curDetM->Print();

					unsigned int chFullId = stId*100 + v_curDetM->GetStChannel();

					// Fill automatically generated histograms
					// if(stName.Contains("Beam_detector_MWPC")){
					// 	fHistoMan_test->fAutoHistos_test.at(chFullId)->Fill(v_curDetM->GetStChannel());
					// }
					// else {
					// 	fHistoMan_test->fAutoHistos_test.at(chFullId)->Fill(v_curDetM->GetValue());
					// }

					//TODO implement here your actions which require processing
					// of several messages simultaneously

					//TODO Look inside
					// this->ProcessMessage(v_curDetM,stName);
				} // end of loop over messages
			} // end of loop over the stations
		} // end of if
	} // end of loop over the sub-elements (detectors)

	// --------------------------

	fEventCounter++;

	return v_isValid;
}

void UserProcTestMonitoring::UserPreLoop()
{
	// Get the all-accessible parameter-set object
	UserParameter* v_params = (UserParameter*)GetParameter("UserParameter");
	fSetupConfig = v_params->GetSetupConfig();

	#ifdef DEBUGTestMON
	cerr << "[DEBUG ] " << "UserProcTestMonitoring::UserPreLoop ====================================" << endl;
	#endif

	fHistoMan_test->GenerateAutoHistos();

	#ifdef DEBUGTestMON
	cerr << "[DEBUG ] " << "=======================================================================" << endl;
	#endif
}

void UserProcTestMonitoring::UserPostLoop()
{
}

void UserProcTestMonitoring::ProcessMessage(DetMessage* p_message, TString stName)
{
	//TODO implement your processing of independent messages here

}

void UserProcTestMonitoring::readParFile(TString parFile){
}

void UserProcTestMonitoring::fill2D(TGo4CompositeEvent* dEvent){

}

void UserProcTestMonitoring::InitPars() {
	fParSi = new SiCalibPars* [fnPars]; // make this another way
	// TODO : the loop over the whole map
  // std::map <TString,Int_t> stMap = { { "SQX_L", 32 },
		//                                  { "SQY_L", 16 },///map явно инициализирована
		//                                  { "SQX_R", 32 },
		//                                  { "SQY_R", 16 },
		//                                	 { "SQ20", 16	} };
	// TString stName; 
	// Int_t nChannels; 		                               	 
  // for(auto it = stMap.begin(); it != stMap.end(); ++it) {
  // 	// cout << it->first << " " << it->second << endl;
  // 	stName = (TString)it->first;
  // 	nChannels = (Int_t)it->second;
  // 	fParSi[i] = (SiCalibPars*) MakeParameter(stName.Data(), "SiCalibPars");
  // 	  	// cout << " no crash yet " << endl;
  // 	fParSi[i]->Init(nChannels,stName);
  // 	i++;
  // }	

  std::pair <TString,Int_t>* stPair = new std::pair <TString,Int_t>[fnPars];
	stPair[0] = make_pair((TString)"SQX_L",32);
	stPair[1] = make_pair((TString)"SQY_L",16);
	stPair[2] = make_pair((TString)"SQX_R",32);
	stPair[3] = make_pair((TString)"SQY_R",16);	
	stPair[4] = make_pair((TString)"SQ300",16);				                               
	for(Int_t i=0; i<5; i++) {
		fParSi[i] = (SiCalibPars*) MakeParameter(stPair[i].first, "SiCalibPars");
		fParSi[i]->Init(stPair[i].second,stPair[i].first);
	}

}
ClassImp(UserProcTestMonitoring)
