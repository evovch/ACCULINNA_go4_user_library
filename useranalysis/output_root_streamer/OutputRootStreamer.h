#ifndef OUTPUTROOTSTREAMER_H
#define OUTPUTROOTSTREAMER_H

#include "TObject.h"
#include "TFile.h"
#include "TTree.h"

#include "TGo4EventProcessor.h"
#include "TGo4EventStore.h"

class TGo4UserStoreParameter;

class OutputRootStreamer : public TGo4EventProcessor {
 public:
  OutputRootStreamer() = default;
  OutputRootStreamer(const char* name = "OutputRootStreamer");
  virtual ~OutputRootStreamer() = default;
  virtual bool BuildEvent(TGo4EventElement* output_event);
  ClassDef(OutputRootStreamer, 1);
};

class OutputRootStore : public TGo4EventStore {
 public:
  OutputRootStore();
  OutputRootStore(TGo4UserStoreParameter*);
  virtual ~OutputRootStore() = default;
  virtual Int_t Store(TGo4EventElement* event);
  virtual TTree* GetTree() { return &tree_; }
 private:
  TFile file_; //!
  TTree tree_; //!
  bool tree_structure_created_ = false;
  ClassDef(OutputRootStore,1)
};

#endif // OUTPUTROOTSTREAMER_H
