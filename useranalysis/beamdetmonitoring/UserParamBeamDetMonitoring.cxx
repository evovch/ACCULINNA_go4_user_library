#include "UserParamBeamDetMonitoring.h"

#include <iostream>
using std::cerr;
using std::endl;

UserParamBeamDetMonitoring::UserParamBeamDetMonitoring(const char* name) :
	TGo4Parameter(name),
	fBeamDetName("Beam_detector_MWPC"),
	fMWPCx1Name("MWPC1"),
	fMWPCy1Name("MWPC2"),
	fMWPCx2Name("MWPC3"),
	fMWPCy2Name("MWPC4")
{
}

UserParamBeamDetMonitoring::~UserParamBeamDetMonitoring()
{
}

Int_t UserParamBeamDetMonitoring::PrintParameter(Text_t* buffer, Int_t buflen)
{
	cerr << endl;
	cerr << "UserParamBeamDetMonitoring object:" << endl;
	cerr << "==================================" << endl;
	cerr << "fBeamDetName = " << fBeamDetName << endl;
	cerr << "fMWPCx1Name = " << fMWPCx1Name << endl;
	cerr << "fMWPCy1Name = " << fMWPCy1Name << endl;
	cerr << "fMWPCx2Name = " << fMWPCx2Name << endl;
	cerr << "fMWPCy2Name = " << fMWPCy2Name << endl;
	cerr << "==================================" << endl;

	return 1; //TODO what should it return?
}

ClassImp(UserParamBeamDetMonitoring)
