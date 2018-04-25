#include <stdio.h> /* for fprintf */

#include "structs.h"
#include "functions.h"

int main(int argc, char** argv)
{
	stc_setup_config setupConfigObj;

	if (argc != 2) {
		fprintf(stderr, "Please, specify input setup config XML file.\n");
		return 1;
	}

	InitStcSetupConfig(&setupConfigObj);
	ImportXML(&setupConfigObj, argv[1]);
	DumpStcSetupConfig(&setupConfigObj);

	DestructStcSetupConfig(&setupConfigObj);

	return 0;
}
