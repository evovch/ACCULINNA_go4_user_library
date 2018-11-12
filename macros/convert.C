/**

	Please, specify correct input root file and correct corresponding setup.xml file.
	Currently the structure of the event in the input root file is not identified automatically
	but taken from the setup.xml file.
	
	nEvents = 0 - process all events.

*/
  // methods 
void readPars(Float_t *par1,Float_t *par2,TString st_Name,Int_t nCh);
void setZero();
Bool_t getStAmp(TGo4CompositeEvent* v_Det,TString st_Name,Float_t *amp,Int_t *mult,Float_t *par1,Float_t *par2);
Bool_t getStTime(TGo4CompositeEvent* v_Det,TString st_Name,Float_t *Time);

void printDetFullEvent(DetEventFull* theEvent);

TTree* GetTheTree(TFile* theFile, TString* treeName);

// athributes
Float_t parSSD20_L_1[16]; 
Float_t parSSD20_L_2[16];;
Float_t parDSDX_L_1[16];
Float_t parDSDX_L_2[16];
Float_t parDSDY_L_1[16];
Float_t parDSDY_L_2[16];
Float_t parSSD_L_1[16];
Float_t parSSD_L_2[16];

Float_t parSSD20_R_1[16];
Float_t parSSD20_R_2[16];
Float_t parSSDY_R_1[16];
Float_t parSSDY_R_2[16];
Float_t parSSD_R_1[16];
Float_t parSSD_R_2[16];

Float_t parDSDX_C_1[32];
Float_t parDSDX_C_2[32];
Float_t parDSDY_C_1[32];
Float_t parDSDY_C_2[32];
Float_t parCsI_1[16];
Float_t parCsI_2[16];

Bool_t is_validRight,is_validLeft,is_valid;

// Output data

Int_t trigger;

Float_t SSD20_L[16],tSSD20_L[16];
Int_t multSSD20_L;

Float_t DSDX_L[16],tDSDX_L[16];
Int_t multDSDX_L;

Float_t DSDY_L[16],tDSDY_L[16];
Int_t multDSDY_L;

Float_t SSD_L[16],tSSD_L[16];
Int_t multSSD_L;


Float_t SSD20_R[16],tSSD20_R[16];
Int_t multSSD20_R;

Float_t SSDY_R[16],tSSDY_R[16];
Int_t multSSDY_R;

Float_t SSD_R[16],tSSD_R[16];
Int_t multSSD_R;


Float_t DSDX_C[32],tDSDX_C[32];
Int_t multDSDX_C;

Float_t DSDY_C[32],tDSDY_C[32];
Int_t multDSDY_C;

Float_t CsI[16],tCsI[16];
Int_t multCsI;

