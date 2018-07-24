void run_reader(TString inFilename="/home/evovch/experimental_data/exp201804_calib/result/si_1000_LR_02_0001.lmd.root",
                TString inSetupConfigFilename="./usr/setup2_exp201803.xml",
                UInt_t nEvents = 10)
{
	// Create the reader object
	Reader* theReader = new Reader();

	theReader->Init(inSetupConfigFilename);

	// Per-se import the root file
	theReader->ProcessFile(inFilename);
}
