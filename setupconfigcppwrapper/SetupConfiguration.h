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
public:
	//TODO restrict default constructor SetupConfiguration()
	// Default constructor is required to enable ROOT streaming?
	SetupConfiguration();
	SetupConfiguration(const char* p_filename);
	virtual ~SetupConfiguration();

	/**
	 * When looking at the decimal representation of the channel unique ID from left to right:
	 * The first 3 digits are the crate ProcID
	 * Next 2 digits are the module address (module for MESYTEC, geo for CAEN)
	 * Last 3 digits are the channel number of the module. 3 digits are reserved,
	 * so we can go up to 999-ch electronics blocks. Ok, at least 128...
	 * Example returned value chUID = 10105029: procid=101, addr=05, ch=029
	 */
	static unsigned int GetChUID(unsigned short p_crateProcid, unsigned short p_addr, unsigned short p_elch)
	{
		return p_crateProcid * 100000 + p_addr * 1000 + p_elch;
	}

	/**
	 * Given an electronics block channel number p_elch and the mapping,
	 * this method returns corresponding detector channel.
	 * A situation may happed (though should not happen) that the tested electronics channel
	 * is not within the set of the channels described by the mapping. This emits an ERROR.
	 * Consider the following mapping with start el.ch = 3, el.ch.step = 2, n.el.ch = 6
	 * and the input p_elch = 6
	 * |   |   |
	 * 3 4 5 6 7 8
	 *       ^
	 * Another example is when the input p_elch is outside of the [3;3+6) range, say, 2 or 9:
	 *     |   |   |
	 * 1 2 3 4 5 6 7 8 9 10
	 *   ^             ^
	 */
	static unsigned short ElChToDetCh(const stc_mapping* p_mapping, unsigned short p_elch);

	/**
	 * Given crate procid from subevent header, address (module for MESYTEC, geo for CAEN)
	 * from subsubevent header, and electronics block channel from the data word,
	 * return the mapped detector channel and write out detector name into o_detector
	 * and folder name into o_folder.
	 */
	unsigned short GetOutput(unsigned short p_crateProcid,
	                         unsigned short p_addr,
	                         unsigned short p_elch,
	                         TString* o_detector,
	                         TString* o_folder) const;

	/**
	 * Perform checks of the imported XML configuration.
	 * Try to identify error os possible mistakes there.
	 */
	bool CheckConsistency(void);

private: // methods
	/**
	 *
	 */
	void Link(void);

private: // data members
	/**
	 *
	 */
	stc_setup_config mConfiguration;

	/**
	 *
	 */
	std::map<unsigned int, stc_mapping*> mMappings; //! key - unique channel ID, see GetChUID()

	ClassDef(SetupConfiguration, 1);
};

#endif // SETUPCONFIGURATION_H
