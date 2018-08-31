#ifndef READER_H
#define READER_H

#include <TObject.h> // mother class

// ROOT
#include <TString.h>

#include "data/DetEventFull.h"

class TFile;
class TTree;

class SetupConfiguration;
class DetEventFull;

class Reader : public TObject
{
public:

	Reader(TString inFilename, TString p_setupfilename);
	virtual ~Reader();

	void ProcessFile(UInt_t nEvents = 10);

	Int_t ReadEvent(Int_t iEvent, DetEventFull* event);

	Long64_t GetNEventsTotal() const;
private: // data members

	/**
	 * Setup configuration object
	 */
	SetupConfiguration* fSetupConfiguration;

	/**
	 * Input file tree object
	 */
	TTree* fInTree;
private: // methods
	
	static TTree* GetTheTree(TFile* theFile, TString* treeName);

	ClassDef(Reader, 1);	
};

#endif // READER_H
