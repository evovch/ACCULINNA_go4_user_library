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

class UserHistosBeamMonitoring : public TObject
{
public:
	UserHistosBeamMonitoring();
	~UserHistosBeamMonitoring();

	void GenerateAutoHistos(void);

	TH1* fHTrigger_Beam;

	//TH2
	TH2* fY1_X1;
	TH2* fY1_X1_C;
	TH2* fY2_X2;
	TH2* fY2_X2_C;
	TH2* fTarget;

	TH2* fdEToF;

	/**
	 * Automatically generated histograms
	 */
	// std::map<unsigned int, TH1*> fAutoHistos_Beam; //!

	ClassDef(UserHistosBeamMonitoring, 1);
};

#endif // USERHISTOSBEAMMONITORING_H
