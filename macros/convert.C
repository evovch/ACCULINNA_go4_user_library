/**

	Please, specify correct input root file and correct corresponding setup.xml file.
	Currently the structure of the event in the input root file is not identified automatically
	but taken from the setup.xml file.
	
	nEvents = 0 - process all events.

*/
  // methods 
void readPars(Float_t *par1,Float_t *par2,TString st_Name);

Bool_t getStAmp(TGo4CompositeEvent* v_Det,TString st_Name,Float_t *value,Int_t *nCh);
Bool_t getStTime(TGo4CompositeEvent* v_Det,TString st_Name,Float_t *value,Int_t nCh);

void printDetFullEvent(DetEventFull* theEvent);

TTree* GetTheTree(TFile* theFile, TString* treeName);


// athributes
Float_t *par20L_1 = new Float_t[16]; 
Float_t *par20L_2 = new Float_t[16];;
Float_t *parSDSXL_1 = new Float_t[16];
Float_t *parSDSXL_2 = new Float_t[16];
Float_t *parSDSYL_1 = new Float_t[16];
Float_t *parSDSYL_2 = new Float_t[16];
Float_t *parSSDL_1 = new Float_t[16];
Float_t *parSSDL_2 = new Float_t[16];

Float_t *par20R_1 = new Float_t[16];
Float_t *par20R_2 = new Float_t[16];
Float_t *parSDSYR_1 = new Float_t[16];
Float_t *parSDSYR_2 = new Float_t[16];
Float_t *parSSDR_1 = new Float_t[16];
Float_t *parSSDR_2 = new Float_t[16];

Bool_t is_validLeft,is_validRight;

// Output data

Int_t trigger;

Float_t SSD20_L,tSSD20_L;
Int_t nSSD20_L;

Float_t DSDX_L,tDSDX_L;
Int_t nDSDX_L;

Float_t DSDY_L,tDSDY_L;
Int_t nDSDY_L;

Float_t SSD_L,tSSD_L;
Int_t nSSD_L;


Float_t SSD20_R,tSSD20_R;
Int_t nSSD20_R;

Float_t SSDY_R,tSSDY_R;
Int_t nSSDY_R;

Float_t SSD_R,tSSD_R;
Int_t nSSD_R;


