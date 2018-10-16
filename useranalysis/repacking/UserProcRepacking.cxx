#include "UserProcRepacking.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// ROOT
#include <TClonesArray.h>
#include <TH1D.h>
//#include <TH2D.h>

// Project
#include "base/Support.h"
#include "UserParameter.h"
#include "unpacking/UserEventUnpacking.h" // input event
#include "data/DetEventFull.h" // output event
#include "UserHistosRepacking.h"
#include "data/DetEventCommon.h"
#include "data/DetEventStation.h"
#include "data/RawMessage.h"
#include "setupconfigcppwrapper/SetupConfiguration.h"

/**
  Uncomment this if you want to see all the debug information.
  This allows you to analyze the raw bytes and bits by your eyes.
  This option produces A LOT OF DATA - run your analysis with a
  small number of events (~10-100)
*/
//#define DEBUGREPACKING

/**
  Uncomment this if you want to see the WARN messages while processing
  messages from the "Ignore" detector
*/
//#define LOUDIGNORE

UserProcRepacking::UserProcRepacking(const char* name) :
	TGo4EventProcessor(name),
	fEventCounter(0)
{
	fHistoMan = new UserHistosRepacking();

	support::CheckThatDirExists("textoutput");

	fFileSummary = fopen("textoutput/summaryRepacking.txt", "w");
	if (fFileSummary == NULL) {
		//TODO warning or fatal?
		//cerr << "[WARN  ] " << "Could not open output text summary file '" << "summaryRepacking.txt" << "'" << endl;
		cerr << "[FATAL ] " << "Could not open output text summary file '" << "textoutput/summaryRepacking.txt" << "'" << endl;
		exit(EXIT_FAILURE);
	}
}

UserProcRepacking::~UserProcRepacking()
{
	if (fHistoMan) delete fHistoMan;
	if (fFileSummary != NULL) {
		fclose(fFileSummary);
	}
}

Bool_t UserProcRepacking::BuildEvent(TGo4EventElement* p_dest)
{
	Bool_t v_isValid = kFALSE;
	//UserEventRepacking* v_outputEvent = (UserEventRepacking*)p_dest;
	DetEventFull* v_outputEvent = (DetEventFull*)p_dest;

	UserEventUnpacking* v_input = (UserEventUnpacking*)GetInputEvent("stepUnpackedProvider1");
	if (v_input == NULL)
	{
		cerr << "[WARN  ] " << "UserProcRepacking::BuildEvent(): no input event!" << endl;
		v_outputEvent->SetValid(v_isValid);
		return v_isValid;
	}
	v_isValid = kTRUE;

	#ifdef DEBUGREPACKING
	cerr << "[DEBUG ] " << "UserProcRepacking: Event " << fEventCounter
	     << " ======================================================================================================"
	     << endl;
	#endif

	fCurrentOutputEvent = v_outputEvent;
	DetEventFull& v_outputEvRef = *fCurrentOutputEvent;
	DetEventCommon* v_evCommon = dynamic_cast<DetEventCommon*>(&v_outputEvRef[0]); // id=0 - DetEventCommon

	// Clear the output event!!!
	//TODO check that this is not done by the framework
	// Seems that indeed this is done by the framework
	//fCurrentOutputEvent->Clear();

	//TODO do the processing of raw messages here
	UInt_t v_messCounter = 0;
	TIter next(v_input->fRawMessages);
	while (RawMessage* v_curMessage = (RawMessage*)next())
	{
		#ifdef DEBUGREPACKING
		cerr << v_messCounter << ": ";
		v_curMessage->Dump(false);
		cerr << "\t\t";
		#endif

		this->ProcessMessageUniversal(v_curMessage);

		v_messCounter++;
	} // end of while

	//TODO check
	// Process trigger
	v_evCommon->trigger = v_input->fTrigger;

	//TODO do the processing of CAMAC MWPC words here
	this->ProcessCAMACmwpcWords(v_input);

	this->FillHistograms();

	// --------------------------

	v_outputEvent->SetValid(v_isValid);

	////v_outputEvent->Print();

	fEventCounter++;

	return v_isValid;
}

