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
	TGo4AnalysisStep* stepBeamDetMonitoring2 = go4->GetAnalysisStep("stepBeamDetMonitoring2");
	TGo4AnalysisStep* stepTestMonitoring = go4->GetAnalysisStep("stepTestMonitoring");

	//TODO enable/disable certain steps
	stepUnpacking->SetProcessEnabled(kTRUE);
	stepRepacking->SetProcessEnabled(kTRUE);
	stepLearn->SetProcessEnabled(kFALSE);
	stepRawMonitoring->SetProcessEnabled(kFALSE);
	stepAdvMonitoring->SetProcessEnabled(kTRUE);
	stepBeamDetMonitoring->SetProcessEnabled(kTRUE);
	stepBeamDetMonitoring2->SetProcessEnabled(kTRUE);
	stepTestMonitoring->SetProcessEnabled(kFALSE);
	//TODO enable/disable autosave
	//go4->SetAutoSaveFile("asf.root");
	go4->SetAutoSave(kFALSE);

	cout << "Macro 'setup.C' finished." << endl;
}
