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
#include "data/DetEventFull.h"

Reader::Reader() :
	TObject()
{
}

void Reader::Init(TString p_setupfilename)
{
	// Construct SetupConfiguration, which includes the input of the XML file
	fSetupConfiguration = new SetupConfiguration(p_setupfilename);
}

Reader::~Reader()
{
	if (fSetupConfiguration) delete fSetupConfiguration;
}

void Reader::ProcessFile(TString inFilename, UInt_t nEvents)
{
	TFile* inFile = new TFile(inFilename, "READ");

	if (inFile->IsZombie()) {
		cerr << "Error opening file " << inFilename.Data() << ". Aborting." << endl;
		return;
	}

	// Leave this string empty to search for the tree automatically
	TString inTreeName("");
	TTree* inTree = Reader::GetTheTree(inFile, &inTreeName);
	if (inTree == NULL) {
		cerr << "Tree '" << inTreeName << "' not found. Aborting." << endl;
		return;
	}

	DetEventFull* theEvent = new DetEventFull("DetEventFull1");
	inTree->SetBranchAddress("DetEventFull1.", &theEvent);

	UInt_t nEventsTotal = inTree->GetEntries();
	if (nEvents == 0) { nEvents = nEventsTotal; }

	// Loop over the events
	for (UInt_t iEvent=0; iEvent<nEvents; iEvent++)
	{
		cerr << "Event "<< iEvent
		     << " =================================================================="
		     << endl;

		inTree->GetEntry(iEvent);

		//TODO implement you actions here

		theEvent->Print();

	}

}

/*static*/
TTree* Reader::GetTheTree(TFile* theFile, TString* treeName)
{
	if (*treeName == "") {
		//// Search for a tree, take the first with the name ending with 'xTree'
		UInt_t v_keysCounter = 0;
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
