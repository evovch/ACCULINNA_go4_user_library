#ifndef STC_MAPPING_H
#define STC_MAPPING_H

#include <TObject.h> // mother class

// STD
#include <stdio.h>

class stc_mapping : public TObject
{
	friend class SetupConfig;

private:
	// This information is filled from the <crate> tag

	char fCrateName[64];
	unsigned short fCrateProcid;

	// This information is filled from the <mapping> tag

	unsigned short fAddr; // Module for MESYTEC or geo for CAEN
	char fElblock[64]; // Name of the electronics block: mTDC, mADC, mQDC, V775, V785, V792, ...
	unsigned short fStartelectrch;
	unsigned short fNelectrch;
	unsigned short fStepelecrtch;
	char fDetector[64]; // Sub-detector: SQX_L, tSQX_L, CsI_L, tCsI_L, F3, F5, F6, tMWPC, neutTDC, ...
	unsigned short fStartdetch;
	char fFolder[64]; // Detector: Left_telescope, Right_telescope, Beam_detector, Neutron_detector

public:
	stc_mapping()
	{
		//fCrateName
		fCrateProcid = 0;
		fAddr = 0;
		//fElblock
		fStartelectrch = 0;
		fNelectrch = 0;
		fStepelecrtch = 1;
		//fDetector
		fStartdetch = 0;
		//fFolder
	}

	virtual ~stc_mapping() {;}

	/*stc_mapping(const char* p_cratename,
		 unsigned short p_crateprocid,
		 unsigned short p_addr,
		 const char* p_elblock,
		 unsigned short p_startelectrch,
		 unsigned short p_nelectrch,
		 unsigned short p_stepelecrtch,
		 const char* p_detector,
		 unsigned short p_startdetch,
		 const char* p_folder)
	{
		strncpy(fCrateName, p_cratename, 64);
		fCrateProcid = p_crateprocid;
		fAddr = p_addr;
		strncpy(fElblock, p_elblock, 64);
		fStartelectrch = p_startelectrch;
		fNelectrch = p_nelectrch;
		fStepelecrtch = p_stepelecrtch;
		strncpy(fDetector, p_detector, 64);
		fStartdetch = p_startdetch;
		strncpy(fFolder, p_folder, 64);
	}*/

	virtual void Dump(void) const // *MENU*
	{
		fprintf(stderr,
		"==================================================================================\n");
		fprintf(stderr,
		"MAPPING cratename='%s' crateprocid='%u' addr='%u' elblock='%s' "
		"startelectrch='%u' nelectrch='%u' stepelecrtch='%u' detector='%s' "
		"startdetch='%u' folder='%s'\n",
		fCrateName, fCrateProcid, fAddr, fElblock, fStartelectrch, fNelectrch,
		fStepelecrtch, fDetector, fStartdetch, fFolder);
		fprintf(stderr,
		"==================================================================================\n");
	}

	ClassDef(stc_mapping, 1);
};

ClassImp(stc_mapping)

#endif // STC_MAPPING_H
