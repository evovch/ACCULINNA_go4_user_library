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
	fHistoMan_test = new UserHistosTestMonitoring();
	readParFile("/media/user/work/data/analysisexp1804/presentPars/csi_r_ec.clb");
	// cerr << " UserProcTestMonitoring CALLED !!! ## &Y$@!UHNEFJNASJDf " << endl;
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
	//cerr << v_NsubElems << " subelements in the input full event." << endl;
	Int_t trigger;
	Int_t x1,y1;
	// Loop over sub-elements. There is one sub-element which is the 'DetEventCommon'
	// and all other are 'DetEventDetector's

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
	fHistoMan_test->fTrigger_test->Fill(trigger);

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
					if(stName.Contains("Beam_detector_MWPC")){
						fHistoMan_test->fAutoHistos_test.at(chFullId)->Fill(v_curDetM->GetStChannel());
					}
					else {
						fHistoMan_test->fAutoHistos_test.at(chFullId)->Fill(v_curDetM->GetValue());
					}

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
	ifstream myfile;
  TString line;
  Int_t count=-2;
  myfile.open("/media/user/work/data/analysisexp1804/presentPars/csi_r_ec.clb");
  while (! myfile.eof() ) {
    line.ReadLine(myfile);
    if(count < 0){
      count++;
      continue;
    }
    if(line.IsNull()) break;
    sscanf(line.Data(),"%lf %lf", parCsI_R_1+count,parCsI_R_2+count);
    count++;
  }  

  // cerr << endl << " pars for CsR crystals" << endl;
  // for(Int_t i=0;i<16;i++) cerr << parCsI_R_1[i] << " " << parCsI_R_2[i] << endl; 
}

void UserProcTestMonitoring::fill2D(TGo4CompositeEvent* dEvent){

}

ClassImp(UserProcTestMonitoring)
