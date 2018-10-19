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
class ElectronicsChars;

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
	void SetInputFilename(TString p_filename) { fInputFilename = p_filename; }

	/**
	 * Modifier
	 */
	void SetSetupConfigFilename(TString p_filename) { fSetupConfigFilename = p_filename; }

	/**
	 * Modifier
	 */
	void SetElectrCharsFilename(TString p_filename) { fElectrCharsFilename = p_filename; }

	/**
	 * Modifier
	 */
	void SetOutputFilename(TString p_filename) { fOutputFilename = p_filename; }

	/**
	 * Accessor
	 */
	//TString GetInputFilename(void) const { return fInputFilename; }

	/**
	 * Accessor
	 */
	//TString GetSetupConfigFilename(void) const { return fSetupConfigFilename; }

	/**
	 * Accessor
	 */
	//TString GetElectrCharsFilename(void) const { return fElectrCharsFilename; }

	/**
	 * Accessor
	 */
	//TString GetOutputFilename(void) const { return fOutputFilename; }

	/**
	 *
	 */
	const SetupConfiguration* GetSetupConfig(void) const { return fSetupConfiguration; }

	/**
	 *
	 */
	const ElectronicsChars* GetElectrChars(void) const { return fElectrChars; }

private:
	/**
	 * Input .lmd raw data file name
	 */
	TString fInputFilename;

	/**
	 * Input .xml setup configuration file name
	 */
	TString fSetupConfigFilename;

	/**
	 * Input .xml electronics characteristics file name
	 */
	TString fElectrCharsFilename;

	/**
	 * Output .root processed data file name
	 */
	TString fOutputFilename;

	/**
	 * Setup configuration object
	 */
	SetupConfiguration* fSetupConfiguration;

	/**
	 * Electronics characteristics object
	 */
	ElectronicsChars* fElectrChars;

	ClassDef(UserParameter, 1);
};

#endif // USERPARAMETER_H
