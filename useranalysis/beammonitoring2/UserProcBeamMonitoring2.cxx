
#include "UserProcBeamMonitoring2.h"

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
#include <TRandom3.h>

// Project
#include "data/DetEventFull.h" // input event
#include "data/DetEventCommon.h"
#include "data/DetEventStation.h"
#include "data/DetMessage.h"

#include "UserHistosBeamMonitoring2.h"
#include "UserParameter.h"
#include "setupconfigcppwrapper/SetupConfiguration.h"
#include "UserParameterBeamDetector.h"

#include <stdlib.h>

using namespace std;
/**
  Uncomment this if you want to see all the debug information.
  This allows you to analyze the raw bytes and bits by your eyes.
  This option produces A LOT OF DATA - run your analysis with a
  small number of events (~10-100)
*/
//#define DEBUGBeamMON

UserProcBeamMonitoring2::UserProcBeamMonitoring2(const char* name) :
	TGo4EventProcessor(name),
	fEventCounter(0),
//	fMultW4(0),
	fSetupConfig(0),
	v_input(0)
{
	//todo rename parameter class
	fParBD = (UserParameterBeamDetector*)MakeParameter("BeamDetPar", "UserParameterBeamDetector");

	fHistoMan = new UserHistosBeamMonitoring2();
	// cerr << " UserProcBeamMonitoring CALLED !!! ## &Y$@!UHNEFJNASJDf " << endl;
	fFileSummary = fopen("textoutput/summaryBeamMonitoring.txt", "w");
	if (fFileSummary == NULL) {
		//TODO error
		cerr << "[WARN  ] " << "Could not open output text summary file '" << "summaryBeamMonitoring.txt" << "'" << endl;
	}
}

UserProcBeamMonitoring2::~UserProcBeamMonitoring2()
{
	if (fHistoMan) delete fHistoMan;
	if (fFileSummary != NULL) {
		fclose(fFileSummary);
	}
	if (v_input) {
		v_input = NULL;
	}
}

Bool_t UserProcBeamMonitoring2::BuildEvent(TGo4EventElement* p_dest)
{
	// cerr << "\t ### UserProcBeamMonitoring::BuildEvent was called ### " <<  endl;

	Bool_t v_isValid = kFALSE;

//	DetEventFull* v_input = (DetEventFull*)GetInputEvent("stepRepackedToBM2Provider");
	v_input = (DetEventFull*)GetInputEvent("stepRepackedProvider3");
//	v_input = (DetEventFull*)GetInputEvent("stepRepackedToBM2Provider");
	if (v_input == NULL)
	{
		cerr << "[WARN  ] " << "UserProcBeamMonitoring2::BuildEvent(): no input event!" << endl;
		return v_isValid;
	}
	v_isValid = kTRUE;

	#ifdef DEBUGBeamMON
	cerr << "[DEBUG ] " << "UserProcBeamMonitoring2: Event " << fEventCounter
	     << " ======================================================================================================"
	     << endl;
	#endif

	// --------------------------

//	Short_t v_NsubElems = v_input->getNElements();
	//cerr << v_NsubElems << " subelements in the input full event." << endl;
	Int_t trigger;
	// Loop over sub-elements. There is one sub-element which is the 'DetEventCommon'
	// and all other are 'DetEventDetector's

	TGo4EventElement* v_comElement = v_input->getEventElement("DetEventCommon",1);
	if(!v_comElement) {
		cout << "Detector DetEventCommon was not found " << endl;
		return kFALSE;
	}
	DetEventCommon* v_commSubEl = (DetEventCommon*)(v_comElement);
	trigger = v_commSubEl->trigger;
	if(trigger>5) {
		cout << " Event wont be processed " << endl;
		return kFALSE;
	}
//	fHistoMan->fTrigger->Fill(trigger);


//////////////////////////////////////
//	filling my own histograms
//////////////////////////////////////

	FillHistograms(trigger);

//////////////////////////////////////
//	filling my own histograms
//////////////////////////////////////

	fEventCounter++;

	v_input = NULL;

	return v_isValid;
}

