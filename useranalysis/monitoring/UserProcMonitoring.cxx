#include "UserProcMonitoring.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// ROOT
#include <TClonesArray.h>
#include <TH1D.h>
//#include <TH2D.h>

// Project
#include "Support.h"
#include "UserParameter.h"
#include "UserEventMonitoring.h"
//#include "data_events/UserEventNew.h"
#include "UserHistosMonitoring.h"
#include "data/RawMessage.h"
#include "unpacking/UserEventUnpacking.h"
#include "setupconfigcppwrapper/SetupConfiguration.h"

/**
  Uncomment this if you want to see all the debug information.
  This allows you to analyze the raw bytes and bits by your eyes.
  This option produces A LOT OF DATA - run your analysis with a
  small number of events (~10-100)
*/
//#define PRINTDEBUGINFO

/**
  Uncomment this if you want to see the WARN messages while processing
  messages from the "Ignore" detector
*/
//#define LOUDIGNORE

UserProcMonitoring::UserProcMonitoring(const char* name) :
	TGo4EventProcessor(name),
	fEventCounter(0)
{
	fHistoMan = new UserHistosMonitoring();
	fFileSummary = fopen("textoutput/summaryMonitoring.txt", "w");
	if (fFileSummary == NULL) {
		//TODO error
		cerr << "[WARN  ] " << "Could not open output text summary file '" << "summaryMonitoring.txt" << "'" << endl;
	}
}

UserProcMonitoring::~UserProcMonitoring()
{
	if (fHistoMan) delete fHistoMan;
	if (fFileSummary != NULL) {
		fclose(fFileSummary);
	}
}

Bool_t UserProcMonitoring::BuildEvent(TGo4EventElement* p_dest)
{
	Bool_t v_isValid = kFALSE;
	UserEventMonitoring* v_outputEvent = (UserEventMonitoring*)p_dest;
	//UserEventNew* v_outputEvent = (UserEventNew*)p_dest;

	UserEventUnpacking* v_input = (UserEventUnpacking*)GetInputEvent("stepUnpackedProvider1");
	if (v_input == NULL)
	{
		cerr << "[WARN  ] " << "UserProcMonitoring::BuildEvent(): no input event!" << endl;
		v_outputEvent->SetValid(v_isValid);
		return v_isValid;
	}
	v_isValid = kTRUE;

	#ifdef PRINTDEBUGINFO
	cerr << "[DEBUG ] " << "UserProcMonitoring: Event " << fEventCounter
	     << " ======================================================================================================"
	     << endl;
	#endif

	fCurrentOutputEvent = v_outputEvent;

	// Clear the output event!!!
	//TODO check that this is not done by the framework
	// Seems that indeed this is done by the framework
	//fCurrentOutputEvent->Clear();

	//TODO do the processing of raw messages here
	UInt_t v_messCounter = 0;
	TIter next(v_input->fRawMessages);
	while (RawMessage* v_curMessage = (RawMessage*)next())
	{

		#ifdef PRINTDEBUGINFO
		cerr << v_messCounter << ": ";
		v_curMessage->Dump(false);
		cerr << "\t\t";
		#endif

		this->ProcessMessageUniversal(v_curMessage);

		//TODO check
		// Process trigger
		fCurrentOutputEvent->trigger = v_input->fTrigger;

		v_messCounter++;
	} // end of while

	//TODO do the processing of CAMAC MWPC words here
	this->ProcessCAMACmwpcWords(v_input);

	this->FillHistograms();

	// --------------------------

	v_outputEvent->SetValid(v_isValid);

	fEventCounter++;

	return v_isValid;
}

void UserProcMonitoring::ProcessMessageUniversal(const RawMessage* p_message)
{
	//TODO check
	// Get the all-accessible parameter-set object
	UserParameter* v_params = (UserParameter*)GetParameter("UserParameter");
	const SetupConfiguration* v_setupConfig = v_params->GetSetupConfig();

	unsigned short v_procid = (unsigned short)p_message->fSubeventProcID;
	unsigned short v_addr;
	support::enu_VENDOR v_messVendor = support::VendorFromChar(p_message->fSubsubeventVendor);
	if (v_messVendor == support::enu_VENDOR::MESYTEC) {
		v_addr = (unsigned short)p_message->fSubsubeventModule;
	} else if (v_messVendor == support::enu_VENDOR::CAEN) {
		v_addr = (unsigned short)p_message->fSubsubeventGeo;
	} else {
		cerr << "[ERROR ]" << " UserProcMonitoring::ProcessMessageUniversal() Unknown vendor." << endl;
		return;
	}
	unsigned short v_ch = (unsigned short)p_message->fChannel;

	if (v_setupConfig == NULL) {
		cerr << "[FATAL ] " << "Setup configuration object is NULL." << endl;
		exit(EXIT_FAILURE);
		return;
	}
	TString v_detector;
	TString v_folder;
	unsigned short v_detChannel = v_setupConfig->GetOutput(v_procid, v_addr, v_ch, &v_detector, &v_folder);

	// Special case for CAEN scalers
	if (v_detector == "scalers") {
		this->ProcessMessageScaler(p_message);
		return;
	}

	#ifndef LOUDIGNORE
	if (v_detector == "Ignore" || v_detector == "ignore") {
		return;
	}
	#endif // LOUDIGNORE

	#ifdef PRINTDEBUGINFO
	cerr << "[DEBUG ] " << v_folder << " /\t" << v_detector << "[" << v_ch << "] =\t"
	     << p_message->fValueQA << "\t(" << p_message->fValueT << ")" << endl;
	#endif

	UShort_t* v_eventDatField = fCurrentOutputEvent->GetFieldByName(v_detector);

	if (v_eventDatField != NULL) {
		//TODO check that the channel has allowed value
		//FIXME or p_message->mValueT ?
		v_eventDatField[v_detChannel] = p_message->fValueQA;
	}
}

