/**
	Usage: ./test ../../usr/setup2_exp201804.xml ../../usr/electronics2.xml
*/

#include <stdio.h> /* for fprintf */

#include "structs.h"
#include "functions.h"

#include "structs_electr.h"
#include "functions_electr.h"

int main(int argc, char** argv)
{
	stc_setup_config setupConfigObj;
	stc_electr_chars electrCharsObj;

	if (argc != 3) {
		fprintf(stderr, "Please, specify input setup config XML file and electronics characteristics XML file.\n");
		return 1;
	}

	InitStcSetupConfig(&setupConfigObj);
	ImportXML(&setupConfigObj, argv[1]);
	DumpStcSetupConfig(&setupConfigObj);

	DestructStcSetupConfig(&setupConfigObj);

	InitStcElectrChars(&electrCharsObj);
	ImportXML_electr(&electrCharsObj, argv[2]);
	DumpStcElectrChars(&electrCharsObj);

	DestructStcElectrChars(&electrCharsObj);

	return 0;
}
