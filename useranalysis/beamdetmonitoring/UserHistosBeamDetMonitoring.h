/**

	@class UserHistosBeamDetMonitoring

*/

#ifndef USERHISTOSBEAMDETMONITORING_H
#define USERHISTOSBEAMDETMONITORING_H

#include <TObject.h> // mother class

// STD
#include <map>

//class TH1;
//class TH2;

class UserHistosBeamDetMonitoring : public TObject
{
public:
	UserHistosBeamDetMonitoring();
	~UserHistosBeamDetMonitoring();

public:

	ClassDef(UserHistosBeamDetMonitoring, 1);
};

#endif // USERHISTOSBEAMDETMONITORING_H