//TODO test
void UserProcMonitoring::ProcessMessageScaler(const RawMessage* p_message)
{
	#ifdef PRINTDEBUGINFO
	cerr << "[DEBUG ] " << "Scalers[" << p_message->fMessageIndex << "] "
	     << support::GetBinaryRepresentation(sizeof(Int_t), &p_message->fRawWord) << "\t0x"
	     << support::GetHexRepresentation(sizeof(Int_t), &p_message->fRawWord) << "\t"
	     << p_message->fRawWord << endl;
	#endif

	//TODO obsolete?
	//TODO check that scalers channel (which is p_message->fMessageIndex) is whithin the allowed range
	// fSetupConfiguration != NULL - check done in ProcessMessageUniversal

	//TODO primitive check by now, still required
	if (p_message->fMessageIndex >= 16) {
		cerr << "[ERROR] " << "Scalers channel >= 16 not within allowed range [0;15]. Skipping this raw message." << endl;
		return;
	}

	fCurrentOutputEvent->scaler[p_message->fMessageIndex] = p_message->fRawWord;
}

void UserProcMonitoring::ProcessCAMACmwpcWords(const UserEventUnpacking* p_inputEvent)
{
	const Short_t* v_inputCAMAC = p_inputEvent->fCAMAC;
/*
	// Just print - shorts
	#ifdef PRINTDEBUGINFO
	cerr << "--------------------------------" << endl;
	cerr << support::GetBinaryRepresentation(sizeof(Short_t), &v_inputCAMAC[1]);
	cerr << support::GetBinaryRepresentation(sizeof(Short_t), &v_inputCAMAC[0]) << endl;
	cerr << support::GetBinaryRepresentation(sizeof(Short_t), &v_inputCAMAC[3]);
	cerr << support::GetBinaryRepresentation(sizeof(Short_t), &v_inputCAMAC[2]) << endl;
	cerr << support::GetBinaryRepresentation(sizeof(Short_t), &v_inputCAMAC[5]);
	cerr << support::GetBinaryRepresentation(sizeof(Short_t), &v_inputCAMAC[4]) << endl;
	cerr << support::GetBinaryRepresentation(sizeof(Short_t), &v_inputCAMAC[7]);
	cerr << support::GetBinaryRepresentation(sizeof(Short_t), &v_inputCAMAC[6]) << endl;
	cerr << "--------------------------------" << endl;
	#endif
*/
	// Transform pairs of shorts into normal ints
	UInt_t v_line[4];
	v_line[0] = ((v_inputCAMAC[1] << 16) & 0xffff0000) |
	            ((v_inputCAMAC[0] << 0)  & 0x0000ffff);
	v_line[1] = ((v_inputCAMAC[3] << 16) & 0xffff0000) |
	            ((v_inputCAMAC[2] << 0)  & 0x0000ffff);
	v_line[2] = ((v_inputCAMAC[5] << 16) & 0xffff0000) |
	            ((v_inputCAMAC[4] << 0)  & 0x0000ffff);
	v_line[3] = ((v_inputCAMAC[7] << 16) & 0xffff0000) |
	            ((v_inputCAMAC[6] << 0)  & 0x0000ffff);

	fCurrentOutputEvent->rx1 = v_line[0];
	fCurrentOutputEvent->ry1 = v_line[1];
	fCurrentOutputEvent->rx2 = v_line[2];
	fCurrentOutputEvent->ry2 = v_line[3];

	// Just print - ints
	#ifdef PRINTDEBUGINFO
	cerr << "--------------------------------" << endl;
	cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_line[0]) << "\t0x"
	     << support::GetHexRepresentation(sizeof(Int_t), &v_line[0]) << "\t"
	     << v_line[0] << endl;
	cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_line[1]) << "\t0x"
	     << support::GetHexRepresentation(sizeof(Int_t), &v_line[1]) << "\t"
	     << v_line[1] << endl;
	cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_line[2]) << "\t0x"
	     << support::GetHexRepresentation(sizeof(Int_t), &v_line[2]) << "\t"
	     << v_line[2] << endl;
	cerr << support::GetBinaryRepresentation(sizeof(Int_t), &v_line[3]) << "\t0x"
	     << support::GetHexRepresentation(sizeof(Int_t), &v_line[3]) << "\t"
	     << v_line[3] << endl;
	cerr << "--------------------------------" << endl;
	#endif
