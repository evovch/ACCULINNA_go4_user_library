/**

	@class UserHistosMonitoring

*/

#ifndef USERHISTOSREPACKING_H
#define USERHISTOSREPACKING_H

#include <TObject.h> // mother class

class TH1;
class TH2;

class TH1D;

class UserHistosRepacking : public TObject
{
public:
	UserHistosRepacking();
	~UserHistosRepacking();

public:

	/*TH1* d1F3[4];
	TH1* d1tF3[4];
	TH1* d1F5[4];
	TH1* d1tF5[4];
	TH1* d1F6[4];
	TH1* d1tF6[4];
	TH1* d1tMWPC[4];*/

	/*TH1* histoMWPCnx1;
	TH1* histoMWPCny1;
	TH1* histoMWPCnx2;
	TH1* histoMWPCny2;
	TH1* histoMWPCx1;
	TH1* histoMWPCy1;
	TH1* histoMWPCx2;
	TH1* histoMWPCy2;
	TH1* histoMWPCrx1;
	TH1* histoMWPCry1;
	TH1* histoMWPCrx2;
	TH1* histoMWPCry2;*/

	ClassDef(UserHistosRepacking, 1);
};

#endif // USERHISTOSREPACKING_H
