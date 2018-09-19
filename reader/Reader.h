#ifndef READER_H
#define READER_H

#include <TObject.h> // mother class

// ROOT
#include <TString.h>

class TFile;
class TTree;

class TGo4EventElement;

class SetupConfiguration;
class DetEventFull;

class Reader : public TObject
{
public:
	Reader(TString inFilename, TString p_setupfilename);
	virtual ~Reader();

	void ProcessFile(UInt_t nEvents = 10);

	DetEventFull* ReadEvent(Int_t iEvent);

	Long64_t GetNEventsTotal(void) const;

	TTree* GetInTree(void) const { return fInTree; } 
	SetupConfiguration* GetSetupConfiguration(void) const { return fSetupConfiguration; }

private: // data members

	/**
	 * Setup configuration object
	 */
	SetupConfiguration* fSetupConfiguration;

	/**
	 * Input file tree object
	 */
	TTree* fInTree;

	/**
	 * Event synchronized with tree
	 */
	DetEventFull* fEvent;

	/**
	 * Event copy
	 */
	TGo4EventElement* fEventCopy;
	
private: // methods
	
	static TTree* GetTheTree(TFile* theFile, TString* treeName);

	ClassDef(Reader, 1);	
};

#endif // READER_H
