/**

	@class DetMessage

*/

#ifndef DETMESSAGE_H
#define DETMESSAGE_H

#include <TObject.h> // mother class

class DetMessage : public TObject
{
public:
	DetMessage();
	virtual ~DetMessage();

	Int_t fStation;
	Int_t fStChannel;
	Int_t fValue;

	ClassDef(DetMessage, 1);
};

#endif // DETMESSAGE_H
