#include "RawMessage.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// Project
#include "Support.h"

//TODO see comment below
RawMessage::RawMessage() :
	TObject(),
	fRawWord(0), // Yes zero here, because we want to clear the raw word with all zeros
	fEventType(-1),
	fEventSubtype(-1),
	fEventDummy(-1),
	fEventTrigger(-1),
	fEventCount(-1),
	fSubeventDlen(-1),
	fSubeventType(-1),
	fSubeventSubcrate(-1),
	fSubeventControl(-1),
	fSubeventFullID(-1),
	fSubeventProcID(-1),
	fSubsubeventVendor(-1),
	fSubsubeventGeo(-1),
	fSubsubeventModule(-1),
	fChannel(-1),
	fValueQA(-1),
	fValueT(-1),
	fMessageIndex(-1),
	fSubsubeventFooterCounter(-1)
{
}

//TODO this copy constructor has to be tediously written correctly
// with all the data members copying. Cause if not - you will have empty
// data fields in the output objects even if you think that you have filled them
// during the unpacking stage.
// Moreover, you will have garbage if you do not zero all the members in the constructor.
RawMessage::RawMessage(const RawMessage &obj) :
	TObject(),
	fRawWord(obj.fRawWord),
	fEventType(obj.fEventType),
	fEventSubtype(obj.fEventSubtype),
	fEventDummy(obj.fEventDummy),
	fEventTrigger(obj.fEventTrigger),
	fEventCount(obj.fEventCount),
	fSubeventDlen(obj.fSubeventDlen),
	fSubeventType(obj.fSubeventType),
	fSubeventSubcrate(obj.fSubeventSubcrate),
	fSubeventControl(obj.fSubeventControl),
	fSubeventFullID(obj.fSubeventFullID),
	fSubeventProcID(obj.fSubeventProcID),
	fSubsubeventVendor(obj.fSubsubeventVendor),
	fSubsubeventGeo(obj.fSubsubeventGeo),
	fSubsubeventModule(obj.fSubsubeventModule),
	fChannel(obj.fChannel),
	fValueQA(obj.fValueQA),
	fValueT(obj.fValueT),
	fMessageIndex(obj.fMessageIndex),
	fSubsubeventFooterCounter(obj.fSubsubeventFooterCounter)
{
}

RawMessage::~RawMessage()
{
}

void RawMessage::Dump(bool p_printEndl) const
{
	cerr << "Raw message:" << "\t"
	     << support::GetHexRepresentation(sizeof(Int_t), &fRawWord) << "\t"
	     << support::VendorAsString((support::enu_VENDOR)fSubsubeventVendor) << "\t" //TODO hack cast!!!
	     << "geo=" << fSubsubeventGeo << "\t"
	     << "module=" << fSubsubeventModule << "\t"
	     << "ch=" << fChannel << "\t"
	     << "valQA=" << fValueQA << "\t"
	     << "valT=" << fValueT << "\t"
	     << "pos=" << fMessageIndex;
	if (p_printEndl) cerr << endl;
}

void RawMessage::ExtDump(bool p_printEndl) const
{
	cerr << "Raw message:" << endl
	     << "\traw=" << support::GetHexRepresentation(sizeof(Int_t), &fRawWord) << endl
	     << "\tev_type=" << fEventType << endl
	     << "\tev_subtype=" << fEventSubtype << endl
	     << "\tev_dummy=" << fEventDummy << endl
	     << "\tev_trigger=" << fEventTrigger << endl
	     << "\tev_count=" << fEventCount << endl
	     << "\tsubev_dlen=" << fSubeventDlen << endl
	     << "\tsubev_type=" << fSubeventType << endl
	     << "\tsubev_subcrate=" << (Int_t)fSubeventSubcrate << endl
	     << "\tsubev_control=" << (Int_t)fSubeventControl << endl
	     << "\tsubev_fullID=" << fSubeventFullID << endl
	     << "\tsubev_procID=" << fSubeventProcID << endl
	     << "\tsubev_vendor=" << support::VendorAsString((support::enu_VENDOR)fSubsubeventVendor) << endl
	     << "\tsubev_geo=" << fSubsubeventGeo << endl
	     << "\tsubev_module=" << fSubsubeventModule << endl
	     << "\tchannel=" << fChannel << endl
	     << "\tvalueQA=" << fValueQA << endl
	     << "\tvalueT=" << fValueT << endl
	     << "\tpos=" << fMessageIndex << endl
	     << "\tfooter_counter=" << fSubsubeventFooterCounter << endl;
	if (p_printEndl) cerr << endl;
}

ClassImp(RawMessage)