void UserProcRepacking::ProcessMessageUniversal(const RawMessage* p_message)
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
		cerr << "[ERROR ]" << " UserProcRepacking::ProcessMessageUniversal() Unknown vendor." << endl;
		return;
	}
	unsigned short v_ch = (unsigned short)p_message->fChannel;

	if (v_setupConfig == NULL) {
		cerr << "[FATAL ] " << "Setup configuration object is NULL." << endl;
		exit(EXIT_FAILURE);
		return;
	}

	if (v_setupConfig->IsMappedToScaler(v_procid, v_addr, v_ch, p_message->fMessageIndex)) {
		this->ProcessMessageScaler(p_message);
		return;
	} else if (v_setupConfig->IsMappedToMachineTime(v_procid, v_addr, v_ch, p_message->fMessageIndex)) {
		this->ProcessMachineTime(p_message);
		return;
	}

	// else {

	TString v_station;
	TString v_detector;
	TString v_elblock;
	unsigned short v_detID;
	unsigned short v_statID;
	unsigned short v_detChannel = v_setupConfig->GetOutput(v_procid, v_addr, v_ch, &v_station, &v_detector, &v_elblock, &v_detID, &v_statID);

	//TODO check
	if (v_detChannel == 9999) {
		#ifdef DEBUGREPACKING
		cerr << "[DEBUG ] "
		     << "Ckipping a message from a channel with no mapping." << endl;
		#endif // DEBUGREPACKING
		return;
	}

	// Ignore those message which are mapped to the 'Ignore' station
	//TODO elaborate nice output
	if (v_station.CompareTo("ignore", TString::kIgnoreCase) == 0) {
		#ifdef DEBUGREPACKING
		//cerr << endl;
		#endif // DEBUGREPACKING
		#ifdef LOUDIGNORE
		//TODO
		cerr << "[DEBUG ] " << v_detector << " /\t" << v_station << "[" << v_ch << "] =\t"
		     << p_message->fValueQA << "\t(" << p_message->fValueT << ")\t" << "Ignored." << endl;
		#endif // LOUDIGNORE
		return; // yes
	} else {
		#ifdef DEBUGREPACKING
		cerr << "[DEBUG ] " << v_detector << " /\t" << v_station << "[" << v_ch << "] =\t"
		     << p_message->fValueQA << "\t(" << p_message->fValueT << ")" << endl;
		#endif
	}

	// Extract the required station event object to write the data in
	DetEventFull& v_outputEvRef = *fCurrentOutputEvent;
	DetEventStation* v_evSt = dynamic_cast<DetEventStation*>(&v_outputEvRef[v_detID][v_statID]);

	// Skip if out-of-range bit of mQDC == 1
	//TODO specific actions here
	if (v_elblock.CompareTo("mqdc", TString::kIgnoreCase) == 0) {
		if (((p_message->fRawWord >> 15) & 0x1) == 1) {
			#ifdef DEBUGREPACKING
			cerr << "[DEBUG ] "
			     << "Skipping a message from mQDC with out-of-range bit." << endl;
			#endif
			return;
		}
	}

	//TODO check that the channel has allowed value
	//TODO specific actions here
	if (v_elblock.CompareTo("mtdc", TString::kIgnoreCase) == 0) {

		if (((p_message->fRawWord >> 21) & 0x1) == 1) {
			#ifdef DEBUGREPACKING
			cerr << "[DEBUG ] "
			     << "Skipping a message from mTDC with trigger-flag bit." << endl;
			#endif
			return;
		}

		////////////////////////////////////////////////////////
		v_evSt->AddDetMessage(v_detChannel, p_message->fValueT);
		////////////////////////////////////////////////////////

		return;
	}

	/////////////////////////////////////////////////////////
	v_evSt->AddDetMessage(v_detChannel, p_message->fValueQA);
	/////////////////////////////////////////////////////////

	// } // end of else
}

