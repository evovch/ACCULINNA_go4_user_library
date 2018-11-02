#include "UserProcAdvMonitoring.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// ROOT
#include <TClonesArray.h>
#include <TH1D.h>
#include <TH2D.h>

// Project
#include "base/Support.h"
#include "data/DetEventFull.h" // input event
#include "data/DetEventCommon.h"
#include "data/DetEventStation.h"
#include "data/DetMessage.h"

#include "UserHistosAdvMonitoring.h"
#include "UserParameter.h"
#include "setupconfigcppwrapper/SetupConfiguration.h"

/**
  Uncomment this if you want to see all the debug information.
  This allows you to analyze the raw bytes and bits by your eyes.
  This option produces A LOT OF DATA - run your analysis with a
  small number of events (~10-100)
*/
//#define DEBUGADVMON

UserProcAdvMonitoring::UserProcAdvMonitoring(const char* name) :
	TGo4EventProcessor(name),
	fEventCounter(0),
	fSetupConfig(0)
{
	fHistoMan = new UserHistosAdvMonitoring();

	support::CheckThatDirExists("textoutput");

	fFileSummary = fopen("textoutput/summaryAdvMonitoring.txt", "w");
	if (fFileSummary == NULL) {
		//TODO warning or fatal?
		//cerr << "[WARN  ] " << "Could not open output text summary file '" << "summaryAdvMonitoring.txt" << "'" << endl;
		cerr << "[FATAL ] " << "Could not open output text summary file '" << "textoutput/summaryAdvMonitoring.txt" << "'" << endl;
		exit(EXIT_FAILURE);
	}
}

UserProcAdvMonitoring::~UserProcAdvMonitoring()
{
	if (fHistoMan) delete fHistoMan;
	if (fFileSummary != NULL) {
		fclose(fFileSummary);
	}
}

Bool_t UserProcAdvMonitoring::BuildEvent(TGo4EventElement* p_dest)
{
	Bool_t v_isValid = kFALSE;

	DetEventFull* v_input = (DetEventFull*)GetInputEvent("stepRepackedProvider1");
	if (v_input == NULL)
	{
		cerr << "[WARN  ] " << "UserProcAdvMonitoring::BuildEvent(): no input event!" << endl;
		return v_isValid;
	}
	v_isValid = kTRUE;

	#ifdef DEBUGADVMON
	cerr << "[DEBUG ] " << "UserProcAdvMonitoring: Event " << fEventCounter
	     << " ======================================================================================================"
	     << endl;
	#endif

	// --------------------------

	Short_t v_NsubElems = v_input->getNElements();
	//cerr << v_NsubElems << " subelements in the input full event." << endl;

	// Loop over sub-elements. There is one sub-element which is the 'DetEventCommon'
	// and all other are 'DetEventDetector's
	for (Short_t i=0; i<v_NsubElems; i++) {
		TGo4EventElement* v_subElement = v_input->getEventElement(i);

		TString curName = v_subElement->GetName();
		Short_t curId = v_subElement->getId();
		//cerr << curId << ") " << curName;

		if (curName == "DetEventCommon") {
			////DetEventCommon* v_commSubEl = (DetEventCommon*)(v_subElement);
			//cerr << endl;

			// Here you can process information from the 'common' sub-element

		} else {
			TGo4CompositeEvent* v_detSubEl = (TGo4CompositeEvent*)(v_subElement);

			Short_t v_NsubSubElems = v_detSubEl->getNElements();
			//cerr << " - " << v_NsubSubElems << " subsubelements." << endl;

			// Loop over the stations of the current detector
			for (Short_t j=0; j<v_NsubSubElems; j++) {

				Short_t stId = curId*100 + j; //FIXME this is quite dangerous

				DetEventStation* v_stSubsubEl = (DetEventStation*)(v_detSubEl->getEventElement(stId));
				//cerr << "\t" << stId << ") " << v_stSubsubEl->GetName() << endl;

				TClonesArray* v_data = v_stSubsubEl->GetDetMessages();

				TIter v_detMiter(v_data);
				DetMessage* v_curDetM;

				// Loop over the messages of the current station 
				while ((v_curDetM = (DetMessage*)v_detMiter.Next())) {
					//v_curDetM->Print();

					unsigned int chFullId = stId*100 + v_curDetM->GetStChannel();

					// Fill automatically generated histograms
					fHistoMan->fAutoHistos.at(chFullId)->Fill(v_curDetM->GetValue());

					//TODO implement here your actions which require processing
					// of several messages simultaneously

					//TODO Look inside
					this->ProcessMessage(v_curDetM);


				} // end of loop over messages
			} // end of loop over the stations
		} // end of if
	} // end of loop over the sub-elements (detectors)

	// --------------------------

	fEventCounter++;

	return v_isValid;
}

void UserProcAdvMonitoring::UserPreLoop()
{
	// Get the all-accessible parameter-set object
	UserParameter* v_params = (UserParameter*)GetParameter("UserParameter");
	fSetupConfig = v_params->GetSetupConfig();

	#ifdef DEBUGADVMON
	cerr << "[DEBUG ] " << "UserProcAdvMonitoring::UserPreLoop ====================================" << endl;
	#endif

	fHistoMan->GenerateAutoHistos();

	#ifdef DEBUGADVMON
	cerr << "[DEBUG ] " << "=======================================================================" << endl;
	#endif
}

void UserProcAdvMonitoring::UserPostLoop()
{
}

void UserProcAdvMonitoring::ProcessMessage(DetMessage* p_message)
{
	//TODO implement your processing of independent messages here
}

ClassImp(UserProcAdvMonitoring)
