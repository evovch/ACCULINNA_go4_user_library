/**

	@class UserHistosBeamMonitoring

*/

#ifndef USERHISTOSBEAMMONITORING2_H
#define USERHISTOSBEAMMONITORING2_H

#include <TObject.h> // mother class

// STD
#include <map>
#include <fstream>
	
class TH1;
class TH2;
class TGo4ShapedCond;

class UserHistosBeamMonitoring2 : public TObject
{
public:
	UserHistosBeamMonitoring2();
	~UserHistosBeamMonitoring2();

//	void GenerateAutoHistos(void);

	//TOF
	TH1* fF3MultA;
	TH1* fF5MultA;
	TH1* fF3MultT;
	TH1* fF5MultT;

	Int_t range_dE;
	Int_t range_TOF;

	TH1* fdE;
	TH1* fTOF;
	TH2* fTOF_ID;

	//MWPC

	TH1* fMWPCwire[4];		//!
	TH1* fMultW[4];		//!
	TH2* fMWPCwireProfile[2];	//!

	TH1* fMWPCcluster[4];		//!
	TH1* fMultC[4];		//!
	TH2* fMWPCclusterProfile[2];	//!

	TH1* fMWPCmmW[4];		//!
	TH2* fMWPCProfileW[3];	//!

	TH1* fMWPCmmC[4];		//!
	TH2* fMWPCProfileC[3];	//!

	TH1* fMWPCwireEff;		//!
	TH1* fMWPCclusterEff;			//!


	//conditions
	TGo4ShapedCond* fBoxCond;		//!

	ClassDef(UserHistosBeamMonitoring2, 1);

	void InitHistograms();
	void InitConditions();
};

#endif // USERHISTOSBEAMMONITORING2_H