//TODO test
void UserProcRepacking::ProcessMessageScaler(const RawMessage* p_message)
{
	#ifdef DEBUGREPACKING
	cerr << "[DEBUG ] " << "Scalers[" << p_message->fMessageIndex << "] "
	     << support::GetBinaryRepresentation(sizeof(Int_t), &p_message->fRawWord) << "\t0x"
	     << support::GetHexRepresentation(sizeof(Int_t), &p_message->fRawWord) << "\t"
	     << p_message->fRawWord << endl;
	#endif

	// Extract the required common event object to write the data in
	DetEventFull& v_outputEvRef = *fCurrentOutputEvent;
	DetEventCommon* v_evCommon = dynamic_cast<DetEventCommon*>(&v_outputEvRef[0]); // id=0 - DetEventCommon

	//TODO obsolete?
	//TODO check that scalers channel (which is p_message->fMessageIndex) is whithin the allowed range
	// fSetupConfiguration != NULL - check done in ProcessMessageUniversal

	//TODO primitive check by now, still required
	if (p_message->fMessageIndex >= 16) {
		cerr << "[ERROR] " << "Scalers channel >= 16 not within allowed range [0;15]. Skipping this raw message." << endl;
		return;
	}

	///////////////////////////////////////////////////////////////////
	v_evCommon->scaler[p_message->fMessageIndex] = p_message->fRawWord;
	///////////////////////////////////////////////////////////////////
}

//TODO test
void UserProcRepacking::ProcessMachineTime(const RawMessage* p_message)
{
	#ifdef DEBUGREPACKING
	cerr << "[DEBUG ] " << "mtime[" << p_message->fMessageIndex << "] "
	     << support::GetBinaryRepresentation(sizeof(Int_t), &p_message->fRawWord) << "\t0x"
	     << support::GetHexRepresentation(sizeof(Int_t), &p_message->fRawWord) << "\t"
	     << p_message->fRawWord << endl;
	#endif

	// Extract the required common event object to write the data in
	DetEventFull& v_outputEvRef = *fCurrentOutputEvent;
	DetEventCommon* v_evCommon = dynamic_cast<DetEventCommon*>(&v_outputEvRef[0]); // id=0 - DetEventCommon

	//TODO
	// We could do something like this:
	// unsigned short v_detChannel = v_setupConfig->GetOutput(v_procid, v_addr, v_ch, &v_station, &v_detector, &v_elblock);
	// and use v_station instead of hardcoded mtime, but as mtime is anyway hardcoded in SetupConfiguration class
	// we can avoid useless operation.
	/*
	UShort_t* v_eventDatField2 = fCurrentOutputEvent->GetFieldByName("mtime"); //TODO hardcode
	v_eventDatField2[0] = (p_message->fRawWord >> 16) & 0xffff;
	v_eventDatField2[1] = (p_message->fRawWord >> 0) & 0xffff;
	*/

	////////////////////////////////////////////////////////////
	v_evCommon->mtime[0] = (p_message->fRawWord >> 16) & 0xffff;
	v_evCommon->mtime[1] = (p_message->fRawWord >> 0) & 0xffff;
	////////////////////////////////////////////////////////////
}

