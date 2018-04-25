#include "SetupConfig.h"

#include <stdio.h>

//TODO check that all data members are initialized
SetupConfig::SetupConfig() :
	TObject(),
	////fBuffer(""), //TODO
	fBufferSize(0),
	fCurTag(NOTAG),
	//fCurCrateName(""),
	//fCurCrateProcId(0),
	//fSetupName(""), //TODO
	fSetupPeriod(0),
	fSetupRun(0)
	//fSetupComment("") //TODO
{
	fNmappings = 0;
	fMappingsList = (struct stc_mapping*)malloc(fNmappings*sizeof(struct stc_mapping));
	//fprintf(stderr, "fMappingsList=%p, fNmappings=%u\n", fMappingsList, fNmappings);
}

SetupConfig::~SetupConfig()
{
	free(fMappingsList);
}

void SetupConfig::ImportXML(const char* p_filename)
{
	FILE* f = fopen(p_filename, "r");

	if (f == NULL) {
		fprintf(stderr, "Failed to open setup config XML file '%s'. Aborting.\n", p_filename);
		return;
	}
	fprintf(stderr, "Opened setup config XML file '%s'.\n", p_filename);

	unsigned short nReadSymbols = 0;

	do {
		if (this->CheckIfNextIsComment(f)) {
			nReadSymbols = this->ReadComment(f);
		} else {
			nReadSymbols = this->ReadTag(f);
			if (nReadSymbols > 0) {
				this->ProcessTag();
			}
		}
	}
	while (nReadSymbols > 0);


	fclose(f);
}

void SetupConfig::Dump() const
{
	fprintf(stderr, "======================== SETUP CONFIGURATION ===================================\n");
	fprintf(stderr, "     name='%s'\n", fSetupName);
	fprintf(stderr, "   period='%u'\n", fSetupPeriod);
	fprintf(stderr, "      run='%u'\n", fSetupRun);
	fprintf(stderr, "  comment='%s'\n", fSetupComment);
	fprintf(stderr, "================================================================================\n");
}

bool SetupConfig::CheckIfNextIsComment(FILE* f) const
{
	//char buffer[1024];
	unsigned short cursor;
	int readChar; // Read char as int - return value of fgetc

	bool insideTag = false;
	bool insideComment = false;

	cursor = 0;
	while ((readChar = fgetc(f)) != EOF) {
		//fprintf(stderr, "Checking: %c\n", (char)readChar);
		//buffer[cursor++] = readChar; //FIXME nasty type cast!
		cursor++; // This is instead of the prev. commented line
		if (readChar == ' ' || readChar == '\t' || readChar == '\n') {
			cursor--;
			// Ignore whitespace between the tags or comments
		}
		if (readChar == '<') {
			insideTag = true;
			break;
		}
	}

	// Look forward and check that the symbols are the expected !--
	// In any case - return back to the position before the '<' sign using fseek
	if (insideTag) {
		readChar = fgetc(f);
		if (readChar == '!') {
			readChar = fgetc(f);
			if (readChar == '-') {
				readChar = fgetc(f);
				if (readChar == '-') {
					insideComment = true;
				} else {
					insideComment = false;
				}
				//long int curPos = ftell(f);
				fseek(f, -4, SEEK_CUR);
				//TODO check fseek return value
			} else {
				//long int curPos = ftell(f);
				fseek(f, -3, SEEK_CUR);
				//TODO check fseek return value
			}
		} else {
			//long int curPos = ftell(f);
			fseek(f, -2, SEEK_CUR);
			//TODO check fseek return value
		}
	} else {
		//long int curPos = ftell(f);
		fseek(f, -1, SEEK_CUR);
		//TODO check fseek return value
	}

	return insideComment;
}