//-----------------------------------------------------------
void convert(Int_t numer)
{
	cout << "macro " << numer << endl;
	UInt_t nEvents = 0;
	TString inSetupConfigFilename="../usr/setup2_exp201811.xml";
	TString fInput;
	fInput.Form("/media/user/work/data/exp201810/data/root/he8_07_000%d.root",numer);

	TString fOutput;
	fOutput.Form("/media/user/work/data/exp201810/workdir/analysed/out%d.root",numer);

	is_validLeft = kFALSE;
	is_validRight = kFALSE;	
	// Construct SetupConfiguration, which includes the input of the XML file
	SetupConfiguration* fSetupConfiguration = new SetupConfiguration(inSetupConfigFilename);
	
	readPars(par20L_1,par20L_2,(TString)"SSD20_L");
	readPars(parSDSXL_1,parSDSXL_2,(TString)"DSDX_L");
	readPars(parSDSYL_1,parSDSYL_2,(TString)"DSDY_L");
	readPars(parSSDL_1,parSSDL_2,(TString)"SSD_L");	

	readPars(par20R_1,par20R_2,(TString)"SSD20_R");
	readPars(parSDSYR_1,parSDSYR_2,(TString)"SSDY_R");
	readPars(parSSDR_1,parSSDR_2,(TString)"SSD_R");

	TChain *chInput = new TChain("stepRepackingxTree");
	chInput->Add(fInput.Data());	

	DetEventFull* theEvent = new DetEventFull("DetEventFull1");
	TGo4EventElement* theEventCopy = theEvent;	
	theEvent->synchronizeWithTree(chInput, &theEventCopy);

	TFile *fOut = new TFile(fOutput.Data(),"RECREATE");
	TTree *tOut = new TTree("tree", "filtred data");
	tOut->Branch("trigger",&trigger,"trigger/I");

	tOut->Branch("SSD20_L",&SSD20_L,"SSD20_L/F");
	tOut->Branch("tSSD20_L",&tSSD20_L,"tSSD20_L/F");
	tOut->Branch("nSSD20_L",&nSSD20_L,"nSSD20_L/I");
	tOut->Branch("DSDX_L",&DSDX_L,"DSDX_L/F");
	tOut->Branch("tDSDX_L",&tDSDX_L,"tDSDX_L/F");
	tOut->Branch("nDSDX_L",&nDSDX_L,"nDSDX_L/I");
	tOut->Branch("DSDY_L",&DSDY_L,"DSDY_L/F");
	tOut->Branch("tDSDY_L",&tDSDY_L,"tDSDY_L/F");
	tOut->Branch("nDSDY_L",&nDSDY_L,"nDSDY_L/I");
	// tOut->Branch("SSD_L",&SSD_L,"SSD_L/F");
	// tOut->Branch("tSSD_L",&tSSD_L,"tSSD_L/F");
	// tOut->Branch("nSSD_L",&nSSD_L,"nSSD_L/I");

	tOut->Branch("SSD20_R",&SSD20_R,"SSD20_R/F");
	tOut->Branch("tSSD20_R",&tSSD20_R,"tSSD20_R/F");
	tOut->Branch("nSSD20_R",&nSSD20_R,"nSSD20_R/I");
	tOut->Branch("SSDY_R",&SSDY_R,"SSDY_R/F");
	tOut->Branch("tSSDY_R",&tSSDY_R,"tSSDY_R/F");
	tOut->Branch("nSSDY_R",&nSSDY_R,"nSSDY_R/I");
	// tOut->Branch("SSD_R",&SSD_R,"SSD_R/F");
	// tOut->Branch("tSSD_R",&tSSD_R,"tSSD_R/F");
	// tOut->Branch("nSSD_R",&nSSD_R,"nSSD_R/I");

	UInt_t nEventsTotal = chInput->GetEntries();
	if (nEvents == 0) { nEvents = nEventsTotal; }

	// Loop over the events
	for (UInt_t iEvent=1; iEvent<nEvents; iEvent++) {
		// cerr << "Event "<< iEvent
		//      << " =================================================================="
		//      << endl;
		if (iEvent%100000==0) cout << "###" << iEvent << "###" << endl;

		chInput->GetEntry(iEvent);

		//TODO implement you actions here
		if(iEvent == 1) printDetFullEvent(theEvent);		
		// theEvent->Print();

		TGo4CompositeEvent* v_RightDet = (TGo4CompositeEvent*)(theEvent->getEventElement("Right_telescope"));
		if (!v_RightDet) { 
			cerr << "Detector Right_telescope was not found." << endl; 
		} 

		is_validRight = getStAmp(v_RightDet,"SSD20_R",&SSD20_R,&nSSD20_R);
		is_validRight = getStTime(v_RightDet,"tSSD20_R",&tSSD20_R,nSSD20_R);
		SSD20_R = SSD20_R*par20R_2[nSSD20_R] + par20R_1[nSSD20_R];
		tSSD20_R = tSSD20_R*0.3;

		is_validRight = getStAmp(v_RightDet,"SSDY_R",&SSDY_R,&nSSDY_R);
		is_validRight = getStTime(v_RightDet,"tSSDY_R",&tSSDY_R,nSSDY_R);
		SSDY_R = SSDY_R*parSDSYR_2[nSSDY_R] + parSDSYR_1[nSSDY_R];
		tSSDY_R = tSSDY_R*0.3;

		TGo4CompositeEvent* v_LeftDet = (TGo4CompositeEvent*)(theEvent->getEventElement("Left_telescope"));
		if (!v_LeftDet) { 
			cerr << "Detector Left_telescope was not found." << endl;
		} 

		is_validLeft = getStAmp(v_LeftDet,"SSD20_L",&SSD20_L,&nSSD20_L);
		is_validLeft = getStTime(v_LeftDet,"tSSD20_L",&tSSD20_L,nSSD20_L);
		if (is_validLeft==kFALSE && is_validRight==kFALSE) continue;
		SSD20_L = SSD20_L*par20L_2[nSSD20_L] + par20L_1[nSSD20_L];
		tSSD20_L = tSSD20_L*0.3;

		is_validLeft = getStAmp(v_LeftDet,"DSDX_L",&DSDX_L,&nDSDX_L);
		is_validLeft = getStTime(v_LeftDet,"tDSDX_L",&tDSDX_L,nDSDX_L);
		if (is_validLeft==kFALSE && is_validRight==kFALSE) continue;
		DSDX_L = DSDX_L*parSDSXL_2[nDSDX_L] + parSDSXL_1[nDSDX_L];
		tDSDX_L = tDSDX_L*0.3;

		is_validLeft = getStAmp(v_LeftDet,"DSDY_L",&DSDY_L,&nDSDY_L);
		is_validLeft = getStTime(v_LeftDet,"tDSDY_L",&tDSDY_L,nDSDY_L);
		if (is_validLeft==kFALSE && is_validRight==kFALSE) continue;
		DSDY_L = DSDY_L*parSDSYL_2[nDSDY_L] + parSDSYL_1[nDSDY_L];
		tDSDY_L = tDSDY_L*0.3;

		TGo4EventElement* v_comElement = theEvent->getEventElement("DetEventCommon",1);
		if(!v_comElement) {
			cout << "Detector DetEventCommon was not found " << endl;
		}
		DetEventCommon* v_commSubEl = (DetEventCommon*)(v_comElement);
		trigger = v_commSubEl->trigger;

		if (is_validLeft==kTRUE || is_validRight==kTRUE) tOut->Fill();
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
Bool_t getStAmp(TGo4CompositeEvent* v_Det,TString st_Name,Float_t *value,Int_t *nCh) {
	TString st_Fullname = v_Det->GetName() + TString("_")+st_Name;

	DetEventStation* station = (DetEventStation*)(v_Det->getEventElement(st_Fullname.Data()));
	if(!station) {
		cout << "Station " << st_Fullname.Data() << " was not found in event " << endl;
		return kFALSE;
	}	
	TClonesArray *array = station->GetDetMessages();
	if (array->GetEntriesFast()==0) return kFALSE;

	// include clusterisation
	if (array->GetEntriesFast()!=1) return kFALSE;
	// 

	Float_t *amp;
	amp = value; 
	Int_t *nChannel;
	nChannel = nCh;

	Int_t mult = 0;
	DetMessage* message;
	for(Int_t i = 0; i<array->GetEntriesFast(); i++) {
		message = (DetMessage*)array->At(i);
		*amp = Float_t(message->GetValue());
		*nChannel = message->GetStChannel();
	}	
	return kTRUE;
}
//-----------------------------------------------------------------------
Bool_t getStTime(TGo4CompositeEvent* v_Det,TString st_Name,Float_t *value,Int_t nCh) {
	TString st_Fullname = v_Det->GetName() + TString("_")+st_Name;

	DetEventStation* station = (DetEventStation*)(v_Det->getEventElement(st_Fullname.Data()));
	if(!station) {
		cout << "Station " << st_Fullname.Data() << " was not found in event " << endl;
		return kFALSE;
	}	
	TClonesArray *array = station->GetDetMessages();
	if (array->GetEntriesFast()==0) return kFALSE;

	Float_t * Time;
	Time = value; 

	DetMessage* message;
	for(Int_t i = 0; i<array->GetEntriesFast(); i++) {
		message = (DetMessage*)array->At(i);
		if (message->GetStChannel() == nCh) {
			*Time = Float_t(message->GetValue());
			return kTRUE;
		}
	}	
	return kFALSE;
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
void readPars(Float_t *par1,Float_t *par2,TString st_Name){

	TString fName = ("/media/user/work/software/fork/useranalysis/calibration/parameters/") + st_Name + ".cal";
	cout << fName << endl;
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
  for(Int_t i=0;i<16;i++) cout << par1[i] << " " << par2[i] << endl; 
}