void UserProcRepacking::ProcessCAMACmwpcWords(const UserEventUnpacking* p_inputEvent)
{
	DetEventFull& v_outputEvRef = *fCurrentOutputEvent;
//	DetEventCommon* v_evCommon = dynamic_cast<DetEventCommon*>(&v_outputEvRef[0]); // id=0 - DetEventCommon

	//TODO check
	// Get the all-accessible parameter-set object
	UserParameter* v_params = (UserParameter*)GetParameter("UserParameter");
	const SetupConfiguration* v_setupConfig = v_params->GetSetupConfig();
	if (v_setupConfig->fMWPCdetectorID == 0 || v_setupConfig->fMWPCstationID == 0) {
		//ERROR
		//TODO
		// assert
	}

	const Short_t* v_inputCAMAC = p_inputEvent->fCAMAC;
/*
	// Just print - shorts
	#ifdef DEBUGREPACKING
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
/*
	v_evCommon->rx1 = v_line[0];
	v_evCommon->ry1 = v_line[1];
	v_evCommon->rx2 = v_line[2];
	v_evCommon->ry2 = v_line[3];
*/
	// Just print - ints
	#ifdef DEBUGREPACKING
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
	#ifdef DEBUGREPACKING
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
/*
	// Store counters' addresses in an array
	UChar_t* v_countersAddrs[4];
	// Here we define the order of words
	v_countersAddrs[0] = &(v_evCommon->nx1);
	v_countersAddrs[1] = &(v_evCommon->ny1);
	v_countersAddrs[2] = &(v_evCommon->nx2);
	v_countersAddrs[3] = &(v_evCommon->ny2);

	// Store arrays' addresses in an array // sorry
	UChar_t* v_arrayAddrs[4];
	// Here we define the order of words
	v_arrayAddrs[0] = &(v_evCommon->x1[0]);
	v_arrayAddrs[1] = &(v_evCommon->y1[0]);
	v_arrayAddrs[2] = &(v_evCommon->x2[0]);
	v_arrayAddrs[3] = &(v_evCommon->y2[0]);


	// But first, let me reset
	for (unsigned int i=0; i<4; i++) {
		*(v_countersAddrs[i]) = 0;
		for (unsigned char v_wire=0; v_wire<32; v_wire++) {
			*(v_arrayAddrs[i] + v_wire) = 0;
		}
	}
*/
	// Per-se count
	for (unsigned int i=0; i<4; i++) {
		for (unsigned char v_wire=0; v_wire<32; v_wire++) {
			unsigned char v_bitValue = (v_line[i] >> v_wire) & 0x1;
			if (v_bitValue == 1) {
				// Put the index of the fired wire
				//*(v_arrayAddrs[i] + *(v_countersAddrs[i])) = v_wire;

				////TODO
				DetEventStation* v_evSt = dynamic_cast<DetEventStation*>
				    (&v_outputEvRef[v_setupConfig->fMWPCdetectorID[i]][v_setupConfig->fMWPCstationID[i]]);

				//cerr << "i=" << i << "\tdet=" << v_setupConfig->fMWPCdetectorID[i]
				//     << "\tst=" << v_setupConfig->fMWPCstationID[i] << endl;

				v_evSt->AddDetMessage(v_wire, 99);

				//*(v_countersAddrs[i]) += 1; // increase number of fired wires by 1
			}
		}
	}

}

void UserProcRepacking::FillHistograms(void) const
{
/*
	DetEventFull& v_outputEvRef = *fCurrentOutputEvent;
	DetEventCommon* v_evCommon = dynamic_cast<DetEventCommon*>(&v_outputEvRef[0]); // id=0 - DetEventCommon

	// nx1, ny1, nx2, ny2
	fHistoMan->histoMWPCnx1->Fill(v_evCommon->nx1);
	fHistoMan->histoMWPCny1->Fill(v_evCommon->ny1);
	fHistoMan->histoMWPCnx2->Fill(v_evCommon->nx2);
	fHistoMan->histoMWPCny2->Fill(v_evCommon->ny2);

	// x1, y1, x2, y2
	for (UInt_t iIndex=0; iIndex<v_evCommon->nx1; iIndex++) {
		fHistoMan->histoMWPCx1->Fill(v_evCommon->x1[iIndex]);
	}
	for (UInt_t iIndex=0; iIndex<v_evCommon->ny1; iIndex++) {
		fHistoMan->histoMWPCy1->Fill(v_evCommon->y1[iIndex]);
	}
	for (UInt_t iIndex=0; iIndex<v_evCommon->nx2; iIndex++) {
		fHistoMan->histoMWPCx2->Fill(v_evCommon->x2[iIndex]);
	}
	for (UInt_t iIndex=0; iIndex<v_evCommon->ny2; iIndex++) {
		fHistoMan->histoMWPCy2->Fill(v_evCommon->y2[iIndex]);
	}

	// rx1, ry1, rx2, ry2
	for (unsigned char v_wire=0; v_wire<32; v_wire++) {
		unsigned char v_bitValue;
		v_bitValue = (v_evCommon->rx1 >> v_wire) & 0x1;
		if (v_bitValue == 1) {
			fHistoMan->histoMWPCrx1->Fill(v_wire);
		}
		v_bitValue = (v_evCommon->ry1 >> v_wire) & 0x1;
		if (v_bitValue == 1) {
			fHistoMan->histoMWPCry1->Fill(v_wire);
		}
		v_bitValue = (v_evCommon->rx2 >> v_wire) & 0x1;
		if (v_bitValue == 1) {
			fHistoMan->histoMWPCrx2->Fill(v_wire);
		}
		v_bitValue = (v_evCommon->ry2 >> v_wire) & 0x1;
		if (v_bitValue == 1) {
			fHistoMan->histoMWPCry2->Fill(v_wire);
		}
	}
*/
}

ClassImp(UserProcRepacking)
