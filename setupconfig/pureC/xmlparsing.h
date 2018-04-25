#ifndef XMLPARSING_H
#define XMLPARSING_H

#include <stdio.h> /* for FILE */

#include "structs.h" /* for stc_setup_config, enuTAG */

#ifdef __cplusplus
extern "C" {
#endif

unsigned short CheckIfNextIsComment(FILE* f);

unsigned short ReadComment(FILE* f);

unsigned short ReadTag(FILE* f, char* o_buffer, unsigned short* o_buffersize);

void ProcessTag(stc_setup_config* ptr, char* o_buffer/*, enuTAG* o_CurTag*/);

void ProcessAttr(stc_setup_config* ptr, char* p_token/*, enuTAG* o_CurTag*/, unsigned short p_lastAttribute);

#ifdef __cplusplus
}
#endif

#endif /* XMLPARSING_H */
