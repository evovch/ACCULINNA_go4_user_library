/**

	@class UserHistosBeamDetMonitoring

*/

#ifndef USERHISTOSBEAMDETMONITORING_H
#define USERHISTOSBEAMDETMONITORING_H

#include <TObject.h> // mother class

class TH1;

class UserHistosBeamDetMonitoring : public TObject
{
public:
	UserHistosBeamDetMonitoring();
	virtual ~UserHistosBeamDetMonitoring();

public:

	TH1* fhMWPC[4];
	TH1* fhMultW[4];

	ClassDef(UserHistosBeamDetMonitoring, 1);
};

#endif // USERHISTOSBEAMDETMONITORING_H