/*
	// Just print - bits
	#ifdef PRINTDEBUGINFO
	cerr << "--------------------------------" << endl;
	for (unsigned int i=0; i<4; i++) {
		for (unsigned char v_wire=0; v_wire<32; v_wire++) {
			unsigned char v_bitValue = (v_line[i] >> (32-v_wire-1)) & 0x1;
			cerr << (int)v_bitValue;
		}
		cerr << endl;
	}
	cerr << "--------------------------------" << endl;
	#endif
*/

	// Count fired wires and
	// Transfer 4 x 32 bits from contineous words into arrays

	// Store counters' addresses in an array
	UChar_t* v_countersAddrs[4];
	// Here we define the order of words
	v_countersAddrs[0] = &(fCurrentOutputEvent->nx1);
	v_countersAddrs[1] = &(fCurrentOutputEvent->ny1);
	v_countersAddrs[2] = &(fCurrentOutputEvent->nx2);
	v_countersAddrs[3] = &(fCurrentOutputEvent->ny2);

	// Store arrays' addresses in an array // sorry
	UChar_t* v_arrayAddrs[4];
	// Here we define the order of words
	v_arrayAddrs[0] = &(fCurrentOutputEvent->x1[0]);
	v_arrayAddrs[1] = &(fCurrentOutputEvent->y1[0]);
	v_arrayAddrs[2] = &(fCurrentOutputEvent->x2[0]);
	v_arrayAddrs[3] = &(fCurrentOutputEvent->y2[0]);


	// But first, let me reset
	for (unsigned int i=0; i<4; i++) {
		*(v_countersAddrs[i]) = 0;
		for (unsigned char v_wire=0; v_wire<32; v_wire++) {
			*(v_arrayAddrs[i] + v_wire) = 0;
		}
	}

	// Per-se count
	for (unsigned int i=0; i<4; i++) {
		for (unsigned char v_wire=0; v_wire<32; v_wire++) {
			unsigned char v_bitValue = (v_line[i] >> v_wire) & 0x1;
			if (v_bitValue == 1) {
				// Put the index of the fired wire
				*(v_arrayAddrs[i] + *(v_countersAddrs[i])) = v_wire;
				*(v_countersAddrs[i]) += 1; // increase number of fired wires by 1
			}
		}
	}

}

void UserProcMonitoring::FillHistograms(void) const
{
	// nx1, ny1, nx2, ny2
	fHistoMan->histoMWPCnx1->Fill(fCurrentOutputEvent->nx1);
	fHistoMan->histoMWPCny1->Fill(fCurrentOutputEvent->ny1);
	fHistoMan->histoMWPCnx2->Fill(fCurrentOutputEvent->nx2);
	fHistoMan->histoMWPCny2->Fill(fCurrentOutputEvent->ny2);

	// x1, y1, x2, y2
	for (UInt_t iIndex=0; iIndex<fCurrentOutputEvent->nx1; iIndex++) {
		fHistoMan->histoMWPCx1->Fill(fCurrentOutputEvent->x1[iIndex]);
	}
	for (UInt_t iIndex=0; iIndex<fCurrentOutputEvent->ny1; iIndex++) {
		fHistoMan->histoMWPCy1->Fill(fCurrentOutputEvent->y1[iIndex]);
	}
	for (UInt_t iIndex=0; iIndex<fCurrentOutputEvent->nx2; iIndex++) {
		fHistoMan->histoMWPCx2->Fill(fCurrentOutputEvent->x2[iIndex]);
	}
	for (UInt_t iIndex=0; iIndex<fCurrentOutputEvent->ny2; iIndex++) {
		fHistoMan->histoMWPCy2->Fill(fCurrentOutputEvent->y2[iIndex]);
	}

	// rx1, ry1, rx2, ry2
	for (unsigned char v_wire=0; v_wire<32; v_wire++) {
		unsigned char v_bitValue;
		v_bitValue = (fCurrentOutputEvent->rx1 >> v_wire) & 0x1;
		if (v_bitValue == 1) {
			fHistoMan->histoMWPCrx1->Fill(v_wire);
		}
		v_bitValue = (fCurrentOutputEvent->ry1 >> v_wire) & 0x1;
		if (v_bitValue == 1) {
			fHistoMan->histoMWPCry1->Fill(v_wire);
		}
		v_bitValue = (fCurrentOutputEvent->rx2 >> v_wire) & 0x1;
		if (v_bitValue == 1) {
			fHistoMan->histoMWPCrx2->Fill(v_wire);
		}
		v_bitValue = (fCurrentOutputEvent->ry2 >> v_wire) & 0x1;
		if (v_bitValue == 1) {
			fHistoMan->histoMWPCry2->Fill(v_wire);
		}
	}
}

ClassImp(UserProcMonitoring)
