/**

	@class MeshDummyEvent

	The only purpose for this class is to trigger the method of
	the corresponding provider processor to get the correct event
	pointer from framework.

*/

#ifndef MESHDUMMYEVENT_H
#define MESHDUMMYEVENT_H

#include <TGo4EventElement.h> // mother class

class MeshProviderProc;

class MeshDummyEvent : public TGo4EventElement
{
public:
	MeshDummyEvent();
	MeshDummyEvent(const char* name);
	virtual ~MeshDummyEvent();

	virtual Int_t Fill();
	virtual void Clear(Option_t* t = "");
	virtual Int_t Init();

private:
	MeshProviderProc* fxProviderProc; //!

	ClassDef(MeshDummyEvent, 1);
};

#endif // MESHDUMMYEVENT_H