//TODO now reading, should just skip
unsigned short SetupConfig::ReadComment(FILE* f) const
{
	char buffer[10240]; //TODO temporarily large buffer.
	unsigned short cursor;
	int readChar; // Read char as int - return value of fgetc
	bool endFound = false;

	cursor = 0;
	while ((readChar = fgetc(f)) != EOF) {
		buffer[cursor++] = readChar; //FIXME nasty type cast!
		if (readChar == '-') {
			readChar = fgetc(f);
			buffer[cursor++] = readChar; //FIXME nasty type cast!
			if (readChar == '-') {
				readChar = fgetc(f);
				buffer[cursor++] = readChar; //FIXME nasty type cast!
				if (readChar == '>') {
					endFound = true;
					// buffer contains now the comment
					// cursor has the size of the buffer
					break;
				} else { }
			} else { }
		} else { }
	}
	// Skip 3 traling symbols --> for printing
	buffer[cursor/*-3*/] = '\0'; // This trailing zero allows using printf

	fprintf(stderr, "============================================================\n");
	fprintf(stderr, "%s\n", buffer/*+4*/); // Skip first 4 symbols <!--
	fprintf(stderr, "============================================================\n");

	return endFound ? cursor : 0;
}

unsigned short SetupConfig::ReadTag(FILE* f)
{
	unsigned short cursor;
	int readChar; // Read char as int - return value of fgetc
	bool endFound = false;

	cursor = 0;
	while ((readChar = fgetc(f)) != EOF) {
		fBuffer[cursor++] = readChar; //FIXME nasty type cast!
		if (readChar == '>') {
			endFound = true;
			// fBuffer contains now the tag
			// cursor has the size of the fBuffer
			break;
		}
	}
	// Skip 1 traling symbol > for further parsing
	fBuffer[cursor/*-1*/] = '\0'; // This trailing zero allows using printf
	fprintf(stderr, "|%s|\n", fBuffer);

	fBufferSize = cursor;

	return endFound ? cursor : 0;
}

void SetupConfig::ProcessTag(void)
{
	fprintf(stderr, "Processing:|%s|\n", fBuffer);

	char* tok;
	size_t toklen;
	const char delims[] = " \t";

	tok = strtok(fBuffer, delims);

	//TODO one could and one should implement many checks here

	while (tok != NULL)
	{
		toklen = strlen(tok);
		//fprintf(stderr, "size=%ld\n", toklen);
		//fprintf(stderr, "|%s|\n", tok);

		// Opening tag with no attributes
		if ((tok[0] == '<') && (tok[1] != '/') && (tok[toklen-2] != '/') && (tok[toklen-1] == '>')) {
			tok[toklen-1] = '\0';
			//fprintf(stderr, "opening no-attr tag: |%s|\n", &tok[1]);

			if (strcmp(tok, "<setup") == 0) {
				fCurTag = SETUP;
			} else if (strcmp(tok, "<crate") == 0) {
				fCurTag = CRATE;
			} else if (strcmp(tok, "<mapping") == 0) {
				fCurTag = MAPPING;
			} else {
				// ERROR
				fprintf(stderr, "ERROR unknown opening tag %s.\n", &tok[1]);
				fCurTag = NOTAG;
				return;
			}
		}

		// Opening tag with no attributes
		if ((tok[0] == '<') && (tok[1] != '/') && (tok[toklen-1] != '>')) {
			//fprintf(stderr, "opening tag: |%s|\n", &tok[1]);

			if (strcmp(tok, "<setup") == 0) {
				fCurTag = SETUP;
			} else if (strcmp(tok, "<crate") == 0) {
				fCurTag = CRATE;
			} else if (strcmp(tok, "<mapping") == 0) {
				fCurTag = MAPPING;
			} else {
				// ERROR
				fprintf(stderr, "ERROR unknown opening tag %s.\n", &tok[1]);
				fCurTag = NOTAG;
				return;
			}
		}

		// Self-closing tag with no attributes
		if ((tok[0] == '<') && (tok[1] != '/') && (tok[toklen-2] == '/') && (tok[toklen-1] == '>')) {
			tok[toklen-2] = '\0';
			//fprintf(stderr, "self-closing no-attr tag: |%s|\n", &tok[1]);
			//TODO what do we do here?
		}

		// Closing tag with no attributes
		if ((tok[0] == '<') && (tok[1] == '/') && (tok[toklen-2] != '/') && (tok[toklen-1] == '>')) {
			tok[toklen-1] = '\0';
			//fprintf(stderr, "closing no-attr tag: |%s|\n", &tok[2]);

			if (strcmp(tok, "</setup") == 0) {
				fCurTag = NOTAG;
			} else if (strcmp(tok, "</crate") == 0) {
				fCurTag = NOTAG;
			} else if (strcmp(tok, "</mapping") == 0) {
				fCurTag = NOTAG;
			} else {
				// ERROR
				fprintf(stderr, "ERROR unknown closing tag %s.\n", &tok[2]);
				fCurTag = NOTAG;
				return;
			}

		}

		// Not the last attribute
		if ((tok[0] != '<') && (tok[toklen-1] != '>')) {
			//fprintf(stderr, "attr: |%s|\n", tok);

			char token2[1024];
			strncpy(token2, tok, 1024);
			this->ProcessAttr(token2);

		}
		// Last attribute
		if ((tok[0] != '<') && (tok[toklen-1] == '>')) {
			if (tok[toklen-2] == '/') {
				// self-closing - trim 2 symbols
				tok[toklen-2] = '\0';
			} else {
				// not self-closing - trim 1 symbol
				tok[toklen-1] = '\0';
			}

			//fprintf(stderr, "last attr: |%s|\n", tok);

			if (strlen(tok) > 0) {
				//TODO костыль, решающий пробелы между последним аттрибутом и знаком '>'
				char token2[1024];
				strncpy(token2, tok, 1024);
				this->ProcessAttr(token2, true);
			}
		}

		// Go to the next token
		tok = strtok (NULL, delims);
	} // end of while over tokens
}