void UserProcBeamMonitoring2::UserPreLoop()
{
	// Get the all-accessible parameter-set object
	UserParameter* v_params = (UserParameter*)GetParameter("UserParameter");
	fSetupConfig = v_params->GetSetupConfig();

	#ifdef DEBUGBeamMON
	cerr << "[DEBUG ] " << "UserProcBeamMonitoring::UserPreLoop ====================================" << endl;
	#endif

	// fHistoMan->GenerateAutoHistos();

	#ifdef DEBUGBeamMON
	cerr << "[DEBUG ] " << "=======================================================================" << endl;
	#endif
}

void UserProcBeamMonitoring2::UserPostLoop()
{
}

void UserProcBeamMonitoring2::ProcessMessage(DetMessage* p_message, TString stName)
{
	//TODO implement your processing of independent messages here
	// cout << stName << " :UserProcBeamMonitoring2::ProcessMessage" << endl;
}

ClassImp(UserProcBeamMonitoring2)

void UserProcBeamMonitoring2::FillHistograms(Int_t curTrigger) {

	//TOF
//	DetEventStation* st_F3a = (DetEventStation*)(v_input->getEventElement("Beam_detector_F3",1));
	DetEventStation* st_F3a = (DetEventStation*)(v_input->getEventElement(fParBD->GetF3aName(),1));
	if(!st_F3a) {
		cout << " station " << fParBD->GetF3aName() << " was not found in event " << fEventCounter << endl;
	}

//	DetEventStation* st_F5a = (DetEventStation*)(v_input->getEventElement("Beam_detector_F5",1));
	DetEventStation* st_F5a = (DetEventStation*)(v_input->getEventElement(fParBD->GetF5aName(),1));
	if(!st_F5a) {
		cout << " station " << fParBD->GetF5aName() << " was not found in event " << fEventCounter << endl;
	}

//	DetEventStation* st_F3t = (DetEventStation*)(v_input->getEventElement("Beam_detector_tF3",1));
	DetEventStation* st_F3t = (DetEventStation*)(v_input->getEventElement(fParBD->GetF3tName(),1));
	if(!st_F3t) {
		cout << " station " << fParBD->GetF3tName() << " was not found in event " << fEventCounter << endl;
	}

//	DetEventStation* st_F5t = (DetEventStation*)(v_input->getEventElement("Beam_detector_tF5",1));
	DetEventStation* st_F5t = (DetEventStation*)(v_input->getEventElement(fParBD->GetF5tName(),1));
	if(!st_F5t) {
		cout << " station " << fParBD->GetF5tName() << " was not found in event " << fEventCounter << endl;
	}

	TOF(st_F3a, st_F5a, st_F3t, st_F5t, curTrigger);

	//MWPC

	DetEventStation* st_MWPCX1 = (DetEventStation*)(v_input->getEventElement(fParBD->GetStationMWPCx1name().Data(),1));
	if(!st_MWPCX1) {
		cout << " station " << fParBD->GetStationMWPCx1name() << " was not found in event " << fEventCounter << endl;
	}

	DetEventStation* st_MWPCY1 = (DetEventStation*)(v_input->getEventElement(fParBD->GetStationMWPCy1name().Data(),1));
	if(!st_MWPCY1) {
		cout << " station " << fParBD->GetStationMWPCy1name() << " was not found in event " << fEventCounter << endl;
	}

	DetEventStation* st_MWPCX2 = (DetEventStation*)(v_input->getEventElement(fParBD->GetStationMWPCx2name().Data(),1));
	if(!st_MWPCX2) {
		cout << " station " << fParBD->GetStationMWPCx2name() << " was not found in event " << fEventCounter << endl;
	}

	DetEventStation* st_MWPCY2 = (DetEventStation*)(v_input->getEventElement(fParBD->GetStationMWPCy2name().Data(),1));
	if(!st_MWPCY2) {
		cout << " station " << fParBD->GetStationMWPCy2name() << " was not found in event " << fEventCounter << endl;
	}

	MWPCprojection(st_MWPCX1, st_MWPCY1, st_MWPCX2, st_MWPCY2, curTrigger);

	return;

}

