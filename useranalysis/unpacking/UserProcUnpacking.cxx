#include "UserProcUnpacking.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// ROOT
#include <TClonesArray.h>

// Go4
#include <TGo4EventElement.h>
#include <TGo4MbsEvent.h>
#include <TGo4MbsSubEvent.h>

// Project
////#include "base/Support.h" // included in the header
#include "UserEventUnpacking.h" // output event
#include "UserHistosUnpacking.h"

/*static*/
RawMessage UserProcUnpacking::fCurMessage;

/*static*/
bool UserProcUnpacking::fInsidePackage = false;

/**
  Uncomment this if you want to see all the debug information.
  This allows you to analyze the raw bytes and bits by your eyes.
  This option produces A LOT OF DATA - run your analysis with a
  small number of events (~10-100)
*/
//#define DEBUGUNPACKING

//TODO check that all necessary data members are reset
#define DORESET

UserProcUnpacking::UserProcUnpacking(const char* name) :
	TGo4EventProcessor(name),
	fEventCounter(0),
	fSubEventCounter(0),
	fHeadersWords(0),
	fNknownWords(0),
	fNunknownWords(0)
{
	fHistoMan = new UserHistosUnpacking();

	support::CheckThatDirExists("textoutput");

	fFileSummary = fopen("textoutput/summaryUnpacking.txt", "w");
	if (fFileSummary == NULL) {
		//TODO warning or fatal?
		//cerr << "[WARN  ] " << "Could not open output text summary file '" << "summaryUnpacking.txt" << "'" << endl;
		cerr << "[FATAL ] " << "Could not open output text summary file '" << "textoutput/summaryUnpacking.txt" << "'" << endl;
		exit(EXIT_FAILURE);
	}
}

UserProcUnpacking::~UserProcUnpacking()
{
	if (fHistoMan) delete fHistoMan;
	if (fFileSummary != NULL) {
		fclose(fFileSummary);
	}
}

Bool_t UserProcUnpacking::BuildEvent(TGo4EventElement* p_dest)
{
	Bool_t v_isValid = kFALSE;
	UserEventUnpacking* v_outputEvent = (UserEventUnpacking*)p_dest;

	TGo4MbsEvent* v_input = (TGo4MbsEvent*)GetInputEvent();
	if (v_input == NULL) {
		cerr << "[WARN  ] " << "UserProcUnpacking::BuildEvent(): no input event!" << endl;
		v_outputEvent->SetValid(v_isValid);
		return v_isValid;
	}
	// Due to some unknown reason the first event in the file seems to be always corrupted.
	// So we just skip it.
	if (fEventCounter == 0) {
		cerr << "[INFO  ] " << "UserProcUnpacking::BuildEvent(): skipping first input event." << endl;
		fEventCounter++;
		v_outputEvent->SetValid(v_isValid);
		return v_isValid;
	}
	
	v_isValid = kTRUE;

	#ifdef DEBUGUNPACKING
	cerr << "[DEBUG ] " << "UserProcUnpacking: Event " << fEventCounter
	     << " ==========================================================================================================="
	     << endl;
	#endif

	fCurrentOutputEvent = v_outputEvent;

	// Clear the output event!!!
	//TODO check that this is not done by the framework
	// Seems that indeed this is done by the framework
	//mCurrentOutputEvent->Clear();

	//TODO do the processing here

	this->ProcessEventHeader(v_input);

	// Set the internal iterator of the subevent array to the beginning of the array.
	// I have no idea why do we have to do this...
	v_input->ResetIterator();

	TGo4MbsSubEvent* v_pSubevent; // Current subevent
	UInt_t v_subEventCounter = 0; // Counter of subevents within the current event
	while ((v_pSubevent = v_input->NextSubEvent()) != NULL)
	{
		#ifdef DEBUGUNPACKING
		cerr << "[DEBUG ] " << "UserProcUnpacking: SubEvent " << v_subEventCounter << " (global subevent counter = " << fSubEventCounter << ")"
		     << " -----------------------------------------------------------------" << endl;
		#endif

		this->ProcessSubevent(v_pSubevent);

		v_subEventCounter++; // local subevent counter - within current event
		fSubEventCounter++; // global subevent counter - total
	}

	v_outputEvent->SetValid(v_isValid);

	if (fEventCounter % 10000 == 0) {
		cerr << "[INFO  ] " << "End of event " << fEventCounter << ".\t"
		     << "Total subevents: " << fSubEventCounter << ".\t"
		     << "Headers' words: " << fHeadersWords << ",\t"
		     << "known words: " << fNknownWords << ",\t"
		     << "unknown words: " << fNunknownWords << ",\t"
		     << "total: " << (fHeadersWords+fNknownWords+fNunknownWords)*sizeof(Int_t) << " bytes."
		     << endl;
	 }

	if (fFileSummary != NULL) {
		if (fEventCounter % 10000 == 0) {
			fprintf(fFileSummary,
			        "End of event %ld.\tTotal subevents: %ld.\tHeaders' words: %ld,\tknown words: %ld,\tunknown words: %ld,\ttotal: %ld bytes.\n",
			        fEventCounter, fSubEventCounter, fHeadersWords, fNknownWords, fNunknownWords,
			        (fHeadersWords+fNknownWords+fNunknownWords)*sizeof(Int_t));
		}
	}

	this->FinishEvent();

	fEventCounter++;

	return v_isValid;
}

