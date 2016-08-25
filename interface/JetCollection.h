
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

    bool isData_;

    // TTreeReaderValues for reading from VHBB_HEPPY
    TTreeReaderArray<float> * jet_pts_;
    TTreeReaderArray<float> * jet_etas_;
    TTreeReaderArray<float> * jet_phis_;
    TTreeReaderArray<float> * jet_masss_;
    TTreeReaderArray<int> * jet_hadflavs_;
    TTreeReaderArray<int> * jet_parflavs_;
    // for jet corr shifts
    TTreeReaderArray<float> * jet_corr_;
    TTreeReaderArray<float> * jet_corr_shifted_;
    double corr_factor_;
    std::vector<std::pair<std::string,std::unique_ptr<TTreeReaderArray<float>>>> discs_trs_; 
    std::vector<std::pair<std::string,std::unique_ptr<TTreeReaderArray<float>>>> btag_weights_; 
    
    JetCollection() : 
     isData_(false),
     jet_pts_(nullptr),
     jet_etas_(nullptr),
     jet_phis_(nullptr),
     jet_masss_(nullptr),
     jet_hadflavs_(nullptr),
     jet_parflavs_(nullptr),
     jet_corr_(nullptr),
     jet_corr_shifted_(nullptr),
     corr_factor_(1.0) {}

    JetCollection(TTreeReader & reader, bool isData = false,
                  const std::vector<std::string> & corr_names = {}, 
                  double corr_factor = 1.0,
                  const std::vector<std::string> & discs = {"CSV","CMVAV2"}) : 
     isData_(isData), 
     jet_pts_(   new TTreeReaderArray<float>(reader, "Jet_pt"  )),
     jet_etas_(  new TTreeReaderArray<float>(reader, "Jet_eta" )),
     jet_phis_(  new TTreeReaderArray<float>(reader, "Jet_phi" )),
     jet_masss_( new TTreeReaderArray<float>(reader, "Jet_mass")),
     jet_hadflavs_(nullptr),
     jet_parflavs_(nullptr),
     jet_corr_(nullptr),
     jet_corr_shifted_(nullptr),
     corr_factor_(corr_factor) 

    {
      for (const auto & disc : discs) {
        discs_trs_.emplace_back(disc,
                                std::unique_ptr<TTreeReaderArray<float>>(
                                new TTreeReaderArray<float>(reader,
                                ("Jet_btag"+disc).c_str())));

      }

      if (!isData) {
        jet_hadflavs_ = new TTreeReaderArray<int>(reader, "Jet_hadronFlavour");
        jet_parflavs_ =  new TTreeReaderArray<int>(reader, "Jet_partonFlavour");

        std::vector<std::string> bw_names = {"JES","LF","HF","LFStats1","LFStats2",
                                           "HFStats1","HFStats2","cErr1","cErr2"};
        std::vector<std::string> shifts = {"Up","Down"};

        btag_weights_.emplace_back("bTagWeight",  
                                   std::unique_ptr<TTreeReaderArray<float>>(
                                   new TTreeReaderArray<float>(reader,
                                   "Jet_bTagWeight"))); 
        for (const auto & bw_name : bw_names) {
          for (const auto & shift : shifts) {  
            auto key = "bTagWeight"+bw_name+shift;
            btag_weights_.emplace_back(key,  
                                       std::unique_ptr<TTreeReaderArray<float>>(
                                       new TTreeReaderArray<float>(reader,
                                       ("Jet_"+key).c_str())));
          }
        }

        if (corr_names.size() == 2) {
          jet_corr_ = new TTreeReaderArray<float>(reader, corr_names.at(0).c_str());
          jet_corr_shifted_ = new TTreeReaderArray<float>(reader, corr_names.at(1).c_str());
        }
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

        std::vector<std::pair<std::string, float>> idPairs;
        for (const auto & bw : btag_weights_) {
          idPairs.emplace_back(bw.first,
                               (*bw.second)[i]);
        } 
        this->back().setJetIDPairs(idPairs);


        if (!isData_) {
          this->back().setHadronFlavour((*jet_hadflavs_)[i]);
          this->back().setPartonFlavour((*jet_parflavs_)[i]);
          if (jet_corr_ != nullptr) {
            float jet_corr_shifted = (*jet_corr_shifted_)[i];
            float jet_corr = (*jet_corr_)[i];
            float jet_corr_scaled = jet_corr+corr_factor_*(jet_corr_shifted-jet_corr);
            this->back().SetPt(this->back().Pt()*jet_corr_scaled/jet_corr);
         }
        }
      }
    }
};