void SetupConfig::ProcessAttr(char* p_token, bool p_lastAttribute)
{
	// Input token should look like
	// name="value"

	//fprintf(stderr, "ProcessAttr: '%s'\n", p_token);

	// Search for the equal sign
	char* equalsign = strchr(p_token, '=');

	// If not found - error
	if (equalsign == NULL) {
		// ERROR
		fprintf(stderr, "ERROR [ProcessAttr] attribute could not be parsed %s: no equal sign found.\n", p_token);
		fprintf(stderr, "      Most probably there is a space in a wrong place.\n");
		return;
	}

	// Get the size of everything on the right from the equal sign
	size_t lastquotepos = strlen(equalsign)-1;
	// Check that the 1) symbols after the equal sign and 2) the last symbol are the double-quote signs
	// If not - error
	if ((equalsign[1] != '\"') || (equalsign[lastquotepos] != '\"')) {
		// ERROR
		fprintf(stderr, "ERROR [ProcessAttr] attribute could not be parsed %s:", p_token);
		fprintf(stderr, "no double-quotes found or there are spaces between the equal sign and the double-quotes.\n");
		return;
	}

	//TODO WOW WOW WOW. Pure C...
	// Break the input token into the name and the value string
	size_t attnamelen = equalsign-p_token;
	p_token[attnamelen] = '\0';
	equalsign[lastquotepos] = '\0';

	//fprintf(stderr, "%s = %s\n", p_token, &equalsign[2]);

	// Extract attributes' values and store them into the current (this) setup config object
	if (fCurTag == SETUP) {
		if (strcmp(p_token, "name") == 0) {
			strncpy(fSetupName, &equalsign[2], 64);
		} else if (strcmp(p_token, "period") == 0) {
			fSetupPeriod = atoi(&equalsign[2]);
		} else if (strcmp(p_token, "run") == 0) {
			fSetupRun = atoi(&equalsign[2]);
		} else if (strcmp(p_token, "comment") == 0) {
			strncpy(fSetupComment, &equalsign[2], 128);
			////fprintf(stderr, "fSetupComment=%s\n", fSetupComment);
		} else {
			//ERROR unknown attribute
			fprintf(stderr, "ERROR [ProcessAttr] unknown attribute '%s'.\n", p_token);
			return;
		}
	} else if (fCurTag == CRATE) {
		if (strcmp(p_token, "name") == 0) {
			//strncpy(fCurCrateName, &equalsign[2], 64);
			strncpy(fCurMappingInfo.fCrateName, &equalsign[2], 64);
		} else if (strcmp(p_token, "procid") == 0) {
			//fCurCrateProcId = atoi(&equalsign[2]);
			fCurMappingInfo.fCrateProcid = atoi(&equalsign[2]);
		} else {
			//ERROR unknown attribute
			fprintf(stderr, "ERROR [ProcessAttr] unknown attribute '%s'.\n", p_token);
			return;
		}
	} else if (fCurTag == MAPPING) {
		if (strcmp(p_token, "addr") == 0) {
			//fprintf(stderr, "crateName='%s' crateProcid='%u' %s='%s'\n",
			//        fCurCrateName, fCurCrateProcId, p_token, &equalsign[2]);
			fCurMappingInfo.fAddr = atoi(&equalsign[2]);
		} else if (strcmp(p_token, "elblock") == 0) {
			//fprintf(stderr, "crateName='%s' crateProcid='%u' %s='%s'\n",
			//        fCurCrateName, fCurCrateProcId, p_token, &equalsign[2]);
			strncpy(fCurMappingInfo.fElblock, &equalsign[2], 64);
		} else if (strcmp(p_token, "startelectrch") == 0) {
			//fprintf(stderr, "crateName='%s' crateProcid='%u' %s='%s'\n",
			//        fCurCrateName, fCurCrateProcId, p_token, &equalsign[2]);
			fCurMappingInfo.fStartelectrch = atoi(&equalsign[2]);
		} else if (strcmp(p_token, "nelectrch") == 0) {
			//fprintf(stderr, "crateName='%s' crateProcid='%u' %s='%s'\n",
			//        fCurCrateName, fCurCrateProcId, p_token, &equalsign[2]);
			fCurMappingInfo.fNelectrch = atoi(&equalsign[2]);
		} else if (strcmp(p_token, "stepelecrtch") == 0) {
			//fprintf(stderr, "crateName='%s' crateProcid='%u' %s='%s'\n",
			//        fCurCrateName, fCurCrateProcId, p_token, &equalsign[2]);
			fCurMappingInfo.fStepelecrtch = atoi(&equalsign[2]);
		} else if (strcmp(p_token, "detector") == 0) {
			//fprintf(stderr, "crateName='%s' crateProcid='%u' %s='%s'\n",
			//        fCurCrateName, fCurCrateProcId, p_token, &equalsign[2]);
			strncpy(fCurMappingInfo.fDetector, &equalsign[2], 64);
		} else if (strcmp(p_token, "startdetch") == 0) {
			//fprintf(stderr, "crateName='%s' crateProcid='%u' %s='%s'\n",
			//        fCurCrateName, fCurCrateProcId, p_token, &equalsign[2]);
			fCurMappingInfo.fStartdetch = atoi(&equalsign[2]);
		} else if (strcmp(p_token, "folder") == 0) {
			//fprintf(stderr, "crateName='%s' crateProcid='%u' %s='%s'\n",
			//        fCurCrateName, fCurCrateProcId, p_token, &equalsign[2]);
			strncpy(fCurMappingInfo.fFolder, &equalsign[2], 64);
		} else {
			//ERROR unknown attribute
			fprintf(stderr, "ERROR [ProcessAttr] unknown attribute '%s'.\n", p_token);
			return;
		}

		if (p_lastAttribute) {
			// Finish current MAPPING entity writeout
			fCurMappingInfo.Dump();

			this->ExtendMappingsList();
			//fMappingsList[fNmappings] = fCurMappingInfo;

			//TODO reset current MAPPING values
		}
	}
}

void SetupConfig::ExtendMappingsList(void)
{
	//fprintf(stderr, "fMappingsList=%p, fNmappings=%u\n", fMappingsList, fNmappings);
	struct stc_mapping* newPointer;
	newPointer = (struct stc_mapping*)realloc(fMappingsList, (fNmappings+1)*sizeof(struct stc_mapping));
	//fprintf(stderr, "newPointer=%p, fMappingsList=%p, fNmappings+1=%u\n", newPointer, fMappingsList, fNmappings+1);
	if (newPointer == NULL) {
		fprintf(stderr, "Memory reallocation failed. newPointer=%p\n", newPointer);
		//TODO fatal error
		return;
	}
	fMappingsList = newPointer;
	fNmappings++;
}

ClassImp(SetupConfig)
