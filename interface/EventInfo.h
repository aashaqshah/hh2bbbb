
#pragma once

#include <vector>
#include <memory>

#include <TTreeReader.h>
#include <TTreeReaderValue.h>

#include "mut_framework/mut_dataformats/interface/EventInfo.h"


class EventInfo : public mut::EventInfo {

  public:

    
    // For reading from VHBB_HEPPY
    TTreeReaderValue<unsigned int> * run_tr_;
    TTreeReaderValue<unsigned long long> * event_tr_;
    std::vector<std::pair<std::string,std::unique_ptr<TTreeReaderValue<int>>>> hlt_bits_; 
    std::vector<std::pair<std::string,std::unique_ptr<TTreeReaderValue<float>>>> weights_; 

    
    EventInfo() : 
     run_tr_(nullptr),
     event_tr_(nullptr) {}

    EventInfo(TTreeReader & reader, std::vector<std::string> hlt_bits = {},
              bool isData = false) : 
     run_tr_(   new TTreeReaderValue<unsigned int>(reader, "run"  )),
     event_tr_(  new TTreeReaderValue<unsigned long long>(reader, "evt" ))
    {
      std::vector<std::string> weights = {}; 
      if (!isData) weights =  {"puWeight", "puWeightUp", "puWeightDown"};

      for (const auto & hlt_bit : hlt_bits) {
        hlt_bits_.emplace_back(hlt_bit, 
                               std::unique_ptr<TTreeReaderValue<int>>(
                               new TTreeReaderValue<int>(reader,
                               hlt_bit.c_str())));
      }

      for (const auto & weight : weights) {
        weights_.emplace_back(weight, 
                              std::unique_ptr<TTreeReaderValue<float>>(
                              new TTreeReaderValue<float>(reader,
                              weight.c_str())));
      }

    } 

    ~EventInfo() {}
    
    void update() {


      this->weightPairs_.clear();
      this->filterPairs_.clear();

      // update event info 
      this->event_ = **event_tr_;
      this->run_ = **run_tr_;

      std::vector<std::pair<std::string, bool>> filterPairs;
      for (const auto & hlt_bit_pair : hlt_bits_) {
        filterPairs.emplace_back( hlt_bit_pair.first,
                                  (double) **hlt_bit_pair.second >  0.5);
      } 
      this->setFilterPairs(filterPairs);

      std::vector<std::pair<std::string, float>> weightPairs;
      for (const auto & weight : weights_) {
        weightPairs.emplace_back( weight.first,
                                  **weight.second);
      } 
      this->setWeightPairs(weightPairs);

    }
};

