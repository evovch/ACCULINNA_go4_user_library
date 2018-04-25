#include "functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xmlparsing.h"

/* ============================================================================================= */
/* stc_mapping ================================================================================= */
/* ============================================================================================= */

/* TODO check that all data members are initialized */
void InitStcMapping(stc_mapping* ptr)
{
	strncpy(ptr->fCrateName, "", 64);
	ptr->fCrateProcid = 0;

	/*ptr->fAddr = 0;
	strncpy(ptr->fElblock, "", 64);
	ptr->fStartelectrch = 0;
	ptr->fNelectrch = 0;
	ptr->fStepelecrtch = 1;
	strncpy(ptr->fDetector, "", 64);
	ptr->fStartdetch = 0;
	strncpy(ptr->fFolder, "", 64);*/

	/* I'd better call for reset here: */
	ResetStcMapping(ptr);
}

/* TODO check that only necessary data members are initialized */
void ResetStcMapping(stc_mapping* ptr)
{
	ptr->fAddr = 0;
	strncpy(ptr->fElblock, "", 64);
	ptr->fStartelectrch = 0;
	ptr->fNelectrch = 0;
	ptr->fStepelecrtch = 1;
	strncpy(ptr->fDetector, "", 64);
	ptr->fStartdetch = 0;
	strncpy(ptr->fFolder, "", 64);
}

void DumpStcMapping(const stc_mapping* ptr)
{
	/* fprintf(stderr,
	   "================================================================================"
	   "================================================================================\n"); */
	fprintf(stderr,
	"MAPPING cratename='%s' crateprocid='%u' addr='%u' elblock='%s' "
	"startelectrch='%u' nelectrch='%u' stepelecrtch='%u' detector='%s' "
	"startdetch='%u' folder='%s'\n",
	ptr->fCrateName, ptr->fCrateProcid, ptr->fAddr, ptr->fElblock,
	ptr->fStartelectrch, ptr->fNelectrch, ptr->fStepelecrtch, ptr->fDetector,
	ptr->fStartdetch, ptr->fFolder);
	/* fprintf(stderr,
	   "================================================================================"
	   "================================================================================\n"); */
}

/* ============================================================================================= */
/* stc_setup_config ============================================================================ */
/* ============================================================================================= */

/* TODO check that all data members are initialized */
void InitStcSetupConfig(stc_setup_config* ptr)
{
	strncpy(ptr->fSetupName, "DefaultName", 64);
	ptr->fSetupPeriod = 0;
	ptr->fSetupRun = 0;
	strncpy(ptr->fSetupComment, "DefaultComment", 128);
	ptr->fNmappings = 0;
	ptr->fMappingsList = (stc_mapping*)malloc(ptr->fNmappings*sizeof(stc_mapping));
	/* fprintf(stderr, "fMappingsList=%p, fNmappings=%u\n", (void*)(ptr->fMappingsList), ptr->fNmappings); */
}

void DestructStcSetupConfig(stc_setup_config* ptr)
{
	free(ptr->fMappingsList);
}

void DumpStcSetupConfig(const stc_setup_config* ptr)
{
	unsigned short i;

	fprintf(stderr, "============================= SETUP CONFIGURATION ==============================\n");
	fprintf(stderr, "     name='%s'\n", ptr->fSetupName);
	fprintf(stderr, "   period='%u'\n", ptr->fSetupPeriod);
	fprintf(stderr, "      run='%u'\n", ptr->fSetupRun);
	fprintf(stderr, "  comment='%s'\n", ptr->fSetupComment);
	fprintf(stderr, "================================================================================\n");

	for (i=0; i<ptr->fNmappings; i++) {
		DumpStcMapping(&ptr->fMappingsList[i]);
	}

	fprintf(stderr, "================================================================================\n");
}

void ExtendMappingsListStcSetupConfig(stc_setup_config* ptr, const stc_mapping* ptrMapping)
{
	/* fprintf(stderr, "fMappingsList=%p, fNmappings=%u\n", (void*)(ptr->fMappingsList), ptr->fNmappings); */
	stc_mapping* newPointer;
	newPointer = (stc_mapping*)realloc(ptr->fMappingsList, (ptr->fNmappings+1)*sizeof(stc_mapping));
	/* fprintf(stderr, "newPointer=%p, fMappingsList=%p, fNmappings+1=%u\n",
	          (void*)newPointer, (void*)(ptr->fMappingsList), ptr->fNmappings+1); */
	if (newPointer == NULL) {
		fprintf(stderr, "ERROR memory reallocation failed. newPointer=%p\n", (void*)newPointer);
		/* TODO fatal error */
		exit(1);
		/* return;*/
	}
	ptr->fMappingsList = newPointer;

	/* Fill */
	strncpy(ptr->fMappingsList[ptr->fNmappings].fCrateName, ptrMapping->fCrateName, 64);
	ptr->fMappingsList[ptr->fNmappings].fCrateProcid = ptrMapping->fCrateProcid;
	ptr->fMappingsList[ptr->fNmappings].fAddr = ptrMapping->fAddr;
	strncpy(ptr->fMappingsList[ptr->fNmappings].fElblock, ptrMapping->fElblock, 64);
	ptr->fMappingsList[ptr->fNmappings].fStartelectrch = ptrMapping->fStartelectrch;
	ptr->fMappingsList[ptr->fNmappings].fNelectrch = ptrMapping->fNelectrch;
	ptr->fMappingsList[ptr->fNmappings].fStepelecrtch = ptrMapping->fStepelecrtch;
	strncpy(ptr->fMappingsList[ptr->fNmappings].fDetector, ptrMapping->fDetector, 64);
	ptr->fMappingsList[ptr->fNmappings].fStartdetch = ptrMapping->fStartdetch;
	strncpy(ptr->fMappingsList[ptr->fNmappings].fFolder, ptrMapping->fFolder, 64);

	/* Increase the counter of the entities in the list */
	ptr->fNmappings++;
}

void ImportXML(stc_setup_config* ptr, const char* filename)
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
				ProcessTag(ptr, fBuffer/*, &fCurTag*/);
			}
		}
	}
	while (nReadSymbols > 0);


	fclose(f);
}
