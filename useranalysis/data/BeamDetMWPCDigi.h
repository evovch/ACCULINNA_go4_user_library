#ifndef BEAMDETMWPCDIGI_H
#define BEAMDETMWPCDIGI_H

#include "TObject.h" // mother class

class BeamDetMWPCDigi : public TObject
{
public:
	/**
	 * Default constructor
	 */
	BeamDetMWPCDigi();

	/**
	 * Constructor with arguments
	 */
	BeamDetMWPCDigi(Int_t id,
	                Int_t mwpcNb,
	                Int_t planeNb,
	                Int_t wireNb,
	                Double_t time,
	                Float_t Edep);

	/**
	 * Copy constructor
	 */
	BeamDetMWPCDigi(const BeamDetMWPCDigi&);

	/**
	 * Destructor
	 */
	virtual ~BeamDetMWPCDigi();

	/**
	 * Output to screen
	 */
	virtual void Print(const Option_t* opt = 0) const;

	/**
	 * Modifiers
	 */
	void SetData(Int_t id,
	             Int_t mwpcNb,
	             Int_t planeNb,
	             Int_t wireNb,
	             Double_t time,
	             Float_t Edep);

	/**
	 * Accessors
	 */
	Int_t    GetID ()     const { return fID; }
	Int_t    GetMWPCNb()  const { return fMWPCNb; }
	Int_t    GetPlaneNb() const { return fPlaneNb; }
	Int_t    GetWireNb()  const { return fWireNb; }
	Double_t GetTime()    const { return fTime; }
	Float_t  GetEdep()    const { return fEdep; }

private:
	Int_t    fID;
	Int_t    fMWPCNb;
	Int_t    fPlaneNb;
	Int_t    fWireNb;
	Double_t fTime;
	Float_t  fEdep;

	ClassDef(BeamDetMWPCDigi, 1);
};

#endif // BEAMDETMWPCDIGI_H