void UserProcUnpacking::ProcessEventHeader(TGo4MbsEvent* p_event)
{
	fCurMessage.fEventType = p_event->GetType();
	fCurMessage.fEventSubtype = p_event->GetSubtype();
	fCurMessage.fEventDummy = p_event->GetDummy();
	fCurMessage.fEventTrigger = p_event->GetTrigger();
	fCurMessage.fEventCount = p_event->GetCount();
	fCurrentOutputEvent->fTrigger = p_event->GetTrigger();

	#ifdef DEBUGUNPACKING
	cerr << "[DEBUG ] Event header:" << "\t"
	     << "type="    << (Int_t)fCurMessage.fEventType << "\t"
	     << "subtype=" << (Int_t)fCurMessage.fEventSubtype << "\t"
	     << "dummy="   << (Int_t)fCurMessage.fEventDummy << "\t"
	     << "trigger=" << (Int_t)fCurMessage.fEventTrigger << "\t"
	     << "count="   << (Int_t)fCurMessage.fEventCount
	     << endl;
	////UserProcUnpacking::DumpEventHeader(p_event);
	#endif

	//TODO I did not actually find the corrent event header length
	fHeadersWords += 4;
}

void UserProcUnpacking::FinishEvent()
{
	#ifdef DORESET
	fCurMessage.fEventType = -1;
	fCurMessage.fEventSubtype = -1;
	fCurMessage.fEventDummy = -1;
	fCurMessage.fEventTrigger = -1;
	fCurMessage.fEventCount = -1;
	////// fCurrentOutputEvent->fTrigger = -1; // ONE SHOULD NOT DO IT! Opposed to the current message, the output event has not been emitted yet.
	#endif // DORESET
}

void UserProcUnpacking::ProcessSubeventHeader(TGo4MbsSubEvent* p_subevent)
{
	fCurMessage.fSubeventDlen = p_subevent->GetDlen();
	fCurMessage.fSubeventType = p_subevent->GetType();
	fCurMessage.fSubeventSubcrate = p_subevent->GetSubcrate();
	fCurMessage.fSubeventControl = p_subevent->GetControl();
	fCurMessage.fSubeventFullID = p_subevent->GetFullId();
	fCurMessage.fSubeventProcID = p_subevent->GetProcid();

	#ifdef DEBUGUNPACKING
	cerr << "[DEBUG ] Subevent header:" << "\t"
	     << "dlen="     << (Int_t)fCurMessage.fSubeventDlen << "\t"
	     << "type="     << (Int_t)fCurMessage.fSubeventType << "\t"
	     << "subcrate=" << (Int_t)fCurMessage.fSubeventSubcrate << "\t"
	     << "control="  << (Int_t)fCurMessage.fSubeventControl << "\t"
	     << "fullid="   << (Int_t)fCurMessage.fSubeventFullID << "\t"
	     << "procid="   << (Int_t)fCurMessage.fSubeventProcID
	     << endl;
	////UserProcUnpacking::DumpSubeventHeader(p_subevent);
	#endif

	//TODO I did not actually find the corrent subevent header length
	fHeadersWords += 3;
}

void UserProcUnpacking::FinishSubevent()
{
	#ifdef DORESET
	fCurMessage.fSubeventDlen = -1;
	fCurMessage.fSubeventType = -1;
	fCurMessage.fSubeventSubcrate = -1;
	fCurMessage.fSubeventControl = -1;
	fCurMessage.fSubeventFullID = -1;
	fCurMessage.fSubeventProcID = -1;
	#endif // DORESET
}

void UserProcUnpacking::ProcessSubevent(TGo4MbsSubEvent* p_subevent)
{
	this->ProcessSubeventHeader(p_subevent);

	//// Extract only what is needed to go deeper

	Short_t v_procID = p_subevent->GetProcid();
	//// Get the size of the current subevent data payload in words as integer
	Int_t v_intLen = p_subevent->GetIntLen();
	//// Get the pointer to the data of the subevent
	Int_t* v_dataField = p_subevent->GetDataField();

	#ifdef DEBUGUNPACKING
	UserProcUnpacking::DumpSubeventData2(v_intLen, v_dataField);
	#endif

	switch (v_procID) {
	case 100:
		#ifdef DEBUGUNPACKING
		cerr << "[DEBUG ] Found procID " << v_procID << " which corresponds to VME0" << endl;
		#endif
		this->ProcessSubeventRaw(v_intLen, v_dataField); // ProcessSubeventRawVME0
		break;
	case 200:
		#ifdef DEBUGUNPACKING
		cerr << "[DEBUG ] Found procID " << v_procID << " which corresponds to VME1" << endl;
		#endif
		this->ProcessSubeventRaw(v_intLen, v_dataField); // ProcessSubeventRawVME1
		break;
	case 101:
		#ifdef DEBUGUNPACKING
		cerr << "[DEBUG ] Found procID " << v_procID << " which corresponds to CAMAC" << endl;
		#endif
		this->ProcessSubeventRawCAMACmwpc(v_intLen, v_dataField);
		break;
	default:
		#ifdef DEBUGUNPACKING
		cerr << "[WARN  ] Found procID " << v_procID << " which is unknown. Skipping subevent." << endl;
		#endif
		fNunknownWords += v_intLen;
		break;
	}

	this->FinishSubevent();
}

