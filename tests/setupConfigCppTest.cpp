#include <stdio.h> /* for fprintf */

#include <TFile.h>

#include "setupconfig/pureC/structs.h"
#include "setupconfig/pureC/functions.h"

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

	TFile* outputFile = new TFile("output.root", "RECREATE");
	setupConfigObj.Write();
	outputFile->Close();

	DestructStcSetupConfig(&setupConfigObj);

	return 0;
}
