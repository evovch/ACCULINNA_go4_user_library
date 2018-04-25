#include "UserParameter.h"

// Project
#include "setupconfigcppwrapper/SetupConfiguration.h"

UserParameter::UserParameter(const char* name) :
	TGo4Parameter(name)
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

ClassImp(UserParameter)