//-----------------------------------------------------------
void convert(Int_t numer=1)
{
	cout << "macro " << numer << endl;
	UInt_t nEvents = 0;
	TString inSetupConfigFilename="../usr/setup2_exp201811.xml";
	TString fInput;
	fInput.Form("/media/user/work/data/exp201810/data/root/he8_07_000%d.root",numer);

	TString fOutput;
	fOutput.Form("/media/user/work/data/exp201810/workdir/analysed/out%d.root",numer);

	Bool_t temp_valid;	
	// Construct SetupConfiguration, which includes the input of the XML file
	SetupConfiguration* fSetupConfiguration = new SetupConfiguration(inSetupConfigFilename);
	
	readPars(parSSD20_L_1,parSSD20_L_2,(TString)"SSD20_L",16);
	readPars(parDSDX_L_1,parDSDX_L_2,(TString)"DSDX_L",16);
	readPars(parDSDY_L_1,parDSDY_L_2,(TString)"DSDY_L",16);
	readPars(parSSD_L_1,parSSD_L_2,(TString)"SSD_L",16);	

	readPars(parSSD20_R_1,parSSD20_R_2,(TString)"SSD20_R",16);
	readPars(parSSDY_R_1,parSSDY_R_2,(TString)"SSDY_R",16);
	readPars(parSSD_R_1,parSSD_R_2,(TString)"SSD_R",16);

	readPars(parDSDX_C_1,parDSDX_C_2,(TString)"DSDX_C",32);
	readPars(parDSDY_C_1,parDSDY_C_2,(TString)"DSDY_C",32);
	readPars(parCsI_1,parCsI_2,(TString)"CsI",16);	

	TChain *chInput = new TChain("stepRepackingxTree");
	chInput->Add(fInput.Data());	

	DetEventFull* theEvent = new DetEventFull("DetEventFull1");
	TGo4EventElement* theEventCopy = theEvent;	
	theEvent->synchronizeWithTree(chInput, &theEventCopy);

	TFile *fOut = new TFile(fOutput.Data(),"RECREATE");
	TTree *tOut = new TTree("tree", "filtred data");
	tOut->Branch("trigger",&trigger,"trigger/I");

	tOut->Branch("SSD20_L",&SSD20_L,"SSD20_L[16]/F");
	tOut->Branch("tSSD20_L",&tSSD20_L,"tSSD20_L[16]/F");
	tOut->Branch("multSSD20_L",&multSSD20_L,"multSSD20_L/I");
	tOut->Branch("DSDX_L",&DSDX_L,"DSDX_L[16]/F");
	tOut->Branch("tDSDX_L",&tDSDX_L,"tDSDX_L[16]/F");
	tOut->Branch("multDSDX_L",&multDSDX_L,"multDSDX_L/I");
	tOut->Branch("DSDY_L",&DSDY_L,"DSDY_L[16]F");
	tOut->Branch("tDSDY_L",&tDSDY_L,"tDSDY_L[16]/F");
	tOut->Branch("multDSDY_L",&multDSDY_L,"multDSDY_L/I");
	tOut->Branch("SSD_L",&SSD_L,"SSD_L[16]/F");
	tOut->Branch("tSSD_L",&tSSD_L,"tSSD_L[16]/F");
	tOut->Branch("multSSD_L",&multSSD_L,"multSSD_L/I");

	tOut->Branch("SSD20_R",&SSD20_R,"SSD20_R[16]/F");
	tOut->Branch("tSSD20_R",&tSSD20_R,"tSSD20_R[16]/F");
	tOut->Branch("multSSD20_R",&multSSD20_R,"multSSD20_R/I");
	tOut->Branch("SSDY_R",&SSDY_R,"SSDY_R[16]/F");
	tOut->Branch("tSSDY_R",&tSSDY_R,"tSSDY_R[16]/F");
	tOut->Branch("multSSDY_R",&multSSDY_R,"multSSDY_R/I");
	tOut->Branch("SSD_R",&SSD_R,"SSD_R[16]/F");
	tOut->Branch("tSSD_R",&tSSD_R,"tSSD_R[16]/F");
	tOut->Branch("multSSD_R",&multSSD_R,"multSSD_R/I");

	tOut->Branch("CsI",&CsI,"CsI[16]/F");
	tOut->Branch("tCsI",&tCsI,"tCsI[16]/F");
	tOut->Branch("multCsI",&multCsI,"multCsI/I");
	tOut->Branch("DSDX_C",&DSDX_C,"DSDX_C[32]/F");
	tOut->Branch("tDSDX_C",&tDSDX_C,"tDSDX_C[32]/F");
	tOut->Branch("multDSDX_C",&multDSDX_C,"multDSDX_C/I");
	tOut->Branch("DSDY_C",&DSDY_C,"DSDY_C[32]/F");
	tOut->Branch("tDSDY_C",&tDSDY_C,"tDSDY_C[32]32/F");
	tOut->Branch("multDSDY_C",&multDSDY_C,"multDSDY_C/I");


	UInt_t nEventsTotal = chInput->GetEntries();
	if (nEvents == 0) { nEvents = nEventsTotal; }

	// Loop over the events
	for (UInt_t iEvent=1; iEvent<10000; iEvent++) {
		setZero();

		if (iEvent%100000==0) cout << "###" << iEvent << "###" << endl;

		chInput->GetEntry(iEvent);

		//TODO implement you actions here
		if(iEvent == 1) printDetFullEvent(theEvent);		
		// theEvent->Print();

		TGo4CompositeEvent* v_RightDet = (TGo4CompositeEvent*)(theEvent->getEventElement("Right_telescope"));
		if (!v_RightDet) { 
			cerr << "Detector Right_telescope was not found." << endl; 
			return;
		} 

		is_validRight = is_validRight*getStAmp(v_RightDet,"SSD20_R",SSD20_R,&multSSD20_R,parSSD20_L_1,parSSD20_L_2);
		is_validRight = is_validRight*getStTime(v_RightDet,"tSSD20_R",tSSD20_R);
		// if(multSSD20_R>0){
		// 	cerr << "Event "<< iEvent
		// 	<< " =================================================================="
		// 	<< endl;

		// 	cout << "  " << multSSD20_R << endl;
		// 	for(Int_t i=0;i<16;i++) {
		// 		if (SSD20_R[i]!=0) cout << i << " " <<  SSD20_R[i] << endl;
		// 		if (tSSD20_R[i]!=0) cout << i << " " <<  tSSD20_R[i] << endl;				
		// 	}
		// }

		is_validRight = is_validRight*getStAmp(v_RightDet,"SSDY_R",SSDY_R,&multSSDY_R,parSSDY_R_1,parSSDY_R_2);
		is_validRight = is_validRight*getStTime(v_RightDet,"tSSDY_R",tSSDY_R);

		is_validRight = is_validRight*getStAmp(v_RightDet,"SSDY_R",SSDY_R,&multSSDY_R,parSSDY_R_1,parSSDY_R_2);
		is_validRight = is_validRight*getStTime(v_RightDet,"tSSDY_R",tSSDY_R);

		is_valid = getStAmp(v_RightDet,"SSDY_R",SSDY_R,&multSSDY_R,parSSDY_R_1,parSSDY_R_2);
		is_valid = getStTime(v_RightDet,"tSSDY_R",tSSDY_R);

		
		TGo4CompositeEvent* v_LeftDet = (TGo4CompositeEvent*)(theEvent->getEventElement("Left_telescope"));
		if (!v_LeftDet) { 
			cerr << "Detector Left_telescope was not found." << endl;
			return;
		} 

		is_validLeft = is_validLeft*getStAmp(v_LeftDet,"SSD20_L",SSD20_L,&multSSD20_L,parSSD20_L_1,parSSD20_L_2);
		is_validLeft = is_validLeft*getStTime(v_LeftDet,"tSSD20_L",tSSD20_L);


		is_validLeft = is_validLeft*getStAmp(v_LeftDet,"DSDX_L",DSDX_L,&multDSDX_L,parDSDX_L_1,parDSDX_L_2);
		is_validLeft = is_validLeft*getStTime(v_LeftDet,"tDSDX_L",tDSDX_L);

		is_validLeft = is_validLeft*getStAmp(v_LeftDet,"DSDY_L",DSDY_L,&multDSDY_L,parDSDY_L_1,parDSDY_L_2);
		is_validLeft = is_validLeft*getStTime(v_LeftDet,"tDSDY_L",tDSDY_L);

		is_valid = getStAmp(v_LeftDet,"SSD_L",SSD_L,&multSSD_L,parSSD_L_1,parSSD_L_2);
		is_valid = getStTime(v_LeftDet,"tSSD_L",tSSD_L);

		TGo4CompositeEvent* v_CentDet = (TGo4CompositeEvent*)(theEvent->getEventElement("Central_telescope"));
		if (!v_CentDet) { 
			cerr << "Detector Central_telescope was not found." << endl;
			return;
		} 
		getStAmp(v_CentDet,"DSDX_C",DSDX_C,&multDSDX_C,parDSDX_C_1,parDSDX_C_2);
		getStTime(v_CentDet,"tDSDX_C",tDSDX_C);

		getStAmp(v_CentDet,"DSDY_C",DSDY_C,&multDSDY_C,parDSDY_C_1,parDSDY_C_2);
		getStTime(v_CentDet,"tDSDY_C",tDSDY_C);

		getStAmp(v_CentDet,"CsI",CsI,&multCsI,parCsI_1,parCsI_2);
		getStTime(v_CentDet,"tCsI",tCsI);				

		TGo4EventElement* v_comElement = theEvent->getEventElement("DetEventCommon",1);
		if(!v_comElement) {
			cout << "Detector DetEventCommon was not found " << endl;
		}
		DetEventCommon* v_commSubEl = (DetEventCommon*)(v_comElement);
		trigger = v_commSubEl->trigger;

		tOut->Fill();
	}
	tOut->Write();
	fOut->Close();

}
//-----------------------------------------------------------------------

