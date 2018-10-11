#include "UserAnalysis.h"

// STD
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

// Go4
#include <TGo4Version.h> // for CheckVersion
#include <TGo4StepFactory.h>
#include <TGo4MbsEvent.h>

// Project
#include "UserParameter.h"

UserAnalysis::UserAnalysis(const char* name) :
	TGo4Analysis(name),
	mEventCounter(0),
	mParams(nullptr)
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
	mParams(nullptr)
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


	//mParams = (UserParameter*)MakeParameter("UserParameter1", "UserParameter", "autoload/set_par.C");

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

	// STEP2.1 - provider - repacking ===============================================================

	TGo4StepFactory* factoryUnpackedProvider1 = new TGo4StepFactory("factoryUnpackedProvider1");
	factoryUnpackedProvider1->DefInputEvent("UserEventUnpacking1", "UserEventUnpacking"); // read full raw event without partial io
	factoryUnpackedProvider1->DefEventProcessor("UserEventUnpacking1_1","MeshProviderProc"); // processorname must match name of input event + "_"
	factoryUnpackedProvider1->DefOutputEvent("Dummy", "MeshDummyEvent");
	TGo4AnalysisStep* stepUnpackedProvider1 = new TGo4AnalysisStep("stepUnpackedProvider1", factoryUnpackedProvider1);
	stepUnpackedProvider1->SetSourceEnabled(kFALSE);
	stepUnpackedProvider1->SetStoreEnabled(kFALSE);
	stepUnpackedProvider1->SetProcessEnabled(kTRUE);
	AddAnalysisStep(stepUnpackedProvider1);

	// STEP2.1 - processor - repacking =============================================================

	TGo4StepFactory* factoryRepacking = new TGo4StepFactory("factoryRepacking");
	//factoryRepacking->DefInputEvent("UserEventUnpacking1", "UserEventUnpacking"); // object name, class name
	factoryRepacking->DefEventProcessor("UserProcRepacking1", "UserProcRepacking"); // object name, class name
	factoryRepacking->DefOutputEvent("DetEventFull1", "DetEventFull"); // object name, class name

	TGo4AnalysisStep* stepRepacking = new TGo4AnalysisStep("stepRepacking", factoryRepacking);

	stepRepacking->SetSourceEnabled(kFALSE);
	stepRepacking->SetProcessEnabled(kTRUE);
	stepRepacking->SetErrorStopEnabled(kTRUE); //TODO probably for repacking this should be false

	TGo4FileStoreParameter* storeRepacking = new TGo4FileStoreParameter(p_outfilename); // "outputRepacking.root"
	stepRepacking->SetEventStore(storeRepacking);
	stepRepacking->SetStoreEnabled(kTRUE);

	AddAnalysisStep(stepRepacking);

	// STEP2.2 - provider - learn ===================================================================
//TODO remove two leading slashes in the following line to disable this step
///*
	TGo4StepFactory* factoryUnpackedProvider2 = new TGo4StepFactory("factoryUnpackedProvider2");
	factoryUnpackedProvider2->DefInputEvent("UserEventUnpacking1", "UserEventUnpacking"); // read full raw event without partial io
	factoryUnpackedProvider2->DefEventProcessor("UserEventUnpacking1_2","MeshProviderProc"); // processorname must match name of input event + "_"
	factoryUnpackedProvider2->DefOutputEvent("Dummy", "MeshDummyEvent");
	TGo4AnalysisStep* stepUnpackedProvider2 = new TGo4AnalysisStep("stepUnpackedProvider2", factoryUnpackedProvider2);
	stepUnpackedProvider2->SetSourceEnabled(kFALSE);
	stepUnpackedProvider2->SetStoreEnabled(kFALSE);
	stepUnpackedProvider2->SetProcessEnabled(kTRUE);
	AddAnalysisStep(stepUnpackedProvider2);

	// // STEP2.2 - processor - learn ==================================================================

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
//*/
	// STEP2.3 - provider - raw monitoring ===============================================================
