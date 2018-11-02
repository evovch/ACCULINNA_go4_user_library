#include "UserProcDigiBuilding.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// Project
#include "data/DetEventFull.h" // input event
#include "UserEventDigiBuilding.h" // output event
#include "UserParameter.h"
#include "setupconfigcppwrapper/SetupConfiguration.h"

/**
  Uncomment this if you want to see all the debug information.
  This allows you to analyze the raw bytes and bits by your eyes.
  This option produces A LOT OF DATA - run your analysis with a
  small number of events (~10-100)
*/
//#define DEBUGDIGIBUILDING

UserProcDigiBuilding::UserProcDigiBuilding(const char* name) :
	TGo4EventProcessor(name),
	fEventCounter(0)
{
}

UserProcDigiBuilding::~UserProcDigiBuilding()
{
}

Bool_t UserProcDigiBuilding::BuildEvent(TGo4EventElement* p_dest)
{
	Bool_t v_isValid = kFALSE;

	UserEventDigiBuilding* v_outputEvent = (UserEventDigiBuilding*)p_dest;

	DetEventFull* v_input = (DetEventFull*)GetInputEvent();
	if (v_input == NULL)
	{
		cerr << "[WARN  ] " << "UserProcDigiBuilding::BuildEvent(): no input event!" << endl;
		v_outputEvent->SetValid(v_isValid);
		return v_isValid;
	}
	v_isValid = kTRUE;

	#ifdef DEBUGDIGIBUILDING
	cerr << "[DEBUG ] " << "UserProcDigiBuilding: Event " << fEventCounter
	     << " ======================================================================================================"
	     << endl;
	#endif

	fCurrentOutputEvent = v_outputEvent;

	// Clear the output event!!!
	//TODO check that this is not done by the framework
	// Seems that indeed this is done by the framework
	//fCurrentOutputEvent->Clear();

	// --------------------------

	//TODO check
	// Get the all-accessible parameter-set object
	////UserParameter* v_params = (UserParameter*)GetParameter("UserParameter");
	////const SetupConfiguration* v_setupConfig = v_params->GetSetupConfig();

	cerr << v_input->getNElements() << endl;

	// --------------------------

	v_outputEvent->SetValid(v_isValid);

	fEventCounter++;

	return v_isValid;
}

ClassImp(UserProcDigiBuilding)