void UserProcBeamMonitoring2::MWPCprojection(DetEventStation* mwpcPlaneX1,
		DetEventStation* mwpcPlaneY1, DetEventStation* mwpcPlaneX2,
		DetEventStation* mwpcPlaneY2, Int_t curTrigger) {


	Float_t x1p, y1p, x2p, y2p, xtw, ytw;
	Float_t x1c, y1c, x2c, y2c, xtc, ytc;

	Int_t wireMult[4] = { 0 }; //x1, y1, x2, y2
	Int_t wire[4]; //x1, y1, x2, y2

	Int_t clusterMult[4] = { 0 }; //x1, y1, x2, y2
	Float_t wireC[4]; //x1, y1, x2, y2


	TClonesArray* v_MWPC[4] = { NULL };	//x1, y1, x2, y2

	if (mwpcPlaneX1) {
		v_MWPC[0] = mwpcPlaneX1->GetDetMessages();
		wireMult[0] = GetWireMult(v_MWPC[0]);
		clusterMult[0] = GetClusterMult(v_MWPC[0]);
	}

	if (mwpcPlaneY1) {
		v_MWPC[1] = mwpcPlaneY1->GetDetMessages();
		wireMult[1] = GetWireMult(v_MWPC[1]);
		clusterMult[1] = GetClusterMult(v_MWPC[1]);
	}

	if (mwpcPlaneX2) {
		v_MWPC[2] = mwpcPlaneX2->GetDetMessages();
		wireMult[2] = GetWireMult(v_MWPC[2]);
		clusterMult[2] = GetClusterMult(v_MWPC[2]);
	}

	if (mwpcPlaneY2) {
		v_MWPC[3] = mwpcPlaneY2->GetDetMessages();
//		wireMult[3] = v_MWPC[3]->GetEntriesFast();
		wireMult[3] = GetWireMult(v_MWPC[3]);
		clusterMult[3] = GetClusterMult(v_MWPC[3]);
	}

	Int_t noCorrPlanesW = 0;
	Int_t noCorrPlanesC = 0;

	for (Int_t i = 0; i < 4; i++) {
		if (wireMult[i]==1) noCorrPlanesW++;
		if (clusterMult[i]==1) noCorrPlanesC++;
	}
	fHistoMan->fMWPCwireEff->Fill(noCorrPlanesW);
	fHistoMan->fMWPCclusterEff->Fill(noCorrPlanesC);

	for (Int_t i = 0; i < 4; i++) {
		if (curTrigger==fParBD->fTriggerCondition) {
			fHistoMan->fMultW[i]->Fill(wireMult[i]);
			fHistoMan->fMultC[i]->Fill(clusterMult[i]);
		}

		//filling if wire multiplicity equal to 1
		if (v_MWPC[i] && curTrigger==fParBD->fTriggerCondition) {
			//add comment here
			if (wireMult[i]==1 ) {
				wire[i] = ((DetMessage*)v_MWPC[i]->At(0))->GetStChannel();
				fHistoMan->fMWPCwire[i]->Fill(wire[i]);
			}

			//filling if cluster multiplicity equal to 1
			if (clusterMult[i]==1) {
				wireC[i] = GetClusterWire(v_MWPC[i]);
				fHistoMan->fMWPCcluster[i]->Fill(wireC[i]);
			}
		}//if one plane is present

	}


	for (Int_t i = 0; i < 2; i++) {
		if (v_MWPC[2*i] && v_MWPC[2*i+1] && curTrigger==fParBD->fTriggerCondition) {

			if (wireMult[2*i]==1 && wireMult[2*i+1]==1) {
				fHistoMan->fMWPCwireProfile[i]->Fill(wire[2*i], wire[2*i+1]);
			}
			if (clusterMult[2*i]==1 && clusterMult[2*i+1]==1) {
				fHistoMan->fMWPCclusterProfile[i]->Fill(wireC[2*i], wireC[2*i+1]);
			}


		}
	}

//	return;

	if (v_MWPC[0] && v_MWPC[1] && v_MWPC[2] && v_MWPC[3] && curTrigger==fParBD->fTriggerCondition) {

		//wire multiplicity equal to 1
		if (wireMult[0]==1 && wireMult[1]==1 && wireMult[2]==1 && wireMult[3]==1) {
			x1p = GetPosition(wire[0], fParBD->fMWPCwireStepX1, fParBD->fMWPC1_X_offset);
			y1p = GetPosition(wire[1], fParBD->fMWPCwireStepY1, fParBD->fMWPC1_Y_offset);

			x2p = GetPosition(wire[2], fParBD->fMWPCwireStepX2, fParBD->fMWPC2_X_offset);
			y2p = GetPosition(wire[3], fParBD->fMWPCwireStepY2, fParBD->fMWPC2_Y_offset);

			//TODO: generalize projection position
			xtw = x1p - (x2p -x1p)*fParBD->fMWPCz1/(fParBD->fMWPCz2-fParBD->fMWPCz1);
			ytw = y1p - (y2p -y1p)*fParBD->fMWPCz1/(fParBD->fMWPCz2-fParBD->fMWPCz1);


			fHistoMan->fMWPCmmW[0]->Fill(x1p);
			fHistoMan->fMWPCmmW[1]->Fill(y1p);
			fHistoMan->fMWPCmmW[2]->Fill(x2p);
			fHistoMan->fMWPCmmW[3]->Fill(y2p);

			fHistoMan->fMWPCProfileW[0]->Fill(x1p, y1p);
			fHistoMan->fMWPCProfileW[1]->Fill(x2p, y2p);
			fHistoMan->fMWPCProfileW[2]->Fill(xtw, ytw);
		}//if wire multiplicity equal to 1

		//cluster multiplicity equal to 1
		if (clusterMult[0]==1 && clusterMult[1]==1 && clusterMult[2]==1 && clusterMult[3]==1) {
			x1c = GetPosition(wireC[0], fParBD->fMWPCwireStepX1, fParBD->fMWPC1_X_offset);
			y1c = GetPosition(wireC[1], fParBD->fMWPCwireStepY1, fParBD->fMWPC1_Y_offset);

			x2c = GetPosition(wireC[2], fParBD->fMWPCwireStepX2, fParBD->fMWPC2_X_offset);
			y2c = GetPosition(wireC[3], fParBD->fMWPCwireStepY2, fParBD->fMWPC2_Y_offset);

			xtc = x1c - (x2c -x1c)*fParBD->fMWPCz1/(fParBD->fMWPCz2-fParBD->fMWPCz1);
			ytc = y1c - (y2c -y1c)*fParBD->fMWPCz1/(fParBD->fMWPCz2-fParBD->fMWPCz1);

//			cout <<
			fHistoMan->fMWPCmmC[0]->Fill(x1c);
			fHistoMan->fMWPCmmC[1]->Fill(y1c);
			fHistoMan->fMWPCmmC[2]->Fill(x2c);
			fHistoMan->fMWPCmmC[3]->Fill(y2c);

			fHistoMan->fMWPCProfileC[0]->Fill(x1c, y1c);
			fHistoMan->fMWPCProfileC[1]->Fill(x2c, y2c);
			fHistoMan->fMWPCProfileC[2]->Fill(xtc, ytc);
		}//if cluster multiplicity equal to 1

	}

}

