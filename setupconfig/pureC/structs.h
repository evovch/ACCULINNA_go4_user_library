#ifndef STRUCTS_H
#define STRUCTS_H

#ifdef __cplusplus
#include <TObject.h> /* mother class */
#include <TObjArray.h>
#endif /* __cplusplus */

/* ============================================================================================= */
/* enumeration type enuTAG ===================================================================== */
/* ============================================================================================= */

typedef enum _enuTAG {NOTAG=0, SETUP=1, CRATE=2, MAPPING=3} enuTAG;

/* ============================================================================================= */
/* structure stc_mapping ======================================================================= */
/* ============================================================================================= */

#ifdef __cplusplus
class stc_mapping : public TObject
{
public:
	stc_mapping() : TObject() {}
	virtual ~stc_mapping() {}

#else
typedef struct _stc_mapping {
#endif /* __cplusplus */

	/* This information is extracted from the <crate> tag */

	char fCrateName[64];
	unsigned short fCrateProcid;

	/* This information is extracted from the <mapping> tag */

	unsigned short fAddr; /* Module for MESYTEC or geo for CAEN */
	char fElblock[64]; /* Name of the electronics block: mTDC, mADC, mQDC, V775, V785, V792, ... */
	unsigned short fStartelectrch;
	unsigned short fNelectrch;
	unsigned short fStepelectrch;
	char fStation[64]; /* Station: SQX_L, tSQX_L, CsI_L, tCsI_L, F3, F5, F6, tMWPC, neutTDC, ... */
	unsigned short fStartstatch;
	char fDetector[64]; /* Detector: Left_telescope, Right_telescope, Beam_detector, Neutron_detector */
	char fDigicomp[64]; /* A string which identifies which component of the digi this is */

#ifdef __cplusplus
	ClassDef(stc_mapping, 1);
};
ClassImp(stc_mapping)
#else
} stc_mapping;
#endif /* __cplusplus */

/* ============================================================================================= */
/* structure stc_setup_config ================================================================== */
/* ============================================================================================= */

#ifdef __cplusplus
class stc_setup_config : public TObject
{
public:
	stc_setup_config() : TObject(), fMappingsList(nullptr) {}
	virtual ~stc_setup_config() {}

#else
typedef struct _stc_setup_config {
#endif /* __cplusplus */

	/* This information is extracted from the <setup> tag */

	char fSetupName[64]; /* Name of the setup */
	unsigned short fSetupPeriod; /* Period */
	unsigned short fSetupRun; /* Run */
	char fSetupComment[128]; /* Comment */

	/* This list is extracted from the <crate> and <mapping> tags */

	signed int fNmappings; /* Number of <mapping> entries */
	#ifdef __cplusplus
	stc_mapping* fMappingsList; //[fNmappings]
	#else
	stc_mapping* fMappingsList;
	#endif /* __cplusplus */

#ifdef __cplusplus
	ClassDef(stc_setup_config, 1);
};
ClassImp(stc_setup_config)
#else
} stc_setup_config;
#endif /* __cplusplus */

/* ============================================================================================= */
/* ============================================================================================= */
/* ============================================================================================= */

#endif /* STRUCTS_H */
