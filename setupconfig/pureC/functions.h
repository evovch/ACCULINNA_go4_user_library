#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "structs.h" /* for stc_mapping, stc_setup_config */

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================================= */
/* stc_mapping ================================================================================= */
/* ============================================================================================= */

/**
 * Reset all the fields.
 */
void InitStcMapping(stc_mapping* ptr);

/**
 * Reset only the fields filled from the <mapping> tag.
 * Leave those filled from the <crate> tag untouched (fCrateName, fCrateProcid).
 */
void ResetStcMapping(stc_mapping* ptr);

/**
 * Print the structure into the stderr stream.
 */
void DumpStcMapping(const stc_mapping* ptr);

/* ============================================================================================= */
/* stc_setup_config ============================================================================ */
/* ============================================================================================= */

void InitStcSetupConfig(stc_setup_config* ptr);

void DestructStcSetupConfig(stc_setup_config* ptr);

void DumpStcSetupConfig(const stc_setup_config* ptr);

void ExtendMappingsListStcSetupConfig(stc_setup_config* ptr, const stc_mapping* ptrMapping);

void ImportXML(stc_setup_config* ptr, const char* filename);

#ifdef __cplusplus
}
#endif

#endif /* FUNCTIONS_H */
