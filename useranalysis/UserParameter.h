/**

	@class UserParameter

	In principle, setup configuration is something which can be considered as parameter
	from Go4 paradigm point of view. However we don't put it here but keep it accessible
	only by the UserRepacking step because it has to be used only by that step.
	Otherwise we loose the modular structure of the analysis.

*/

#ifndef USERPARAMETER_H
#define USERPARAMETER_H

#include <TGo4Parameter.h> // mother class

// ROOT
#include <TString.h>

class SetupConfiguration;

class UserParameter : public TGo4Parameter {
public:
	UserParameter(const char* name = "UserParameter");
	virtual ~UserParameter();

	/**
	 * Print the parameter set into the stderr stream
	 */
	virtual Int_t PrintParameter(Text_t* buffer=0, Int_t buflen=0); // *MENU*

	/**
	 * Import setup configuration XML file
	 */
	void Init(void);

	/**
	 * Modifier
	 */
	void SetInputFilename(TString p_filename) { mInputFilename = p_filename; }

	/**
	 * Modifier
	 */
	void SetSetupConfigFilename(TString p_filename) { mSetupConfigFilename = p_filename; }

	/**
	 * Modifier
	 */
	void SetOutputFilename(TString p_filename) { mOutputFilename = p_filename; }

	/**
	 * Accessor
	 */
	//TString GetInputFilename(void) const { return mInputFilename; }

	/**
	 * Accessor
	 */
	//TString GetSetupConfigFilename(void) const { return mSetupConfigFilename; }

	/**
	 * Accessor
	 */
	//TString GetOutputFilename(void) const { return mOutputFilename; }

	/**
	 *
	 */
	const SetupConfiguration* GetSetupConfig(void) const { return fSetupConfiguration; }

private:
	/**
	 * Input .lmd raw data file name
	 */
	TString mInputFilename;

	/**
	 * Input .xml setup configuration file name
	 */
	TString mSetupConfigFilename;

	/**
	 * Output .root processed data file name
	 */
	TString mOutputFilename;

	/**
	 * Setup configuration object
	 */
	SetupConfiguration* fSetupConfiguration;

	ClassDef(UserParameter, 1);
};

#endif // USERPARAMETER_H
