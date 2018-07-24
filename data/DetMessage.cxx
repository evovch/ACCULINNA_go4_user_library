#include "DetMessage.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

DetMessage::DetMessage() :
	TObject(),
	fDetector(-1),
	fStation(-1),
	fStChannel(-1),
	fValue(-1)
{
}

DetMessage::DetMessage(Short_t p_det, Short_t p_stat, Int_t p_stch, Int_t p_val) :
	TObject(),
	fDetector(p_det),
	fStation(p_stat),
	fStChannel(p_stch),
	fValue(p_val)
{
}

DetMessage::~DetMessage()
{
}

void DetMessage::Clear(Option_t* option)
{
	//cerr << "DetMessage::Clear" << endl;
	fDetector = -1;
	fStation = -1;
	fStChannel = -1;
	fValue = -1;
}

void DetMessage::Print(Option_t* option) const
{
	cerr << "DetMessage:"
	     << "\tdet=" << fDetector
	     << "\tst=" << fStation
	     << "\tch=" << fStChannel
	     << "\tval=" << fValue
	     << endl;
}

ClassImp(DetMessage)
