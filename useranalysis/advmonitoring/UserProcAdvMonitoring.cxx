#include "UserProcAdvMonitoring.h"

// STD
#include <fstream>
#include <iostream>
using std::cerr;
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

#include "UserHistosAdvMonitoring.h"
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
//#define DEBUGADVMON

UserProcAdvMonitoring::UserProcAdvMonitoring(const char* name) :
	TGo4EventProcessor(name),
	fEventCounter(0)
{
	fHistoMan = new UserHistosAdvMonitoring();
	readParFile("/media/user/work/data/analysisexp1804/presentPars/csi_r_ec.clb");
	// cerr << " UserProcAdvMonitoring CALLED !!! ## &Y$@!UHNEFJNASJDf " << endl;
	fFileSummary = fopen("textoutput/summaryAdvMonitoring.txt", "w");
	if (fFileSummary == NULL) {
		//TODO error
		cerr << "[WARN  ] " << "Could not open output text summary file '" << "summaryAdvMonitoring.txt" << "'" << endl;
	}
}

UserProcAdvMonitoring::~UserProcAdvMonitoring()
{
	if (fHistoMan) delete fHistoMan;
	if (fFileSummary != NULL) {
		fclose(fFileSummary);
	}
}

Bool_t UserProcAdvMonitoring::BuildEvent(TGo4EventElement* p_dest)
{
	// cerr << "\t ### Build Event was called! next EVENT ### " <<  endl;

	Bool_t v_isValid = kFALSE;

	DetEventFull* v_input = (DetEventFull*)GetInputEvent("stepRepackedProvider1");
	if (v_input == NULL)
	{
		cerr << "[WARN  ] " << "UserProcAdvMonitoring::BuildEvent(): no input event!" << endl;
		return v_isValid;
	}
	v_isValid = kTRUE;

	#ifdef DEBUGADVMON
	cerr << "[DEBUG ] " << "UserProcAdvMonitoring: Event " << fEventCounter
	     << " ======================================================================================================"
	     << endl;
	#endif

	// --------------------------

	Short_t v_NsubElems = v_input->getNElements();
	//cerr << v_NsubElems << " subelements in the input full event." << endl;
	Int_t trigger;
	Int_t x1,y1;
	// Loop over sub-elements. There is one sub-element which is the 'DetEventCommon'
	// and all other are 'DetEventDetector's
	for (Short_t i=0; i<v_NsubElems; i++) {
		TGo4EventElement* v_subElement = v_input->getEventElement(i);

		TString curName = v_subElement->GetName();
		Short_t curId = v_subElement->getId();
		// cerr << curId << ") " << curName << " this is it!! " << endl;
		if (curName == "DetEventCommon") {
			DetEventCommon* v_commSubEl = (DetEventCommon*)(v_subElement);
			trigger = v_commSubEl->trigger;
			fHistoMan->fTrigger->Fill(v_commSubEl->trigger);
			//cerr << endl; 
			// Here you can process information from the 'common' sub-element
		} else {
			TGo4CompositeEvent* v_detSubEl = (TGo4CompositeEvent*)(v_subElement);

			Short_t v_NsubSubElems = v_detSubEl->getNElements();
			//cerr << " - " << v_NsubSubElems << " subsubelements." << endl;

			if(curName == "Beam_detector" || trigger==1) {
				// cerr << curId << ") " << curName << " BeamDet detector found " << endl;
				fill2D(v_detSubEl);
			}

			// Loop over the stations of the current detector
			for (Short_t j=0; j<v_NsubSubElems; j++) {

				Short_t stId = curId*100 + j; //FIXME this is quite dangerous

				DetEventStation* v_stSubsubEl = (DetEventStation*)(v_detSubEl->getEventElement(stId));
				TString stName = v_stSubsubEl->GetName();
				// if(stName!="Left_telescope_SQ300") continue;
				// cerr << "\t THIS IS STATION NAME!!! " << stId << ") " << v_stSubsubEl->GetName() << endl;

				TClonesArray* v_data = v_stSubsubEl->GetDetMessages();

				TIter v_detMiter(v_data);
				DetMessage* v_curDetM;

				// Loop over the messages of the current station 
				while ((v_curDetM = (DetMessage*)v_detMiter.Next())) {
					//v_curDetM->Print();


					unsigned int chFullId = stId*100 + v_curDetM->GetStChannel();

					// Fill automatically generated histograms
					fHistoMan->fAutoHistos.at(chFullId)->Fill(v_curDetM->GetValue());

					//TODO implement here your actions which require processing
					// of several messages simultaneously

					//TODO Look inside
					this->ProcessMessage(v_curDetM,stName);
				} // end of loop over messages
			} // end of loop over the stations
		} // end of if
	} // end of loop over the sub-elements (detectors)

	// --------------------------

	fEventCounter++;

	return v_isValid;
}

