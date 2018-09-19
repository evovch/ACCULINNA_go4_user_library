/**

	@class UserHistosAdvMonitoring

*/

#ifndef USERHISTOSADVMONITORING_H
#define USERHISTOSADVMONITORING_H

#include <TObject.h> // mother class

class TH1;
class TH2;

class UserHistosAdvMonitoring : public TObject
{
public:
	UserHistosAdvMonitoring();
	~UserHistosAdvMonitoring();

public:

	ClassDef(UserHistosAdvMonitoring, 1);
};

#endif // USERHISTOSADVMONITORING_H
