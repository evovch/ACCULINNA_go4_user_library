/*void UserProc::ProcessSubeventRaw_MESYTEC(Int_t* p_startAddress, Int_t p_cursor)
{
	int v_type = (p_startAddress[p_cursor] >> 30) & 0x3;
	cerr << "VME0 MESYTEC type=" << v_type << endl;

	int v_module_id;

	switch (v_type) {
	case 1: // MESYTEC header
		v_module_id = (p_startAddress[p_cursor] >> 16) & 0xff;
		cerr << "header module_id=" << v_module_id << endl;
		break;
	case 3: // MESYTEC footer
		cerr << "footer" << endl;
		fprintf(stderr, "%02x", p_startAddress[p_cursor]);
		break;
	case 0: // MESYTEC data
	default:
		cerr << "unknown MESYTEC type" << endl;
		fprintf(stderr, "%02x", p_startAddress[p_cursor]);
		break;
	}
}*/

/*void UserProc::ProcessSubeventRaw_CAEN(Int_t* p_startAddress, Int_t p_cursor)
{
	Int_t tmp = p_startAddress[p_cursor];
	////Int_t v_curWord = ((tmp << 16) & 0xffff0000) | ((tmp >> 16) & 0x0000ffff);
	Int_t v_curWord = tmp;

	int v_type = (v_curWord >> 24) & 0x7;
	int v_geo = v_curWord >> 27; // TODO bitmask!
	int v_adc;
	int v_channel;

	////cerr << p_cursor << ": CAEN type=" << v_type << " geo=" << v_geo << endl;

	fprintf(stderr, "%08x", v_curWord);

	switch (v_type) {
	case 2: // CAEN header
		cerr << "\t" << p_cursor << ":\t" << "CAEN header" << "\t\t\ttype=" << v_type << " geo=" << v_geo << endl;
		mInsidePackage = true;
		break;
	case 4: // CAEN footer
		cerr << "\t" << p_cursor << ":\t" << "CAEN footer" << "\t\t\ttype=" << v_type << " geo=" << v_geo << endl;
		mInsidePackage = false;
		break;
	case 6: // CAEN no valid data
		cerr << "\t" << p_cursor << ":\t" << "CAEN no valid data" << "\ttype=" << v_type << " geo=" << v_geo << endl;
		break;
	case 0: // CAEN data

		v_channel = (v_curWord >> 16) & 0x1f;
		v_adc = v_curWord & 0xfff;

		cerr << "\t" << p_cursor << ":\t" << "CAEN data" << "\t\t\ttype=" << v_type << " geo=" << v_geo
		     << " ch=" << v_channel << " adc=" << v_adc << endl;

		if (!mInsidePackage) {
			cerr << "[ERROR ] " << "Data word found not between the header and the footer." << endl;
		}
		break;
	default:
		cerr << "\t" << p_cursor << ":\t" << "CAEN unknown" << "\t\t\ttype=" << v_type << " geo=" << v_geo << endl;
		break;
	}
}*/
