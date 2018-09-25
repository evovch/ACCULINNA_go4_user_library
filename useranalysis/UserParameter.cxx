#include "UserParameter.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// Project
#include "setupconfigcppwrapper/SetupConfiguration.h"

UserParameter::UserParameter(const char* name) :
	TGo4Parameter(name),
	fSetupConfiguration(nullptr)
{
}

void UserParameter::Init(void)
{
	// Construct SetupConfiguration, which includes the input of the XML file
	fSetupConfiguration = new SetupConfiguration(mSetupConfigFilename);
}

UserParameter::~UserParameter()
{
	if (fSetupConfiguration) delete fSetupConfiguration;
}

Int_t UserParameter::PrintParameter(Text_t* buffer, Int_t buflen)
{
	cerr << "mInputFilename       = '" << mInputFilename << "'" << endl;
	cerr << "mSetupConfigFilename = '" << mSetupConfigFilename << "'" << endl;
	cerr << "mOutputFilename      = '" << mOutputFilename << "'" << endl;
	fSetupConfiguration->Print();

	return 1; //TODO what should it return?
}

ClassImp(UserParameter)
