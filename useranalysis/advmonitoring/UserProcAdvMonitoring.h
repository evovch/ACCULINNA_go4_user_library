/**

	@class UserProcAdvMonitoring

*/

#ifndef USERPROCADVMONITORING_H
#define USERPROCADVMONITORING_H

#include <TGo4EventProcessor.h> // mother class

class TGo4EventElement;

class UserHistosAdvMonitoring;

class UserProcAdvMonitoring : public TGo4EventProcessor
{
public: // methods

	UserProcAdvMonitoring(const char* name = "UserProcAdvMonitoring");
	virtual ~UserProcAdvMonitoring();

	virtual Bool_t BuildEvent(TGo4EventElement* p_dest);

private: // data members
	/**
	 * Counter or processed events
	 */
	unsigned long int fEventCounter;

	/**
	 * Put all your output histograms inside this object.
	 * See UserHistosAdvMonitoring class.
	 */
	UserHistosAdvMonitoring* fHistoMan;

	/**
	 * Summary stream
	 */
	FILE* fFileSummary;

	ClassDef(UserProcAdvMonitoring, 1);
};

#endif // USERPROCADVMONITORING_H