// Only data (payload) is processed here. Header and footer of the current subevent are already left out.
void UserProcUnpacking::ProcessSubeventRaw(Int_t p_size, const Int_t* p_startAddress)
{
	for (Int_t v_cursor=0; v_cursor<p_size; /*no action here*/) // v_cursor is incremented inside
	{
		Int_t v_leftSize = p_size - v_cursor;
		Int_t v_footerPosition = 0; //TODO or better init with -1 ?
		Int_t v_footerCounter = -1;

		switch (UserProcUnpacking::CheckNextHeader(&p_startAddress[v_cursor])) {
		case support::enu_VENDOR::MESYTEC:

			#ifdef DEBUGUNPACKING
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &p_startAddress[v_cursor]) << "  ";
			cerr << "[" << v_cursor << "]\t" << "MESYTEC header" << endl;
			#endif

			//// Here we look forward and search for the footer.
			//// Not only we count the number of words in the subsubevent block but also
			//// we extract the counter from the footer and use it for all the data words
			//// of this subsubevent
			v_footerPosition = UserProcUnpacking::FindMESYTECfooter(v_leftSize, &p_startAddress[v_cursor], &v_footerCounter);
			fCurMessage.fSubsubeventFooterCounter = v_footerCounter;

			if (v_footerPosition > -1) {

				#ifdef DEBUGUNPACKING
				cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &p_startAddress[v_cursor+v_footerPosition]) << "  ";
				cerr << "[" << v_cursor+v_footerPosition << "]\t" << "MESYTEC footer";
				cerr << "\tshift=" << v_footerPosition << endl;
				#endif

				this->ProcessSubsubevent_MESYTEC(v_footerPosition+1, &p_startAddress[v_cursor]);

				v_cursor += (v_footerPosition + 1); // right after the footer
			} else {
				//#ifdef DEBUGUNPACKING
				cerr << "[WARN  ] MESYTEC footer not found!" << endl;
				//#endif
				v_cursor++;
			}

			break;
		case support::enu_VENDOR::CAEN:

			#ifdef DEBUGUNPACKING
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &p_startAddress[v_cursor]) << "  ";
			cerr << "[" << v_cursor << "]\t" << "CAEN header" << endl;
			#endif

			//// Here we look forward and search for the footer.
			//// Not only we count the number of words in the subsubevent block but also
			//// we extract the counter from the footer and use it for all the data words
			//// of this subsubevent
			v_footerPosition = UserProcUnpacking::FindCAENfooter(v_leftSize, &p_startAddress[v_cursor], &v_footerCounter);
			fCurMessage.fSubsubeventFooterCounter = v_footerCounter;

			if (v_footerPosition > -1) {

				#ifdef DEBUGUNPACKING
				cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &p_startAddress[v_cursor+v_footerPosition]) << "  ";
				cerr << "[" << v_cursor+v_footerPosition << "]\t" << "CAEN footer";
				cerr << "\tshift=" << v_footerPosition << endl;
				#endif

				this->ProcessSubsubevent_CAEN(v_footerPosition+1, &p_startAddress[v_cursor]);

				v_cursor += (v_footerPosition + 1); // right after the footer
			} else {
				//#ifdef DEBUGUNPACKING
				cerr << "[WARN  ] CAEN footer not found!" << endl;
				//#endif
				v_cursor++;
			}

			break;
		case support::enu_VENDOR::AFFEAFFE:
			#ifdef DEBUGUNPACKING
			cerr << "[DEBUG ] AFFEAFFE. Skipping one 32-bit word." << endl;
			#endif
			fNknownWords++;
			v_cursor++;
			break;
		case support::enu_VENDOR::CAENNOTVALID:
			#ifdef DEBUGUNPACKING
			cerr << "[DEBUG ] CAENNOTVALID. Skipping one 32-bit word." << endl;
			#endif
			fNknownWords++;
			v_cursor++;
			break;
		case support::enu_VENDOR::OTHER:
			//#ifdef DEBUGUNPACKING
			cerr << "[WARN  ] OTHER VENDOR. Skipping one 32-bit word:" << "\t";
			cerr << support::GetHexRepresentation(sizeof(Int_t), &p_startAddress[v_cursor]) << "\t";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &p_startAddress[v_cursor]) << endl;
			//#endif
			fNunknownWords++;
			v_cursor++;
			break;
		//// default section has no meaning here because OTHER already counts that
		}
	}
}

