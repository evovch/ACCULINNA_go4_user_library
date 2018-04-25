#include "UserProcStep2.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// ROOT
#include <TClonesArray.h>

// Project
#include "UserEventStep2.h"
#include "unpacking/UserEventUnpacking.h"
#include "data/BeamDetMWPCDigi.h"

/**
  Uncomment this if you want to see all the debug information.
  This allows you to analyze the raw bytes and bits by your eyes.
  This option produces A LOT OF DATA - run your analysis with a
  small number of events (~10-100)
*/
//#define PRINTDEBUGINFO

UserProcStep2::UserProcStep2(const char* name) :
	TGo4EventProcessor(name),
	fEventCounter(0)
{
}

UserProcStep2::~UserProcStep2()
{
}

Bool_t UserProcStep2::BuildEvent(TGo4EventElement* p_dest)
{
	Bool_t v_isValid = kFALSE;
	UserEventStep2* v_outputEvent = (UserEventStep2*)p_dest;

	UserEventUnpacking* v_input = (UserEventUnpacking*)GetInputEvent();
	if (v_input == NULL)
	{
		cerr << "UserProcStep2::BuildEvent(): no input event!" << endl;
		v_outputEvent->SetValid(v_isValid);
		return v_isValid;
	}
	v_isValid = kTRUE;

	#ifdef PRINTDEBUGINFO
	cerr << "[DEBUG ] " << "UserProcStep2: Event " << fEventCounter
	     << " ==========================================================================================================="
	     << endl;
	#endif

	fCurrentOutputEvent = v_outputEvent;

	// Clear the output event!!!
	//TODO check that this is not done by the framework
	// Seems that indeed this is done by the framework
	//mCurrentOutputEvent->Clear();

	//TODO do the processing here

	Short_t* v_inputCAMAC = v_input->fCAMAC;

	// Transform pairs of shorts into normal ints
	Int_t v_line[4];
	v_line[0] = ((v_inputCAMAC[1] << 16) & 0xffff0000) |
	            ((v_inputCAMAC[0] << 0)  & 0x0000ffff);
	v_line[1] = ((v_inputCAMAC[3] << 16) & 0xffff0000) |
	            ((v_inputCAMAC[2] << 0)  & 0x0000ffff);
	v_line[2] = ((v_inputCAMAC[5] << 16) & 0xffff0000) |
	            ((v_inputCAMAC[4] << 0)  & 0x0000ffff);
	v_line[3] = ((v_inputCAMAC[7] << 16) & 0xffff0000) |
	            ((v_inputCAMAC[6] << 0)  & 0x0000ffff);

	// Just print - shorts
	#ifdef PRINTDEBUGINFO
	fprintf(stderr, "--------------------------------\n");
	PrintBits(sizeof(Short_t), &v_inputCAMAC[1]);
	PrintBits(sizeof(Short_t), &v_inputCAMAC[0]);
	fprintf(stderr, "\n");
	PrintBits(sizeof(Short_t), &v_inputCAMAC[3]);
	PrintBits(sizeof(Short_t), &v_inputCAMAC[2]);
	fprintf(stderr, "\n");
	PrintBits(sizeof(Short_t), &v_inputCAMAC[5]);
	PrintBits(sizeof(Short_t), &v_inputCAMAC[4]);
	fprintf(stderr, "\n");
	PrintBits(sizeof(Short_t), &v_inputCAMAC[7]);
	PrintBits(sizeof(Short_t), &v_inputCAMAC[6]);
	fprintf(stderr, "\n");
	fprintf(stderr, "--------------------------------\n");
	#endif

	// Just print - ints
	#ifdef PRINTDEBUGINFO
	fprintf(stderr, "--------------------------------\n");
	PrintBits(sizeof(Int_t), &v_line[0]);	fprintf(stderr, "\n");
	PrintBits(sizeof(Int_t), &v_line[1]);	fprintf(stderr, "\n");
	PrintBits(sizeof(Int_t), &v_line[2]);	fprintf(stderr, "\n");
	PrintBits(sizeof(Int_t), &v_line[3]);	fprintf(stderr, "\n");
	fprintf(stderr, "--------------------------------\n");
	#endif

	// Just print - bits
	#ifdef PRINTDEBUGINFO
	fprintf(stderr, "--------------------------------\n");
	for (unsigned int i=0; i<4; i++) {
		for (unsigned char v_wire=0; v_wire<32; v_wire++) {
			unsigned char v_bitValue = (v_line[i] >> (32-v_wire-1)) & 0x1;
			fprintf(stderr, "%d", v_bitValue);
		}
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "--------------------------------\n");
	#endif

	unsigned int v_globalId = 0;

	// Analyse - extract necessary numbers and fill the output structures
	for (unsigned int i=0; i<4; i++) {
		unsigned char v_id = 0;
		unsigned char v_plane = (i%2)+1;
		unsigned char v_mwpc = (i/2)+1;
		for (unsigned char v_wire=0; v_wire<32; v_wire++) {
			unsigned char v_bitValue = (v_line[i] >> (32-v_wire)) & 0x1;
			if (v_bitValue == 1) {
				#ifdef PRINTDEBUGINFO
				fprintf(stderr, "planeNb=%d mwpcNb=%d ID=%d wire=%d\n",
				    v_plane, v_mwpc, v_id, v_wire);
				#endif
				// HERE WE HAVE IT

				//BeamDetMWPCDigi* v_curDigi = new BeamDetMWPCDigi(v_id, v_mwpc, v_plane, v_wire, 0., 0.f);

				//TClonesArray* v_outArray = mCurrentOutputEvent->mMWPCdigi;

				BeamDetMWPCDigi* v_constructedObject =
				    (BeamDetMWPCDigi*)fCurrentOutputEvent->fMWPCdigi->ConstructedAt(v_globalId);
				v_constructedObject->SetData(v_id, v_mwpc, v_plane, v_wire, 0., 0.f);

				//new(&v_outArray[v_globalId]) BeamDetMWPCDigi(v_id, v_mwpc, v_plane, v_wire, 0., 0.f);


				v_id++;
				v_globalId++;
			}
		}
	}

	// --------------------------

	v_outputEvent->SetValid(v_isValid);

	fEventCounter++;

	return v_isValid;
}

// Assumes little endian //TODO not sure
void UserProcStep2::PrintBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1; i>=0; i--)
    {
        for (j=7; j>=0; j--)
        {
            byte = (b[i] >> j) & 1;
            fprintf(stderr, "%u", byte);
        }
    }
    //puts(""); // ?
}

ClassImp(UserProcStep2)