TTree* GetTheTree(TFile* theFile, TString* treeName)
{
	if (*treeName == "") {
		//// Search for a tree, take the first with the name ending with 'xTree'
		UInt_t v_keysCounter = 0;
		TList* v_keys = theFile->GetListOfKeys();
		TIter v_nextkey(v_keys);
		while (TKey* v_curKey = (TKey*)v_nextkey()) {
			TString v_curKeyStr(v_curKey->GetName());
			////cerr << "key " << v_keysCounter++ << ": " << v_curKeyStr << endl;
			if (v_curKeyStr.EndsWith("xTree")) {
				*treeName = v_curKeyStr;
				cerr << "Automatically defined tree name: '"
				     << *treeName << "'" << endl;
				break;
			}
		} // end of while loop
	}

	TTree* foundTree = (TTree*)theFile->Get(*treeName);
	if (foundTree != NULL) {
		cerr << "Found the tree '" << *treeName << "'." << endl;
		return foundTree;
	} else {
		cerr << "Could not find the tree '" << *treeName << "'. Aborting." << endl;
		return NULL;
	}
}
//-----------------------------------------------------------------------
Bool_t getStAmp(TGo4CompositeEvent* v_Det,TString st_Name,Float_t *amp,Int_t *mult,Float_t *par1,Float_t *par2) {
	// cout << "getStAmp" << endl;
	TString st_Fullname = v_Det->GetName() + TString("_")+st_Name;

	DetEventStation* station = (DetEventStation*)(v_Det->getEventElement(st_Fullname.Data()));
	if(!station) {
		cout << "Station " << st_Fullname.Data() << " was not found in event " << endl;
		return kFALSE;
	}	
	TClonesArray *array = station->GetDetMessages();
	if (array->GetEntriesFast()==0) return kFALSE;

	*mult = array->GetEntriesFast();
	Int_t nChannel;

	DetMessage* message;
	Float_t p1,p2;
	for(Int_t i = 0; i<*mult; i++) {
		message = (DetMessage*)array->At(i);
		nChannel = message->GetStChannel();
		p1 = *(par1+nChannel);
		p2 = *(par2+nChannel);
		*(amp+nChannel) = Float_t(message->GetValue())*p2 + p1;
	}	

	return kTRUE;
}