/*void UserProcUnpacking::ProcessSubeventRawVME0(Int_t p_size, const Int_t* p_startAddress)
{
	#ifdef DEBUGUNPACKING
	cerr << "[DEBUG ] Processing raw subevent from VME0 with size=" << p_size << endl;
	#endif
	this->ProcessSubeventRaw(p_size, p_startAddress);
}

void UserProcUnpacking::ProcessSubeventRawVME1(Int_t p_size, const Int_t* p_startAddress)
{
	#ifdef DEBUGUNPACKING
	cerr << "[DEBUG ] Processing raw subevent from VME1 with size=" << p_size << endl;
	#endif
	this->ProcessSubeventRaw(p_size, p_startAddress);
}*/

/*void UserProcUnpacking::ProcessSubeventRawCAMAC(Int_t p_size, const Int_t* p_startAddress)
{
	#ifdef DEBUGUNPACKING
	cerr << "[DEBUG ] Processing raw subevent from CAMAC with size=" << p_size << endl;
	//cerr << "\t" << "Skipping." << endl;
	#endif

	for (Int_t v_cursor=0; v_cursor<p_size; v_cursor++) {
		Int_t v_curWord = p_startAddress[v_cursor];
		Int_t v_geo = (v_curWord >> 27) & 0x1f; //TODO mask is unknown to me // 5 bits?
		fprintf(stderr, "%d: %08x\tgeo=%u\n", v_cursor, p_startAddress[v_cursor], v_geo);
	}
}*/

void UserProcUnpacking::ProcessSubeventRawCAMACmwpc(Int_t p_size, const Int_t* p_startAddress)
{
	#ifdef DEBUGUNPACKING
	cerr << "[DEBUG ] Processing raw subevent from CAMAC with size=" << p_size << " as from MWPC." << endl;
	#endif

	// Currently we rely on the fact that p_size=16
	// We also rely that the words come in order from the blocks
	// with geo = 20, 21, 22, 23

	if (p_size != 16) {
		cerr << "[WARN ] CAMAC subevent size != 16. Skipping subevent." << endl;
		return;
	}

	fNknownWords += 16;

	Int_t v_curWord;
	//Int_t v_geo;

	// HEADER
	v_curWord = p_startAddress[0];
	//v_geo = (v_curWord >> 27) & 0x1f; //TODO mask is unknown to me // 5 bits?
	// DATA0
	v_curWord = p_startAddress[1];
	fCurrentOutputEvent->fCAMAC[0] = (v_curWord & 0xFFFF);
	// DATA1
	v_curWord = p_startAddress[2];
	fCurrentOutputEvent->fCAMAC[1] = (v_curWord & 0xFFFF);
	// FOOTER
	v_curWord = p_startAddress[3];
	//v_geo = (v_curWord >> 27) & 0x1f; //TODO mask is unknown to me // 5 bits?

	// HEADER
	v_curWord = p_startAddress[4];
	//v_geo = (v_curWord >> 27) & 0x1f; //TODO mask is unknown to me // 5 bits?
	// DATA2
	v_curWord = p_startAddress[5];
	fCurrentOutputEvent->fCAMAC[2] = (v_curWord & 0xFFFF);
	// DATA3
	v_curWord = p_startAddress[6];
	fCurrentOutputEvent->fCAMAC[3] = (v_curWord & 0xFFFF);
	// FOOTER
	v_curWord = p_startAddress[7];
	//v_geo = (v_curWord >> 27) & 0x1f; //TODO mask is unknown to me // 5 bits?

	// HEADER
	v_curWord = p_startAddress[8+0];
	//v_geo = (v_curWord >> 27) & 0x1f; //TODO mask is unknown to me // 5 bits?
	// DATA4
	v_curWord = p_startAddress[8+1];
	fCurrentOutputEvent->fCAMAC[4] = (v_curWord & 0xFFFF);
	// DATA5
	v_curWord = p_startAddress[8+2];
	fCurrentOutputEvent->fCAMAC[5] = (v_curWord & 0xFFFF);
	// FOOTER
	v_curWord = p_startAddress[8+3];
	//v_geo = (v_curWord >> 27) & 0x1f; //TODO mask is unknown to me // 5 bits?

	// HEADER
	v_curWord = p_startAddress[8+4];
	//v_geo = (v_curWord >> 27) & 0x1f; //TODO mask is unknown to me // 5 bits?
	// DATA6
	v_curWord = p_startAddress[8+5];
	fCurrentOutputEvent->fCAMAC[6] = (v_curWord & 0xFFFF);
	// DATA7
	v_curWord = p_startAddress[8+6];
	fCurrentOutputEvent->fCAMAC[7] = (v_curWord & 0xFFFF);
	// FOOTER
	v_curWord = p_startAddress[8+7];
	//v_geo = (v_curWord >> 27) & 0x1f; //TODO mask is unknown to me // 5 bits?

	#ifdef DEBUGUNPACKING
	Short_t v_subword;
	cerr << "         -----------------------------------------------------------" << endl;
	for (Int_t v_cursor=0; v_cursor<p_size; v_cursor+=4)
	{
		v_curWord = p_startAddress[v_cursor+0];
		cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  "
		     << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
		cerr << "[" << v_cursor+0 << "]\t" << "CAMAC MWPC header geo=" << ((v_curWord >> 27) & 0x1f);
		cerr << endl;

		v_curWord = p_startAddress[v_cursor+1];
		v_subword = v_curWord & 0xFFFF;
		cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  "
		     << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
		cerr << "[" << v_cursor+1 << "]\t" << "CAMAC MWPC data "
		     << support::GetBinaryRepresentation(sizeof(Short_t), &v_subword)
		     << endl;

		v_curWord = p_startAddress[v_cursor+2];
		v_subword = v_curWord & 0xFFFF;
		cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  "
		     << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
		cerr << "[" << v_cursor+2 << "]\t" << "CAMAC MWPC data "
		     << support::GetBinaryRepresentation(sizeof(Short_t), &v_subword)
		     << endl;

		v_curWord = p_startAddress[v_cursor+3];
		cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  "
		     << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
		cerr << "[" << v_cursor+3 << "]\t" << "CAMAC MWPC footer geo=" << ((v_curWord >> 27) & 0x1f);
		cerr << endl;
	}
	cerr << "         -----------------------------------------------------------" << endl;
	#endif // DEBUGUNPACKING
}

