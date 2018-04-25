/**

	@class SetupConfig

	We have to do using as simple as possible because there is a plan
	to try to re-use this code on the RIO controllers.
	I would try to use pure C.
	Don't blame me for this code. This could have been done so much nicer
	with C++ or an external library such as libxml. However this way it is fun!
	Luckily, the parsed config files are small so we can use no dynamic
	memory.

*/

#ifndef SETUPCONFIG_H
#define SETUPCONFIG_H

#include <TObject.h> // mother class

#include "stc_mapping.h"

enum enuTAG {NOTAG=0, SETUP=1, CRATE=2, MAPPING=3};

class SetupConfig : public TObject
{
public: // methods
	SetupConfig();
	virtual ~SetupConfig();

	void ImportXML(const char* p_filename);

	virtual void Dump() const; // *MENU*

private: // data members

	//TODO seems not to work
	// Data members required only during parsing
	// Transient data members. See:
	// https://root.cern.ch/root/htmldoc/guides/users-guide/InputOutput.html#transient-data-members

	char fBuffer[1024]; //! Buffer for input file reading
	unsigned short fBufferSize; //! Current size of the buffer
	enuTAG fCurTag; //! Current tag being processed

	stc_mapping fCurMappingInfo; //!

	//char fCurCrateName[64]; //! Current crate name
	//unsigned short fCurCrateProcId; //! Current crate procid

private: // methods
	bool CheckIfNextIsComment(FILE* f) const;
	unsigned short ReadComment(FILE* f) const;
	unsigned short ReadTag(FILE* f);

	/**
	 * Process the last read tag which is inside fBuffer and has the size fBufferSize
	 */
	void ProcessTag(void);

	void ProcessAttr(char* p_token, bool p_lastAttribute = false);

	void ExtendMappingsList(void);

private: // data members

	// Data members which need to be stored

	// This information is extracted from the <setup> tag
	char fSetupName[64]; // Name of the setup
	unsigned short fSetupPeriod; // Period
	unsigned short fSetupRun; // Run
	char fSetupComment[128]; // Comment

	// TClonesArray should have been used. But we want to use no ROOT!
	//! This is not correct! We want to save this in the output ROOT file
	stc_mapping* fMappingsList; //!

	unsigned short fNmappings; // Number of <mapping> entries

	ClassDef(SetupConfig, 1);
};

#endif // SETUPCONFIG_H
