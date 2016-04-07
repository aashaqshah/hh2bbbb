
#pragma once

#include <vector>
#include <algorithm>
#include <memory>

#include <TTreeReader.h>
#include <TTreeReaderArray.h>

#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"

#include "mut_framework/mut_dataformats/interface/Jet.h"

typedef ROOT::Math::PtEtaPhiMVector PtEtaPhiMVector;

class JetCollection : public std::vector<mut::Jet> {

  public:

    
    // TTreeReaderValues for reading from VHBB_HEPPY
    TTreeReaderArray<float> * jet_pts_;
    TTreeReaderArray<float> * jet_etas_;
    TTreeReaderArray<float> * jet_phis_;
    TTreeReaderArray<float> * jet_masss_;
    TTreeReaderArray<int> * jet_hadflavs_;
    TTreeReaderArray<int> * jet_parflavs_;
    std::vector<std::pair<std::string,std::unique_ptr<TTreeReaderArray<float>>>> discs_trs_; 
    
    JetCollection() : 
     jet_pts_(nullptr),
     jet_etas_(nullptr),
     jet_phis_(nullptr),
     jet_masss_(nullptr),
     jet_hadflavs_(nullptr),
     jet_parflavs_(nullptr) {}

    JetCollection(TTreeReader & reader, std::vector<std::string> discs = {"CSV","CMVAV2"}) : 
     jet_pts_(   new TTreeReaderArray<float>(reader, "Jet_pt"  )),
     jet_etas_(  new TTreeReaderArray<float>(reader, "Jet_eta" )),
     jet_phis_(  new TTreeReaderArray<float>(reader, "Jet_phi" )),
     jet_masss_( new TTreeReaderArray<float>(reader, "Jet_mass")),
     jet_hadflavs_( new TTreeReaderArray<int>(reader, "Jet_hadronFlavour")),
     jet_parflavs_( new TTreeReaderArray<int>(reader, "Jet_partonFlavour"))
    {
      for (const auto & disc : discs) {
        discs_trs_.emplace_back(disc,                                                                 
                                std::unique_ptr<TTreeReaderArray<float>>(                                
                                new TTreeReaderArray<float>(reader,                                      
                                ("Jet_btag"+disc).c_str())));                                                      

      }
    } 

    ~JetCollection() {}
    
    void update() {

      // delete previous elements
      this->clear();

      // jets ordered in pt
      std::vector<int> order(jet_pts_->GetSize());
      std::iota(order.begin(), order.end(), 0); 
      auto comparator = [&](int a, int b){ return (*jet_pts_)[a] > (*jet_pts_)[b]; };
      std::sort(order.begin(), order.end(), comparator);

      // iterate over jets in order 
      for (const auto & i : order) {
        
        PtEtaPhiMVector jet_lv((*jet_pts_)[i],
                               (*jet_etas_)[i],
                               (*jet_phis_)[i],
                               (*jet_masss_)[i]); 
        // new element using constructor from PtEtaPhiM
      	this->emplace_back(jet_lv);

        std::vector<std::pair<std::string, float>> disPairs;
        for (const auto & disc_trs : discs_trs_) {
          disPairs.emplace_back( disc_trs.first,
                                 (*disc_trs.second)[i]);
        } 
        this->back().setDiscriminatorPairs(disPairs);
        this->back().setHadronFlavour((*jet_hadflavs_)[i]);
        this->back().setPartonFlavour((*jet_parflavs_)[i]);
        
      }
    }
};