void UserProcUnpacking::ProcessSubsubevent_MESYTEC(Int_t p_size, const Int_t* p_startAddress)
{
	#ifdef DEBUGUNPACKING
	cerr << "[DEBUG ] Processing MESYTEC subsubevent with size=" << p_size << endl;
	cerr << "         -----------------------------------------------------------" << endl;
	#endif

	Int_t v_dataWordsCounter = 0;

	fCurMessage.fSubsubeventVendor = (Char_t)support::enu_VENDOR::MESYTEC; // MESYTEC=1 //TODO explicit cast?

	for (Int_t v_cursor=0; v_cursor<p_size; v_cursor++) {

		Int_t tmp = p_startAddress[v_cursor];
		Int_t v_curWord = tmp;

		// Common for all types of messages
		Int_t v_type = (v_curWord >> 30) & 0x3; // 2 bits

		Int_t v_module_id = -1;
		#ifdef DEBUGUNPACKING
		Int_t v_subsubeventSize = -1;
		Int_t v_eventCounter = -1; // event counter or time stamp
		#endif
		Int_t v_channel = -1;
		Int_t v_valueQA = -1; // QDC and ADC
		Int_t v_valueT = -1; // TDC

		switch (v_type) {
		case 1: // MESYTEC header
			fInsidePackage = true;
			v_module_id = (v_curWord >> 16) & 0xff; // 8 bits
			fNknownWords++;
			#ifdef DEBUGUNPACKING
			v_subsubeventSize = v_curWord & 0x3ff; // 10 bits
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "MESYTEC header"
			     << "\ttype=" << v_type
			     << "\tmodule_id=" << v_module_id
			     << "\tsize=" << v_subsubeventSize
			     << endl;
			#endif

			fCurMessage.fSubsubeventModule = v_module_id;

			break;
		case 3: // MESYTEC footer
			fInsidePackage = false;
			fNknownWords++;
			#ifdef DEBUGUNPACKING
			v_eventCounter = UserProcUnpacking::ExtractCounterFromMESYTECfooter(v_curWord);
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "MESYTEC footer"
			     << "\ttype=" << v_type
			     << "\teventCounter=" << v_eventCounter
			     << endl;
			#endif

			//// Also this value should be reset somewhere.
			#ifdef DORESET
			fCurMessage.fSubsubeventFooterCounter = -1;
			#endif // DORESET

			break;
		case 0: // MESYTEC data
			//TODO check
			v_channel = (v_curWord >> 16) & 0x1f; // 5 bits
			v_valueQA = v_curWord & 0x1fff; // 13 bits // ADC and QDC
			v_valueT = v_curWord & 0xffff; // 16 bits // TDC
			fNknownWords++;
			#ifdef DEBUGUNPACKING
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "MESYTEC data"
			     << "\ttype=" << v_type
			     << "\tch=" << v_channel
			     << "\tvalueQA=" << v_valueQA
			     << "\tvalueT=" << v_valueT
			     << endl;
			#endif

			// HERE WE WRITE OUT
			fCurMessage.fRawWord = v_curWord;
			fCurMessage.fChannel = v_channel;
			fCurMessage.fValueQA = v_valueQA;
			fCurMessage.fValueT = v_valueT;
			fCurMessage.fMessageIndex = v_dataWordsCounter;

			//TODO patch for padding zero word handling
			if ((v_curWord == 0) && (v_cursor == p_size-2)) {

				#ifdef DEBUGUNPACKING
				cerr << "THE LAST PADDING ZERO WORD WILL BE SKIPPED" << endl;
				#endif
				//cerr << "v_cursor=" << v_cursor << "\t\t" << "p_size-2=" << p_size-2 << endl;

				// SKIP this padding zero word

			} else {
				// Otherwise - in a normal situation...

				this->PushOutputRawMessage();

			}

			#ifdef DORESET
			fCurMessage.fRawWord = 0; // Yes zero here, because we want to clear the raw word with all zeros
			fCurMessage.fChannel = -1;
			fCurMessage.fValueQA = -1;
			fCurMessage.fValueT = -1;
			fCurMessage.fMessageIndex = -1;
			#endif // DORESET

			// Just count data words. This counter is pushed into the output raw message.
			v_dataWordsCounter++;

			if (!fInsidePackage) {
				cerr << "[ERROR ] MESYTEC data word found not between the header and the footer." << endl;
			}
			break;
		default:
			fNunknownWords++;
			//#ifdef DEBUGUNPACKING
			cerr << "[ERROR ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "MESYTEC unknown"
			     << "\ttype=" << v_type << endl;
			//#endif
			break;
		} // end of switch
	} // end of for

	//// Don't forget to reset the values defined in the subsubevent header
	fCurMessage.fSubsubeventModule = -1; // Currently we reset module and geo independent of the precompiler variable
	#ifdef DORESET
	fCurMessage.fSubsubeventVendor = -1;
	#endif // DORESET

	#ifdef DEBUGUNPACKING
	cerr << "         -----------------------------------------------------------" << endl;
	#endif
}

