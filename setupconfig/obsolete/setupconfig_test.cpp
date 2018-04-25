// STD
#include <iostream>

// ROOT
#include <TFile.h>

// Project
#include "SetupConfig.h"

int main(int argc, char** argv)
{
	SetupConfig* cfg = new SetupConfig();
	cfg->ImportXML("usr/setup.xml");
	cfg->Dump();

	TFile* cfgRootFile = new TFile("setupCfg.root", "RECREATE");
	cfg->Write();
	cfgRootFile->Close();

	return 0;
}

/**

g++ setupconfig_test.cpp -o test.exe -I`root-config --incdir` -std=c++11 -L. -lSetupConfig `root-config --libs`

*/
