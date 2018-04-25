#include "UserProcLearn.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// ROOT
#include <TClonesArray.h>
#include <TH2D.h>

// Go4
#include <TGo4Analysis.h>

// Project
#include "UserEventLearn.h"
#include "UserParameter.h"
#include "data/RawMessage.h"
#include "unpacking/UserEventUnpacking.h"
#include "setupconfigcppwrapper/SetupConfiguration.h"

/**
  Uncomment this if you want to see all the debug information.
  This allows you to analyze the raw bytes and bits by your eyes.
  This option produces A LOT OF DATA - run your analysis with a
  small number of events (~10-100)
*/
//#define PRINTDEBUGINFO

UserProcLearn::UserProcLearn(const char* name) :
	TGo4EventProcessor(name),
	fEventCounter(0)
{
	TGo4Analysis* a = TGo4Analysis::Instance();

	const char* procids[3] = {"100", "101", "200"};
	fHistoAddrVsProcid = a->MakeTH2('D', "fHistoAddrVsProcid", "addr vs. procid;procid;addr",
	                                3, 0., 3., 32, 0., 32.);

	fFileSummary = fopen("textoutput/summaryLearn.txt", "w");
	if (fFileSummary == NULL) {
		//TODO error
		cerr << "[WARN  ] " << "Could not open output text summary file '" << "summaryLearn.txt" << "'" << endl;
	}
}

UserProcLearn::~UserProcLearn()
{
	if (fFileSummary != NULL) {
		fclose(fFileSummary);
	}
}

Bool_t UserProcLearn::BuildEvent(TGo4EventElement* p_dest)
{
	Bool_t v_isValid = kFALSE;
	UserEventLearn* v_outputEvent = (UserEventLearn*)p_dest;

	UserEventUnpacking* v_input = (UserEventUnpacking*)GetInputEvent("stepUnpackedProvider2");
	if (v_input == NULL)
	{
		cerr << "UserProcLearn::BuildEvent(): no input event!" << endl;
		v_outputEvent->SetValid(v_isValid);
		return v_isValid;
	}
	v_isValid = kTRUE;

	#ifdef PRINTDEBUGINFO
	cerr << "[DEBUG ] " << "UserProcLearn: Event " << fEventCounter
	     << " ==========================================================================================================="
	     << endl;
	#endif

	fCurrentOutputEvent = v_outputEvent;

	// Clear the output event!!!
	//TODO check that this is not done by the framework
	// Seems that indeed this is done by the framework
	//mCurrentOutputEvent->Clear();

	//TODO do the processing here

	UInt_t v_messCounter = 0;
	TIter next(v_input->fRawMessages);
	while (RawMessage* v_curMessage = (RawMessage*)next())
	{

		#ifdef PRINTDEBUGINFO
		cerr << v_messCounter << ": ";
		v_curMessage->Dump(false);
		cerr << endl;
		#endif

		this->ProcessMessage(v_curMessage);

		v_messCounter++;
	} // end of while

	// --------------------------

	v_outputEvent->SetValid(v_isValid);

	fEventCounter++;

	return v_isValid;
}

void UserProcLearn::ProcessMessage(const RawMessage* p_message)
{
	UShort_t v_addr = 999;
	if (p_message->fSubsubeventGeo == -1 && p_message->fSubsubeventModule != -1) {
		v_addr = p_message->fSubsubeventModule;
	} else if (p_message->fSubsubeventGeo != -1 && p_message->fSubsubeventModule == -1) {
		v_addr = p_message->fSubsubeventGeo;
	} else {
		//ERROR
	}

	unsigned int curChUID = SetupConfiguration::GetChUID(p_message->fSubeventProcID, v_addr,
		                                             p_message->fChannel);

	// Treat messages from scalers diffrently

	if (fUsedChUIDs.find(curChUID) == fUsedChUIDs.end()) {
		fUsedChUIDs.insert(curChUID);
		////cerr << "Adding " << curChUID << endl;
	}

	Short_t v_prodidBin = -1;
	switch(p_message->fSubeventProcID) {
		case 100: v_prodidBin = 0; break;
		case 101: v_prodidBin = 1; break;
		case 200: v_prodidBin = 2; break;
	}

	fHistoAddrVsProcid->Fill(v_prodidBin, v_addr);


}

void UserProcLearn::UserPreLoop()
{
}

void UserProcLearn::UserPostLoop()
{
	//TODO check
	// Get the all-accessible parameter-set object
	UserParameter* v_params = (UserParameter*)GetParameter("UserParameter");
	const SetupConfiguration* v_setupConfig = v_params->GetSetupConfig();

	TString v_detector;
	TString v_folder;

	fprintf(fFileSummary, "========================== SUMMARY ==========================\n");
	fprintf(fFileSummary, "The following channels have been detected in the input file:\n");

	for (auto v_chuid : fUsedChUIDs) {
		unsigned short v_procid = v_chuid/100000;
		unsigned short v_addr = (v_chuid%100000) / 1000;
		unsigned short v_ch = v_chuid%1000;
		unsigned short v_det_ch = v_setupConfig->GetOutput(v_procid, v_addr, v_ch, &v_detector, &v_folder);

		fprintf(fFileSummary, "%ld: procid=%u\taddr=%u\tch=%u\tmapped to %s[%u]\tfrom %s\n",
		        v_chuid, v_procid, v_addr, v_ch, v_detector.Data(), v_det_ch, v_folder.Data());

		/*cerr << v_chuid << ": procid=" << v_procid
		     << "\taddr=" << v_addr
		     << "\tch=" << v_ch
		     << "\tmapped to " << v_detector << "[" << v_det_ch << "]"
		     << "\t from " << v_folder
		     << endl;*/
	}

	fprintf(fFileSummary, "=============================================================\n");
}

ClassImp(UserProcLearn)
