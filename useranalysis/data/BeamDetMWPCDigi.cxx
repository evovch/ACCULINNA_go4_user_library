#include "BeamDetMWPCDigi.h"

/*#include <iostream>
using std::cerr;
using std::endl;*/

BeamDetMWPCDigi::BeamDetMWPCDigi() :
	TObject(),
	fID(0),
	fMWPCNb(0),
	fPlaneNb(0),
	fWireNb(0),
	fTime(0.),
	fEdep(0.f)
{
}

BeamDetMWPCDigi::BeamDetMWPCDigi(Int_t id,
                                 Int_t mwpcNb,
                                 Int_t planeNb,
                                 Int_t wireNb,
                                 Double_t time,
                                 Float_t Edep) :
	TObject(),
	fID(id),
	fMWPCNb(mwpcNb),
	fPlaneNb(planeNb),
	fWireNb(wireNb),
	fTime(time),
	fEdep(Edep)
{
}

BeamDetMWPCDigi::BeamDetMWPCDigi(const BeamDetMWPCDigi& right) :
	TObject(),
	fID(right.fID),
	fMWPCNb(right.fMWPCNb),
	fPlaneNb(right.fPlaneNb),
	fWireNb(right.fWireNb),
	fTime(right.fTime),
	fEdep(right.fEdep)
{
}

BeamDetMWPCDigi::~BeamDetMWPCDigi()
{
}

void BeamDetMWPCDigi::Print(const Option_t* /*opt*/ /* = 0 */) const
{
}

void BeamDetMWPCDigi::SetData(Int_t id,
                              Int_t mwpcNb,
                              Int_t planeNb,
                              Int_t wireNb,
                              Double_t time,
                              Float_t Edep)
{
	fID = id;
	fMWPCNb = mwpcNb;
	fPlaneNb = planeNb;
	fWireNb = wireNb;
	fTime = time;
	fEdep = Edep;
}

ClassImp(BeamDetMWPCDigi)
