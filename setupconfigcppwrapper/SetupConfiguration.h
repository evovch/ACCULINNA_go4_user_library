/**

	@class SetupConfiguration

	C++ wrapper class around C structure stc_setup_config with some functionality.

*/

#ifndef SETUPCONFIGURATION_H
#define SETUPCONFIGURATION_H

#include <TObject.h> // mother class

// STD
#include <map>

// ROOT
#include <TString.h>

// Project
#include "setupconfig/pureC/structs.h"

class SetupConfiguration : public TObject
{
//TODO test this singleton implementation
private:
	static SetupConfiguration* mInstance;
public:
	static SetupConfiguration& GetInstance(void);
// end of test section

public:
	//TODO restrict default constructor SetupConfiguration()
	// Default constructor is required to enable ROOT streaming?
	SetupConfiguration();
	SetupConfiguration(const char* p_filename);
	virtual ~SetupConfiguration();

	/**
	 * Print configuration into the stderr stream
	 */
	void Print(Option_t* option = "") const;

	/**
	 * Return true if the given electronics channel is mapped
	 * to some detector channel.
	 */
	bool IsMapped(unsigned short p_crateProcid,
	              unsigned short p_addr,
	              unsigned short p_elch) const;

	/**
	 * When looking at the decimal representation of the channel unique ID from left to right:
	 * The first 3 digits are the crate ProcID;
	 * Next 2 digits are the module address (module for MESYTEC, geo for CAEN);
	 * Last 3 digits are the channel number of the module. 3 digits are reserved,
	 * so we can go up to 999-ch electronics blocks. Ok, at least 128...
	 * Example returned value chUID = 10105029: procid=101, addr=05, ch=029
	 */
	static unsigned int GetChUID(unsigned short p_crateProcid,
	                             unsigned short p_addr,
	                             unsigned short p_elch)
	{
		return p_crateProcid * 100000 + p_addr * 1000 + p_elch;
	}

	/**
	 * Extract procID (crate) from the UID (see comment of the GetChUID() method).
	 */
	static unsigned short GetProcIdFromUID(unsigned int p_uid) { return p_uid / 100000; }

	/**
	 * Extract address (geo/module) from the UID (see comment of the GetChUID() method).
	 */
	static unsigned short GetAddrFromUID(unsigned int p_uid) { return (p_uid % 100000) / 1000; }

	/**
	 * Extract electronics channel from the UID (see comment of the GetChUID() method).
	 */
	static unsigned short GetElChFromUID(unsigned int p_uid) { return p_uid % 1000; }

	/**
	 * Given an electronics block channel number p_elch and the mapping,
	 * this method returns corresponding detector channel.
	 * A situation may happed (though should not happen) that the tested electronics channel
	 * is not within the set of the channels described by the mapping. This emits an ERROR.
	 *
	 * Consider the following mapping with start el.ch = 3, el.ch.step = 2, n.el.ch = 6
	 * and the input p_elch = 6
	 *
	 *     |   |   |
	 *     3 4 5 6 7 8
	 *           ^
	 *
	 * Another example is when the input p_elch is outside of the [3;3+6) range, say, 2 or 9:
	 *
	 *         |   |   |
	 *     1 2 3 4 5 6 7 8 9 10
	 *       ^             ^
	 *
	 */
	static unsigned short ElChToDetCh(const stc_mapping* p_mapping, unsigned short p_elch);

	/**
	 * Given crate procid from subevent header, address (module for MESYTEC, geo for CAEN)
	 * from subsubevent header, and electronics block channel from the data word,
	 * return the mapped detector channel and write out detector name into o_detector
	 * and station name into o_station.
	 *
	 * TODO check - in case of CAEN scalers, input parameter p_elch is misused - //TODO re-check
	 *
	 * The raw message index is passed as channel (which corresponds to reality unfortunately)
	 */
	unsigned short GetOutput(unsigned short p_crateProcid,
	                         unsigned short p_addr,
	                         unsigned short p_elch,
	                         TString* o_station = NULL,
	                         TString* o_detector = NULL,
	                         TString* o_elblock = NULL,
	                         unsigned short* o_detid = NULL,
	                         unsigned short* o_statid = NULL) const;

