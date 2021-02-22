#ifndef ROOTSTORE_H
#define ROOTSTORE_H

#include "TObject.h"
#include "TFile.h"
#include "TTree.h"

#include "TGo4EventStore.h"

class TGo4UserStoreParameter;

class RootStore : public TGo4EventStore {
 public:
  RootStore();
  RootStore(TGo4UserStoreParameter*);
  virtual ~RootStore();
  virtual Int_t Store(TGo4EventElement* event);
  virtual TTree* GetTree() { return &tree_; }
 private:
  void CreateOutputStructure();
  void ResetEventData();
  TFile file_; //!
  TTree tree_; //!
  std::map<TString, std::map<TString, short*>> event_data_;
  std::map<TString, std::map<TString, unsigned short>> channels_counts_;
  bool output_structure_created_ = false;
  ClassDef(RootStore, 1)
};

#endif // ROOTSTORE_H