//-----------------------------------------------------------------------
Bool_t getStTime(TGo4CompositeEvent* v_Det,TString st_Name,Float_t *Time) {
	TString st_Fullname = v_Det->GetName() + TString("_")+st_Name;

	DetEventStation* station = (DetEventStation*)(v_Det->getEventElement(st_Fullname.Data()));
	if(!station) {
		cout << "Station " << st_Fullname.Data() << " was not found in event " << endl;
		return kFALSE;
	}	
	TClonesArray *array = station->GetDetMessages();
	if (array->GetEntriesFast()==0) return kFALSE;

	Int_t nChannel;

	DetMessage* message;
	for(Int_t i = 0; i<array->GetEntriesFast(); i++) {
		message = (DetMessage*)array->At(i);
		nChannel = message->GetStChannel();
		*(Time+nChannel) = Float_t(message->GetValue())*0.3;
	}	
	return kTRUE;
}
//-----------------------------------------------------------------------
void printDetFullEvent(DetEventFull* theEvent) {
	cout << "###XML LIST###" << endl; 
	Short_t v_NsubElems = theEvent->getNElements();
	//cerr << v_NsubElems << " subelements in the input full event." << endl;

	// Loop over sub-elements. There is one sub-element which is the 'DetEventCommon'
	// and all other are 'DetEventDetector's
	for (Short_t i=0; i<v_NsubElems; i++) {
		TGo4EventElement* v_subElement = theEvent->getEventElement(i);

		TString curName = v_subElement->GetName();
		Short_t curId = v_subElement->getId();
		cerr << curId << ") " << curName << endl;;

		if (curName!= "DetEventCommon"){
			TGo4CompositeEvent* v_detSubEl = (TGo4CompositeEvent*)(v_subElement);

			Short_t v_NsubSubElems = v_detSubEl->getNElements();
			//cerr << " - " << v_NsubSubElems << " subsubelements." << endl;

			// Loop over the stations of the current detector
			for (Short_t j=0; j<v_NsubSubElems; j++) {

				Short_t stId = curId*100 + j; //FIXME this is quite dangerous

				DetEventStation* v_stSubsubEl = (DetEventStation*)(v_detSubEl->getEventElement(stId));
				cerr << "\t" << stId << ") " << v_stSubsubEl->GetName() << endl;


			} // end of loop over the stations
		} // end of if
	} // end of loop over the sub-elements (detectors)
	cout << "###END of XML LIST###" << endl << endl; 
}
//-----------------------------------------------------------------------
void readPars(Float_t *par1,Float_t *par2,TString st_Name,Int_t nCh){

	TString fName = ("/media/user/work/software/fork/useranalysis/calibration/parameters/") + st_Name + ".cal";
	// cout << fName << endl;
  // for 1 mm Si detector
  TString line;
  ifstream myfile;
  Int_t count=-2;
  myfile.open(fName.Data());
  while (! myfile.eof() ){
    line.ReadLine(myfile);
    if(count < 0){
      count++;
      continue;
    }
    if(line.IsNull()) break;
    sscanf(line.Data(),"%f %f", par1+count,par2+count);
    count++;
  }
  cout << endl << " pars for " << st_Name <<  " strips" << endl;
  for(Int_t i=0;i<nCh;i++) cout << par1[i] << " " << par2[i] << endl; 
}
//-----------------------------------------------------------------------
void setZero() {
	is_valid = kTRUE;
	is_validRight = kTRUE;
	is_validLeft = kTRUE;

	multSSD20_L = 0;
	multDSDX_L = 0;
	multDSDY_L = 0;
	multSSD_L = 0;

	multSSD20_R = 0;
	multSSDY_R = 0;
	multSSD_R = 0;

	multDSDX_C = 0;
	multDSDY_C = 0;
	multCsI = 0;

	for(Int_t i=0;i<16;i++){
		SSD20_L[i] = 0.; tSSD20_L[i] = 0.;
		DSDX_L[i] = 0.; tDSDX_L[i] = 0.;
		DSDY_L[i] = 0.; tDSDY_L[i] = 0.;
		SSD_L[i] = 0; tSSD_L[i] = 0;

		SSD20_R[i] = 0.; tSSD20_R[i] = 0.;
		SSDY_R[i] = 0.; tSSDY_R[i] = 0.;
		SSD_R[i] = 0; tSSD_L[i] = 0;

		CsI[i] = 0; tCsI[i] = 0;			
	}
	for(Int_t i=0;i<32;i++){
		DSDX_C[i] = 0; tDSDX_C[i] = 0;
		DSDY_C[i] = 0; tDSDY_C[i] = 0;		
	}
}
//-----------------------------------------------------------------------