	/**
	 * Returns true if the input tuple crate/addr/ch/messindex is mapped to one of the scalers
	 * channels. Unfortunately, there is no nice and neat way to identify whether some message
	 * comes from scalers or not. If we take message index as channel and compute unique channel
	 * ID (see GetChUID() method), then we may run into non-existing values, which, if we want
	 * to be on the safe side, should generate errors or warnings. Due to this reason, error
	 * messages are commented in the implementation. This does not affect the functionality,
	 * only limits verbosity.
	 */
	bool IsMappedToScaler(unsigned short p_crateProcid,
	                      unsigned short p_addr,
	                      unsigned short p_elch,
	                      unsigned short p_messindex) const;

	/**
	 * Returns true if the input pair p_crateProcid/p_addr is mapped to machine time.
	 * p_elch is ignored. p_messindex is checked to be 0.
	 * Unfortunately, there is no nice and neat way to identify whether some message
	 * comes from RIO-machine-time-source or not. If we take message index as channel and
	 * compute unique channel ID (see GetChUID() method), then we may run into non-existing
	 * values, which, if we want to be on the safe side, should generate errors or warnings.
	 * Due to this reason, error messages are commented in the implementation.
	 * This does not affect the functionality, only limits verbosity.
	 */
	bool IsMappedToMachineTime(unsigned short p_crateProcid,
	                           unsigned short p_addr,
	                           unsigned short p_elch,
	                           unsigned short p_messindex) const;

	/**
	 * Perform checks of the imported XML configuration.
	 * Try to identify error or possible mistakes there.
	 */
	bool CheckConsistency(void);

	/**
	 *
	 */
	unsigned short GetStationID(TString p_detector, TString p_station) const;
	unsigned short GetStationID2(TString p_detector, TString p_station) const;

	/**
	 * Get the list of detectors imported from the setup configuration file.
	 */
	std::map<TString, unsigned short> const GetDetectorList(void) const { return mDetectors; }

	/**
	 * Get the number of detectors.
	 */
	size_t GetNdetectors(void) const { return mDetectors.size(); }

	/**
	 *
	 */
	unsigned short GetDetectorID(TString p_detector) const;
	unsigned short GetDetectorID2(TString p_detector) const;

	/**
	 * Get the list of stations for the given detector.
	 */
	std::map<TString, unsigned short> const GetStationList(TString detector) const;
	std::map<TString, unsigned short> const GetStationList2(TString detector) const;

	/**
	 *
	 */
	std::map<unsigned int, stc_mapping*> const GetMappings(void) const { return mMappings; }

private: // methods
	/**
	 * The most importatnd method. Goes through the imported structure and
	 * creates different maps, sets, ...
	 */
	void Link(void);

private: // data members
	/**
	 *
	 */
	stc_setup_config mConfiguration;

	/**
	 * Filled during the Link() method.
	 */
	std::map<unsigned int, stc_mapping*> mMappings; //! key - unique channel ID, see GetChUID()

	/**
	 * List of detectors.
	 * Filled during the Link() method.
	 */
	std::map<TString, unsigned short> mDetectors; //! key - detector ID

	/**
	 * List of stations per detector.
	 * Filled during the Link() method.
	 */
	std::map< TString, std::map<TString, unsigned short> > mStationsPerDet; //! TODO probably, we don't need to stream this

// ------------------------------------------------------------------------------------------------
// MWPC specific
// ------------------------------------------------------------------------------------------------
public:

	unsigned short fMWPCdetectorID[4];
	unsigned short fMWPCstationID[4];

// ------------------------------------------------------------------------------------------------

	ClassDef(SetupConfiguration, 1);
};

#endif // SETUPCONFIGURATION_H
