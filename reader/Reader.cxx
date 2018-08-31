#include "Reader.h"

// STD
#include <iostream>
//using std::cout;
using std::cerr;
using std::endl;

// ROOT
#include <TFile.h>
#include <TTree.h>
#include <TKey.h>

// Project
#include "setupconfigcppwrapper/SetupConfiguration.h"
#include "data/DetEventCommon.h"

Reader::Reader(TString inFilename, TString p_setupfilename) :
	TObject(),
	fSetupConfiguration(NULL),
	fInTree(NULL)
{
	// Construct SetupConfiguration, which includes the input of the XML file
	fSetupConfiguration = new SetupConfiguration(p_setupfilename);

	TFile* inFile = new TFile(inFilename, "READ");

	if (inFile->IsZombie()) {
		cerr << "Error opening file " << inFilename.Data() << ". Aborting." << endl;
		return;
	}

	// Leave this string empty to search for the tree automatically
	TString inTreeName("");
	fInTree = Reader::GetTheTree(inFile, &inTreeName);
	if (fInTree == NULL) {
		cerr << "Tree '" << inTreeName << "' not found. Aborting." << endl;
		return;
	}
}

Reader::~Reader()
{
	if (fSetupConfiguration) delete fSetupConfiguration;
}

void Reader::ProcessFile(UInt_t nEvents)
{
	DetEventFull* theEvent = new DetEventFull("DetEventFull1");
	TGo4EventElement* theEventCopy = theEvent;
	theEvent->synchronizeWithTree(fInTree, &theEventCopy);

	if (nEvents == 0) { nEvents = GetNEventsTotal(); }

	// Loop over the events
	for (UInt_t iEvent=0; iEvent<nEvents; iEvent++)
	{
		cerr << "Event "<< iEvent
		     << " =================================================================="
		     << endl;

		fInTree->GetEntry(iEvent);

		//TODO implement you actions here
		theEvent->Print();
	}
}

Int_t Reader::ReadEvent(Int_t iEvent, DetEventFull* event)
{
	if (iEvent < 0 || iEvent >= GetNEventsTotal()) {
		cerr << "The event number is greater than total events number in input file!";
		return -1;
	}

	if (!event) {
		cerr << "Event object is null!";
		return -1;
	}

	TGo4EventElement* theEventCopy = event;
	event->synchronizeWithTree(fInTree, &theEventCopy);

	fInTree->GetEntry(iEvent);

	return 0;
}

Long64_t Reader::GetNEventsTotal() const
{
	if (fInTree)
		return fInTree->GetEntries();
	else
		return -1;
}

/*static*/
TTree* Reader::GetTheTree(TFile* theFile, TString* treeName)
{
	if (*treeName == "") {
		//// Search for a tree, take the first with the name ending with 'xTree'
		// UInt_t v_keysCounter = 0;
		TList* v_keys = theFile->GetListOfKeys();
		TIter v_nextkey(v_keys);
		while (TKey* v_curKey = (TKey*)v_nextkey()) {
			TString v_curKeyStr(v_curKey->GetName());
			////cerr << "key " << v_keysCounter++ << ": " << v_curKeyStr << endl;
			if (v_curKeyStr.EndsWith("xTree")) {
				*treeName = v_curKeyStr;
				cerr << "Automatically defined tree name: '"
				     << *treeName << "'" << endl;
				break;
			}
		} // end of while loop
	}

	TTree* foundTree = (TTree*)theFile->Get(*treeName);
	if (foundTree != NULL) {
		cerr << "Found the tree '" << *treeName << "'." << endl;
		return foundTree;
	} else {
		cerr << "Could not find the tree '" << *treeName << "'. Aborting." << endl;
		return NULL;
	}
}

ClassImp(Reader)
