#include "commonxml.h"

unsigned short CheckIfNextIsComment(FILE* f)
{
	/* char buffer[1024]; */
	unsigned short cursor;
	int readChar; /* Read char as int - return value of fgetc */

	unsigned short insideTag = 0;
	unsigned short insideComment = 0;

	cursor = 0;
	while ((readChar = fgetc(f)) != EOF) {
		/* fprintf(stderr, "Checking: %c\n", (char)readChar); */
		/* buffer[cursor++] = readChar; // FIXME nasty type cast! */
		cursor++; /* This is instead of the prev. commented line */
		if (readChar == ' ' || readChar == '\t' || readChar == '\n') {
			cursor--;
			/* Ignore whitespace between the tags or comments */
		}
		if (readChar == '<') {
			insideTag = 1;
			break;
		}
	}

	/* Look forward and check that the symbols are the expected !-- */
	/* In any case - return back to the position before the '<' sign using fseek */
	if (insideTag) {
		readChar = fgetc(f);
		if (readChar == '!') {
			readChar = fgetc(f);
			if (readChar == '-') {
				readChar = fgetc(f);
				if (readChar == '-') {
					insideComment = 1;
				} else {
					insideComment = 0;
				}
				/* long int curPos = ftell(f); */
				fseek(f, -4, SEEK_CUR);
				/* TODO check fseek return value */
			} else {
				/* long int curPos = ftell(f); */
				fseek(f, -3, SEEK_CUR);
				/* TODO check fseek return value */
			}
		} else {
			/* long int curPos = ftell(f); */
			fseek(f, -2, SEEK_CUR);
			/* TODO check fseek return value */
		}
	} else {
		/* long int curPos = ftell(f); */
		fseek(f, -1, SEEK_CUR);
		/* TODO check fseek return value */
	}

	return insideComment;
}

/* TODO now reading, should just skip */
unsigned short ReadComment(FILE* f)
{
	char buffer[10240]; /* TODO temporarily large buffer */
	unsigned short cursor;
	int readChar; /*  Read char as int - return value of fgetc */
	unsigned short endFound = 0;

	cursor = 0;
	while ((readChar = fgetc(f)) != EOF) {
		buffer[cursor++] = readChar; /* FIXME nasty type cast! */
		if (readChar == '-') {
			readChar = fgetc(f);
			buffer[cursor++] = readChar; /* FIXME nasty type cast! */
			if (readChar == '-') {
				readChar = fgetc(f);
				buffer[cursor++] = readChar; /* FIXME nasty type cast! */
				if (readChar == '>') {
					endFound = 1;
					/*  buffer contains now the comment */
					/*  cursor has the size of the buffer */
					break;
				} else { }
			} else { }
		} else { }
	}
	/* Skip 3 traling symbols --> for printing */
	buffer[cursor/*-3*/] = '\0'; /* This trailing zero allows using printf */

	/* fprintf(stderr, "============================================================\n");
	   fprintf(stderr, "%s\n", buffer); // (buffer+4) to skip first 4 symbols <!--
	   fprintf(stderr, "============================================================\n"); */

	return (endFound==1) ? cursor : 0;
}

unsigned short ReadTag(FILE* f, char* o_buffer, unsigned short* o_buffersize)
{
	unsigned short cursor;
	int readChar; /* Read char as int - return value of fgetc */
	unsigned short endFound = 0;

	cursor = 0;
	while ((readChar = fgetc(f)) != EOF) {
		o_buffer[cursor++] = readChar; /* FIXME nasty type cast! */
		if (readChar == '>') {
			endFound = 1;
			/* o_buffer contains now the tag */
			/* cursor has the size of the o_buffer */
			break;
		}
	}
	/* Skip 1 traling symbol > for further parsing */
	o_buffer[cursor/*-1*/] = '\0'; /* This trailing zero allows using printf */
	/* fprintf(stderr, "|%s|\n", o_buffer); */

	*o_buffersize = cursor;

	return (endFound==1) ? cursor : 0;
}
