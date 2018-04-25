#include "UserAnalysis.h"

// STD
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

// Go4
#include <TGo4Version.h> // for CheckVersion
#include <TGo4StepFactory.h>

// Project
#include "UserParameter.h"

UserAnalysis::UserAnalysis(const char* name) :
	TGo4Analysis(name),
	mEventCounter(0),
	//TODO not used by now ---------
	mMbsEvent(nullptr),
	mUserEventUnpack(nullptr),
	mUserEventMonitoring(nullptr),
	mUserEventStep2(nullptr)
	//------------------------------
{
	if (!TGo4Version::CheckVersion(__GO4BUILDVERSION__)) {
		cout << "Go4 version mismatch! Aborting." << endl;
		exit(EXIT_FAILURE);
	}

	//TODO use default setup.xml file
	//TODO or better force the user to support the correct setup configuration xml file?
	//this->Construct("output.root", "usr/setup.xml");

	TGo4Log::Error("Deprecated constructor of UserAnalysis without arguments!\n"
	               "Please supply the output and the setup configuration files' names.\n"
	               "go4analysis ... -args output.root setup.xml");
	exit(EXIT_FAILURE);
}

UserAnalysis::UserAnalysis(int argc, char** argv) :
	TGo4Analysis(argc, argv),
	mEventCounter(0),
	//TODO not used by now ---------
	mMbsEvent(nullptr),
	mUserEventUnpack(nullptr),
	mUserEventMonitoring(nullptr),
	mUserEventStep2(nullptr)
	//------------------------------
{
	if (!TGo4Version::CheckVersion(__GO4BUILDVERSION__)) {
		cout << "Go4 version mismatch! Aborting." << endl;
		exit(EXIT_FAILURE);
	}

	if (argc != 3) {
		TGo4Log::Error("Wrong number of arguments for the UserAnalysis constructor!\n"
		               "Please supply the output and the setup configuration files' names.\n"
		               "go4analysis ... -args output.root setup.xml");
		exit(EXIT_FAILURE);
	}

	/*for (int i=0; i<argc; i++) {
		cout << "argv[" << i << "]=" << argv[i] << endl;
	}*/

	this->Construct(argv[1], argv[2]);
	cout << "UserAnalysis constructed 2." << endl;
}

UserAnalysis::~UserAnalysis()
{
	cout << "UserAnalysis destructed." << endl;
}

