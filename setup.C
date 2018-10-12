/**
	Note that if an error occurs in this macro
	go4 will give no error message.
*/

void setup()
{
	cout << "Macro 'setup.C' started." << endl;

	TGo4AnalysisStep* stepUnpacking = go4->GetAnalysisStep("stepUnpacking");
	TGo4AnalysisStep* stepRepacking = go4->GetAnalysisStep("stepRepacking");
	TGo4AnalysisStep* stepLearn = go4->GetAnalysisStep("stepLearn");
	TGo4AnalysisStep* stepRawMonitoring = go4->GetAnalysisStep("stepRawMonitoring");
	TGo4AnalysisStep* stepAdvMonitoring = go4->GetAnalysisStep("stepAdvMonitoring");
	TGo4AnalysisStep* stepBeamDetMonitoring = go4->GetAnalysisStep("stepBeamDetMonitoring");

	//TODO enable/disable certain steps
	stepUnpacking->SetProcessEnabled(kTRUE);
	stepRepacking->SetProcessEnabled(kTRUE);
	stepLearn->SetProcessEnabled(kFALSE);
	stepRawMonitoring->SetProcessEnabled(kFALSE);
	stepAdvMonitoring->SetProcessEnabled(kFALSE);
	stepBeamDetMonitoring->SetProcessEnabled(kFALSE);

	//TODO enable/disable autosave
	//go4->SetAutoSaveFile("asf.root");
	go4->SetAutoSave(kTRUE);

	cout << "Macro 'setup.C' finished." << endl;
}
