void run_reader(TString inFilename="/home/evovch/experimental_data/exp201804_calib/result/si_1000_LR_02_0001.lmd.root",
                TString inSetupConfigFilename="../usr/setup2_exp201803.xml",
                UInt_t nEvents = 10)
{
	// Create the reader object
	Reader* theReader = new Reader(inFilename, inSetupConfigFilename);

	for (Int_t iEvent = 0; iEvent < nEvents; iEvent++) {
		cerr << iEvent << endl;
		const DetEventFull* event = theReader->ReadEvent(iEvent);
		event->Print();
	}
}
