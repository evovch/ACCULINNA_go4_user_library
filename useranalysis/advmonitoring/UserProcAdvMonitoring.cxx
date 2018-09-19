#include "UserProcAdvMonitoring.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// Project
#include "unpacking/UserEventUnpacking.h" // input event
#include "UserHistosAdvMonitoring.h"

/**
  Uncomment this if you want to see all the debug information.
  This allows you to analyze the raw bytes and bits by your eyes.
  This option produces A LOT OF DATA - run your analysis with a
  small number of events (~10-100)
*/
//#define DEBUGADVMON

UserProcAdvMonitoring::UserProcAdvMonitoring(const char* name) :
	TGo4EventProcessor(name),
	fEventCounter(0)
{
	fHistoMan = new UserHistosAdvMonitoring();
	fFileSummary = fopen("textoutput/summaryAdvMonitoring.txt", "w");
	if (fFileSummary == NULL) {
		//TODO error
		cerr << "[WARN  ] " << "Could not open output text summary file '" << "summaryAdvMonitoring.txt" << "'" << endl;
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

	UserEventUnpacking* v_input = (UserEventUnpacking*)GetInputEvent("stepUnpackedProvider1");
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

	fEventCounter++;

	return v_isValid;
}

ClassImp(UserProcAdvMonitoring)