void UserProcUnpacking::ProcessSubsubevent_CAEN(Int_t p_size, const Int_t* p_startAddress)
{
	#ifdef DEBUGUNPACKING
	cerr << "[DEBUG ] Processing CAEN subsubevent with size=" << p_size << endl;
	cerr << "         -----------------------------------------------------------" << endl;
	#endif

	fCurMessage.fSubsubeventVendor = (Char_t)support::enu_VENDOR::CAEN; // CAEN=2 //TODO explicit cast?

	Int_t v_dataWordsCounter = 0;

	for (Int_t v_cursor=0; v_cursor<p_size; v_cursor++) {

		Int_t tmp = p_startAddress[v_cursor];
		Int_t v_curWord = tmp;

		// Common for all types of messages
		Int_t v_type = (v_curWord >> 24) & 0x7; // 3 bits

		Int_t v_geo = -1;
		Int_t v_eventCounter = -1;
		Int_t v_channel = -1;
		Int_t v_value = -1;
		#ifdef DEBUGUNPACKING
		Int_t v_subsubeventSize = -1;
		Int_t v_crate = -1;
		#endif

		switch (v_type) {
		case 2: // CAEN header
			v_geo = (v_curWord >> 27) & 0x1f; // 5 bits
			fInsidePackage = true;
			fNknownWords++;
			#ifdef DEBUGUNPACKING
			v_subsubeventSize = (v_curWord >> 8) & 0x3f; // 6 bits
			v_crate = (v_curWord >> 16) & 0xff; // 8 bits
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "CAEN header"
			     << "\ttype=" << v_type
			     << "\tgeo=" << v_geo
			     << "\tcrate=" << v_crate
			     << "\tsize=" << v_subsubeventSize
			     << endl;
			#endif

			fCurMessage.fSubsubeventGeo = v_geo;

			break;
		case 4: // CAEN footer
			fInsidePackage = false;
			v_eventCounter = UserProcUnpacking::ExtractCounterFromCAENfooter(v_curWord);
			v_geo = (v_curWord >> 27) & 0x1f; // 5 bits // This is not really needed here
			fNknownWords++;
			#ifdef DEBUGUNPACKING
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "CAEN footer"
			     << "\ttype=" << v_type
			     << "\tgeo=" << v_geo
			     << "\teventCounter=" << v_eventCounter
			     << endl;
			#endif

			//// IN PRINCIPLE, WE COULD WRITE THIS OUT
			fCurMessage.fSubsubeventFooterCounter = v_eventCounter;

			//// WRITE OUT HERE?

			//// Also this value should be reset somewhere.
			//// In principle one could reset it immediately right after writeout
			#ifdef DORESET
			fCurMessage.fSubsubeventFooterCounter = -1;
			#endif // DORESET

			break;
		case 6: // CAEN no valid data
			fNknownWords++;
			#ifdef DEBUGUNPACKING
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "CAEN no valid data"
			     << "\ttype=" << v_type
			     << "\tgeo=" << v_geo
			     << endl;
			#endif
			break;
		case 0: // CAEN data
			v_channel = (v_curWord >> 16) & 0x1f; // 5 bits //TODO check
			v_value = v_curWord & 0xfff; // 12 bits //TODO check
			fNknownWords++;
			#ifdef DEBUGUNPACKING
			cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
			cerr << "[" << v_cursor << "]\t" << "CAEN data"
			     << "\ttype=" << v_type
			     << "\tgeo=" << v_geo
			     << "\tch=" << v_channel
			     << "\tvalue=" << v_value
			     << endl;
			#endif

			//// HERE WE WRITE OUT
			fCurMessage.fRawWord = v_curWord;
			fCurMessage.fChannel = v_channel;
			fCurMessage.fValueQA = v_value;
			fCurMessage.fValueT = v_value;
			fCurMessage.fMessageIndex = v_dataWordsCounter;

			this->PushOutputRawMessage();

			#ifdef DORESET
			fCurMessage.fRawWord = 0; // Yes zero here, because we want to clear the raw word with all zeros
			fCurMessage.fChannel = -1;
			fCurMessage.fValueQA = -1;
			fCurMessage.fValueT = -1;
			fCurMessage.fMessageIndex = -1;
			#endif // DORESET

			// Just count data words. This counter is pushed into the output raw message.
			v_dataWordsCounter++;

			if (!fInsidePackage) {
				cerr << "[ERROR ] CAEN data word found not between the header and the footer." << endl;
			}
			break;
		default:

			//TODO bug fix for the machine time inside the CAEN subsubevent
			//cerr << "geo from the header = " << fCurMessage.fSubsubeventGeo << endl;
			if (fCurMessage.fSubsubeventGeo == 30) {
				//cerr << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;

				fNknownWords++;
				#ifdef DEBUGUNPACKING
				cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
				cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
				cerr << "[" << v_cursor << "]\t" << "MACHINE TIME" << endl;
				#endif

				//// HERE WE WRITE OUT
				fCurMessage.fRawWord = v_curWord;
				fCurMessage.fChannel = 0; //TODO check
				fCurMessage.fValueQA = 0; //TODO check
				fCurMessage.fValueT = 0; //TODO check
				fCurMessage.fMessageIndex = v_dataWordsCounter;

				this->PushOutputRawMessage();

				#ifdef DORESET
				fCurMessage.fRawWord = 0; // Yes zero here, because we want to clear the raw word with all zeros
				fCurMessage.fChannel = -1;
				fCurMessage.fValueQA = -1;
				fCurMessage.fValueT = -1;
				fCurMessage.fMessageIndex = -1;
				#endif // DORESET

				// Just count data words. This counter is pushed into the output raw message.
				v_dataWordsCounter++;

			} else {

				fNunknownWords++;
				//#ifdef DEBUGUNPACKING
				cerr << "[ERROR ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "  ";
				cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "  ";
				cerr << "[" << v_cursor << "]\t" << "CAEN unknown"
				     << "\ttype=" << v_type
				     << "\tgeo=" << v_geo
				     << endl;
				//#endif
				break;

			}

		} // end of switch
	} // end of for

	//// Don't forget to reset the values defined in the subsubevent header
	//// For this particular case, this geo is read from all types of words
	fCurMessage.fSubsubeventGeo = -1; // Currently we reset module and geo independent of the precompiler variable
	#ifdef DORESET
	fCurMessage.fSubsubeventVendor = -1;
	#endif // DORESET

	#ifdef DEBUGUNPACKING
	cerr << "         -----------------------------------------------------------" << endl;
	#endif
}

