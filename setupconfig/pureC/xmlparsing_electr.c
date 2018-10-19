#include "xmlparsing_electr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "enutag.h"
#include "structs_electr.h"
#include "functions_electr.h"

/* ================================================================================================ */
/* Global variables =============================================================================== */
/* ================================================================================================ */

enuTAG gCurTag2;
stc_elblock gCurBlockInfo;

/* ================================================================================================ */
/* ================================================================================================ */
/* ================================================================================================ */

void ProcessTag_electr(stc_electr_chars* ptr, char* o_buffer/*, enuTAG* o_CurTag*/)
{
	/* fprintf(stderr, "Processing:|%s|\n", o_buffer); */

	char* tok;
	size_t toklen;
	const char delims[] = " \t";

	/* One could init (i.e.) clear here - before processing the next tag */
	gCurTag2 = NOTAG; /* Current type of the tag being processed */

	ResetStcElblock(&gCurBlockInfo); /* Current elblock structure */

	tok = strtok(o_buffer, delims);

	/* TODO one could and one should implement many checks here */

	while (tok != NULL)
	{
		toklen = strlen(tok);
		/* fprintf(stderr, "size=%ld\n", toklen); */
		/* fprintf(stderr, "|%s|\n", tok); */

		/* Opening tag with no attributes */
		if ((tok[0] == '<') && (tok[1] != '/') && (tok[toklen-2] != '/') && (tok[toklen-1] == '>')) {
			tok[toklen-1] = '\0';
			/* fprintf(stderr, "opening no-attr tag: |%s|\n", &tok[1]); */

			if (strcmp(tok, "<electronics") == 0) {
				gCurTag2 = ELECTRONICS;
			} else if (strcmp(tok, "<elblock") == 0) {
				gCurTag2 = ELBLOCK;
			} else {
				/* ERROR */
				fprintf(stderr, "ERROR unknown opening tag %s.\n", &tok[1]);
				gCurTag2 = NOTAG;
				return;
			}
		}

		/* Opening tag with attributes */
		if ((tok[0] == '<') && (tok[1] != '/') && (tok[toklen-1] != '>')) {
			/* fprintf(stderr, "opening tag: |%s|\n", &tok[1]); */

			if (strcmp(tok, "<electronics") == 0) {
				gCurTag2 = ELECTRONICS;
			} else if (strcmp(tok, "<elblock") == 0) {
				gCurTag2 = ELBLOCK;
			} else {
				/* ERROR */
				fprintf(stderr, "ERROR unknown opening tag %s.\n", &tok[1]);
				gCurTag2 = NOTAG;
				return;
			}
		}

		/* Self-closing tag with no attributes */
		if ((tok[0] == '<') && (tok[1] != '/') && (tok[toklen-2] == '/') && (tok[toklen-1] == '>')) {
			tok[toklen-2] = '\0';
			/* fprintf(stderr, "self-closing no-attr tag: |%s|\n", &tok[1]); */
			/* TODO what do we do here? */
		}

		/* Closing tag with no attributes */
		if ((tok[0] == '<') && (tok[1] == '/') && (tok[toklen-2] != '/') && (tok[toklen-1] == '>')) {
			tok[toklen-1] = '\0';
			/* fprintf(stderr, "closing no-attr tag: |%s|\n", &tok[2]); */

			if (strcmp(tok, "</electronics") == 0) {
				gCurTag2 = NOTAG;
			} else if (strcmp(tok, "</elblock") == 0) {
				/* There should be no such closing tag.
				   The <elblock/> tag is self-closing */
				gCurTag2 = NOTAG;
			} else {
				/* ERROR */
				fprintf(stderr, "ERROR unknown closing tag %s.\n", &tok[2]);
				gCurTag2 = NOTAG;
				return;
			}

		}

		/* Not the last attribute */
		if ((tok[0] != '<') && (tok[toklen-1] != '>')) {
			/* fprintf(stderr, "attr: |%s|\n", tok); */

			char token2[1024];
			strncpy(token2, tok, 1024);
			ProcessAttr_electr(ptr, token2/*, o_CurTag*/, 0);

		}
		/* Last attribute */
		if ((tok[0] != '<') && (tok[toklen-1] == '>')) {
			if (tok[toklen-2] == '/') {
				/* self-closing - trim 2 symbols */
				tok[toklen-2] = '\0';
			} else {
				/* not self-closing - trim 1 symbol */
				tok[toklen-1] = '\0';
			}

			/* fprintf(stderr, "last attr: |%s|\n", tok); */

			if (strlen(tok) > 0) {
				/* TODO костыль, решающий пробелы между последним аттрибутом и знаком '>' */
				char token2[1024];
				strncpy(token2, tok, 1024);
				ProcessAttr_electr(ptr, token2/*, o_CurTag*/, 1);
			}
		}

		/* Go to the next token */
		tok = strtok (NULL, delims);
	} /* end of while over tokens */
}

