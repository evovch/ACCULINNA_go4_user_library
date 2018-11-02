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

	fHistoMan = new UserHistosTestMonitoring(fstPair,fnPars);
	fFileSummary = fopen("textoutput/summaryTestMonitoring.txt", "w");
	if (fFileSummary == NULL) {
		//TODO error
		cerr << "[WARN  ] " << "Could not open output text summary file '" << "summaryTestMonitoring.txt" << "'" << endl;
	}
}

UserProcTestMonitoring::~UserProcTestMonitoring()
{
	cout << "UserProcTestMonitoring destructor called " << endl; 

	if (fParSi) delete [] fParSi;
	if (fstPair) delete [] fstPair;
	if (fHistoMan) delete fHistoMan;
	if (fFileSummary != NULL) fclose(fFileSummary);
}

Bool_t UserProcTestMonitoring::BuildEvent(TGo4EventElement* p_dest)
{
	// cerr << "\t ### UserProcTestMonitoring::BuildEvent was called ### " <<  endl;

	Bool_t v_isValid = kFALSE;

	DetEventFull* v_input = (DetEventFull*)GetInputEvent("stepRepackedProvider4");
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
	Short_t v_NsubElems = v_input->getNElements();
	for(Int_t pNum=0; pNum<fnPars; pNum++) {
		for (Short_t i=0; i<v_NsubElems; i++) {
			TGo4EventElement* v_subElement = v_input->getEventElement(i);
			TString calibName = v_subElement->GetName() + TString("_") + fstPair[pNum].first;
			Short_t curId = v_subElement->getId();

			if (!calibName.Contains("DetEventCommon")) {
				TGo4CompositeEvent* v_detSubEl = (TGo4CompositeEvent*)(v_subElement);

				Short_t v_NsubSubElems = v_detSubEl->getNElements();

				// Loop over the stations of the current detector
				for (Short_t j=0; j<v_NsubSubElems; j++) {

					Short_t stId = curId*100 + j; //FIXME this is quite dangerous

					DetEventStation* v_stSubsubEl = (DetEventStation*)(v_detSubEl->getEventElement(stId));
					TString stName = v_stSubsubEl->GetName();

					if (stName==calibName) {
						TClonesArray* v_data = v_stSubsubEl->GetDetMessages();
						TIter v_detMiter(v_data);
						DetMessage* v_curDetM;

						// Loop over the messages of the current station 
						while ((v_curDetM = (DetMessage*)v_detMiter.Next())) {
							Double_t par1 = fParSi[pNum]->getPar1(v_curDetM->GetStChannel());
							Double_t par2 = fParSi[pNum]->getPar2(v_curDetM->GetStChannel());
							fHistoMan->detSi[pNum]->Fill(v_curDetM->GetValue()*par2 + par1);
						}
					}
				} // end of loop over the stations
			} // end of if
		} // end of loop over the sub-elements (detectors)
	} // loop over the calibration index (0-4)
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

void UserProcTestMonitoring::ProcessMessage(DetEventStation* p_message,Int_t parNum)
{

}

void UserProcTestMonitoring::InitPars() {
	fParSi = new SiCalibPars* [fnPars]; // make this another way
	// TODO : the loop over the whole map
  fstPair = new std::pair <TString,Int_t>[fnPars];
	fstPair[0] = make_pair((TString)"SSD20_L",16);
	fstPair[1] = make_pair((TString)"DSDX_L",16);
	fstPair[2] = make_pair((TString)"DSDY_L",16);
	fstPair[3] = make_pair((TString)"SSD_L",16);

	fstPair[4] = make_pair((TString)"SSD20_R",16);
	fstPair[5] = make_pair((TString)"DSDX_R",16);	
	fstPair[6] = make_pair((TString)"DSDY_R",16);	
	fstPair[7] = make_pair((TString)"SSD_R",16);		

	for(Int_t i=0; i<fnPars; i++) {
		fParSi[i] = (SiCalibPars*) MakeParameter(fstPair[i].first, "SiCalibPars");
		fParSi[i]->Init(fstPair[i].second,fstPair[i].first);
	}
}

void UserProcTestMonitoring::calibSi(TGo4CompositeEvent* p_message,std::pair <TString,Int_t> pair)
{

}

ClassImp(UserProcTestMonitoring)
