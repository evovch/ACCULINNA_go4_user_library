/**

	Example of a native ROOT macro to analyse Go4-produced output root files.

	Note, that an important action is done in the 'rootlogon.C' ROOT startup macro.

	Search for TODO and perform all necessary steps written after this word.

*/

TTree* GetTheTree(TFile* theFile, TString* treeName);

//TODO specify how many events do you want to process.
// If nEvents=0 - process all
void analyse2(TString inFilename="/home/evovch/Downloads/FLNR_data/exp201803/run13_0037.lmd.root", UInt_t nEvents = 10)
{
	TFile* inFile = new TFile(inFilename, "READ");

	if (inFile->IsZombie()) {
		cerr << "Error opening file " << inFilename.Data() << ". Aborting." << endl;
		return;
	}

	// Leave this string empty to search for the tree automatically
	TString inTreeName("");
	TTree* inTree = GetTheTree(inFile, &inTreeName);
	if (inTree == NULL) {
		return;
	}

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

	}

	cout << "Macro finished successfully." << endl;
}

// ============================================================================
// The code below should not worry you.
// These are just a couple of functions for automatic tree and branch finding.

TTree* GetTheTree(TFile* theFile, TString* treeName)
{
	if (*treeName == "") {
		//// Search for a tree, take the first with the name ending with 'xTree'
		UInt_t v_keysCounter = 0;
		TList* v_keys = theFile->GetListOfKeys();
		TIter v_nextkey(v_keys);
		while (TKey* v_curKey = (TKey*)v_nextkey()) {
			TString v_curKeyStr(v_curKey->GetName());
			////cerr << "key " << v_keysCounter << ": " << v_curKeyStr << endl;
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
