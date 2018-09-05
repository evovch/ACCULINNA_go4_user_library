/**

	@class DetMessage

*/

#ifndef DETMESSAGE_H
#define DETMESSAGE_H

#include <TObject.h> // mother class

class DetMessage : public TObject
{
public:
	/**
	 * Default constructor is required for correct ROOT IO.
	 */
	DetMessage();
	DetMessage(Short_t p_det, Short_t p_stat, Int_t p_stch, Int_t p_val);
	virtual ~DetMessage();

	virtual void Clear(Option_t* option = "");

	virtual void Print(Option_t* option = "") const;

public:
	/**
	 * Accessor
	 */
	Short_t GetDetector(void) const { return fDetector; }
	
	/**
	 * Accessor
	 */
	Short_t GetStation(void) const { return fStation; }
	
	/**
	 * Accessor
	 */
	Int_t GetStChannel(void) const { return fStChannel; }
	
	/**
	 * Accessor
	 */
	Int_t GetValue(void) const { return fValue; }

private:
	Short_t fDetector;
	Short_t fStation;
	Int_t fStChannel;
	Int_t fValue;

	ClassDef(DetMessage, 1);
};

#endif // DETMESSAGE_H
