#include "SetupConfiguration.h"

// STD //TODO delete?
#include <iostream>
using std::cerr;
using std::endl;

// Project
#include "setupconfig/pureC/functions.h"

/*static*/
SetupConfiguration* SetupConfiguration::mInstance = nullptr;

/*static*/
SetupConfiguration& SetupConfiguration::GetInstance(void) {
	if (mInstance == nullptr) {
		cerr << "SetupConfiguration singleton object does not exist. Aborting." << endl;
		exit(EXIT_FAILURE);
	}
	return *mInstance;
}

SetupConfiguration::SetupConfiguration() :
	TObject()
{
	cerr << "DEFAULT SetupConfiguration CONSTRUCTOR" << endl;
	mInstance = this; // to be on the safe side
}

SetupConfiguration::SetupConfiguration(const char* p_filename) :
	TObject()
{
	mInstance = this;

	// Import XML setup config file
	InitStcSetupConfig(&mConfiguration);
	ImportXML(&mConfiguration, p_filename);
	DumpStcSetupConfig(&mConfiguration);

	if (!(this->CheckConsistency())) {
		cerr << "[FATAL ] Imported XML setup configuration is inconsistent. Please check it. Aborting." << endl;
		exit(1); //TODO
	}

	this->Link();
}

SetupConfiguration::~SetupConfiguration()
{
}

void SetupConfiguration::Print(Option_t* option) const
{
	DumpStcSetupConfig(&mConfiguration);
}

bool SetupConfiguration::IsMapped(unsigned short p_crateProcid,
                                  unsigned short p_addr,
                                  unsigned short p_elch) const
{
	unsigned int v_chUID = SetupConfiguration::GetChUID(p_crateProcid, p_addr, p_elch);

	std::map<unsigned int, stc_mapping*>::const_iterator iter = mMappings.find(v_chUID);
	if (iter != mMappings.end()) {
		return true;
	} else {
		return false;
	}
}

bool SetupConfiguration::CheckConsistency(void)
{
	//TODO any additional checks
	return true;
}

void SetupConfiguration::Link(void)
{
	unsigned short v_detCounter = 1;

	// For each mapping
	for (unsigned short iMapping=0; iMapping<mConfiguration.fNmappings; iMapping++) {
		stc_mapping* v_curMapping = &(mConfiguration.fMappingsList[iMapping]);

		// Check that the channel step > 0, otherwise we will run unto the infinite loop
		if (v_curMapping->fStepelectrch == 0) {
			// Actually, this should not happen. To be on the safe side we reset it to 1.
			v_curMapping->fStepelectrch = 1;
		}

		////DumpStcMapping(v_curMapping);

		// For each electronics channel
		unsigned short counter = 0;
		for (unsigned short iCh = v_curMapping->fStartelectrch;
		                    iCh < v_curMapping->fStartelectrch + v_curMapping->fNelectrch;
		                    iCh += v_curMapping->fStepelectrch)
		{
			unsigned int v_curMappingChID = SetupConfiguration::GetChUID(v_curMapping->fCrateProcid,
			                                                             v_curMapping->fAddr,
			                                                             (unsigned int)iCh);

			// Add current per-channel mapping
			std::pair<std::map<unsigned int, stc_mapping*>::iterator, bool> ret;
			ret = this->mMappings.insert(std::pair<unsigned int, stc_mapping*>(v_curMappingChID, v_curMapping));
			if (ret.second == false) {
				// Key already exists in the map, this means that the channels' mapping is inconsistent
				cerr << "[FATAL ] Imported XML setup configuration is inconsistent. Please check it. Aborting." << endl;
				cerr << "         Channel " << "procid=" << v_curMapping->fCrateProcid << "\t"
				     << "addr=" << v_curMapping->fAddr << "\t"
				     << "ch=" << iCh << "\t"
				     << "from mapping (A) is already assigned in the mapping (B):" << endl;
				cerr << "         (A) "; DumpStcMapping(v_curMapping);
				cerr << "         (B) "; DumpStcMapping(ret.first->second);
				exit(EXIT_FAILURE);
				return;

			}

			//cerr << "PER-CHANNEL MAPPING: " << v_curMappingChID << " - " << v_curMapping->fStation << endl;

			unsigned short v_stationID = 65535;

			// Search for the current detector
			if (mDetectors.find(v_curMapping->fDetector) == mDetectors.end()) {
				// New detector found
				this->mDetectors.insert(std::pair<TString, unsigned short>(v_curMapping->fDetector, v_detCounter));

				cerr << "Found detector " << v_detCounter << ": " << v_curMapping->fDetector << " for the first time." << endl;

				// Add new station list for this detector with one station
				std::map<TString, unsigned short> v_newSet;

				v_stationID = v_detCounter * 100 + 0;

				v_newSet.insert(std::pair<TString, unsigned short>(v_curMapping->fStation, v_stationID));
				this->mStationsPerDet.insert(std::pair<TString, std::map<TString, unsigned short> >
				                            (v_curMapping->fDetector, v_newSet));

				cerr << "Adding station " << v_stationID << ": " << v_curMapping->fStation
				     << " into " << v_curMapping->fDetector << endl;

				v_detCounter++;

			} else {
				// Not new detector found
				// Add stations into the list

				std::map< TString, std::map<TString, unsigned short> >::iterator iterDet =
				    this->mStationsPerDet.find(v_curMapping->fDetector);

				if (iterDet == this->mStationsPerDet.end()) {
					// This should never happen
					cerr << "[FATAL ] No stations list was found for the detector." << endl;
					exit(EXIT_FAILURE);
					return;
				}

				// Find station
				std::map<TString, unsigned short>::iterator iterSt = iterDet->second.find(v_curMapping->fStation);

				if (iterSt == iterDet->second.end()) {
					// Station not found

					// Current size of the stations list of the current detector
					unsigned short v_curSize = iterDet->second.size();

					v_stationID = this->GetDetectorID(v_curMapping->fDetector) * 100 + v_curSize;

					iterDet->second.insert(std::pair<TString, unsigned short>(v_curMapping->fStation, v_stationID));
					cerr << "Adding station " << v_stationID << ": " << v_curMapping->fStation
					     << " into " << v_curMapping->fDetector << endl;
				} /*else {
					cerr << "Not new detector, not new station." << endl;
				}*/
			}

			// ------------------------------------------------------------------------------------------------
			// MWPC specific
			// ------------------------------------------------------------------------------------------------

			// Try to identify MWPC
			TString v_stationName(v_curMapping->fStation);
			if (v_stationName.Contains("mwpc", TString::kIgnoreCase) &&
			   !v_stationName.Contains("tmwpc", TString::kIgnoreCase) &&
			   v_stationID != 65535) {
				cerr << v_curMapping->fStation << " mapping found!" << endl;

				Ssiz_t pos = v_stationName.Index("mwpc", 0, TString::kIgnoreCase);
				TSubString v_MWPCst = v_stationName(pos+4, 2);
				TString v_MWPCst2(v_MWPCst);

				// MWPC stations start numbering from 1 but arrays - from 0
				Int_t MWPCstIndex = v_MWPCst2.Atoi() - 1;

				// We can call for this method here. I hope.
				fMWPCdetectorID[MWPCstIndex] = this->GetDetectorID(v_curMapping->fDetector);
				fMWPCstationID[MWPCstIndex] = v_stationID;

				//cerr << "i=" << MWPCstIndex << "\tdet=" << fMWPCdetectorID[MWPCstIndex]
				//     << "\tst=" << fMWPCstationID[MWPCstIndex] << endl;
			}

			// ------------------------------------------------------------------------------------------------

			if (++counter >= 10000) {
				// Something went completely wrong.
				cerr << "[FATAL ] Stuck in an infinite loop in SetupConfiguration::Link(). Aborting." << endl;
				exit(EXIT_FAILURE);
				return;
			}
		}

	}
}

