#include "functions_electr.h"

#include <stdlib.h>
#include <string.h>

/* ============================================================================================= */
/* stc_elblock ================================================================================= */
/* ============================================================================================= */

void InitStcElblock(stc_elblock* ptr)
{
	strncpy(ptr->fBlockName, "", 64);
	ptr->fNchannels = 32;
	ptr->fRangeLow = 0;
	ptr->fRangeHigh = 1;
	ptr->fNbins = 1;
	strncpy(ptr->fType, "", 64);
	strncpy(ptr->fVendor, "", 64);
}

/* ============================================================================================= */
/* stc_electr_chars ============================================================================ */
/* ============================================================================================= */

void InitStcElectrChars(stc_electr_chars* ptr)
{
	ptr->fNblocks = 0;
	#ifdef __cplusplus
	ptr->fBlocksList = new stc_elblock[ptr->fNblocks];
	#else
	ptr->fBlocksList = (stc_elblock*)malloc(ptr->fNblocks*sizeof(stc_elblock));
	#endif /* __cplusplus */

	/* TODO finalize... */
}
