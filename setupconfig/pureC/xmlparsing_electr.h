#ifndef XMLPARSINGELECTR_H
#define XMLPARSINGELECTR_H

/*#include "enutag.h"*/ /* for enuTAG */
#include "structs_electr.h" /* for stc_electr_chars */

#ifdef __cplusplus
extern "C" {
#endif

void ProcessTag_electr(stc_electr_chars* ptr, char* o_buffer/*, enuTAG* o_CurTag*/);

void ProcessAttr_electr(stc_electr_chars* ptr, char* p_token/*, enuTAG* o_CurTag*/, unsigned short p_lastAttribute);

#ifdef __cplusplus
}
#endif

#endif /* XMLPARSINGELECTR_H */
