#include "functions_electr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commonxml.h"
#include "xmlparsing_electr.h"

/* ============================================================================================= */
/* stc_elblock ================================================================================= */
/* ============================================================================================= */

void InitStcElblock(stc_elblock* ptr)
{
	ResetStcElblock(ptr);
}

void DumpStcElblock(const stc_elblock* ptr)
{
	/* fprintf(stderr,
	   "================================================================================"
	   "================================================================================\n"); */
	fprintf(stderr,
	"ELBLOCK name='%s' nchannels='%u' rangelow='%d' rangehigh='%d' "
	"nbins='%u' type='%s' vendor='%s'\n",
	ptr->fBlockName, ptr->fNchannels, ptr->fRangeLow, ptr->fRangeHigh,
	ptr->fNbins, ptr->fType, ptr->fVendor);
	/* fprintf(stderr,
	   "================================================================================"
	   "================================================================================\n"); */
}

void ResetStcElblock(stc_elblock* ptr)
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
	fprintf(stderr, "fBlocksList=%p, fNblocks=%u\n", (void*)(ptr->fBlocksList), ptr->fNblocks);
}

void DestructStcElectrChars(stc_electr_chars* ptr)
{
	#ifdef __cplusplus
	delete [] ptr->fBlocksList;
	#else
	free(ptr->fBlocksList);
	#endif /* __cplusplus */
}

void DumpStcElectrChars(const stc_electr_chars* ptr)
{
	unsigned short i;

	fprintf(stderr, "================================================================================\n");

	for (i=0; i<ptr->fNblocks; i++) {
		DumpStcElblock(&ptr->fBlocksList[i]);
	}

	fprintf(stderr, "================================================================================\n");
}

void ExtendBlocksListStcElectrChars(stc_electr_chars* ptr, const stc_elblock* ptrBlock)
{
	stc_elblock* curBlock;
	/* fprintf(stderr, "fBlocksList=%p, fNblocks=%u\n", (void*)(ptr->fBlocksList), ptr->fNblocks); */
	stc_elblock* newPointer;

	#ifdef __cplusplus
	newPointer = new stc_elblock[ptr->fNblocks+1];
	for (signed int i=0; i<ptr->fNblocks; i++) {
		newPointer[i] = ptr->fBlocksList[i];
	}
	delete [] ptr->fBlocksList;
	#else
	newPointer = (stc_elblock*)realloc(ptr->fBlocksList, (ptr->fNblocks+1)*sizeof(stc_elblock));
	#endif /* __cplusplus */

	/* fprintf(stderr, "newPointer=%p, fBlocksList=%p, fNblocks+1=%u\n",
	          (void*)newPointer, (void*)(ptr->fBlocksList), ptr->fNblocks+1); */
	if (newPointer == NULL) {
		fprintf(stderr, "ERROR memory reallocation failed. newPointer=%p\n", (void*)newPointer);
		/* TODO fatal error */
		exit(1);
		/* return;*/
	}
	ptr->fBlocksList = newPointer;

	/* Fill */
	curBlock = &(ptr->fBlocksList[ptr->fNblocks]);

	strncpy(curBlock->fBlockName, ptrBlock->fBlockName, 64);
	curBlock->fNchannels = ptrBlock->fNchannels;
	curBlock->fRangeLow = ptrBlock->fRangeLow;
	curBlock->fRangeHigh = ptrBlock->fRangeHigh;
	curBlock->fNbins = ptrBlock->fNbins;
	strncpy(curBlock->fType, ptrBlock->fType, 64);
	strncpy(curBlock->fVendor, ptrBlock->fVendor, 64);

	/* Increase the counter of the entities in the list */
	ptr->fNblocks++;
}

void ImportXML_electr(stc_electr_chars* ptr, const char* filename)
{
	/* Parsing state machine variables ------------------------ */
	char fBuffer[1024]; /* Buffer for input file reading */
	unsigned short fBufferSize=0; /* Current size of the buffer */
	/* -------------------------------------------------------- */

	unsigned short nReadSymbols = 0;

	FILE* f = fopen(filename, "r");

	if (f == NULL) {
		fprintf(stderr, "Failed to open setup config XML file '%s'. Aborting.\n", filename);
		exit(1);
		/* return; */
	}
	fprintf(stderr, "Opened setup config XML file '%s'.\n", filename);

	do {
		if (CheckIfNextIsComment(f)) {
			nReadSymbols = ReadComment(f);
		} else {
			nReadSymbols = ReadTag(f, fBuffer, &fBufferSize);
			if (nReadSymbols > 0) {
				ProcessTag_electr(ptr, fBuffer/*, &fCurTag*/);
			}
		}
	}
	while (nReadSymbols > 0);

	fclose(f);
}
