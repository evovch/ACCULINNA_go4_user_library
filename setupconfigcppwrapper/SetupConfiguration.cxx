#include "SetupConfiguration.h"

// STD //TODO delete?
#include <iostream>
using std::cerr;
using std::endl;

// Project
#include "setupconfig/pureC/functions.h"

SetupConfiguration::SetupConfiguration() :
	TObject()
{
}

SetupConfiguration::SetupConfiguration(const char* p_filename) :
	TObject()
{
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
	// For each mapping
	for (unsigned short iMapping=0; iMapping<mConfiguration.fNmappings; iMapping++) {
		stc_mapping* v_curMapping = &(mConfiguration.fMappingsList[iMapping]);

		// Check that the channel step > 0, otherwise we will run unto the infinite loop
		if (v_curMapping->fStepelecrtch == 0) {
			// Actually, this should not happen. To be on the safe side we reset it to 1.
			v_curMapping->fStepelecrtch = 1;
		}

		////DumpStcMapping(v_curMapping);

		unsigned short counter = 0;
		for (unsigned short iCh = v_curMapping->fStartelectrch;
		                    iCh < v_curMapping->fStartelectrch + v_curMapping->fNelectrch;
		                    iCh += v_curMapping->fStepelecrtch)
		{
			unsigned int v_curMappingChID = SetupConfiguration::GetChUID(v_curMapping->fCrateProcid,
			                                                             v_curMapping->fAddr,
			                                                             (unsigned int)iCh);

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

			cerr << "PER-CHANNEL MAPPING: " << v_curMappingChID << " - " << v_curMapping->fDetector << endl;

			if (mDetectors.find(v_curMapping->fFolder) == mDetectors.end()) {
				// New folder (actually detector, not station) found
				this->mDetectors.insert(v_curMapping->fFolder);
				std::set<TString> v_newSet;
				v_newSet.insert(v_curMapping->fDetector);
				this->mStationsPerDet.insert(std::pair<TString, std::set<TString> >
				                            (v_curMapping->fFolder, v_newSet));

				cerr << "Found folder " << v_curMapping->fFolder << " for the first time." << endl;
				cerr << "Adding station " << v_curMapping->fDetector << " into " << v_curMapping->fFolder << endl;

			} else {
				// Not new folder (actually detector, not station) found
				// Add stations (which is called detector here, at least by now) into the list

				std::map< TString, std::set<TString> >::iterator iterDet =
				    this->mStationsPerDet.find(v_curMapping->fFolder);

				std::set<TString>::iterator iterSt = iterDet->second.find(v_curMapping->fDetector);

				if (iterSt == iterDet->second.end()) {
					iterDet->second.insert(v_curMapping->fDetector);
					cerr << "Adding station " << v_curMapping->fDetector << " into " << v_curMapping->fFolder << endl;
				}
			}

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
	unsigned short v_stepElCh = p_mapping->fStepelecrtch;

	if ((p_elch < v_stElCh) || (p_elch >= v_stElCh+v_nElCh) || ((p_elch - v_stElCh) % v_stepElCh != 0) )
	{
		//ERROR
		cerr << "[ERROR ] Tested channel not in the expected channel set." << endl;
		//TODO think what to do in such situations
		return 9999;
	}

	unsigned short v_stDetCh = p_mapping->fStartdetch;

	return v_stDetCh + ((p_elch - v_stElCh) / v_stepElCh);
}

unsigned short SetupConfiguration::GetOutput(unsigned short p_crateProcid,
                                             unsigned short p_addr,
                                             unsigned short p_elch,
                                             TString* o_detector,
                                             TString* o_folder,
                                             TString* o_elblock) const
{
	unsigned int v_detCh;
	unsigned int v_chUID = SetupConfiguration::GetChUID(p_crateProcid, p_addr, p_elch);

	//cerr << "[DEBUG] SetupConfiguration::GetOutput("
	//     << p_crateProcid << ", " << p_addr << ", " << p_elch << ") = (" << v_chUID << ") = ";

	std::map<unsigned int, stc_mapping*>::const_iterator iter = mMappings.find(v_chUID);
	if (iter != mMappings.end()) {
		if (o_detector != NULL) { *o_detector = iter->second->fDetector; }
		if (o_folder != NULL) { *o_folder = iter->second->fFolder; }
		if (o_elblock != NULL) {*o_elblock = iter->second->fElblock; }

		//TODO computation of the output detector channel is here
		v_detCh = SetupConfiguration::ElChToDetCh(iter->second, p_elch);
	} else {
		//ERROR
		cerr << "[ERROR ] GetOutput() "
		     << "No mapping found for the given channel. "
		     << "procid=" << p_crateProcid << "\t"
		     << "addr=" << p_addr << "\t"
		     << "ch=" << p_elch << "\t"
		     << endl;
		//TODO think what to do in such situations
		v_detCh = 9999;
	}

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
		TString v_detectorName(iter->second->fDetector);
		v_detectorName.ToLower();
		if (v_detectorName == "scalers") {
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
		TString v_detectorName(iter->second->fDetector);
		v_detectorName.ToLower();
		if (v_detectorName == "mtime") {
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

std::set<TString> const SetupConfiguration::GetStationList(TString detector) const
{
	std::map< TString, std::set<TString> >::const_iterator iterDet = this->mStationsPerDet.find(detector);
	return iterDet->second;
}

ClassImp(SetupConfiguration)
