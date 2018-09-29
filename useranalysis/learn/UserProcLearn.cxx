#include "UserProcLearn.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// ROOT
#include <TClonesArray.h>
#include <TH2D.h>

// Go4
#include <TGo4Analysis.h>

// Project
#include "base/Support.h"
#include "UserEventLearn.h"
#include "UserParameter.h"
#include "data/RawMessage.h"
#include "unpacking/UserEventUnpacking.h"
#include "setupconfigcppwrapper/SetupConfiguration.h"

/**
  Uncomment this if you want to see all the debug information.
  This allows you to analyze the raw bytes and bits by your eyes.
  This option produces A LOT OF DATA - run your analysis with a
  small number of events (~10-100)
*/
//#define DEBUGLEARN

/**
  Uncomment this if you want to hide the messages about the channels
  which are mapped and with data coming from them.
  This option reduces the text output in the summaryLearn.txt file
  a little bit.
*/
//#define SUPPRESSMAPPEDWITHDATA

UserProcLearn::UserProcLearn(const char* name) :
	TGo4EventProcessor(name),
	fEventCounter(0)
	/*fCurrentOutputEvent(NULL)*/
{
	TGo4Analysis* a = TGo4Analysis::Instance();

	/*const char* procids[3] = {"100", "101", "200"};*/
	fHistoAddrVsProcid = a->MakeTH2('D', "fHistoAddrVsProcid", "Messages per block;procid;addr",
	                                3, 0., 3., 32, 0., 32.);

	support::CheckThatDirExists("textoutput");

	fFileSummary = fopen("textoutput/summaryLearn.txt", "w");
	if (fFileSummary == NULL) {
		//TODO warning or fatal?
		//cerr << "[WARN  ] " << "Could not open output text summary file '" << "summaryLearn.txt" << "'" << endl;
		cerr << "[FATAL ] " << "Could not open output text summary file '" << "textoutput/summaryLearn.txt" << "'" << endl;
		exit(EXIT_FAILURE);
	}
}

UserProcLearn::~UserProcLearn()
{
	if (fFileSummary != NULL) {
		fclose(fFileSummary);
	}
}

Bool_t UserProcLearn::BuildEvent(TGo4EventElement* p_dest)
{
	Bool_t v_isValid = kFALSE;
	UserEventLearn* v_outputEvent = (UserEventLearn*)p_dest;

	UserEventUnpacking* v_input = (UserEventUnpacking*)GetInputEvent("stepUnpackedProvider2");
	if (v_input == NULL)
	{
		cerr << "UserProcLearn::BuildEvent(): no input event!" << endl;
		v_outputEvent->SetValid(v_isValid);
		return v_isValid;
	}
	v_isValid = kTRUE;

	#ifdef DEBUGLEARN
	cerr << "[DEBUG ] " << "UserProcLearn: Event " << fEventCounter
	     << " ==========================================================================================================="
	     << endl;
	#endif

	fCurrentOutputEvent = v_outputEvent;

	// Clear the output event!!!
	//TODO check that this is not done by the framework
	// Seems that indeed this is done by the framework
	//mCurrentOutputEvent->Clear();

	//TODO do the processing here

	UInt_t v_messCounter = 0;
	TIter next(v_input->fRawMessages);
	while (RawMessage* v_curMessage = (RawMessage*)next())
	{

		#ifdef DEBUGLEARN
		cerr << v_messCounter << ": ";
		v_curMessage->Dump(false);
		cerr << endl;
		#endif

		this->ProcessMessage(v_curMessage);

		v_messCounter++;
	} // end of while

	// --------------------------

	v_outputEvent->SetValid(v_isValid);

	fEventCounter++;

	return v_isValid;
}

void UserProcLearn::ProcessMessage(const RawMessage* p_message)
{
	UShort_t v_addr = 999;
	if (p_message->fSubsubeventGeo == -1 && p_message->fSubsubeventModule != -1) {
		v_addr = p_message->fSubsubeventModule;
	} else if (p_message->fSubsubeventGeo != -1 && p_message->fSubsubeventModule == -1) {
		v_addr = p_message->fSubsubeventGeo;
	} else {
		//ERROR
		cerr << "[FATAL ] " << "" << endl;
		exit(EXIT_FAILURE);
		return;
	}


	if (fSetupConfig == NULL) {
		cerr << "[FATAL ] " << "Setup configuration object is NULL." << endl;
		exit(EXIT_FAILURE);
		return;
	}
	//TString v_station;
	//TString v_detector;
	///*unsigned short v_detChannel = */fSetupConfig->GetOutput(p_message->fSubeventProcID, v_addr, p_message->fChannel, &v_station, &v_detector);

	unsigned int curChUID;

	// TODO special case for CAEN scalers
	if (fSetupConfig->IsMappedToScaler(p_message->fSubeventProcID,
	                                   v_addr, p_message->fChannel, p_message->fMessageIndex) ||
	    fSetupConfig->IsMappedToMachineTime(p_message->fSubeventProcID,
	                                        v_addr, p_message->fChannel, p_message->fMessageIndex))
	{
		curChUID = SetupConfiguration::GetChUID(p_message->fSubeventProcID,
		                                        v_addr, p_message->fMessageIndex);
	} else {
		curChUID = SetupConfiguration::GetChUID(p_message->fSubeventProcID,
		                                        v_addr, p_message->fChannel);
	}

	if (fUsedChUIDs.find(curChUID) == fUsedChUIDs.end()) {
		fUsedChUIDs.insert(curChUID);
		//cerr << "Adding " << curChUID << endl;
	}

	Short_t v_prodidBin = -1;
	switch(p_message->fSubeventProcID) {
		case 100: v_prodidBin = 0; break;
		case 101: v_prodidBin = 1; break;
		case 200: v_prodidBin = 2; break;
	}

	fHistoAddrVsProcid->Fill(v_prodidBin, v_addr);

}

