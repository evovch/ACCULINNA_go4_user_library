#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// Manager
#pragma link C++ class UserAnalysis+;
#pragma link C++ class UserParameter+;
#pragma link C++ class MeshDummyEvent+;
#pragma link C++ class MeshProviderProc+;

// Unpacking step
#pragma link C++ class UserEventUnpacking+;
#pragma link C++ class UserProcUnpacking+;
#pragma link C++ class UserHistosUnpacking+;

// Repacking step
#pragma link C++ class DetEventFull+;
#pragma link C++ class UserEventRepacking+;
#pragma link C++ class UserProcRepacking+;
#pragma link C++ class UserHistosRepacking+;

// Learn step
#pragma link C++ class UserEventLearn+;
#pragma link C++ class UserProcLearn+;

// DigiBuilding step
#pragma link C++ class UserEventDigiBuilding+;
#pragma link C++ class UserProcDigiBuilding+;

#endif // __CINT__
