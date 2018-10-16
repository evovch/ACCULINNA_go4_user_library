#ifndef COMMON_XML_H
#define COMMON_XML_H

#include <stdio.h> /* for FILE */

#ifdef __cplusplus
extern "C" {
#endif

unsigned short CheckIfNextIsComment(FILE* f);

unsigned short ReadComment(FILE* f);

unsigned short ReadTag(FILE* f, char* o_buffer, unsigned short* o_buffersize);

#ifdef __cplusplus
}
#endif

#endif /* COMMON_XML_H */