/*static*/
unsigned short SetupConfiguration::ElChToDetCh(const stc_mapping* p_mapping, unsigned short p_elch)
{
	unsigned short v_stElCh = p_mapping->fStartelectrch;
	unsigned short v_nElCh = p_mapping->fNelectrch;
	unsigned short v_stepElCh = p_mapping->fStepelectrch;

	if ((p_elch < v_stElCh) || (p_elch >= v_stElCh+v_nElCh) || ((p_elch - v_stElCh) % v_stepElCh != 0) )
	{
		//ERROR
		cerr << "[ERROR ] Tested channel not in the expected channel set." << endl;
		//TODO think what to do in such situations
		return 9999;
	}

	unsigned short v_stStationCh = p_mapping->fStartstatch;

	return v_stStationCh + ((p_elch - v_stElCh) / v_stepElCh);
}

unsigned short SetupConfiguration::GetOutput(unsigned short p_crateProcid,
                                             unsigned short p_addr,
                                             unsigned short p_elch,
                                             TString* o_station,
                                             TString* o_detector,
                                             TString* o_elblock,
                                             unsigned short* o_detid,
                                             unsigned short* o_statid) const
{
	unsigned int v_detCh;
	unsigned int v_chUID = SetupConfiguration::GetChUID(p_crateProcid, p_addr, p_elch);

	//cerr << "[DEBUG] SetupConfiguration::GetOutput("
	//     << p_crateProcid << ", " << p_addr << ", " << p_elch << ") = (" << v_chUID << ") = ";

	//std::map<unsigned int, stc_mapping*>::const_iterator iter = mMappings.find(v_chUID);
	//if (iter != mMappings.end()) {

	const stc_mapping* theMapping = mMappings.at(v_chUID);
	{
		if (o_station != NULL) { *o_station = theMapping->fStation; }
		if (o_detector != NULL) { *o_detector = theMapping->fDetector; }
		if (o_elblock != NULL) {*o_elblock = theMapping->fElblock; }

		//TODO computation of the output detector channel is here
		v_detCh = SetupConfiguration::ElChToDetCh(theMapping, p_elch);

		//TODO
		if (o_detid != NULL) { *o_detid = this->GetDetectorID2(theMapping->fDetector); }
		if (o_statid != NULL) { *o_statid = this->GetStationID2(theMapping->fDetector, theMapping->fStation); }
	}/* else {
		//ERROR
		cerr << "[ERROR ] GetOutput() "
		     << "No mapping found for the given channel. "
		     << "procid=" << p_crateProcid << "\t"
		     << "addr=" << p_addr << "\t"
		     << "ch=" << p_elch << "\t"
		     << endl;
		//TODO think what to do in such situations
		v_detCh = 9999;
	}*/

	//cerr << *o_detector << endl;

	return v_detCh;
}

