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

  // Auto histos for all detectors
	TH1** detSi;

  // Right telescope
  TH2* dE_E_Right;
  // Right telescope
  TH2* dE_E_Left;
  // Central telescope
  TH2** dE_E_Central;
  TH1* mult_Central; // multiplicity in Central_Si

	ClassDef(UserHistosTestMonitoring, 1);
};

#endif // USERHISTOSTESTMONITORING_H
