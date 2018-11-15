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
#include <TH2F.h>
#include <TFile.h>

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
	// reading thickness distr for thin detectors
	this->readThickness();

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

	TGo4CompositeEvent* v_CentralDet = (TGo4CompositeEvent*)(v_input->getEventElement("Central_telescope"));
	if (!v_CentralDet) { 
		cerr << "Detector Central_telescope was not found." << endl;
		return kFALSE; 
	} 
	filldE_E_Central(v_CentralDet);

	FillAutoHistosCal(v_input);
/*
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
	// --------------------------*/

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
	fstPair[5] = make_pair((TString)"SSDY_R",16);	
	fstPair[6] = make_pair((TString)"SSD_R",16);

	fstPair[7] = make_pair((TString)"DSDX_C",32);
	fstPair[8] = make_pair((TString)"DSDY_C",32);
			
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
	TString detName = d_Event->GetName() + TString("_");

	DetEventStation* st_SSD20_R = (DetEventStation*)(d_Event->getEventElement(detName+fParTest->f20_R_Name));
	if(!st_SSD20_R) {
		cout << " station " << detName+fParTest->f20_R_Name << " was not found in event " << endl;
	}	
	TClonesArray *v_SSD20_R = st_SSD20_R->GetDetMessages();
	Int_t mult_SSD20_R = 0;
	Int_t nCh20;
	DetMessage* m_SSD20_R;
	if (fParTest->fThreshSSD20_R == 0) {
		mult_SSD20_R = v_SSD20_R->GetEntriesFast();
	}
	else {
		for(Int_t i = 0; i<v_SSD20_R->GetEntriesFast(); i++) {
			DetMessage* message = (DetMessage*)v_SSD20_R->At(i);
			if(message->GetValue()>fParTest->fThreshSSD20_R) { // threshold for this 20um detector
				nCh20 = message->GetStChannel();
				m_SSD20_R = (DetMessage*)v_SSD20_R->At(i);			
				mult_SSD20_R++;
			}
		}
	}
	if(mult_SSD20_R!=1) {
		// cerr << "Multiplicity in SSD20_R!=1 " <<mult_SSD20_R<< endl;
		return;
	}

	DetEventStation* st_DSD_R = (DetEventStation*)(d_Event->getEventElement(detName+fParTest->f1_R_Name));
	if(!st_DSD_R) {
		cout << "station " << detName+fParTest->f1_R_Name << " was not found in event " << endl;
	}	
	TClonesArray *v_DSD_R = st_DSD_R->GetDetMessages();
	Int_t mult_DSD_R = 0;
	Int_t nCh100;
	DetMessage* m_DSD_R;
	if (fParTest->fThreshSSDY_R == 0) {
		mult_DSD_R = v_DSD_R->GetEntriesFast();
	}
	else {
		for(Int_t i = 0; i<v_DSD_R->GetEntriesFast(); i++) {
			DetMessage* message = (DetMessage*)v_DSD_R->At(i);
			if(message->GetValue()>fParTest->fThreshSSDY_R) { // threshold for this 1 mm detector
				nCh100 = message->GetStChannel();
				m_DSD_R = (DetMessage*)v_DSD_R->At(i);
				mult_DSD_R++;
			}
		}
	}
	if(mult_DSD_R!=1) {
		// cerr << "Multiplicity in DSDX_R!=1 " <<mult_DSDX_R<< endl;
		return;
	}

	Double_t par20_1 = getSiPar(fParTest->f20_R_Name)->getPar1(nCh20);
	Double_t par20_2 = getSiPar(fParTest->f20_R_Name)->getPar2(nCh20);
	// cout << "par for 20_" << nCh20 << " " <<  m_SSD20_R->GetStChannel() << " " << par20_1 << "," << par20_2 << endl;

	Double_t par100_1 = getSiPar(fParTest->f1_R_Name)->getPar1(nCh100);
	Double_t par100_2 = getSiPar(fParTest->f1_R_Name)->getPar2(nCh100);
	// cout << "par for 100_" << m_DSDX_R->GetStChannel() << " " << par100_1 << "," << par100_2 << endl;

	// check if thickness if reasonable
	if(fThicknessRight[nCh20][nCh100]<10 || fThicknessRight[nCh20][nCh100] > 30) return;

	Double_t dE = m_SSD20_R->GetValue()*par20_2 + par20_1;
	dE = dE*20./fThicknessRight[nCh20][nCh100];
	Double_t Etotal = m_DSD_R->GetValue()*par100_2 + par100_1 + dE;

	fHistoMan->dE_E_Right->Fill(Etotal,dE);	
}
//-----------------------------------------------------------------------
void UserProcTestMonitoring::filldE_E_Left(TGo4CompositeEvent* d_Event) {
	TString detName = d_Event->GetName() + TString("_");

	DetEventStation* st_SSD20 = (DetEventStation*)(d_Event->getEventElement(detName+fParTest->f20_L_Name));
	if(!st_SSD20) {
		cout << " station " << detName+fParTest->f20_L_Name << " was not found in event " << endl;
	}	
	TClonesArray *v_SSD20 = st_SSD20->GetDetMessages();
	Int_t mult_SSD20 = 0;
	Int_t nCh20;
	DetMessage* m_SSD20;
	if (fParTest->fThreshSSD20_L == 0) {
		mult_SSD20 = v_SSD20->GetEntriesFast();
	}
	else {
		for(Int_t i = 0; i<v_SSD20->GetEntriesFast(); i++) {
			DetMessage* message = (DetMessage*)v_SSD20->At(i);
			if(message->GetValue()>fParTest->fThreshSSD20_L) { // threshold for this 20um detector
				nCh20 = message->GetStChannel();
				m_SSD20 = (DetMessage*)v_SSD20->At(i);			
				mult_SSD20++;
			}
		}
	}
	if(mult_SSD20!=1) {
		return;
	}

	DetEventStation* st_DSDX = (DetEventStation*)(d_Event->getEventElement(detName+fParTest->f1_L_Name));
	if(!st_DSDX) {
		cout << "station " << detName+fParTest->f1_L_Name << " was not found in event " << endl;
	}	
	TClonesArray *v_DSDX = st_DSDX->GetDetMessages();
	Int_t mult_DSDX = 0;
	Int_t nCh100;
	DetMessage* m_DSDX;

	if (fParTest->fThreshDSDX_L == 0) {
		mult_DSDX = v_DSDX->GetEntriesFast();
	}
	else {
		for(Int_t i = 0; i<v_DSDX->GetEntriesFast(); i++) {
			DetMessage* message = (DetMessage*)v_DSDX->At(i);
			if(message->GetValue()>fParTest->fThreshDSDX_L) { // threshold for this 1 mm detector
				nCh100 = message->GetStChannel();
				m_DSDX = (DetMessage*)v_DSDX->At(i);
				mult_DSDX++;
			}
		}
	}
	if(mult_DSDX!=1) {
		return;
	}

	Double_t par20_1 = getSiPar(fParTest->f20_L_Name)->getPar1(nCh20);
	Double_t par20_2 = getSiPar(fParTest->f20_L_Name)->getPar2(nCh20);
	// cout << "Lpar for 20_" << nCh20 << " " <<  m_SSD20->GetStChannel() << " " << par20_1 << "," << par20_2 << endl;

	Double_t par100_1 = getSiPar(fParTest->f1_L_Name)->getPar1(nCh100);
	Double_t par100_2 = getSiPar(fParTest->f1_L_Name)->getPar2(nCh100);
	// cout << "Lpar for 100_" << m_DSDX->GetStChannel() << " " << par100_1 << "," << par100_2 << endl;

	// check if thickness if reasonable
	if(fThicknessLeft[nCh20][nCh100]<10 || fThicknessLeft[nCh20][nCh100] > 30) return;

	Double_t dE = m_SSD20->GetValue()*par20_2 + par20_1;
	dE = dE*20./fThicknessLeft[nCh20][nCh100];
	Double_t Etotal = m_DSDX->GetValue()*par100_2 + par100_1 + dE;

	fHistoMan->dE_E_Left->Fill(Etotal,dE);	
}
//-----------------------------------------------------------------------
void UserProcTestMonitoring::filldE_E_Central(TGo4CompositeEvent* d_Event) {
	TString detName = d_Event->GetName() + TString("_");

	DetEventStation* st_DSSD_C = (DetEventStation*)(d_Event->getEventElement(detName+fParTest->fX_C_Name));
	if(!st_DSSD_C) {
		cout << " station " << fParTest->fX_C_Name << " was not found in event " << endl;
	}	
	TClonesArray *v_DSSD_C = st_DSSD_C->GetDetMessages();
	Int_t mult_DSSD_C = 0;
	Int_t nCh100;
	DetMessage* m_DSSD_C;
	if (fParTest->fThreshSSD_C == 0) {
		mult_DSSD_C = v_DSSD_C->GetEntriesFast();
	}
	else {
		for(Int_t i = 0; i<v_DSSD_C->GetEntriesFast(); i++) {
			DetMessage* message = (DetMessage*)v_DSSD_C->At(i);
			if(message->GetValue()>fParTest->fThreshSSD_C) { // threshold for this 20um detector
				nCh100 = message->GetStChannel();
				m_DSSD_C = (DetMessage*)v_DSSD_C->At(i);			
				mult_DSSD_C++;
			}
		}
	}
	fHistoMan->mult_Central->Fill(mult_DSSD_C);
	// TODO clusterisation
	if(mult_DSSD_C!=1) {
		return;
	}

	DetEventStation* st_CsI = (DetEventStation*)(d_Event->getEventElement(detName+fParTest->fCsI_Name));
	if(!st_CsI) {
		cout << " station " << fParTest->fCsI_Name << " was not found in event " << endl;
	}	
	TClonesArray *v_CsI = st_CsI->GetDetMessages();
	Int_t nChCsI;
	DetMessage* m_CsI;
	Double_t maxAmp = 0;
	// find maximum 
	for(Int_t i = 0; i < v_CsI->GetEntriesFast(); i++) {
		m_CsI = ((DetMessage*)v_CsI->At(i));
		if(m_CsI->GetValue() > maxAmp) {
			maxAmp = m_CsI->GetValue();
			nChCsI = m_CsI->GetStChannel();	
		} 
	}
	// check if there is only 1 maximum
	Int_t nMax = 0;
	for(Int_t i=0; i<v_CsI->GetEntriesFast(); i++) {
		if (((DetMessage*)v_CsI->At(i))->GetValue() == maxAmp) nMax++;
	}
	if (nMax!=1) return;

	Double_t par100_1 = getSiPar(fParTest->fX_C_Name)->getPar1(nCh100);
	Double_t par100_2 = getSiPar(fParTest->fX_C_Name)->getPar2(nCh100);

	fHistoMan->dE_E_Central[nChCsI]->Fill(maxAmp,m_DSSD_C->GetValue()*par100_2 + par100_1);		

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
void UserProcTestMonitoring::FillAutoHistosCal(DetEventFull *v_input) {
	// cout << "FillAutoHistosCal called " << endl;

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
}
//-----------------------------------------------------------------------
void UserProcTestMonitoring::readThickness() {
  cout << "thickness Left detector " << endl;
  TFile *f = new TFile("/media/user/work/software/fork/useranalysis/calibration/parameters/thicknessLeft.root","READ");
  if (f->IsZombie()) {
    for(Int_t i = 0; i<16; i++) {
      for(Int_t j = 0; j<16; j++) {
        fThicknessLeft[i][j] = 20.;
        cout << fThicknessLeft[i][j] << " ";
      }
      cout << endl;
    }

  }
  else {
	  TH2F *hThick = (TH2F*)f->Get("hTh");
	  for(Int_t i = 0; i<16; i++) {
	    for(Int_t j = 0; j<16; j++) {
	      fThicknessLeft[i][j] = hThick->GetBinContent(i+1,j+1);
	      cout << fThicknessLeft[i][j] << " ";
	    }
	    cout << endl;
	  }
  }
  delete f;

  cout << "thickness Right detector " << endl;
  TFile *f1 = new TFile("/media/user/work/software/fork/useranalysis/calibration/parameters/thicknessRight.root","READ");
  if (f1->IsZombie()) {
    for(Int_t i = 0; i<16; i++) {
      for(Int_t j = 0; j<16; j++) {
        fThicknessRight[i][j] = 20.;
        cout << fThicknessRight[i][j] << " ";
      }
      cout << endl;
    }
  }
  else {
	  TH2F *hThick = (TH2F*)f1->Get("hTh");
	  for(Int_t i = 0; i<16; i++) {
	    for(Int_t j = 0; j<16; j++) {
	      fThicknessRight[i][j] = hThick->GetBinContent(i+1,j+1);
	      cout << fThicknessRight[i][j] << " ";
	    }
	    cout << endl;
	  }
  }
  delete f1;
  return;
}
//-----------------------------------------------------------------------s

ClassImp(UserProcTestMonitoring)