void UserProcUnpacking::PushOutputRawMessage(void)
{
	TClonesArray& v_coll = *(fCurrentOutputEvent->fRawMessages);
	Int_t v_size = v_coll.GetEntriesFast();
	//// The RawMessage class copy constructor is invoked here (I've checked, it is!)
	new(v_coll[v_size])RawMessage(fCurMessage);
}

// static methods =================================================================================

/*static*/
support::enu_VENDOR UserProcUnpacking::CheckNextHeader(const Int_t* p_startAddress)
{
	Int_t tmp = p_startAddress[0];
	Int_t v_curWord = tmp;
	////cerr << "[DEBUG ] " << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "\t";

	Int_t v_mesytec_flag = (v_curWord >> 24) & 0xff; // 8 bits // = 0x40
	Int_t v_caen_header = (v_curWord >> 24) & 0x7; // 3 bits // = 0x2

	if (v_mesytec_flag == 0x40) {
		#ifdef DEBUGUNPACKING
		cerr << "[DEBUG ] Checking "
		     << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "\t";
		cerr << "Identified as MESYTEC block" << endl;
		#endif
		return support::enu_VENDOR::MESYTEC;
	} else if (v_caen_header == 2) { // 010 (binary)
		#ifdef DEBUGUNPACKING
		cerr << "[DEBUG ] Checking "
		     << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "\t";
		cerr << "Identified as CAEN block" << endl;
		#endif
		return support::enu_VENDOR::CAEN;
	} else if ((v_curWord ^ 0xaffeaffe) == 0) {
		//TODO this not a very nice hack
		//// This is related to what is done in f_user.C
		// This is not the most intuitive, but yet very correct way to check the
		// raw representations of an int. ^ stands for binary XOR.
		// If all bits are the same, the results will be 0x00000000.
		#ifdef DEBUGUNPACKING
		cerr << "[DEBUG ] Checking "
		     << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "\t";
		cerr << "Identified as AFFEAFFE block" << endl;
		#endif
		return support::enu_VENDOR::AFFEAFFE;
	} else if (((v_curWord >> 24) & 0x7) == 6) {
		//TODO this not a very nice hack
		//// For some reason CAEN not-valid-datum words are coming outside of the
		//// subsubevent block - not between the header and the footer
		#ifdef DEBUGUNPACKING
		cerr << "[DEBUG ] Checking "
		     << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "\t";
		cerr << "Identified as CAENNOTVALID block" << endl;
		#endif
		return support::enu_VENDOR::CAENNOTVALID;
	} else {
		// Should not emit any warnings here.
		// The return value of this method is processed and warnings are emitted there.
		#ifdef DEBUGUNPACKING
		cerr << "[DEBUG ] Checking "
		     << support::GetHexRepresentation(sizeof(Int_t), &v_curWord) << "\t"
		     << support::GetBinaryRepresentation(sizeof(Int_t), &v_curWord) << "\t";
		cerr << "Identified as UNKNOWN block" << endl;
		#endif
		return support::enu_VENDOR::OTHER;
	}
}

