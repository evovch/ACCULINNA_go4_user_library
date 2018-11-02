#ifndef XMLPARSING_H
#define XMLPARSING_H

/*#include "enutag.h"*/ /* for enuTAG */
#include "structs.h" /* for stc_setup_config */

#ifdef __cplusplus
extern "C" {
#endif

void ProcessTag(stc_setup_config* ptr, char* o_buffer/*, enuTAG* o_CurTag*/);

void ProcessAttr(stc_setup_config* ptr, char* p_token/*, enuTAG* o_CurTag*/, unsigned short p_lastAttribute);

#ifdef __cplusplus
}
#endif

#endif /* XMLPARSING_H */
