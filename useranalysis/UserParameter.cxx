#include "UserParameter.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// Project
#include "setupconfigcppwrapper/SetupConfiguration.h"
#include "setupconfigcppwrapper/ElectronicsChars.h"

UserParameter::UserParameter(const char* name) :
	TGo4Parameter(name),
	fSetupConfiguration(nullptr),
	fElectrChars(nullptr)
{
}

void UserParameter::Init(void)
{
	// Construct SetupConfiguration, which includes the input of the XML file
	fSetupConfiguration = new SetupConfiguration(fSetupConfigFilename);
	fElectrChars = new ElectronicsChars(fElectrCharsFilename);
}

UserParameter::~UserParameter()
{
	if (fSetupConfiguration) { delete fSetupConfiguration; fSetupConfiguration = nullptr; }
	if (fElectrChars) { delete fElectrChars; fElectrChars = nullptr; }
}

Int_t UserParameter::PrintParameter(Text_t* buffer, Int_t buflen)
{
	cerr << "fInputFilename       = '" << fInputFilename << "'" << endl;
	cerr << "fSetupConfigFilename = '" << fSetupConfigFilename << "'" << endl;
	cerr << "fElectrCharsFilename = '" << fElectrCharsFilename << "'" << endl;
	cerr << "fOutputFilename      = '" << fOutputFilename << "'" << endl;
	fSetupConfiguration->Print();
	fElectrChars->Print();
	
	return 1; //TODO what should it return?
}

ClassImp(UserParameter)
