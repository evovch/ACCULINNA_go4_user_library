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
#pragma link C++ class RawMessage+;
#pragma link C++ class DetMessage+;
#pragma link C++ class DetEventFull+;
#pragma link C++ class DetEventCommon+;
#pragma link C++ class DetEventDetector+;
#pragma link C++ class DetEventStation+;

// Unpacking step
#pragma link C++ class UserEventUnpacking+;
#pragma link C++ class UserProcUnpacking+;
#pragma link C++ class UserHistosUnpacking+;

// Monitoring step
#pragma link C++ class UserEventMonitoring+;
#pragma link C++ class UserProcMonitoring+;
#pragma link C++ class UserHistosMonitoring+;

// Learn step
#pragma link C++ class UserEventLearn+;
#pragma link C++ class UserProcLearn+;

#endif // __CINT__