Int_t UserProcBeamMonitoring2::GetWireMult(TClonesArray *data) {

	if (!data) return 0;

	return data->GetEntriesFast();

}

Int_t UserProcBeamMonitoring2::GetClusterMult(TClonesArray *data)
{

	if (!data) return 0;

	Int_t entries = data->GetEntriesFast();

	if (entries<2) return entries;

	Int_t wire1, wire2;
	Int_t noclusters = 1;


//	cout << "///\t" << fEventCounter << ":\t/////////////" << endl;

	for (Int_t i = 1; i < entries; i++) {
		//check if entries are in specific order
		wire1 = ((DetMessage*)data->At(i))->GetStChannel();
		wire2 = ((DetMessage*)data->At(i-1))->GetStChannel();

//		cout << wire2 << "\t" << wire1 << endl;
		//todo number 32 is related to number of wires
		// and should be taken from Parameters
		if ( abs(wire1 - wire2) > 1 && abs(wire1 - wire2) < 32) noclusters++;
	}

	return noclusters;
} //--------------------------------------------------------------------

Float_t UserProcBeamMonitoring2::GetClusterWire(TClonesArray *data)
{
	//working correctly only for cluster multiplicity equal to 1
	//get wire number in, generally, half-numbers, i.e. if cluster
	//consists from 2 wires n and n+1, cluster position is n+0.5

	Int_t wire1 = ((DetMessage*)data->At(0))->GetStChannel();

	return (Float_t)wire1 + 0.5*((Float_t)data->GetEntriesFast()-1.);

} //--------------------------------------------------------------------

