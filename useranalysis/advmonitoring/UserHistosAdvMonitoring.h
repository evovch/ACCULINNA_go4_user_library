/**

	@class UserHistosAdvMonitoring

*/

#ifndef USERHISTOSADVMONITORING_H
#define USERHISTOSADVMONITORING_H

#include <TObject.h> // mother class

// STD
#include <map>
#include <fstream>
	
class TH1;
class TH2;

class UserHistosAdvMonitoring : public TObject
{
public:
	UserHistosAdvMonitoring();
	~UserHistosAdvMonitoring();

	void GenerateAutoHistos(void);

	TH1* fTrigger;	

	//		RAW
	//	CsI
	TH1* fCsI_L[16];
	TH1* fCsI_R[16];
	TH1* ftCsI_L[16];
	TH1* ftCsI_R[16];
	//	Si LEFT
	TH1* fSQX_L[32];
	TH1* fSQY_L[16];
	TH1* ftSQX_L[32];
	TH1* ftSQY_L[16];
	TH1* fSQ20[16];
	TH1* ftSQ20[16];
	//	Si RIGHT
	TH1* fSQX_R[32];
	TH1* fSQY_R[16];
	TH1* ftSQX_R[32];
	TH1* ftSQY_R[16];
	//	ToF + MWPC
	TH1* fF3[4];
	TH1* ftF3[4];
	TH1* fF5[4];
	TH1* ftF5[4];
	TH1* ftMWPC[4];
	//WIRES
	TH1* fNX1;
	TH1* fNY1;
	TH1* fNX2;
	TH1* fNY2;

	//reconstructed
	TH2* fY1_X1;
	TH2* fY1_X1_C;

	//calibrated
	TH1* fCsI_R_C[16];

	/**
	 * Automatically generated histograms
	 */
	std::map<unsigned int, TH1*> fAutoHistos; //!

	ClassDef(UserHistosAdvMonitoring, 1);
};

#endif // USERHISTOSADVMONITORING_H
