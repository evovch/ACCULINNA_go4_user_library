/**

	@class UserHistosAdvMonitoring

*/

#ifndef USERHISTOSADVMONITORING_H
#define USERHISTOSADVMONITORING_H

#include <TObject.h> // mother class
#include <TString.h>

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

	/**
	 * Automatically generated histograms
	 */
	std::map<unsigned int, TH1*> fAutoHistos; //!

private:
	TString fst_MWPC; // name of the MWPC station

	ClassDef(UserHistosAdvMonitoring, 1);
};

#endif // USERHISTOSADVMONITORING_H
