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
				exit(1); //TODO
				return;

			}

			//cerr << v_curMappingChID << ": " << v_curMapping->fDetector << endl;

			if (++counter >= 10000) {
				// Something went completely wrong.
				cerr << "[FATAL ] Stuck in an infinite loop in SetupConfiguration::Link(). Aborting." << endl;
				exit(1); //TODO
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

/**
 * Return detector channel.
 * Write detector name into o_detector and folder name into o_folder
 */
unsigned short SetupConfiguration::GetOutput(unsigned short p_crateProcid,
                                             unsigned short p_addr,
                                             unsigned short p_elch,
                                             TString* o_detector,
                                             TString* o_folder) const
{
	unsigned int v_detCh;
	unsigned int v_chUID = SetupConfiguration::GetChUID(p_crateProcid, p_addr, p_elch);

	//cerr << "[DEBUG] SetupConfiguration::GetOutput("
	//     << p_crateProcid << ", " << p_addr << ", " << p_elch << ") = (" << v_chUID << ") = ";

	std::map<unsigned int, stc_mapping*>::const_iterator iter = mMappings.find(v_chUID);
	if (iter != mMappings.end()) {
		*o_detector = iter->second->fDetector;
		*o_folder = iter->second->fFolder;
		//TODO computation of the output detector channel is here
		v_detCh = SetupConfiguration::ElChToDetCh(iter->second, p_elch);
	} else {
		//ERROR
		cerr << "[ERROR ] " << "No mapping found for the given channel. "
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

ClassImp(SetupConfiguration)
