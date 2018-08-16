/**

	Please, specify correct input root file and correct corresponding setup.xml file.
	Currently the structure of the event in the input root file is not identified automatically
	but taken from the setup.xml file.
	
	nEvents = 0 - process all events.

*/

TTree* GetTheTree(TFile* theFile, TString* treeName);

void analyse4(TString inFilename="/home/evovch/experimental_data/exp201804_calib/result/si_1000_LR_02_0001.lmd.root",
              TString inSetupConfigFilename="../usr/setup2_exp201803.xml",
              UInt_t nEvents = 0)
{
	// Construct SetupConfiguration, which includes the input of the XML file
	SetupConfiguration* fSetupConfiguration = new SetupConfiguration(inSetupConfigFilename);

	TFile* inFile = new TFile(inFilename, "READ");

	if (inFile->IsZombie()) {
		cerr << "Error opening file " << inFilename.Data() << ". Aborting." << endl;
		return;
	}

	// Leave this string empty to search for the tree automatically
	TString inTreeName("");
	TTree* inTree = GetTheTree(inFile, &inTreeName);
	if (inTree == NULL) {
		cerr << "Tree '" << inTreeName << "' not found. Aborting." << endl;
		return;
	}

	DetEventFull* theEvent = new DetEventFull("DetEventFull1");
	TGo4EventElement* theEventCopy = theEvent;
	theEvent->synchronizeWithTree(inTree, &theEventCopy);

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

// ============================================================================
// The code below should not worry you.
// This is just a function for automatic tree finding.

TTree* GetTheTree(TFile* theFile, TString* treeName)
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