void UserAnalysis::Construct(TString p_outfilename, TString p_setupfilename)
{
	//cout << "UserAnalysis::Construct()" << endl;

	//TODO check this method implementation

	// Fill the parameter-set object and attach it to the analysis
	mParams = new UserParameter();
	mParams->SetInputFilename(this->GetInputFileName());
	mParams->SetSetupConfigFilename(p_setupfilename);
	mParams->SetOutputFilename(p_outfilename);
	mParams->Init(); //TODO User function to perform XML import. Probably there should be a more nice way to do this.
	AddParameter(mParams);

	//TODO
	SetStepChecking(kFALSE); // necessary for non-subsequent mesh analysis

	// STEP1 - source - unpacker ==================================================================

	TGo4StepFactory* factoryUnpacking = new TGo4StepFactory("factoryUnpacking");
	factoryUnpacking->DefEventProcessor("UserProcUnpacking1", "UserProcUnpacking"); // object name, class name
	factoryUnpacking->DefOutputEvent("UserEventUnpacking1", "UserEventUnpacking"); // object name, class name

	TGo4AnalysisStep* stepUnpacking = new TGo4AnalysisStep("stepUnpacking", factoryUnpacking);

	stepUnpacking->SetSourceEnabled(kTRUE);
	stepUnpacking->SetProcessEnabled(kTRUE);
	stepUnpacking->SetErrorStopEnabled(kTRUE);

	TGo4FileStoreParameter* storeUnpacking = new TGo4FileStoreParameter("outputUnpacking.root");
	stepUnpacking->SetEventStore(storeUnpacking);
	stepUnpacking->SetStoreEnabled(kFALSE); //TODO enable/disable

	AddAnalysisStep(stepUnpacking);

	// STEP2 - provider - monitoring ===============================================================

	TGo4StepFactory* factoryUnpackedProvider1 = new TGo4StepFactory("factoryUnpackedProvider1");
	factoryUnpackedProvider1->DefInputEvent("UserEventUnpacking1", "UserEventUnpacking"); // read full raw event without partial io
	factoryUnpackedProvider1->DefEventProcessor("UserEventUnpacking1_1","MeshProviderProc"); // processorname must match name of input event + "_"
	factoryUnpackedProvider1->DefOutputEvent("Dummy", "MeshDummyEvent");
	TGo4AnalysisStep* stepUnpackedProvider1 = new TGo4AnalysisStep("stepUnpackedProvider1", factoryUnpackedProvider1);
	stepUnpackedProvider1->SetSourceEnabled(kFALSE);
	stepUnpackedProvider1->SetStoreEnabled(kFALSE);
	stepUnpackedProvider1->SetProcessEnabled(kTRUE);
	AddAnalysisStep(stepUnpackedProvider1);

	// STEP2 - processor - monitoring =============================================================

	TGo4StepFactory* factoryMonitoring = new TGo4StepFactory("factoryMonitoring");
	//factoryMonitoring->DefInputEvent("UserEventUnpacking1", "UserEventUnpacking"); // object name, class name
	factoryMonitoring->DefEventProcessor("UserProcMonitoring1", "UserProcMonitoring"); // object name, class name
	factoryMonitoring->DefOutputEvent("UserEventMonitoring1", "UserEventMonitoring"); // object name, class name

	TGo4AnalysisStep* stepMonitoring = new TGo4AnalysisStep("stepMonitoring", factoryMonitoring);

	stepMonitoring->SetSourceEnabled(kFALSE);
	stepMonitoring->SetProcessEnabled(kTRUE);
	stepMonitoring->SetErrorStopEnabled(kTRUE); //TODO probably for monitoring this should be false

	TGo4FileStoreParameter* storeMonitoring = new TGo4FileStoreParameter(p_outfilename); // "outputMonitoring.root"
	stepMonitoring->SetEventStore(storeMonitoring);
	stepMonitoring->SetStoreEnabled(kTRUE);

	AddAnalysisStep(stepMonitoring);

	// STEP2 - provider - learn ===================================================================

	TGo4StepFactory* factoryUnpackedProvider2 = new TGo4StepFactory("factoryUnpackedProvider2");
	factoryUnpackedProvider2->DefInputEvent("UserEventUnpacking1", "UserEventUnpacking"); // read full raw event without partial io
	factoryUnpackedProvider2->DefEventProcessor("UserEventUnpacking1_2","MeshProviderProc"); // processorname must match name of input event + "_"
	factoryUnpackedProvider2->DefOutputEvent("Dummy", "MeshDummyEvent");
	TGo4AnalysisStep* stepUnpackedProvider2 = new TGo4AnalysisStep("stepUnpackedProvider2", factoryUnpackedProvider2);
	stepUnpackedProvider2->SetSourceEnabled(kFALSE);
	stepUnpackedProvider2->SetStoreEnabled(kFALSE);
	stepUnpackedProvider2->SetProcessEnabled(kTRUE);
	AddAnalysisStep(stepUnpackedProvider2);

	// STEP2 - processor - learn ==================================================================

	TGo4StepFactory* factoryLearn = new TGo4StepFactory("factoryLearn");
	//factoryLearn->DefInputEvent("UserEventUnpacking1", "UserEventUnpacking"); // object name, class name
	factoryLearn->DefEventProcessor("UserProcLearn1", "UserProcLearn"); // object name, class name
	factoryLearn->DefOutputEvent("UserEventLearn1", "UserEventLearn"); // object name, class name

	TGo4AnalysisStep* stepLearn = new TGo4AnalysisStep("stepLearn", factoryLearn);

	stepLearn->SetSourceEnabled(kFALSE);
	stepLearn->SetProcessEnabled(kTRUE);
	stepLearn->SetErrorStopEnabled(kTRUE);

	TGo4FileStoreParameter* storeLearn = new TGo4FileStoreParameter("outputLearn.root");
	stepLearn->SetEventStore(storeLearn);
	stepLearn->SetStoreEnabled(kFALSE); //TODO enable/disable

	AddAnalysisStep(stepLearn);

	// ============================================================================================
}

Int_t UserAnalysis::UserPreLoop(void)
{
	//cout << "UserAnalysis::UserPreLoop()." << endl;
	//cerr << "Starting UserAnalysis." << endl;
	mEventCounter = 0;
	return 0;
}

// This function is called once for each event after all steps
Int_t UserAnalysis::UserEventFunc(void)
{
	//cout << "UserAnalysis::UserEventFunc()." << endl;
	mEventCounter++;
	return 0;
}

Int_t UserAnalysis::UserPostLoop(void)
{
	//cout << "UserAnalysis::UserPostLoop()." << endl;
	cerr << "Finished UserAnalysis. Total " << mEventCounter << " events." << endl;
	return 0;
}

ClassImp(UserAnalysis)