//TODO remove two leading slashes in the following line to disable this step
///*
	TGo4StepFactory* factoryUnpackedProvider3 = new TGo4StepFactory("factoryUnpackedProvider3");
	factoryUnpackedProvider3->DefInputEvent("UserEventUnpacking1", "UserEventUnpacking"); // read full raw event without partial io
	factoryUnpackedProvider3->DefEventProcessor("UserEventUnpacking1_3","MeshProviderProc"); // processorname must match name of input event + "_"
	factoryUnpackedProvider3->DefOutputEvent("Dummy", "MeshDummyEvent");
	TGo4AnalysisStep* stepUnpackedProvider3 = new TGo4AnalysisStep("stepUnpackedProvider3", factoryUnpackedProvider3);
	stepUnpackedProvider3->SetSourceEnabled(kFALSE);
	stepUnpackedProvider3->SetStoreEnabled(kFALSE);
	stepUnpackedProvider3->SetProcessEnabled(kTRUE);
	AddAnalysisStep(stepUnpackedProvider3);

	// // STEP2.3 - processor - raw monitoring =============================================================

	TGo4StepFactory* factoryRawMonitoring = new TGo4StepFactory("factoryRawMonitoring");
	//factoryRawMonitoring->DefInputEvent("UserEventUnpacking1", "UserEventUnpacking"); // object name, class name
	factoryRawMonitoring->DefEventProcessor("UserProcRawMonitoring1", "UserProcRawMonitoring"); // object name, class name
	factoryRawMonitoring->DefOutputEvent("UserEventRawMonitoring1", "UserEventRawMonitoring"); // object name, class name

	TGo4AnalysisStep* stepRawMonitoring = new TGo4AnalysisStep("stepRawMonitoring", factoryRawMonitoring);

	stepRawMonitoring->SetSourceEnabled(kFALSE);
	stepRawMonitoring->SetProcessEnabled(kTRUE);
	stepRawMonitoring->SetErrorStopEnabled(kFALSE);

	//TGo4FileStoreParameter* storeRawMonitoring = new TGo4FileStoreParameter("rawmonitoring.root"); //TODO
	//stepRawMonitoring->SetEventStore(storeRawMonitoring);
	//stepRawMonitoring->SetStoreEnabled(kTRUE);
	stepRawMonitoring->SetStoreEnabled(kFALSE);

	AddAnalysisStep(stepRawMonitoring);
//*/
	// STEP3.1 - provider - advanced monitoring ===============================================================
//TODO remove two leading slashes in the following line to disable this step
///*
	TGo4StepFactory* factoryRepackedProvider1 = new TGo4StepFactory("factoryRepackedProvider1");
	factoryRepackedProvider1->DefInputEvent("DetEventFull1", "DetEventFull"); // read full raw event without partial io
	factoryRepackedProvider1->DefEventProcessor("DetEventFull1_1","MeshProviderProc"); // processorname must match name of input event + "_"
	factoryRepackedProvider1->DefOutputEvent("Dummy", "MeshDummyEvent");
	TGo4AnalysisStep* stepRepackedProvider1 = new TGo4AnalysisStep("stepRepackedProvider1", factoryRepackedProvider1);
	stepRepackedProvider1->SetSourceEnabled(kFALSE);
	stepRepackedProvider1->SetStoreEnabled(kFALSE);
	stepRepackedProvider1->SetProcessEnabled(kTRUE);
	AddAnalysisStep(stepRepackedProvider1);

	// // STEP3.1 - processor - advanced monitoring =============================================================

	TGo4StepFactory* factoryAdvMonitoring = new TGo4StepFactory("factoryAdvMonitoring");
	//factoryAdvMonitoring->DefInputEvent("DetEventFull1", "DetEventFull"); // object name, class name
	factoryAdvMonitoring->DefEventProcessor("UserProcAdvMonitoring1", "UserProcAdvMonitoring"); // object name, class name
	factoryAdvMonitoring->DefOutputEvent("UserEventAdvMonitoring1", "UserEventAdvMonitoring"); // object name, class name

	TGo4AnalysisStep* stepAdvMonitoring = new TGo4AnalysisStep("stepAdvMonitoring", factoryAdvMonitoring);

	stepAdvMonitoring->SetSourceEnabled(kFALSE);
	stepAdvMonitoring->SetProcessEnabled(kTRUE);
	stepAdvMonitoring->SetErrorStopEnabled(kFALSE);

	//TGo4FileStoreParameter* storeAdvMonitoring = new TGo4FileStoreParameter("advmonitoring.root"); //TODO
	//stepAdvMonitoring->SetEventStore(storeAdvMonitoring);
	//stepAdvMonitoring->SetStoreEnabled(kTRUE);
	stepAdvMonitoring->SetStoreEnabled(kFALSE);

	AddAnalysisStep(stepAdvMonitoring);

