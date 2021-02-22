#include "RootStore.h"

#include <iostream>

#include "TString.h"
#include "TClonesArray.h"

#include "TGo4UserStoreParameter.h"

#include "setupconfigcppwrapper/SetupConfiguration.h"
#include "data/DetEventFull.h"
#include "data/DetEventDetector.h"
#include "data/DetEventStation.h"
#include "data/DetMessage.h"

RootStore::RootStore(TGo4UserStoreParameter* store_parameter)
  : TGo4EventStore(store_parameter->GetName()),
    file_(store_parameter->GetName(), "RECREATE"),
    tree_("accdaq", "Tree for accdaq output")
{}

Int_t RootStore::Store(TGo4EventElement* event) {
  if (!output_structure_created_)
    CreateOutputStructure();
  ResetEventData();
  DetEventFull* full_event = dynamic_cast<DetEventFull*>(event);
  auto& setup = SetupConfiguration::GetInstance();
  for (const auto& detector_name_and_id : setup.GetDetectorList()) {
    const auto detector_name = detector_name_and_id.first;
    auto* detector_event = dynamic_cast<DetEventDetector*>(full_event->GetChild(detector_name));
    if (!detector_event)
      continue;
    for (const auto& station_name_and_id : setup.GetStationList(detector_name)) {
      const auto station_name = station_name_and_id.first;
      const auto station_key = TString::Format("%s_%s", detector_name.Data(), station_name.Data());
      auto* station_event = dynamic_cast<DetEventStation*>(detector_event->GetChild(station_key));
      if (!station_event)
        continue;
      TIter iter(station_event->GetDetMessages());
      while (auto* detector_message = dynamic_cast<DetMessage*>(iter.Next())) {
        const auto channel = detector_message->GetStChannel();
        const auto value = detector_message->GetValue();
        if (channel > channels_counts_[detector_name][station_name]) {
          std::cerr << "RootStore: channel number " << channel 
                    << " in data greater then channels count " 
                    << channels_counts_[detector_name][station_name] << " in setup file for "
                    << station_name << " from " << detector_name << std::endl;
          continue;
        }
        event_data_[detector_name][station_name][channel] = value;
      }
    }
  }
  tree_.Fill();
  return 0;
}

void RootStore::CreateOutputStructure() {
  auto& setup = SetupConfiguration::GetInstance();
  for (const auto& detector_name_and_id : setup.GetDetectorList()) {
    const auto detector_name = detector_name_and_id.first;
    for (const auto& station_name_and_id : setup.GetStationList(detector_name)) {
      const auto station_name = station_name_and_id.first;
      const auto channel_count = setup.GetChannelCount(detector_name, station_name);
      event_data_[detector_name][station_name] = new short[channel_count];
      channels_counts_[detector_name][station_name] = channel_count;
      tree_.Branch(station_name, event_data_[detector_name][station_name],
                   TString::Format("%s[%d]/S", station_name.Data(), channel_count));
    }
  }
  file_.cd();
  tree_.Write();
  output_structure_created_ = true;
}

void RootStore::ResetEventData() {
  for (auto& detector_data : event_data_) {
    const auto detector_name = detector_data.first;
    for (auto& station_data : detector_data.second) {
      const auto station_name = station_data.first;
      for (short i_channel = 0; i_channel < channels_counts_[detector_name][station_name]; i_channel++) {
        station_data.second[i_channel] = -1;
      }
    }
  }
}

RootStore::~RootStore() {
  auto* file = tree_.GetCurrentFile();
  file->cd();
  tree_.Write(0, TObject::kOverwrite);
}
