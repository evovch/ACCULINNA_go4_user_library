#include "MeshProviderProc.h"

// Go4
#include <TGo4Log.h>
#include <TGo4Analysis.h>
#include <TGo4EventElement.h>
#include <TGo4UserException.h>

MeshProviderProc::MeshProviderProc(const char* name) :
	TGo4EventProcessor(name)
{
	TGo4Log::Info("MeshProviderProc: Create %s", name);
}

MeshProviderProc::MeshProviderProc() :
	TGo4EventProcessor("ProviderProcessor")
{
}

MeshProviderProc::~MeshProviderProc()
{
}

void MeshProviderProc::SetRealInput()
{
	TGo4EventElement* myinput = GetInputEvent();
	if (myinput==0 || (myinput != 0 && myinput->CheckEventSource("TGo4EventProcessor")))
	{
		// source of our input event is processor and not file:
		// we have to reset the pointer from framework!
		// the trick is: name of this processor (up to the "_") is name of required event
		// note that TGo4StepFactory forbids to use same name for different objects,
		// since object name is used as pointer name in the processline call
		TString eventname = GetName();
		eventname.Resize(eventname.Index("_", 1));
		//std::cout << "SetRealInput of" << GetName()
		//          << " sees eventname " << eventname.Data() << std::endl;
		myinput = TGo4Analysis::Instance()->GetEventStructure(eventname.Data());
		if (myinput==0) {
			throw TGo4UserException(3, "Error setting real input event %s", GetName());
		} else {
			SetInputEvent(myinput);
		}
	}
	//else if(myinput!=0 && myinput->CheckEventSource("TGo4FileSource"))
	//   {
	//      // this provider reads from file. ok
	//
	//   }
	//else
	//   {
	//      // neither processor or file input: something is very wrong
	//      throw TGo4UserException(3,"No source found for provider %s", GetName());
	//   }
	// we skip the last checks for performance reasons...
}
