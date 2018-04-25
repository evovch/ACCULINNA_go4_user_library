#ifndef STRUCTS_H
#define STRUCTS_H

/* ============================================================================================= */
/* enumeration type enuTAG ===================================================================== */
/* ============================================================================================= */

typedef enum _enuTAG {NOTAG=0, SETUP=1, CRATE=2, MAPPING=3} enuTAG;

/* ============================================================================================= */
/* structure stc_mapping ======================================================================= */
/* ============================================================================================= */

typedef struct _stc_mapping {

	/* This information is extracted from the <crate> tag */

	char fCrateName[64];
	unsigned short fCrateProcid;

	/* This information is extracted from the <mapping> tag */

	unsigned short fAddr; /* Module for MESYTEC or geo for CAEN */
	char fElblock[64]; /* Name of the electronics block: mTDC, mADC, mQDC, V775, V785, V792, ... */
	unsigned short fStartelectrch;
	unsigned short fNelectrch;
	unsigned short fStepelecrtch;
	char fDetector[64]; /* Sub-detector: SQX_L, tSQX_L, CsI_L, tCsI_L, F3, F5, F6, tMWPC, neutTDC, ... */
	unsigned short fStartdetch;
	char fFolder[64]; /* Detector: Left_telescope, Right_telescope, Beam_detector, Neutron_detector */

} stc_mapping;

/* ============================================================================================= */
/* structure stc_setup_config ================================================================== */
/* ============================================================================================= */

typedef struct _stc_setup_config {

	/* This information is extracted from the <setup> tag */

	char fSetupName[64]; /* Name of the setup */
	unsigned short fSetupPeriod; /* Period */
	unsigned short fSetupRun; /* Run */
	char fSetupComment[128]; /* Comment */

	/* This list is extracted from the <crate> and <mapping> tags */

	unsigned short fNmappings; /* Number of <mapping> entries */
	stc_mapping* fMappingsList;

} stc_setup_config;

/* ============================================================================================= */
/* ============================================================================================= */
/* ============================================================================================= */

#endif /* STRUCTS_H */
