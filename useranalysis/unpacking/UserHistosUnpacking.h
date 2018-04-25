/**

	@class UserHistosUnpacking

*/

#ifndef USERHISTOSUNPACKING_H
#define USERHISTOSUNPACKING_H

#include <TObject.h> // mother class

class TH1;
class TH2;

class UserHistosUnpacking : public TObject
{
public:
	UserHistosUnpacking();
	~UserHistosUnpacking();

public:

	ClassDef(UserHistosUnpacking, 1);
};

#endif // USERHISTOSUNPACKING_H
