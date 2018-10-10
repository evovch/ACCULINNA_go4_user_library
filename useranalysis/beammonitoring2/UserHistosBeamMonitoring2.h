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

class UserHistosBeamMonitoring2 : public TObject
{
public:
	UserHistosBeamMonitoring2();
	~UserHistosBeamMonitoring2();

	void GenerateAutoHistos(void);

	TH1* fTrigger;

	/**
	 * Automatically generated histograms
	 */
	std::map<unsigned int, TH1*> fAutoHistos_Beam; //!

	ClassDef(UserHistosBeamMonitoring2, 1);
};

#endif // USERHISTOSBEAMMONITORING2_H