void UserProcAdvMonitoring::UserPreLoop()
{
	// Get the all-accessible parameter-set object
	UserParameter* v_params = (UserParameter*)GetParameter("UserParameter");
	fSetupConfig = v_params->GetSetupConfig();

	#ifdef DEBUGADVMON
	cerr << "[DEBUG ] " << "UserProcAdvMonitoring::UserPreLoop ====================================" << endl;
	#endif

	fHistoMan->GenerateAutoHistos();

	#ifdef DEBUGADVMON
	cerr << "[DEBUG ] " << "=======================================================================" << endl;
	#endif
}

void UserProcAdvMonitoring::UserPostLoop()
{
}

void UserProcAdvMonitoring::ProcessMessage(DetMessage* p_message, TString stName)
{
	//TODO implement your processing of independent messages here

	if(stName=="Left_telescope_CsI_L"){
		fHistoMan->fCsI_L[p_message->GetStChannel()]->Fill(p_message->GetValue());
	}
	if(stName=="Left_telescope_tCsI_L"){
		fHistoMan->ftCsI_L[p_message->GetStChannel()]->Fill(p_message->GetValue());
	}
	if(stName=="Right_telescope_CsI_R"){
		fHistoMan->fCsI_R[p_message->GetStChannel()]->Fill(p_message->GetValue());
		fHistoMan->fCsI_R_C[p_message->GetStChannel()]->Fill(p_message->GetValue()*parCsI_R_2[p_message->GetStChannel()]+parCsI_R_1[p_message->GetStChannel()]);
	}
	if(stName=="Right_telescope_tCsI_R"){
		fHistoMan->ftCsI_R[p_message->GetStChannel()]->Fill(p_message->GetValue());
	}
	if(stName=="Left_telescope_SQ300"){
		fHistoMan->fSQ20[p_message->GetStChannel()]->Fill(p_message->GetValue());
	}
	if(stName=="Left_telescope_tSQ300"){
		fHistoMan->ftSQ20[p_message->GetStChannel()]->Fill(p_message->GetValue());
	}
	if(stName=="Left_telescope_SQY_L"){
		fHistoMan->fSQY_L[p_message->GetStChannel()]->Fill(p_message->GetValue());
	}
	if(stName=="Left_telescope_tSQY_L"){
		fHistoMan->ftSQY_L[p_message->GetStChannel()]->Fill(p_message->GetValue());
	}
	if(stName=="Right_telescope_SQY_R"){
		fHistoMan->fSQY_R[p_message->GetStChannel()]->Fill(p_message->GetValue());
	}
	if(stName=="Right_telescope_tSQY_R"){
		fHistoMan->ftSQY_R[p_message->GetStChannel()]->Fill(p_message->GetValue());
	}
	if(stName=="Left_telescope_SQX_L"){
		fHistoMan->fSQX_L[p_message->GetStChannel()]->Fill(p_message->GetValue());
	}
	if(stName=="Left_telescope_tSQX_L"){
		fHistoMan->ftSQX_L[p_message->GetStChannel()]->Fill(p_message->GetValue());
	}
	if(stName=="Right_telescope_SQX_R"){
		fHistoMan->fSQX_R[p_message->GetStChannel()]->Fill(p_message->GetValue());
	}
	if(stName=="Right_telescope_tSQX_R"){
		fHistoMan->ftSQX_R[p_message->GetStChannel()]->Fill(p_message->GetValue());
	}
	if(stName=="Beam_detector_F5"){
		fHistoMan->fF5[p_message->GetStChannel()]->Fill(p_message->GetValue());
	}
	if(stName=="Beam_detector_tF5"){
		fHistoMan->ftF5[p_message->GetStChannel()]->Fill(p_message->GetValue());
	}
	if(stName=="Beam_detector_F3"){
		fHistoMan->fF3[p_message->GetStChannel()]->Fill(p_message->GetValue());
	}
	if(stName=="Beam_detector_tF3"){
		fHistoMan->ftF3[p_message->GetStChannel()]->Fill(p_message->GetValue());
	}
	if(stName=="Beam_detector_tMWPC"){
		fHistoMan->ftMWPC[p_message->GetStChannel()]->Fill(p_message->GetValue());
	}
	if(stName=="Beam_detector_MWPC1"){
		fHistoMan->fNX1->Fill(p_message->GetStChannel());
	}
	if(stName=="Beam_detector_MWPC2"){
		fHistoMan->fNY1->Fill(p_message->GetStChannel());
	}
	if(stName=="Beam_detector_MWPC3"){
		fHistoMan->fNX2->Fill(p_message->GetStChannel());
	}
	if(stName=="Beam_detector_MWPC4"){
		fHistoMan->fNY2->Fill(p_message->GetStChannel());
	}
}

