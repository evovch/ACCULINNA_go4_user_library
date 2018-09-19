/**

	@class UserHistosRawMonitoring

*/

#ifndef USERHISTOSRAWMONITORING_H
#define USERHISTOSRAWMONITORING_H

#include <TObject.h> // mother class

class TH1;
class TH2;

class UserHistosRawMonitoring : public TObject
{
public:
	UserHistosRawMonitoring();
	~UserHistosRawMonitoring();

public:
	
	TH1* mhEvType;
	TH1* mhEvSubType;
	TH1* mhEvDummy;
	TH1* mhEvTrigger;
	TH1* mhEvCount;
	TH1* mhSubEvDlen;
	TH1* mhSubEvType;
	TH1* mhSubEvSubcrate;
	TH1* mhSubEvControl;
	TH1* mhSubEvFullID;
	TH1* mhSubEvProcID;
	TH1* mhSubsubEvVendor;
	TH1* mhSubsubEvGeo;
	TH1* mhSubsubEvModule;
	TH1* mhChannel;
	TH1* mhValueQA;
	TH1* mhValueT;
	TH1* mhMessageIndex;
	TH1* mhSubsubEvFooterCntr;

	ClassDef(UserHistosRawMonitoring, 1);
};

#endif // USERHISTOSRAWMONITORING_H
