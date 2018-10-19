#ifndef FUNCTIONS_ELECTR_H
#define FUNCTIONS_ELECTR_H

#include "structs_electr.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================================= */
/* stc_elblock ================================================================================= */
/* ============================================================================================= */

/**
 * Just call for ResetStcElblock.
 */
void InitStcElblock(stc_elblock* ptr);

/**
 * Reset all the fields.
 */
void ResetStcElblock(stc_elblock* ptr);

/**
 * Print the structure into the stderr stream.
 */
void DumpStcElblock(const stc_elblock* ptr);

/* ============================================================================================= */
/* stc_electr_chars ============================================================================ */
/* ============================================================================================= */

void InitStcElectrChars(stc_electr_chars* ptr);

void DestructStcElectrChars(stc_electr_chars* ptr);

void DumpStcElectrChars(const stc_electr_chars* ptr);

void ExtendBlocksListStcElectrChars(stc_electr_chars* ptr, const stc_elblock* ptrBlock);

void ImportXML_electr(stc_electr_chars* ptr, const char* filename);

#ifdef __cplusplus
}
#endif

#endif /* FUNCTIONS_ELECTR_H */