void ProcessAttr_electr(stc_electr_chars* ptr, char* p_token/*, enuTAG* o_CurTag*/, unsigned short p_lastAttribute)
{
	/* Input token should look like */
	/* name="value" */

	size_t lastquotepos;
	size_t attnamelen;

	/* fprintf(stderr, "ProcessAttr_electr: '%s'\n", p_token); */

	/* Search for the equal sign */
	char* equalsign = strchr(p_token, '=');

	/* If not found - error */
	if (equalsign == NULL) {
		/* ERROR */
		fprintf(stderr, "ERROR [ProcessAttr_electr] attribute could not be parsed %s: no equal sign found.\n", p_token);
		fprintf(stderr, "      Most probably there is a space in a wrong place.\n");
		return;
	}

	/* Get the size of everything on the right from the equal sign */
	lastquotepos = strlen(equalsign)-1;
	/* Check that the 1) symbols after the equal sign and 2) the last symbol are the double-quote signs */
	/* If not - error */
	if ((equalsign[1] != '\"') || (equalsign[lastquotepos] != '\"')) {
		/* ERROR */
		fprintf(stderr, "ERROR [ProcessAttr_electr] attribute could not be parsed %s:", p_token);
		fprintf(stderr, "no double-quotes found or there are spaces between the equal sign and the double-quotes.\n");
		return;
	}

	/* TODO WOW WOW WOW. Pure C... */
	/* Break the input token into the name and the value string */
	attnamelen = equalsign-p_token;
	p_token[attnamelen] = '\0';
	equalsign[lastquotepos] = '\0';

	/* fprintf(stderr, "%s = %s\n", p_token, &equalsign[2]); */

	/* Extract attributes' values and store them into the current (this) electronics characteristics object */
	if (gCurTag2 == ELBLOCK) {
		if (strcmp(p_token, "name") == 0) {
			strncpy(gCurBlockInfo.fBlockName, &equalsign[2], 64);
		} else if (strcmp(p_token, "nchannels") == 0) {
			gCurBlockInfo.fNchannels = atoi(&equalsign[2]);
		} else if (strcmp(p_token, "rangelow") == 0) {
			gCurBlockInfo.fRangeLow = atoi(&equalsign[2]);
		} else if (strcmp(p_token, "rangehigh") == 0) {
			gCurBlockInfo.fRangeHigh = atoi(&equalsign[2]);
		} else if (strcmp(p_token, "nbins") == 0) {
			gCurBlockInfo.fNbins = atoi(&equalsign[2]);
		} else if (strcmp(p_token, "type") == 0) {
			strncpy(gCurBlockInfo.fType, &equalsign[2], 64);
		} else if (strcmp(p_token, "vendor") == 0) {
			strncpy(gCurBlockInfo.fVendor, &equalsign[2], 64);
		} else {
			/* ERROR unknown attribute */
			fprintf(stderr, "ERROR [ProcessAttr_electr] unknown attribute '%s'.\n", p_token);
			return;
		}

		if (p_lastAttribute == 1) {
			/*  Finish current ELBLOCK entity writeout */
			/*  DumpStcElblock(&gCurBlockInfo); */

			ExtendBlocksListStcElectrChars(ptr, &gCurBlockInfo);

			/* TODO reset current ELBLOCK values */
		}
	}
}
