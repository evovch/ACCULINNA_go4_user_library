#ifndef READER_H
#define READER_H

#include <TObject.h> // mother class

// ROOT
#include <TString.h>

class TFile;
class TTree;

class SetupConfiguration;

class Reader : public TObject
{
public:

	Reader();
	virtual ~Reader();

	void Init(TString p_setupfilename);

	void ProcessFile(TString inFilename, UInt_t nEvents = 10);

private: // data members

	/**
	 * Setup configuration object
	 */
	SetupConfiguration* fSetupConfiguration;

private: // methods
	
	TTree* GetTheTree(TFile* theFile, TString* treeName);

	ClassDef(Reader, 1);	
};

#endif // READER_H
