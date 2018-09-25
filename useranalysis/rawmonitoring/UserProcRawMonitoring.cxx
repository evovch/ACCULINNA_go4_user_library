#include "UserProcRawMonitoring.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// ROOT
#include <TClonesArray.h> // for TIter
#include <TH1D.h>
#include <TH2D.h>

// Project
#include "unpacking/UserEventUnpacking.h" // input event
#include "data/RawMessage.h"
#include "UserHistosRawMonitoring.h"

/**
  Uncomment this if you want to see all the debug information.
  This allows you to analyze the raw bytes and bits by your eyes.
  This option produces A LOT OF DATA - run your analysis with a
  small number of events (~10-100)
*/
//#define DEBUGRAWMON

UserProcRawMonitoring::UserProcRawMonitoring(const char* name) :
	TGo4EventProcessor(name),
	fEventCounter(0)
{
	fHistoMan = new UserHistosRawMonitoring();
	fFileSummary = fopen("textoutput/summaryRawMonitoring.txt", "w");
	if (fFileSummary == NULL) {
		//TODO error
		cerr << "[WARN  ] " << "Could not open output text summary file '" << "summaryRawMonitoring.txt" << "'" << endl;
	}
}

UserProcRawMonitoring::~UserProcRawMonitoring()
{
	if (fHistoMan) delete fHistoMan;
	if (fFileSummary != NULL) {
		fclose(fFileSummary);
	}
}

Bool_t UserProcRawMonitoring::BuildEvent(TGo4EventElement* p_dest)
{
	Bool_t v_isValid = kFALSE;

	UserEventUnpacking* v_input = (UserEventUnpacking*)GetInputEvent("stepUnpackedProvider1");
	if (v_input == NULL)
	{
		cerr << "[WARN  ] " << "UserProcRawMonitoring::BuildEvent(): no input event!" << endl;
		return v_isValid;
	}
	v_isValid = kTRUE;

	#ifdef DEBUGRAWMON
	cerr << "[DEBUG ] " << "UserProcRawMonitoring: Event " << fEventCounter
	     << " ======================================================================================================"
	     << endl;
	#endif

	// --------------------------

	UInt_t v_messCounter = 0;
	TIter next(v_input->fRawMessages);
	while (RawMessage* v_curMessage = (RawMessage*)next())
	{

		#ifdef DEBUGRAWMON
		cerr << v_messCounter << ": ";
		v_curMessage->Dump(false);
		cerr << endl;
		#endif

		this->ProcessMessage(v_curMessage);

		v_messCounter++;
	} // end of while

	// --------------------------

	fEventCounter++;

	return v_isValid;
}

void UserProcRawMonitoring::UserPreLoop()
{
}

void UserProcRawMonitoring::UserPostLoop()
{
}

void UserProcRawMonitoring::ProcessMessage(const RawMessage* p_message)
{

	fHistoMan->mhEvType->Fill(p_message->fEventType);
	fHistoMan->mhEvSubType->Fill(p_message->fEventSubtype);
	fHistoMan->mhEvDummy->Fill(p_message->fEventDummy);
	fHistoMan->mhEvTrigger->Fill(p_message->fEventTrigger);
	fHistoMan->mhEvCount->Fill(p_message->fEventCount);
	fHistoMan->mhSubEvDlen->Fill(p_message->fSubeventDlen);
	fHistoMan->mhSubEvType->Fill(p_message->fSubeventType);
	fHistoMan->mhSubEvSubcrate->Fill(p_message->fSubeventSubcrate);
	fHistoMan->mhSubEvControl->Fill(p_message->fSubeventControl);
	fHistoMan->mhSubEvFullID->Fill(p_message->fSubeventFullID);
	fHistoMan->mhSubEvProcID->Fill(p_message->fSubeventProcID);
	fHistoMan->mhSubsubEvVendor->Fill(p_message->fSubsubeventVendor);
	fHistoMan->mhSubsubEvGeo->Fill(p_message->fSubsubeventGeo);
	fHistoMan->mhSubsubEvModule->Fill(p_message->fSubsubeventModule);
	fHistoMan->mhChannel->Fill(p_message->fChannel);
	fHistoMan->mhValueQA->Fill(p_message->fValueQA);
	fHistoMan->mhValueT->Fill(p_message->fValueT);
	fHistoMan->mhMessageIndex->Fill(p_message->fMessageIndex);
	fHistoMan->mhSubsubEvFooterCntr->Fill(p_message->fSubsubeventFooterCounter);
}

ClassImp(UserProcRawMonitoring)