// STEP3.2 - processor - test monitoring =============================================================

	// TGo4StepFactory* factoryTestMonitoring = new TGo4StepFactory("factoryTestMonitoring");
	// //factoryAdvMonitoring->DefInputEvent("DetEventFull1", "DetEventFull"); // object name, class name
	// factoryTestMonitoring->DefEventProcessor("UserProcTestMonitoring1", "UserProcTestMonitoring"); // object name, class name
	// factoryTestMonitoring->DefOutputEvent("UserEventTestMonitoring1", "UserEventTestMonitoring"); // object name, class name

	// TGo4AnalysisStep* stepTestMonitoring = new TGo4AnalysisStep("stepTestMonitoring", factoryTestMonitoring);

	// stepTestMonitoring->SetSourceEnabled(kFALSE);
	// stepTestMonitoring->SetProcessEnabled(kTRUE);
	// stepTestMonitoring->SetErrorStopEnabled(kFALSE);

	// //TGo4FileStoreParameter* storeAdvMonitoring = new TGo4FileStoreParameter("advmonitoring.root"); //TODO
	// //stepAdvMonitoring->SetEventStore(storeAdvMonitoring);
	// //stepAdvMonitoring->SetStoreEnabled(kTRUE);
	// stepTestMonitoring->SetStoreEnabled(kFALSE);

	// AddAnalysisStep(stepTestMonitoring);

	// STEP3.3 - provider - beam monitoring ===============================================================
//TODO remove two leading slashes in the following line to disable this step
///*
	TGo4StepFactory* factoryRepackedProvider2 = new TGo4StepFactory("factoryRepackedProvider2");
	factoryRepackedProvider2->DefInputEvent("DetEventFull1", "DetEventFull"); // read full raw event without partial io
	factoryRepackedProvider2->DefEventProcessor("DetEventFull1_1","MeshProviderProc"); // processorname must match name of input event + "_"
	factoryRepackedProvider2->DefOutputEvent("Dummy", "MeshDummyEvent");
	TGo4AnalysisStep* stepRepackedProvider2 = new TGo4AnalysisStep("stepRepackedProvider2", factoryRepackedProvider2);
	stepRepackedProvider2->SetSourceEnabled(kFALSE);
	stepRepackedProvider2->SetStoreEnabled(kFALSE);
	stepRepackedProvider2->SetProcessEnabled(kTRUE);
	AddAnalysisStep(stepRepackedProvider2);

// STEP3.4 - processor - beam monitoring =============================================================

	TGo4StepFactory* factoryBeamMonitoring = new TGo4StepFactory("factoryBeamMonitoring");
	//factoryAdvMonitoring->DefInputEvent("DetEventFull1", "DetEventFull"); // object name, class name
	factoryBeamMonitoring->DefEventProcessor("UserProcBeamMonitoring1", "UserProcBeamMonitoring"); // object name, class name
	factoryBeamMonitoring->DefOutputEvent("UserEventBeamMonitoring1", "UserEventBeamMonitoring"); // object name, class name

	TGo4AnalysisStep* stepBeamMonitoring = new TGo4AnalysisStep("stepBeamMonitoring", factoryBeamMonitoring);

	stepBeamMonitoring->SetSourceEnabled(kFALSE);
	stepBeamMonitoring->SetProcessEnabled(kTRUE);
	stepBeamMonitoring->SetErrorStopEnabled(kFALSE);

	//TGo4FileStoreParameter* storeAdvMonitoring = new TGo4FileStoreParameter("advmonitoring.root"); //TODO
	//stepAdvMonitoring->SetEventStore(storeAdvMonitoring);
	//stepAdvMonitoring->SetStoreEnabled(kTRUE);
	stepBeamMonitoring->SetStoreEnabled(kFALSE);

	AddAnalysisStep(stepBeamMonitoring);

	// STEP3.5 - provider - beam monitoring 2 ===============================================================