bool SetupConfiguration::IsMappedToScaler(unsigned short p_crateProcid,
                                          unsigned short p_addr,
                                          unsigned short p_elch,
                                          unsigned short p_messindex) const
{
	// p_elch is ignored here. p_messindex is taken as detector channel.

	//TODO one could check that the input channel is within the allowed range

	unsigned int v_chUID = SetupConfiguration::GetChUID(p_crateProcid, p_addr, p_messindex); //TODO check

	std::map<unsigned int, stc_mapping*>::const_iterator iter = mMappings.find(v_chUID);

	if (iter != mMappings.end())
	{
		TString v_stationName(iter->second->fStation);
		v_stationName.ToLower();
		if (v_stationName == "scalers") {
			return true;
		} else {
			return false;
		}
	} else {
		/*cerr << "[ERROR ] IsMappedToScaler() "
		     << "No mapping found for the given channel. "
		     << "procid=" << p_crateProcid << "\t"
		     << "addr=" << p_addr << "\t"
		     << "ch=" << p_elch << "\t"
		     << "messindex=" << p_messindex << "\t"
		     << endl;*/
		return false;
	}
}

bool SetupConfiguration::IsMappedToMachineTime(unsigned short p_crateProcid,
                                               unsigned short p_addr,
                                               unsigned short p_elch,
                                               unsigned short p_messindex) const
{
	// p_elch is ignored. p_messindex is checked to be 0

	// The machine time should be the only message of the subsubevent.
	if (p_messindex != 0) {
		return false;
	}

	unsigned int v_chUID = SetupConfiguration::GetChUID(p_crateProcid, p_addr, p_messindex); //TODO check

	std::map<unsigned int, stc_mapping*>::const_iterator iter = mMappings.find(v_chUID);

	if ((iter != mMappings.end()))
	{
		TString v_stationName(iter->second->fStation);
		v_stationName.ToLower();
		if (v_stationName == "mtime") {
			return true;
		} else {
			return false;
		}
	} else {
		/*cerr << "[ERROR ] IsMappedToMachineTime() "
		     << "No mapping found for the given channel. "
		     << "procid=" << p_crateProcid << "\t"
		     << "addr=" << p_addr << "\t"
		     << "ch=" << p_elch << "\t"
		     << "messindex=" << p_messindex << "\t"
		     << endl;*/
		return false;
	}
}

unsigned short SetupConfiguration::GetStationID(TString p_detector, TString p_station) const
{
	std::map< TString, std::map<TString, unsigned short> >::const_iterator iterDet = this->mStationsPerDet.find(p_detector);

	if (iterDet != this->mStationsPerDet.end()) {

		std::map<TString, unsigned short>::const_iterator iterStat = iterDet->second.find(p_station);

		if (iterStat != iterDet->second.end()) {
			return iterStat->second;
		} else {
			//ERROR
			cerr << "SetupConfiguration::GetStationID: ERROR" << endl;
			return 65535; //TODO
		}

	} else {
		//ERROR
		cerr << "SetupConfiguration::GetStationID: ERROR" << endl;
		return 65535; //TODO
	}
}

unsigned short SetupConfiguration::GetStationID2(TString p_detector, TString p_station) const
{
	return this->mStationsPerDet.at(p_detector).at(p_station);
}

unsigned short SetupConfiguration::GetDetectorID(TString p_detector) const
{
	std::map<TString, unsigned short>::const_iterator iter = this->mDetectors.find(p_detector);

	if (iter != this->mDetectors.end()) {
		return iter->second;
	} else {
		//ERROR
		cerr << "SetupConfiguration::GetDetectorID: ERROR" << endl;
		return 65535; //TODO
	}
}

unsigned short SetupConfiguration::GetDetectorID2(TString p_detector) const
{
	return this->mDetectors.at(p_detector);
}

std::map<TString, unsigned short> const SetupConfiguration::GetStationList(TString detector) const
{
	std::map< TString, std::map<TString, unsigned short> >::const_iterator iterDet = this->mStationsPerDet.find(detector);

	if (iterDet != this->mStationsPerDet.end()) {
		return iterDet->second;
	} else {
		//ERROR
		cerr << "SetupConfiguration::GetStationList: ERROR" << endl;
		//TODO return ?
	}
}

std::map<TString, unsigned short> const SetupConfiguration::GetStationList2(TString detector) const
{
	return this->mStationsPerDet.at(detector);
}

ClassImp(SetupConfiguration)
