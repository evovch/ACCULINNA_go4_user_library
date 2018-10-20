#include "UserProcTestMonitoring.h"

// STD
#include <fstream>
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

// ROOT
#include <TClonesArray.h>
#include <TH1D.h>
#include <TH2D.h>

// Project
#include "data/DetEventFull.h" // input event
#include "data/DetEventCommon.h"
#include "data/DetEventStation.h"
#include "data/DetMessage.h"
#include "useranalysis/calibration/SiCalibPars.h"

#include "UserHistosTestMonitoring.h"
#include "UserParameter.h"
#include "setupconfigcppwrapper/SetupConfiguration.h"

#include <stdlib.h>

using namespace std;
/**
  Uncomment this if you want to see all the debug information.
  This allows you to analyze the raw bytes and bits by your eyes.
  This option produces A LOT OF DATA - run your analysis with a
  small number of events (~10-100)
*/
//#define DEBUGTestMON

UserProcTestMonitoring::UserProcTestMonitoring(const char* name) :
	TGo4EventProcessor(name),
	fEventCounter(0)
{	
	// Creating and filling TGo4Parameter objects
	this->InitPars();

	fHistoMan_test = new UserHistosTestMonitoring(fstPair,fnPars);
	fFileSummary = fopen("textoutput/summaryTestMonitoring.txt", "w");
	if (fFileSummary == NULL) {
		//TODO error
		cerr << "[WARN  ] " << "Could not open output text summary file '" << "summaryTestMonitoring.txt" << "'" << endl;
	}
}

UserProcTestMonitoring::~UserProcTestMonitoring()
{
	if (fHistoMan_test) delete fHistoMan_test;
	if (fFileSummary != NULL) {
		fclose(fFileSummary);
	}
}

Bool_t UserProcTestMonitoring::BuildEvent(TGo4EventElement* p_dest)
{
	// cerr << "\t ### UserProcTestMonitoring::BuildEvent was called ### " <<  endl;

	Bool_t v_isValid = kFALSE;

	DetEventFull* v_input = (DetEventFull*)GetInputEvent("stepRepackedProvider1");
	if (v_input == NULL)
	{
		cerr << "[WARN  ] " << "UserProcTestMonitoring::BuildEvent(): no input event!" << endl;
		return v_isValid;
	}
	v_isValid = kTRUE;

	#ifdef DEBUGTestMON
	cerr << "[DEBUG ] " << "UserProcTestMonitoring: Event " << fEventCounter
	     << " ======================================================================================================"
	     << endl;
	#endif

	// --------------------------
	TGo4EventElement* v_subElement = v_input->getEventElement("Left_telescope",1);
	if(!v_subElement) {
		cout << "Detector Left_telescope was not found " << endl;
		return kFALSE;
	}
	TGo4CompositeEvent* dEvent = (TGo4CompositeEvent*)(v_subElement);
	ProcessMessage(dEvent);

	// DetEventStation* v_subSubElement = (DetEventStation*)(dEvent->getEventElement("SQX_L",1));
	// if (!v_subSubElement) {
	// 	cout << " station " << st_Name1.Data() <<  " was not found " << endl;
	// 	return kFALSE;
	// }
	// --------------------------

	fEventCounter++;

	return v_isValid;
}

void UserProcTestMonitoring::UserPreLoop()
{
	// Get the all-accessible parameter-set object
	UserParameter* v_params = (UserParameter*)GetParameter("UserParameter");
	fSetupConfig = v_params->GetSetupConfig();

	#ifdef DEBUGTestMON
	cerr << "[DEBUG ] " << "UserProcTestMonitoring::UserPreLoop ====================================" << endl;
	#endif

	#ifdef DEBUGTestMON
	cerr << "[DEBUG ] " << "=======================================================================" << endl;
	#endif
}

void UserProcTestMonitoring::UserPostLoop()
{
}

void UserProcTestMonitoring::ProcessMessage(TGo4CompositeEvent* p_message)
{

	//TODO implement your processing of independent messages here
	cout << " ### ProcessMessage was called ###" << endl;
	cout << endl;
	// TODO we should somehow also give the index (0) into the calibSi. I need it for histos
	calibSi(p_message,fstPair[0]);


}

void UserProcTestMonitoring::InitPars() {
	fParSi = new SiCalibPars* [fnPars]; // make this another way
	// TODO : the loop over the whole map
  fstPair = new std::pair <TString,Int_t>[fnPars];
	fstPair[0] = make_pair((TString)"SQX_L",32);
	fstPair[1] = make_pair((TString)"SQY_L",16);
	fstPair[2] = make_pair((TString)"SQX_R",32);
	fstPair[3] = make_pair((TString)"SQY_R",16);	
	fstPair[4] = make_pair((TString)"SQ300",16);				                               
	for(Int_t i=0; i<5; i++) {
		fParSi[i] = (SiCalibPars*) MakeParameter(fstPair[i].first, "SiCalibPars");
		fParSi[i]->Init(fstPair[i].second,fstPair[i].first);
	}
}

void UserProcTestMonitoring::calibSi(TGo4CompositeEvent* p_message,std::pair <TString,Int_t> pair)
{
	cout << " ### calibSi was called ###" << endl;
	cout << " with such arguments: " << pair.first << endl;
	TString st_Name = (TString)p_message->GetName() + "_" + pair.first;

	DetEventStation* st_Si = (DetEventStation*)(p_message->getEventElement(st_Name.Data(),1));
	if (!st_Si) {
		cout << " station " << st_Name.Data() <<  " was not found " << endl;
		return;
	}

	TClonesArray* v_Si = st_Si->GetDetMessages();

	for(Int_t i=0; i<v_Si->GetEntriesFast(); i++) {
		DetMessage *mes_Si = (DetMessage*)v_Si->At(i);
		cout << mes_Si->GetStChannel() << " " << mes_Si->GetValue() << endl;
		// bla bla blas
	}
}

ClassImp(UserProcTestMonitoring)
