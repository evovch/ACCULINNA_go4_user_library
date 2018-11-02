/**

	@class UserParamBeamDetMonitoring

*/

#ifndef USERPARAMBEAMDETMONITORING_H
#define USERPARAMBEAMDETMONITORING_H

#include <TGo4Parameter.h> // mother class

// ROOT
#include <TString.h>

class UserParamBeamDetMonitoring : public TGo4Parameter
{
public:
	UserParamBeamDetMonitoring(const char* name = "ParBeamDetMon");
	virtual ~UserParamBeamDetMonitoring(void);

	virtual Int_t PrintParameter(Text_t* buffer=0, Int_t buflen=0); // *MENU*

	const TString GetBeamDetName(void) const { return fBeamDetName; }
	const TString GetStationMWPCx1name(void) const { return fBeamDetName + "_" + fMWPCx1Name; }
	const TString GetStationMWPCy1name(void) const { return fBeamDetName + "_" + fMWPCy1Name; }
	const TString GetStationMWPCx2name(void) const { return fBeamDetName + "_" + fMWPCx2Name; }
	const TString GetStationMWPCy2name(void) const { return fBeamDetName + "_" + fMWPCy2Name; }

private:

	TString fBeamDetName; // Name of the beam detector required to identify it among all the detectors

	TString fMWPCx1Name; // Name of the X1 MWPC station of the beam detector required to identify it among all the stations
	TString fMWPCy1Name; // Name of the Y1 MWPC station of the beam detector required to identify it among all the stations
	TString fMWPCx2Name; // Name of the X2 MWPC station of the beam detector required to identify it among all the stations
	TString fMWPCy2Name; // Name of the Y2 MWPC station of the beam detector required to identify it among all the stations

	ClassDef(UserParamBeamDetMonitoring, 1);
};

#endif // USERPARAMBEAMDETMONITORING_H