/*static*/
Int_t UserProcUnpacking::FindMESYTECfooter(Int_t p_maxSize, const Int_t* p_startAddress, Int_t* o_counter)
{
	for (Int_t v_cursor=0; v_cursor<p_maxSize; v_cursor++) {
		Int_t tmp = p_startAddress[v_cursor];
		Int_t v_curWord = tmp;
		Int_t v_type = (v_curWord >> 30) & 0x3; // 2 bits
		////fprintf(stderr, "%08x type=%d\n", v_curWord, v_type);
		if (v_type == 3) {
			// type==3 - MESYTEC footer
			*o_counter = UserProcUnpacking::ExtractCounterFromMESYTECfooter(v_curWord);
			return v_cursor;
		}
	}
	return -1;
}

/*static*/
Int_t UserProcUnpacking::FindCAENfooter(Int_t p_maxSize, const Int_t* p_startAddress, Int_t* o_counter)
{
	for (Int_t v_cursor=0; v_cursor<p_maxSize; v_cursor++) {
		Int_t tmp = p_startAddress[v_cursor];
		Int_t v_curWord = tmp;
		Int_t v_type = (v_curWord >> 24) & 0x7; // 3 bits
		////fprintf(stderr, "%08x type=%d\n", v_curWord, v_type);
		if (v_type == 4) {
			// type==4 - CAEN footer
			*o_counter = UserProcUnpacking::ExtractCounterFromCAENfooter(v_curWord);
			return v_cursor;
		}
	}
	return -1;
}

/*static*/
void UserProcUnpacking::DumpEventHeader(/*const*/ TGo4MbsEvent* p_event)
{
	//TODO change to 'normal' interface methods
	s_filhe* head = p_event->GetMbsSourceHeader();
	if (head != NULL)
	{
		cerr << "found filhe structure:" << endl;
		cerr << "\tdatalen: " << head->filhe_dlen << endl;
		cerr << "\tfilename_l: " << head->filhe_file_l << endl;
		cerr << "\tfilename: " << head->filhe_file << endl;
		cerr << "\ttype: " << head->filhe_type << endl;
		cerr << "\tsubtype: " << head->filhe_subtype << endl;
		cerr << "\t#commentlines: " << head->filhe_lines << endl;
	} else {
		cerr << "zero file header" << endl;
	}
}

/*static*/
void UserProcUnpacking::DumpSubeventHeader(/*const*/ TGo4MbsSubEvent* p_subevent)
{
	Int_t v_dLen = p_subevent->GetDlen();
	Short_t v_type = p_subevent->GetType();
	Char_t v_subcrate = p_subevent->GetSubcrate();
	Char_t v_control = p_subevent->GetControl();
	Short_t v_procID = p_subevent->GetProcid();
	Int_t v_byteLen = p_subevent->GetByteLen();

	Int_t v_intLen = p_subevent->GetIntLen(); // Get the size of the current subevent
	//Int_t* v_dataField = p_subevent->GetDataField(); // Get the pointer to the data of the subevent

	cerr << "[DEBUG ] Subevent header: "
	     << "dLen=" << v_dLen << "\t"
	     << "type=" << v_type << "\t"
	     << "subcrate=" << (Int_t)v_subcrate << "\t"
	     << "control=" << (Int_t)v_control << "\t"
	     << "procID=" << v_procID << "\t"
	     << "intLen=" << v_intLen << "\t"
	     << "byteLen=" << v_byteLen
	     << endl;
}

/*static*/
void UserProcUnpacking::DumpSubeventData(Int_t p_size, const Int_t* p_startAddress)
{
	cerr << "SubEvent size = " << p_size << endl;
	for (Int_t i=0; i<p_size; i++) {
		Int_t v_curWord = p_startAddress[i];
		fprintf(stderr, "%02x", ((v_curWord >> 0) & 0xff));
		fprintf(stderr, "%02x ", ((v_curWord >> 8) & 0xff));
		fprintf(stderr, "%02x", ((v_curWord >> 16) & 0xff));
		fprintf(stderr, "%02x", ((v_curWord >> 24) & 0xff));
		fprintf(stderr, " ");
		if ((i+1)%8 == 0) { cerr << endl; }
	}
	cerr << endl;
}

/*static*/
void UserProcUnpacking::DumpSubeventData2(Int_t p_size, const Int_t* p_startAddress)
{
	cerr << "SubEvent size = " << p_size << endl;
	for (Int_t i=0; i<p_size; i++) {
		Int_t v_curWord = p_startAddress[i];
		fprintf(stderr, "%02x", ((v_curWord >> 24) & 0xff));
		fprintf(stderr, "%02x", ((v_curWord >> 16) & 0xff));
		fprintf(stderr, "%02x", ((v_curWord >> 8) & 0xff));
		fprintf(stderr, "%02x", ((v_curWord >> 0) & 0xff));
		fprintf(stderr, " ");
		if ((i+1)%8 == 0) { cerr << endl; }
	}
	cerr << endl;
}

ClassImp(UserProcUnpacking)
