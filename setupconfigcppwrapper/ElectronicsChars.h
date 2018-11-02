/**

	@class ElectronicsChars

	Wrapper class around the stc_electr_chars structure/class.
	Contains information imported from the electronics characteristics XML file.
	The idea of this class is similar to the idea of the SetupConfiguration class.
	It basically provides a more user-friendly interface to the data contained
	in its main data member stc_electr_chars fCharacteristics.

*/

#ifndef ELECTRONICSCHARS_H
#define ELECTRONICSCHARS_H

#include <TObject.h> // mother class

// STD
#include <map>

// ROOT
#include <TString.h>

// Project
#include "setupconfig/pureC/structs_electr.h"

class ElectronicsChars : public TObject
{
public:
	ElectronicsChars();
	ElectronicsChars(const char* p_filename);
	virtual ~ElectronicsChars();

	/**
	 * Print characteristics into the stderr stream
	 */
	void Print(Option_t* option = "") const;

	/**
	 * Perform checks of the imported XML characteristics.
	 * Try to identify error or possible mistakes there.
	 */
	bool CheckConsistency(void);

	signed short GetRangeLow(TString p_blockName) const;

	signed short GetRangeHigh(TString p_blockName) const;

	unsigned short GetNbins(TString p_blockName) const;

private: // methods
	/**
	 *
	 */
	void Link(void);

private: // data members
	/**
	 *
	 */
	stc_electr_chars fCharacteristics;

	/**
	 * Filled during the Link() method.
	 */
	std::map<TString, stc_elblock*> fBlocks;

	ClassDef(ElectronicsChars, 1);
};

#endif // ELECTRONICSCHARS_H
