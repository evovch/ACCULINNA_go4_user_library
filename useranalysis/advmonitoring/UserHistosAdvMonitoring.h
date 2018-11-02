/**

	@class UserHistosAdvMonitoring

*/

#ifndef USERHISTOSADVMONITORING_H
#define USERHISTOSADVMONITORING_H

#include <TObject.h> // mother class

// STD
#include <map>

class TH1;
class TH2;

class UserHistosAdvMonitoring : public TObject
{
public:
	UserHistosAdvMonitoring();
	~UserHistosAdvMonitoring();

	void GenerateAutoHistos(void);

//	TH1* fTrigger;	
public:

	TH1* mhHisto1;
	TH2* mhHisto2;

	/**
	 * Automatically generated histograms
	 */
	std::map<unsigned int, TH1*> fAutoHistos; //!

	ClassDef(UserHistosAdvMonitoring, 1);
};

#endif // USERHISTOSADVMONITORING_H
