/**

	@class MeshProviderProc

*/

#ifndef MESHPROVIDERPROC_H
#define MESHPROVIDERPROC_H

#include <TGo4EventProcessor.h> // mother class

class MeshProviderProc : public TGo4EventProcessor
{
public:
	MeshProviderProc();
	MeshProviderProc(const char* name);
	virtual ~MeshProviderProc() ;

	/**
	 * This method will check if input event is correct and
	 * might reset the input event pointer for the subsequent steps
	 */
	void SetRealInput();

	ClassDef(MeshProviderProc, 1);
};

#endif //MESHPROVIDERPROC_H
