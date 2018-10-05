/**

	@class UserHistosTestMonitoring

*/

#ifndef USERHISTOSTESTMONITORING_H
#define USERHISTOSTESTMONITORING_H

#include <TObject.h> // mother class

// STD
#include <map>
#include <fstream>
	
class TH1;
class TH2;

class UserHistosTestMonitoring : public TObject
{
public:
	UserHistosTestMonitoring();
	~UserHistosTestMonitoring();

	void GenerateAutoHistos(void);

	TH1* fTrigger_test;	


	/**
	 * Automatically generated histograms
	 */
	std::map<unsigned int, TH1*> fAutoHistos_test; //!

	ClassDef(UserHistosTestMonitoring, 1);
};

#endif // USERHISTOSTESTMONITORING_H
