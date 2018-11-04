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
	UserHistosTestMonitoring(std::pair <TString,Int_t>* pairs,Int_t npairs);
	~UserHistosTestMonitoring();

	TH1** detSi;
  TH2* dE_E_Right;
  TH2* dE_E_Left;

	ClassDef(UserHistosTestMonitoring, 1);
};

#endif // USERHISTOSTESTMONITORING_H