void UserProcLearn::UserPreLoop()
{
	// Get the all-accessible parameter-set object
	UserParameter* v_params = (UserParameter*)GetParameter("UserParameter");
	fSetupConfig = v_params->GetSetupConfig();
}

void UserProcLearn::UserPostLoop()
{
	TString v_station;
	TString v_detector;

	fprintf(fFileSummary, "=============================== SUMMARY ==============================\n");

	fprintf(fFileSummary, "The following channels are mapped:\n");
	this->ProcessUnmappedChannels();

	fprintf(fFileSummary, "The following channels have been detected in the input file(no matter what the mappings are):\n");

	for (auto v_chuid : fUsedChUIDs) {
		unsigned short v_procid = v_chuid/100000;
		unsigned short v_addr = (v_chuid%100000) / 1000;
		unsigned short v_ch = v_chuid%1000;
		unsigned short v_det_ch = fSetupConfig->GetOutput(v_procid, v_addr, v_ch, &v_station, &v_detector);

		TString v_stationLcase(v_station);
		v_stationLcase.ToLower();

		if (v_det_ch == 9999) {
			fprintf(fFileSummary, "======================================================================\n");
			fprintf(fFileSummary, "%d: procid=%u\taddr=%u\tch=%u\tIS NOT MAPPED!\n",
			        v_chuid, v_procid, v_addr, v_ch);
			fprintf(fFileSummary, "======================================================================\n");
			continue;
		}

		// TODO special case for CAEN scalers and machine time
		if ((v_stationLcase == "scalers") || (v_stationLcase == "mtime")) {
			// The difference is that the output (station) channel = input (electronics) channel
			// Moreover, the input (electronics) channel is not really a channel, extracted from
			// the raw data word, but it is taken as the position of the raw data word within the
			// subsubevent header. Sorry.
			fprintf(fFileSummary, "%d: procid=%u\taddr=%u\tch=%u\tmapped to %s[%u]\tfrom %s\n",
			        v_chuid, v_procid, v_addr, v_ch, v_station.Data(), v_ch, v_detector.Data());
		} else {
			fprintf(fFileSummary, "%d: procid=%u\taddr=%u\tch=%u\tmapped to %s[%u]\tfrom %s\n",
			        v_chuid, v_procid, v_addr, v_ch, v_station.Data(), v_det_ch, v_detector.Data());
		}

		/*cerr << v_chuid << ": procid=" << v_procid
		     << "\taddr=" << v_addr
		     << "\tch=" << v_ch
		     << "\tmapped to " << v_station << "[" << v_det_ch << "]"
		     << "\t from " << v_detector
		     << endl;*/
	}

	fprintf(fFileSummary, "======================================================================\n");
}

void UserProcLearn::ProcessUnmappedChannels(void) const
{
// This code snippet may be useful in the future
// but you will not find it when you need it...
/*
	std::map<TString, unsigned short> v_detectorList = fSetupConfig->GetDetectorList();
	std::map<TString, unsigned short>::const_iterator v_detIter = v_detectorList.begin();

	for ( ; v_detIter != v_detectorList.end(); ++v_detIter) {
		std::map<TString, unsigned short> v_stationsList = fSetupConfig->GetStationList(v_detIter->first);
		std::map<TString, unsigned short>::const_iterator v_stIter = v_stationsList.begin();

		for ( ; v_stIter != v_stationsList.end(); ++v_stIter) {
			cerr << v_detIter->first << ": " << v_stIter->first << endl;
		}
	}
*/
	std::map<unsigned int, stc_mapping*> v_mappings = fSetupConfig->GetMappings();
	std::map<unsigned int, stc_mapping*>::const_iterator v_mapIter = v_mappings.begin();

	for ( ; v_mapIter != v_mappings.end(); ++v_mapIter) {
		unsigned int v_uid = v_mapIter->first;
		//cerr << v_uid << endl;

		unsigned short v_procid = fSetupConfig->GetProcIdFromUID(v_uid);
		unsigned short v_addr = fSetupConfig->GetAddrFromUID(v_uid);
		unsigned short v_ch = fSetupConfig->GetElChFromUID(v_uid);

		auto search = fUsedChUIDs.find(v_uid);

		if (search != fUsedChUIDs.end()) {
			#ifndef SUPPRESSMAPPEDWITHDATA
			fprintf(fFileSummary, "%d: procid=%u\taddr=%u\tch=%u\tok\n", v_uid, v_procid, v_addr, v_ch);
			#endif
		} else {
			fprintf(fFileSummary, "%d: procid=%u\taddr=%u\tch=%u\tNO DATA\n", v_uid, v_procid, v_addr, v_ch);
		}
	}
}

ClassImp(UserProcLearn)