//TODO remove two leading slashes in the following line to disable this step
///*
	TGo4StepFactory* factoryRepackedProvider3 = new TGo4StepFactory("factoryRepackedProvider3");
	factoryRepackedProvider3->DefInputEvent("DetEventFull1", "DetEventFull"); // read full raw event without partial io
	factoryRepackedProvider3->DefEventProcessor("DetEventFull1_1","MeshProviderProc"); // processorname must match name of input event + "_"
	factoryRepackedProvider3->DefOutputEvent("Dummy", "MeshDummyEvent");
	TGo4AnalysisStep* stepRepackedProvider3 = new TGo4AnalysisStep("stepRepackedProvider3", factoryRepackedProvider3);
	stepRepackedProvider3->SetSourceEnabled(kFALSE);
	stepRepackedProvider3->SetStoreEnabled(kFALSE);
	stepRepackedProvider3->SetProcessEnabled(kTRUE);
	AddAnalysisStep(stepRepackedProvider3);

// STEP3.6 - processor - beam monitoring 2 =============================================================

	TGo4StepFactory* factoryBeamMonitoring2 = new TGo4StepFactory("factoryBeamMonitoring2");
	//factoryAdvMonitoring->DefInputEvent("DetEventFull1", "DetEventFull"); // object name, class name
	factoryBeamMonitoring2->DefEventProcessor("UserProcBeamMonitoring2", "UserProcBeamMonitoring2"); // object name, class name
	factoryBeamMonitoring2->DefOutputEvent("UserEventBeamMonitoring2", "UserEventBeamMonitoring2"); // object name, class name

	TGo4AnalysisStep* stepBeamMonitoring2 = new TGo4AnalysisStep("stepBeamMonitoring2", factoryBeamMonitoring2);

	stepBeamMonitoring2->SetSourceEnabled(kFALSE);
	stepBeamMonitoring2->SetProcessEnabled(kTRUE);
	stepBeamMonitoring2->SetErrorStopEnabled(kFALSE);

	//TGo4FileStoreParameter* storeAdvMonitoring = new TGo4FileStoreParameter("advmonitoring.root"); //TODO
	//stepAdvMonitoring->SetEventStore(storeAdvMonitoring);
	//stepAdvMonitoring->SetStoreEnabled(kTRUE);
	stepBeamMonitoring2->SetStoreEnabled(kFALSE);

	AddAnalysisStep(stepBeamMonitoring2);


//*/
	// STEP3.3 - digibuilding =====================================================================
/*
	TGo4StepFactory* factoryDigiBuilding = new TGo4StepFactory("factoryDigiBuilding");
	factoryDigiBuilding->DefInputEvent("DetEventFull1", "DetEventFull"); // object name, class name
	factoryDigiBuilding->DefEventProcessor("UserProcDigiBuilding1", "UserProcDigiBuilding"); // object name, class name
	factoryDigiBuilding->DefOutputEvent("UserEventDigiBuilding1", "UserEventDigiBuilding"); // object name, class name

	TGo4AnalysisStep* stepDigiBuilding = new TGo4AnalysisStep("stepDigiBuilding", factoryDigiBuilding);

	stepDigiBuilding->SetSourceEnabled(kFALSE);
	stepDigiBuilding->SetProcessEnabled(kTRUE);
	stepDigiBuilding->SetErrorStopEnabled(kTRUE);

	TGo4FileStoreParameter* storeDigiBuilding = new TGo4FileStoreParameter("outputDigiBuilding.root");
	stepDigiBuilding->SetEventStore(storeDigiBuilding);
	stepDigiBuilding->SetStoreEnabled(kTRUE); //TODO enable/disable

	AddAnalysisStep(stepDigiBuilding);
*/
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
