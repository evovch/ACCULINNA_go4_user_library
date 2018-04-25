#include "MeshDummyEvent.h"

// Project
#include "MeshProviderProc.h"

MeshDummyEvent::MeshDummyEvent() :
	TGo4EventElement("DummyEvent"),
	fxProviderProc(0)
{
}

MeshDummyEvent::MeshDummyEvent(const char* name) :
	TGo4EventElement(name),
	fxProviderProc(0)
{
}

MeshDummyEvent::~MeshDummyEvent()
{
}

Int_t MeshDummyEvent::Init()
{
	// check for different source types
	Int_t rev = 0;
	Clear();
	if (CheckEventSource("MeshProviderProc")) {
		fxProviderProc = dynamic_cast<MeshProviderProc*>(GetEventSource());
		// std::cout << "**** MeshDummyEvent init"<< std::endl;
	} else {
		rev = 1;
	}
	return rev;
}

Int_t MeshDummyEvent::Fill()
{
	// check for different source types
	Int_t rev = 0;
	if (fxProviderProc) {
		fxProviderProc->SetRealInput();
	} else {
		rev = 1;
	}
	return rev;
}

void MeshDummyEvent::Clear(Option_t* /*t*/)
{
}
