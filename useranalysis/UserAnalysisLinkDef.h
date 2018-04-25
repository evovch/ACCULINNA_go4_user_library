#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// Manager
#pragma link C++ class UserAnalysis+;
#pragma link C++ class MeshDummyEvent+;
#pragma link C++ class MeshProviderProc+;
#pragma link C++ class UserParameter+;

// Data
#pragma link C++ class BeamDetMWPCDigi+;
#pragma link C++ class RawMessage+;

// Unpacking step
#pragma link C++ class UserEventUnpacking+;
#pragma link C++ class UserProcUnpacking+;
#pragma link C++ class UserHistosUnpacking+;

// Monitoring step
#pragma link C++ class UserEventMonitoring+;

#pragma link C++ class UserEventNew+;
#pragma link C++ class UserEventLeftTele+;
#pragma link C++ class UserEventRightTele+;
//#pragma link C++ class UserEventTele3+;
//#pragma link C++ class UserEventTele4+;
#pragma link C++ class UserEventAnnularTele+;
#pragma link C++ class UserEventNeutronDet+;
#pragma link C++ class UserEventBeamDetTof+;
#pragma link C++ class UserEventBeamDetMwpc+;

#pragma link C++ class UserProcMonitoring+;
#pragma link C++ class UserHistosMonitoring+;

// Learn step
#pragma link C++ class UserEventLearn+;
#pragma link C++ class UserProcLearn+;

// Analysis step
#pragma link C++ class UserEventStep2+;
#pragma link C++ class UserProcStep2+;

#endif // __CINT__
