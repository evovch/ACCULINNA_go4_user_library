	/**
	 *
	 */
	//void ProcessMESYTECmessage(const RawMessage* p_message);

	/**
	 *
	 */
	//void ProcessMESYTECmessageVME0(const RawMessage* p_message);
	/**
	 *
	 */
	//void ProcessMESYTECmessageVME1(const RawMessage* p_message);

	/**
	 *
	 */
	//void ProcessCAENmessage(const RawMessage* p_message);

	/**
	 *
	 */
	//void ProcessCAENmessageVME0(const RawMessage* p_message);

	/**
	 *
	 */
	//void ProcessCAENmessageVME1(const RawMessage* p_message);

//// From BuildEvent:

/*		support::enu_VENDOR v_messVendor = support::VendorFromChar(v_curMessage->mSubsubeventVendor);
		switch (v_messVendor) {
		case support::enu_VENDOR::MESYTEC:
			this->ProcessMESYTECmessage(v_curMessage);
			break;
		case support::enu_VENDOR::CAEN:
			this->ProcessCAENmessage(v_curMessage);
			break;
		default:
			//// All fine
			break;
		}; // end of switch
*/


/*
void UserProcMonitoring::ProcessMESYTECmessage(const RawMessage* p_message)
{
	switch (p_message->mSubeventProcID) {
	case 100: // VME0
		this->ProcessMESYTECmessageVME0(p_message);
		break;
	case 200: // VME1
		this->ProcessMESYTECmessageVME1(p_message);
		break;
	default:
		#ifdef PRINTDEBUGINFO
		cerr << "unidentified MESYTEC message: "; // << endl;
		p_message->ExtDump();
		#endif
		break;
	};
}

void UserProcMonitoring::ProcessMESYTECmessageVME0(const RawMessage* p_message)
{
	Int_t v_ch = p_message->mChannel;

	switch (p_message->mSubsubeventModule) {
	case 5:  // MESYTEC mADC: SQX left
		if (v_ch<32) {
			#ifdef PRINTDEBUGINFO
			cerr << "SQX_L[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->SQX_L[v_ch] = p_message->mValueQA;
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=32) SQX_L[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 6:  // MESYTEC mADC: SQX right
		if (v_ch<32) {
			#ifdef PRINTDEBUGINFO
			cerr << "SQX_R[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->SQX_R[v_ch] = p_message->mValueQA;
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=32) SQX_R[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 7:  // MESYTEC mADC: SQY left and right
		if (v_ch<16) {
			#ifdef PRINTDEBUGINFO
			cerr << "SQY_L[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->SQY_L[v_ch] = p_message->mValueQA;
		} else if (v_ch<32) {
			#ifdef PRINTDEBUGINFO
			cerr << "SQY_R[" << v_ch-16 << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->SQY_R[v_ch-16] = p_message->mValueQA;
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=32) SQY_(L/R)[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 8:  // MESYTEC mADC: CsI left and right
		if (v_ch<16) {
			#ifdef PRINTDEBUGINFO
			cerr << "CsI_L[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->CsI_L[v_ch] = p_message->mValueQA;
		} else if (v_ch<32) {
			#ifdef PRINTDEBUGINFO
			cerr << "CsI_R[" << v_ch-16 << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->CsI_R[v_ch-16] = p_message->mValueQA;
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=32) CsI_(L/R)[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 9:  // MESYTEC mADC: SQ20
		if (v_ch<16) {
			#ifdef PRINTDEBUGINFO
			cerr << "SQ20[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->SQ20[v_ch] = p_message->mValueQA;
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=16) SQ20[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 10: // MESYTEC mQDC: F3, F5, F6
		if (v_ch<4) {
			#ifdef PRINTDEBUGINFO
			cerr << "F3[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->F3[v_ch] = p_message->mValueQA;
			//if (p_message->mValueQA > 0) { mHistoMan->d1F3[v_ch]->Fill(p_message->mValueQA); } //TODO which value to use?
		} else if (v_ch<8) {
			#ifdef PRINTDEBUGINFO
			cerr << "F5[" << v_ch-4 << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->F5[v_ch-4] = p_message->mValueQA;
			//if (p_message->mValueQA > 0) { mHistoMan->d1F5[v_ch-4]->Fill(p_message->mValueQA); } //TODO which value to use?
		} else if (v_ch<12) {
			#ifdef PRINTDEBUGINFO
			cerr << "F6[" << v_ch-8 << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->F6[v_ch-8] = p_message->mValueQA;
			//if (p_message->mValueQA > 0) { mHistoMan->d1F6[v_ch-8]->Fill(p_message->mValueQA); } //TODO which value to use?
		} else if (v_ch<16) {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=12 && ch<16) [" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=16) [" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 11: // MESYTEC mTDC: tF3, tF5, tF6, tMWPC - time
		//TODO T-bit analysis???
		if (v_ch<4) {
			#ifdef PRINTDEBUGINFO
			cerr << "tF3[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->tF3[v_ch] = p_message->mValueT;
			//if (p_message->mValueT > 0) { mHistoMan->d1tF3[v_ch]->Fill(p_message->mValueT); } //TODO which value to use?
		} else if (v_ch<8) {
			#ifdef PRINTDEBUGINFO
			cerr << "tF5[" << v_ch-4 << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->tF5[v_ch-4] = p_message->mValueT;
			//if (p_message->mValueT > 0) { mHistoMan->d1tF5[v_ch-4]->Fill(p_message->mValueT); } //TODO which value to use?
		} else if (v_ch<12) {
			#ifdef PRINTDEBUGINFO
			cerr << "tF6[" << v_ch-8 << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->tF6[v_ch-8] = p_message->mValueT;
			//if (p_message->mValueT > 0) { mHistoMan->d1tF6[v_ch-8]->Fill(p_message->mValueT); } //TODO which value to use?
		} else if (v_ch<16) {
			#ifdef PRINTDEBUGINFO
			cerr << "tMWPC[" << v_ch-12 << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->tMWPC[v_ch-12] = p_message->mValueT;
			//if (p_message->mValueT > 0) { mHistoMan->d1tMWPC[v_ch-12]->Fill(p_message->mValueT); } //TODO which value to use?
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=16) [" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 12: // MESYTEC mTDC: tSQ20 - time
		//TODO T-bit analysis???
		if (v_ch<16) {
			#ifdef PRINTDEBUGINFO
			cerr << "tSQ20[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->tSQ20[v_ch] = p_message->mValueT;
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=16) tSQ20[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	default:
		#ifdef PRINTDEBUGINFO
		cerr << "unidentified MESYTEC message from VME0: "; // << endl;
		p_message->ExtDump();
		#endif
		break;
	};
}

void UserProcMonitoring::ProcessMESYTECmessageVME1(const RawMessage* p_message)
{
	Int_t v_ch = p_message->mChannel;

	switch (p_message->mSubsubeventModule) {
	case 2:  // MESYTEC ???? //TODO
		#ifdef PRINTDEBUGINFO
		cerr << "neutTDC[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
		#endif
		//TODO implement channel number check
		// should be 32
		break;
	default:
		#ifdef PRINTDEBUGINFO
		cerr << "unidentified MESYTEC message from VME1: "; // << endl;
		p_message->ExtDump();
		#endif
		break;
	};
}

void UserProcMonitoring::ProcessCAENmessage(const RawMessage* p_message)
{
	switch (p_message->mSubeventProcID) {
	case 100: // VME0
		this->ProcessCAENmessageVME0(p_message);
		break;
	case 200: // VME1
		this->ProcessCAENmessageVME1(p_message);
		break;
	default:
		#ifdef PRINTDEBUGINFO
		cerr << "unidentified CAEN message: "; // << endl;
		p_message->ExtDump();
		#endif
		break;
	};
}

void UserProcMonitoring::ProcessCAENmessageVME0(const RawMessage* p_message)
{
	Int_t v_ch = p_message->mChannel;

	switch (p_message->mSubsubeventGeo) {
	case 0:  // CAEN V560 scaler
		#ifdef PRINTDEBUGINFO
		cerr << "CAEN V560 scaler: "
		     << support::GetHexRepresentation(sizeof(Int_t), &(p_message->mRawWord)) << "  "
		     << support::GetBinaryRepresentation(sizeof(Int_t), &(p_message->mRawWord))
		     << endl;
		#endif
		break;
	case 1:  // CAEN TDC: tSQX left - time
		if (v_ch<32) {
			#ifdef PRINTDEBUGINFO
			cerr << "tSQX_L[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->tSQX_L[v_ch] = p_message->mValueT;
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=32) tSQX_L[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 2:  // CAEN TDC: tSQX right - time
		if (v_ch<32) {
			#ifdef PRINTDEBUGINFO
			cerr << "tSQX_R[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->tSQX_R[v_ch] = p_message->mValueT;
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=32) tSQX_R[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 3:  // CAEN TDC: tSQY left and right - time
		if (v_ch<16) {
			#ifdef PRINTDEBUGINFO
			cerr << "tSQY_L[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->tSQY_L[v_ch] = p_message->mValueT;
		} else if (v_ch<32) {
			#ifdef PRINTDEBUGINFO
			cerr << "tSQY_R[" << v_ch-16 << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->tSQY_R[v_ch-16] = p_message->mValueT;
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=32) tSQY_(L/R)[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 4:  // CAEN TDC: tCsI left and right - time
		if (v_ch<16) {
			#ifdef PRINTDEBUGINFO
			cerr << "tCsI_L[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->tCsI_L[v_ch] = p_message->mValueT;
		} else if (v_ch<32) {
			#ifdef PRINTDEBUGINFO
			cerr << "tCsI_R[" << v_ch-16 << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
			mCurrentOutputEvent->tCsI_R[v_ch-16] = p_message->mValueT;
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=32) tCsI_(L/R)[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 30:  // RIO machine time, not really CAEN
		#ifdef PRINTDEBUGINFO
		cerr << "RIO machine time: "
		     << support::GetHexRepresentation(sizeof(Int_t), &(p_message->mRawWord)) << "  "
		     << support::GetBinaryRepresentation(sizeof(Int_t), &(p_message->mRawWord))
		     << endl;
		#endif
		break;
	default:
		#ifdef PRINTDEBUGINFO
		cerr << "unidentified CAEN message from VME0: "; // << endl;
		p_message->ExtDump();
		#endif
		break;
	};
}

void UserProcMonitoring::ProcessCAENmessageVME1(const RawMessage* p_message)
{
	Int_t v_ch = p_message->mChannel;

	switch (p_message->mSubsubeventGeo) {
	case 0:  // CAEN ???? //TODO
		if (v_ch<32) {
			if (v_ch%2 == 0) {
				#ifdef PRINTDEBUGINFO
				cerr << "neutAmp[" << (v_ch/2) << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
				#endif
				mCurrentOutputEvent->neutAmp[v_ch/2] = p_message->mValueQA; //TODO which value to use?
			} else {
				#ifdef PRINTDEBUGINFO
				cerr << "neutTAC[" << (v_ch/2) << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
				#endif
				mCurrentOutputEvent->neutTAC[v_ch/2] = p_message->mValueT; //TODO which value to use?
			}
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=32) neut(Amp/TAC)[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	case 1:  // CAEN ???? //TODO
		if (v_ch<32) {
			if (v_ch%2 == 0) {
				#ifdef PRINTDEBUGINFO
				cerr << "neutAmp[" << (v_ch/2)+16 << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
				#endif
				mCurrentOutputEvent->neutAmp[(v_ch/2)+16] = p_message->mValueQA; //TODO which value to use?
			} else {
				#ifdef PRINTDEBUGINFO
				cerr << "neutTAC[" << (v_ch/2)+16 << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
				#endif
				mCurrentOutputEvent->neutTAC[(v_ch/2)+16] = p_message->mValueT; //TODO which value to use?
			}
		} else {
			//// Unexpected channel value
			#ifdef PRINTDEBUGINFO
			cerr << "Unexpected channel value (ch>=32) neut(Amp/TAC)[" << v_ch << "]=" << p_message->mValueQA << "(" << p_message->mValueT << ")" << endl;
			#endif
		}
		break;
	default:
		#ifdef PRINTDEBUGINFO
		cerr << "unidentified CAEN message from VME1: "; // << endl;
		p_message->ExtDump();
		#endif
		break;
	};
}
*/
