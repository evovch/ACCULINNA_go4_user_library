#include "OutputRootStreamer.h"

#include <iostream>

#include "TString.h"

#include "TGo4UserStoreParameter.h"

#include "UserParameter.h"
#include "data/DetEventFull.h"

OutputRootStreamer::OutputRootStreamer(const char* name/*= "OutputRootStreamer"*/)
  : TGo4EventProcessor(name)
{}

bool OutputRootStreamer::BuildEvent(TGo4EventElement* output_event) {
  output_event = GetInputEvent("stepRepackedProviderForStreamer"); 
  return true;
}

OutputRootStore::OutputRootStore(TGo4UserStoreParameter* store_parameter)
  : TGo4EventStore(store_parameter->GetName()),
    file_(store_parameter->GetName(), "RECREATE"),
    tree_("accdaq", "Tree for accdaq output")
{
  tree_.Write();
}

Int_t OutputRootStore::Store(TGo4EventElement* event) {
  DetEventFull* full_event = dynamic_cast<DetEventFull*>(event);
  full_event->Print();
  return 0;
}