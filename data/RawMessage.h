/**

	@class RawMessage

*/

#ifndef RAWMESSAGE_H
#define RAWMESSAGE_H

#include <TObject.h> // mother class

class RawMessage : public TObject
{
public:
	/**
	 * Default constructor is required for correct ROOT IO.
	 */
	RawMessage();
	RawMessage(const RawMessage &obj);
	virtual ~RawMessage();

	/**
	 * Short dump
	 */
	void Dump(bool p_printEndl = true) const;

	/**
	 * Extended dump
	 */
	void ExtDump(bool p_printEndl = true) const;

public:
	/**
	 * Raw 32-bit word as it comes.
	 * Note that headers are footers are saved and raw messages.
	 * Also note that a raw word itself does not represent full information
	 * because some values which are common for a group of raw messages
	 * should be taken from the [sub][sub]event headers
	 */
	Int_t fRawWord;

	/**
	 * From the event header
	 */
	Short_t fEventType;
	Short_t fEventSubtype;
	Short_t fEventDummy;
	Short_t fEventTrigger;
	Int_t   fEventCount;

	/**
	 * From the subevent header
	 */
	Int_t   fSubeventDlen;
	Short_t fSubeventType;
	Char_t  fSubeventSubcrate;
	Char_t  fSubeventControl;
	Int_t   fSubeventFullID;
	Short_t fSubeventProcID;

	/**
	 * From the subsubevent header
	 * See Support.h header
	 * {OTHER=0, MESYTEC=1, CAEN=2, AFFEAFFE=3}
	 */
	Char_t fSubsubeventVendor;

	/**
	 * From the subsubevent header
	 * CAEN only
	 * Geo is actually taken from each CAEN word, while module - only from the MESYTEC header
	 * Geo is written by the DAQ system into all types of words of the CAEN block
	 * (the header, data word, footer, etc.)
	 * Module is written only in the header of the MESYTEC block
	 */
	Int_t fSubsubeventGeo;

	/**
	 * From the subsubevent header
	 * MESYTEC only
	 * Geo is actually taken from each CAEN word, while module - only from the MESYTEC header
	 * Geo is written by the DAQ system into all types of words of the CAEN block
	 * (the header, data word, footer, etc.)
	 * Module is written only in the header of the MESYTEC block
	 */
	Int_t fSubsubeventModule;

	/**
	 * From the message
	 */
	Int_t fChannel;
	Int_t fValueQA;
	Int_t fValueT;

	/**
	 * From the message.
	 * Starting from 0.
	 * Position of the data word within the subsubevent.
	 * Used by the CAEN scalers.
	 */
	Short_t fMessageIndex;

	/**
	 * From the subsubevent footer
	 * Both CAEN and MESYTEC write some counter in the footer
	 * though the formats are different
	 */
	Int_t fSubsubeventFooterCounter;

	ClassDef(RawMessage, 1);
};

#endif // RAWMESSAGE_H
