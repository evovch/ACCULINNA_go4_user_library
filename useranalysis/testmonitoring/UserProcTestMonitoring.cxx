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
#include "UserParameterTest.h"
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
//-----------------------------------------------------------------------
UserProcTestMonitoring::UserProcTestMonitoring(const char* name) :
	TGo4EventProcessor(name),
	fEventCounter(0)
{	
	// create UserTestParameter
	fParTest = (UserParameterTest*) MakeParameter("UserTestPar", "UserParameterTest");
	// Creating and filling SiPars objects
	this->InitPars();

	fHistoMan = new UserHistosTestMonitoring(fstPair,fnPars);
	fFileSummary = fopen("textoutput/summaryTestMonitoring.txt", "w");
	if (fFileSummary == NULL) {
		//TODO error
		cerr << "[WARN  ] " << "Could not open output text summary file '" << "summaryTestMonitoring.txt" << "'" << endl;
	}
}
//-----------------------------------------------------------------------
UserProcTestMonitoring::~UserProcTestMonitoring()
{
	cout << "UserProcTestMonitoring destructor called " << endl; 

	// if (fParTest != NULL) delete fParTest;
	if (fParSi != NULL) delete [] fParSi;
	if (fstPair != NULL) delete [] fstPair;
	if (fHistoMan != NULL) delete fHistoMan;
	if (fFileSummary != NULL) fclose(fFileSummary);
}
//-----------------------------------------------------------------------
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

	TGo4CompositeEvent* v_RightDet = (TGo4CompositeEvent*)(v_input->getEventElement("Right_telescope"));
	if (!v_RightDet) { 
		cerr << "Detector Right_telescope was not found." << endl; 
		return kFALSE;
	} 
	filldE_E_Right(v_RightDet);

	TGo4CompositeEvent* v_LeftDet = (TGo4CompositeEvent*)(v_input->getEventElement("Left_telescope"));
	if (!v_LeftDet) { 
		cerr << "Detector Left_telescope was not found." << endl;
		return kFALSE; 
	} 
	filldE_E_Left(v_LeftDet);

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
//-----------------------------------------------------------------------
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
//-----------------------------------------------------------------------
void UserProcTestMonitoring::UserPostLoop()
{
}
//-----------------------------------------------------------------------
void UserProcTestMonitoring::ProcessMessage(DetEventStation* p_message,Int_t parNum)
{

}
//-----------------------------------------------------------------------
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

	fstPair[8] = make_pair((TString)"DSDX_C",16);
	fstPair[9] = make_pair((TString)"DSDY_C",16);
			
	for(Int_t i=0; i<fnPars; i++) {
		fParSi[i] = (SiCalibPars*) MakeParameter(fstPair[i].first, "SiCalibPars");
		fParSi[i]->Init(fstPair[i].second,fstPair[i].first);
	}
}
//-----------------------------------------------------------------------
void UserProcTestMonitoring::calibSi(TGo4CompositeEvent* p_message,std::pair <TString,Int_t> pair)
{

}
//-----------------------------------------------------------------------
void UserProcTestMonitoring::filldE_E_Right(TGo4CompositeEvent* d_Event) {

	DetEventStation* st_SSD20_R = (DetEventStation*)(d_Event->getEventElement("Right_telescope_"+fParTest->f20_R_Name));
	if(!st_SSD20_R) {
		cout << " station SSD20_R was not found in event " << endl;
	}	
	TClonesArray *v_SSD20_R = st_SSD20_R->GetDetMessages();
	Int_t mult_SSD20_R = 0;
	Int_t nCh20;
	DetMessage* m_SSD20_R;
	for(Int_t i = 0; i<v_SSD20_R->GetEntriesFast(); i++) {
		DetMessage* message = (DetMessage*)v_SSD20_R->At(i);
		if(message->GetValue()>100) { // threshold for this 20um detector
			nCh20 = message->GetStChannel();
			m_SSD20_R = (DetMessage*)v_SSD20_R->At(i);			
			mult_SSD20_R++;
		}
	}
	if(mult_SSD20_R!=1) {
		// cerr << "Multiplicity in SSD20_R!=1 " <<mult_SSD20_R<< endl;
		return;
	}

	DetEventStation* st_DSDX_R = (DetEventStation*)(d_Event->getEventElement("Right_telescope_"+fParTest->f1_R_Name));
	if(!st_DSDX_R) {
		cout << "station DSDX_R was not found in event " << endl;
	}	
	TClonesArray *v_DSDX_R = st_DSDX_R->GetDetMessages();
	Int_t mult_DSDX_R = 0;
	Int_t nCh100;
	DetMessage* m_DSDX_R;
	for(Int_t i = 0; i<v_DSDX_R->GetEntriesFast(); i++) {
		DetMessage* message = (DetMessage*)v_DSDX_R->At(i);
		if(message->GetValue()>120) { // threshold for this 1 mm detector
			nCh100 = message->GetStChannel();
			m_DSDX_R = (DetMessage*)v_DSDX_R->At(i);
			mult_DSDX_R++;
		}
	}
	if(mult_DSDX_R!=1) {
		// cerr << "Multiplicity in DSDX_R!=1 " <<mult_DSDX_R<< endl;
		return;
	}

	this->getSiPar(fParTest->f20_R_Name);
	Double_t par20_1 = getSiPar(fParTest->f20_R_Name)->getPar1(nCh20);
	Double_t par20_2 = getSiPar(fParTest->f20_R_Name)->getPar2(nCh20);
	// cout << "par for 20_" << nCh20 << " " <<  m_SSD20_R->GetStChannel() << " " << par20_1 << "," << par20_2 << endl;

	this->getSiPar(fParTest->f1_R_Name);
	Double_t par100_1 = getSiPar(fParTest->f1_R_Name)->getPar1(nCh100);
	Double_t par100_2 = getSiPar(fParTest->f1_R_Name)->getPar2(nCh100);
	// cout << "par for 100_" << m_DSDX_R->GetStChannel() << " " << par100_1 << "," << par100_2 << endl;

	Double_t dE = m_SSD20_R->GetValue()*par20_2 + par20_1;
	Double_t Etotal = m_DSDX_R->GetValue()*par100_2 + par100_1 + dE;

	fHistoMan->dE_E_Right->Fill(Etotal,dE);	
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
void UserProcTestMonitoring::filldE_E_Left(TGo4CompositeEvent* d_Event) {

	DetEventStation* st_SSD20 = (DetEventStation*)(d_Event->getEventElement("Left_telescope_"+fParTest->f20_L_Name));
	if(!st_SSD20) {
		cout << " station SSD20_L was not found in event " << endl;
	}	
	TClonesArray *v_SSD20 = st_SSD20->GetDetMessages();
	Int_t mult_SSD20 = 0;
	Int_t nCh20;
	DetMessage* m_SSD20;
	for(Int_t i = 0; i<v_SSD20->GetEntriesFast(); i++) {
		DetMessage* message = (DetMessage*)v_SSD20->At(i);
		if(message->GetValue()>100) { // threshold for this 20um detector
			nCh20 = message->GetStChannel();
			m_SSD20 = (DetMessage*)v_SSD20->At(i);			
			mult_SSD20++;
		}
	}
	if(mult_SSD20!=1) {
		return;
	}

	DetEventStation* st_DSDX = (DetEventStation*)(d_Event->getEventElement("Left_telescope_"+fParTest->f1_L_Name));
	if(!st_DSDX) {
		cout << "station DSDX_L was not found in event " << endl;
	}	
	TClonesArray *v_DSDX = st_DSDX->GetDetMessages();
	Int_t mult_DSDX = 0;
	Int_t nCh100;
	DetMessage* m_DSDX;
	for(Int_t i = 0; i<v_DSDX->GetEntriesFast(); i++) {
		DetMessage* message = (DetMessage*)v_DSDX->At(i);
		if(message->GetValue()>120) { // threshold for this 1 mm detector
			nCh100 = message->GetStChannel();
			m_DSDX = (DetMessage*)v_DSDX->At(i);
			mult_DSDX++;
		}
	}
	if(mult_DSDX!=1) {
		return;
	}

	this->getSiPar(fParTest->f20_L_Name);
	Double_t par20_1 = getSiPar(fParTest->f20_L_Name)->getPar1(nCh20);
	Double_t par20_2 = getSiPar(fParTest->f20_L_Name)->getPar2(nCh20);
	cout << "Lpar for 20_" << nCh20 << " " <<  m_SSD20->GetStChannel() << " " << par20_1 << "," << par20_2 << endl;

	this->getSiPar(fParTest->f1_L_Name);
	Double_t par100_1 = getSiPar(fParTest->f1_L_Name)->getPar1(nCh100);
	Double_t par100_2 = getSiPar(fParTest->f1_L_Name)->getPar2(nCh100);
	cout << "Lpar for 100_" << m_DSDX->GetStChannel() << " " << par100_1 << "," << par100_2 << endl;

	Double_t dE = m_SSD20->GetValue()*par20_2 + par20_1;
	Double_t Etotal = m_DSDX->GetValue()*par100_2 + par100_1 + dE;

	fHistoMan->dE_E_Left->Fill(Etotal,dE);	
}
//-----------------------------------------------------------------------

SiCalibPars* UserProcTestMonitoring::getSiPar(TString st_Name) {
	// cout << "UserProcTestMonitoring::getSiPar was called! " << endl;
	Int_t i = 0;
	while(i<fnPars) {
		if(st_Name == fParSi[i]->GetName()) break;
		i++;
	}
	return fParSi[i];
}
//-----------------------------------------------------------------------
Int_t UserProcTestMonitoring::getMultiplicity(TClonesArray *v_array,TString st_Name) {
	cout << "UserProcTestMonitoring::getMultiplicity was called " << endl;
}
//-----------------------------------------------------------------------
ClassImp(UserProcTestMonitoring)
