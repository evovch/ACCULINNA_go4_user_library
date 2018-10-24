/**

	@class UserHistosBeamMonitoring

*/

#ifndef USERHISTOSBEAMMONITORING_H
#define USERHISTOSBEAMMONITORING_H

#include <TObject.h> // mother class

// STD
#include <map>
#include <fstream>
	
class TH1;
class TH2;
class TGo4ShapedCond;
class TGo4WinCond;
class TGo4PolyCond;

class UserHistosBeamMonitoring : public TObject
{
public:
	UserHistosBeamMonitoring();
	~UserHistosBeamMonitoring();

	void GenerateAutoHistos(void);

	//TH2
	TH2* fY1_X1;
	TH2* fY1_X1_C;
	TH2* fY2_X2;
	TH2* fY2_X2_C;
	TH2* fTarget;

	TH2* fdEToF;

  // Conditions
  TGo4ShapedCond* fBoxCond;
  TGo4WinCond* fWinCond;
  TGo4PolyCond* fPolyCond;
	/**
	 * Automatically generated histograms
	 */
	// std::map<unsigned int, TH1*> fAutoHistos_Beam; //!

	ClassDef(UserHistosBeamMonitoring, 1);
};

#endif // USERHISTOSBEAMMONITORING_H