void UserProcBeamMonitoring2::TOF(DetEventStation* F3a, DetEventStation* F5a,
		DetEventStation* F3t, DetEventStation* F5t,
		Int_t curTrigger) {


	TClonesArray* v_F3a = NULL;	//x1, y1, x2, y2
	TClonesArray* v_F5a = NULL;	//x1, y1, x2, y2
	TClonesArray* v_F3t = NULL;	//x1, y1, x2, y2
	TClonesArray* v_F5t = NULL;	//x1, y1, x2, y2

	Int_t F3MultA = 0;
	Int_t F5MultA = 0;
	Int_t F3MultT = 0;
	Int_t F5MultT = 0;

	if (F3a) {
		v_F3a = F3a->GetDetMessages();
		F3MultA = v_F3a->GetEntriesFast();
	}

	if (F5a) {
		v_F5a = F5a->GetDetMessages();
		F5MultA = v_F5a->GetEntriesFast();
	}

	if (F3t) {
		v_F3t = F3t->GetDetMessages();
		F3MultT = v_F3t->GetEntriesFast();
	}

	if (F5t) {
		v_F5t = F5t->GetDetMessages();
		F5MultT = v_F5t->GetEntriesFast();
	}

	if (curTrigger==fParBD->fTriggerCondition) {
		fHistoMan->fF3MultA->Fill(F3MultA);
		fHistoMan->fF5MultA->Fill(F5MultA);
		fHistoMan->fF3MultT->Fill(F3MultT);
		fHistoMan->fF5MultT->Fill(F5MultT);
	}

	//todo: should we initialize them?
	Float_t dEbeam = 0.;
	Float_t TOF = 0.;

	//todo: number of PMT's as parameter
//	const Int_t noPMTf3 = 4;
//	const Int_t noPMTf5 = 4;
//	const Float_t timeCal = 0.125;
//	const Float_t constTOF = 89.165;


	if (curTrigger==fParBD->fTriggerCondition) {

		if (F5MultA == fParBD->fNoPMTf5) {
			for (Int_t i = 0; i < fParBD->fNoPMTf5; i++) {
				dEbeam += ((DetMessage*)v_F5a->At(i))->GetValue();
			}
			dEbeam = dEbeam/(Float_t)fParBD->fNoPMTf5;
			fHistoMan->fdE->Fill(dEbeam);
		}

		if (F3MultT == fParBD->fNoPMTf3 && F5MultT == fParBD->fNoPMTf5) {
			Float_t timeF3, timeF5;
			for (Int_t i = 0; i < fParBD->fNoPMTf3; i++) {
				timeF3 = ((DetMessage*)v_F3t->At(i))->GetValue();
			}
			timeF3 = timeF3/(Float_t)fParBD->fNoPMTf3;

			for (Int_t i = 0; i < fParBD->fNoPMTf5; i++) {
				timeF5 = ((DetMessage*)v_F5t->At(i))->GetValue();
			}
			timeF5 = timeF5/(Float_t)fParBD->fNoPMTf5;

			TOF = (timeF5 - timeF3)*fParBD->fTimeCal + fParBD->fTOFconst;
			fHistoMan->fTOF->Fill(TOF);
		}

		if (F5MultA == fParBD->fNoPMTf5 && F3MultT == fParBD->fNoPMTf3 && F5MultT == fParBD->fNoPMTf5) {
			fHistoMan->fTOF_ID->Fill(TOF, dEbeam);
		}

	}//if current trigger
}

Float_t UserProcBeamMonitoring2::GetPosition(Float_t wire, Float_t wireStep,
		Float_t planeOffset) {
	//TODO: number of wires (16) as parameter
	return (wire + gRandom->Uniform(-0.5, 0.5) + 0.5 - 16)*wireStep + planeOffset;
}