void UserProcAdvMonitoring::readParFile(TString parFile){
	ifstream myfile;
  TString line;
  Int_t count=-2;
  myfile.open("/media/user/work/data/analysisexp1804/presentPars/csi_r_ec.clb");
  while (! myfile.eof() ) {
    line.ReadLine(myfile);
    if(count < 0){
      count++;
      continue;
    }
    if(line.IsNull()) break;
    sscanf(line.Data(),"%lf %lf", parCsI_R_1+count,parCsI_R_2+count);
    count++;
  }  

  // cerr << endl << " pars for CsR crystals" << endl;
  // for(Int_t i=0;i<16;i++) cerr << parCsI_R_1[i] << " " << parCsI_R_2[i] << endl; 
}

void UserProcAdvMonitoring::fill2D(TGo4CompositeEvent* dEvent){
	vector <Int_t> nx1;
	vector <Int_t> nx2;
	// cerr << " number of stations is: " << dEvent->getNElements() << endl;
	Short_t curId = dEvent->getId();
	// Loop over the stations of the detector
	for (Short_t j=0; j<dEvent->getNElements(); j++) {
		Short_t stId = curId*100 + j; //FIXME this is quite dangerous
		DetEventStation* v_stSubsubEl = (DetEventStation*)(dEvent->getEventElement(stId));
		TString stName = v_stSubsubEl->GetName();
		// cerr << stName << endl;
		TClonesArray* v_data = v_stSubsubEl->GetDetMessages();

		if(stName == "Beam_detector_MWPC1"){
			Int_t mx = v_data->GetEntriesFast();
			for(Int_t i = 0; i < mx; i++){
				DetMessage *mes_MWPC = (DetMessage*)v_data->At(i);
				// cerr << " this is wire number in MWPC1 " << mes_MWPC1->GetStChannel() << endl;
				nx1.push_back(mes_MWPC->GetStChannel());
			}
		}

		if(stName == "Beam_detector_MWPC2"){
			Int_t mx = v_data->GetEntriesFast();
			for(Int_t i = 0; i < mx; i++){
				DetMessage *mes_MWPC = (DetMessage*)v_data->At(i);
				// cerr << " this is wire number in MWPC1 " << mes_MWPC1->GetStChannel() << endl;
				nx2.push_back(mes_MWPC->GetStChannel());
			}
		}
	}
	for(Int_t i = 0; i < nx1.size();i++){
		for(Int_t j = 0; j < nx2.size(); j++){
			fHistoMan->fY1_X1->Fill(nx1.at(i),nx2.at(j));
		}
	}
	// cerr << " number of fired wires in 1 and 2 planes: " << nx1.size() << " " << nx2.size() << endl;

}

ClassImp(UserProcAdvMonitoring)
