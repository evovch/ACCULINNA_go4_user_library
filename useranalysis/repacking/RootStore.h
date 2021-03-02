/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ROOTSTORE_H
#define ROOTSTORE_H

#include "TFile.h"
#include "TTree.h"

#include "TGo4EventStore.h"

#include "setupconfigcppwrapper/SetupConfiguration.h"

class TGo4UserStoreParameter;
class DetEventCommon;

/** Class to store in output file results of Repacking step using raw C++ arrays.
    Structure of data depends on SetupConfiguration.
    Each branch contains data for one station( from DetEventStation object).
    DetEventCommon stores as EventCommon.
    SetupConfiguration is also store in output file to be used for reading. **/ 
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
  void FillCommonData(const DetEventCommon&);
  TFile file_; //!
  TTree tree_; //!
  std::map<TString, std::map<TString, short*>> event_data_; //!
  std::map<TString, std::map<TString, unsigned short>> channels_counts_; //!
  EventCommon event_common_; //!
  bool output_structure_created_ = false; //!
  ClassDef(RootStore, 1)
};

#endif // ROOTSTORE_H
