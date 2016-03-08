
#pragma once

#include <algorithm>
#include <math.h>

#include "TMVA/Reader.h"

#include "BaseOperator.h"

#include "mut_framework/mut_dataformats/interface/Jet.h"
#include "mut_framework/mut_dataformats/interface/Candidate.h"

#include "mut_framework/mut_utils/interface/combinations.h"
#include "mut_framework/mut_utils/interface/prettyprint.hpp"


template <class EventClass> class MVAPairSelection : public BaseOperator<EventClass> {

  public:
 
    typedef ROOT::Math::PtEtaPhiEVector PtEtaPhiEVector;
    typedef std::vector<std::size_t>::iterator It;

    // TMVA xml files
    std::vector<std::string> mva_xmls_; 
    // pointer to the TMVA::Reader
    std::unique_ptr<TMVA::Reader> reader_; 
    // variables
    float delta_r_h1, delta_r_h2;
    float delta_m_hh, delta_pt_hh; 
    float min_csv;
    std::size_t n_jets; 
    bool well_matched;
    std::vector<PtEtaPhiEVector> * comb_jets_PtEtaPhiEVector = nullptr;
    std::vector<float> * comb_jets_CSV = nullptr;
    std::vector<PtEtaPhiEVector> * dijets_PtEtaPhiEVector = nullptr;
    PtEtaPhiEVector * fourjet_PtEtaPhiEVector = nullptr;

     MVAPairSelection(std::vector<std::string> mva_xmls = {}) : 
      mva_xmls_(mva_xmls), 
      reader_(new TMVA::Reader("Silent")) {

        // add variables
        reader_->AddVariable("TMath::Sqrt((comb_jets_PtEtaPhiEVector[2].Phi()-comb_jets_PtEtaPhiEVector[3].Phi())**2+(comb_jets_PtEtaPhiEVector[2].Eta()-comb_jets_PtEtaPhiEVector[3].Eta())**2)", &delta_r_h2);
        reader_->AddVariable("TMath::Sqrt((comb_jets_PtEtaPhiEVector[0].Phi()-comb_jets_PtEtaPhiEVector[1].Phi())**2+(comb_jets_PtEtaPhiEVector[0].Eta()-comb_jets_PtEtaPhiEVector[1].Eta())**2)", &delta_r_h1);
        reader_->AddVariable("dijets_PtEtaPhiEVector[0].M()-dijets_PtEtaPhiEVector[1].M()", &delta_m_hh);
        reader_->AddVariable("TMath::Sqrt((dijets_PtEtaPhiEVector[0].Px()-dijets_PtEtaPhiEVector[1].Px())*(dijets_PtEtaPhiEVector[0].Px()-dijets_PtEtaPhiEVector[1].Px())+(dijets_PtEtaPhiEVector[0].Py()-dijets_PtEtaPhiEVector[1].Py())*(dijets_PtEtaPhiEVector[0].Py()-dijets_PtEtaPhiEVector[1].Py()))", &delta_pt_hh);
        reader_->AddVariable("Min$(comb_jets_CSV)", &min_csv);

        // add methods
        for (const auto & mva_xml : mva_xmls_ ) {
          reader_->BookMVA(mva_xml.c_str(), mva_xml.c_str());
        }

      }
    virtual ~MVAPairSelection() {}

    virtual bool process( EventClass & ev ) {

    // matching  
    const auto & matchs = ev.reco_jet_matchs_;
    std::vector<std::set<std::size_t>> gen_b_is {{0},{1},{2},{3}};
    std::vector<std::size_t> matched_is;
    for (const auto & gen_b_i : gen_b_is) {
      matched_is.emplace_back(std::distance(matchs.begin(),
                              std::find(matchs.begin(), matchs.end(), gen_b_i)));
    }
    std::sort(matched_is.begin()+0, matched_is.begin()+2);
    std::sort(matched_is.begin()+2, matched_is.begin()+4);
    if (*(matched_is.begin()+0) > *(matched_is.begin()+2)) {
      std::iter_swap(matched_is.begin()+0, matched_is.begin()+2);
      std::iter_swap(matched_is.begin()+1, matched_is.begin()+3);
    }

    // vector with jet indexes  
    std::vector<std::size_t> jet_is(ev.jets_.size());
    std::iota(jet_is.begin(), jet_is.end(), 0);

    std::size_t n_jets = jet_is.size();
    std::string mva_to_use;
    if ( (n_jets - 4) < mva_xmls_.size()) {
      mva_to_use = mva_xmls_.at((n_jets - 4) < mva_xmls_.size());
    } else {
      mva_to_use = mva_xmls_.back(); 
    }

    // index vector of the best combination
    std::vector<std::size_t> max_is = jet_is;
    double mva_max = -100000.;

    // choose fist dijet pair -> first H candidate 
    for_each_combination(jet_is.begin(), jet_is.begin()+2,
                         jet_is.end(), [&](It fo, It lo) -> bool {
      // choose second dijet pair -> second H candidate 
    	for_each_combination(jet_is.begin()+2, jet_is.begin()+4, 
                           jet_is.end(), [&](It fi, It li) -> bool {
        // neat trick to skip permutations                   
        if ( *(jet_is.begin()+0) <  *(jet_is.begin()+2)) {
          bool is_good_comb = std::equal(matched_is.begin(), matched_is.end(), jet_is.begin());

          // to fill tree
          comb_jets_PtEtaPhiEVector = new std::vector<PtEtaPhiEVector>();
          comb_jets_CSV = new std::vector<float>();
          dijets_PtEtaPhiEVector = new std::vector<PtEtaPhiEVector>();

          n_jets = ev.jets_.size();
          well_matched = is_good_comb;

          // this could be done faster with VectorUtil function                   
  				double mass_one = (ev.jets_.at(*(jet_is.begin()+0))+ ev.jets_.at(*(jet_is.begin()+1))).M();
	  			double mass_two = (ev.jets_.at(*(jet_is.begin()+2))+ ev.jets_.at(*(jet_is.begin()+3))).M();
          for (std::size_t i = 0; i < 4 ; i++) {
            const auto & jet = ev.jets_.at(jet_is.at(i));
            comb_jets_PtEtaPhiEVector->emplace_back(jet); 
            comb_jets_CSV->emplace_back(jet.getDiscriminator("CSV"));
          }

          if (mass_one < mass_two) {
            std::iter_swap(comb_jets_PtEtaPhiEVector->begin()+0,
                           comb_jets_PtEtaPhiEVector->begin()+2);
            std::iter_swap(comb_jets_PtEtaPhiEVector->begin()+1,
                           comb_jets_PtEtaPhiEVector->begin()+3);
            std::iter_swap(comb_jets_CSV->begin()+0,
                           comb_jets_CSV->begin()+2);
            std::iter_swap(comb_jets_CSV->begin()+1,
                           comb_jets_CSV->begin()+3);
          }
          

          dijets_PtEtaPhiEVector->emplace_back(comb_jets_PtEtaPhiEVector->at(0)+
                                               comb_jets_PtEtaPhiEVector->at(1)); 
          dijets_PtEtaPhiEVector->emplace_back(comb_jets_PtEtaPhiEVector->at(2)+
                                               comb_jets_PtEtaPhiEVector->at(3)); 
          fourjet_PtEtaPhiEVector = new PtEtaPhiEVector(dijets_PtEtaPhiEVector->at(0)+
                                                        dijets_PtEtaPhiEVector->at(1)); 

          delta_r_h2 = ROOT::Math::VectorUtil::DeltaR(comb_jets_PtEtaPhiEVector->at(2),
                                                      comb_jets_PtEtaPhiEVector->at(3));
          delta_r_h1 = ROOT::Math::VectorUtil::DeltaR(comb_jets_PtEtaPhiEVector->at(0),
                                                      comb_jets_PtEtaPhiEVector->at(1));
          delta_m_hh = dijets_PtEtaPhiEVector->at(0).M()-dijets_PtEtaPhiEVector->at(1).M();
          delta_pt_hh = (dijets_PtEtaPhiEVector->at(0)-dijets_PtEtaPhiEVector->at(1)).Pt();
          min_csv = *std::min_element(comb_jets_CSV->begin(), comb_jets_CSV->end());

          double mva_value = reader_->EvaluateMVA(mva_to_use.c_str());

          if ( mva_value > mva_max) {
            mva_max = mva_value;
            max_is.clear();
            if (mass_one > mass_two) {
              max_is.insert(max_is.begin(), jet_is.begin(), jet_is.end());
            } else {
              max_is.insert(max_is.begin(), jet_is.begin()+2, jet_is.begin()+4);
              max_is.insert(max_is.begin()+2, jet_is.begin(), jet_is.begin()+2);
              max_is.insert(max_is.begin()+4, jet_is.begin()+4, jet_is.end());
            }
          }

          delete comb_jets_PtEtaPhiEVector;
          delete comb_jets_CSV;
          delete dijets_PtEtaPhiEVector;
          delete fourjet_PtEtaPhiEVector;
          

        } 
        return false;
    	});
      return false;
		});

      // the fist pair of elements of the min_is variable are
      // the indexes of the fist pair and the folowign two
      // are indexes for the second pair 

      // use same order for jet collection (copy overhead as it is now)
      auto ordered_jets = std::vector<mut::Jet>{}; 
      for (std::size_t i = 0; i < ev.jets_.size(); i++ ) {
        ordered_jets.emplace_back(ev.jets_.at(max_is.at(i)));
      }
      for (std::size_t i = 0; i < ev.jets_.size(); i++ ) {
        ev.jets_.at(i) = ordered_jets.at(i);
      }  
        
      // fill dijet objects
      ev.dijets_.clear();
      ev.dijets_.emplace_back(ev.jets_.at(0) + ev.jets_.at(1));
      ev.dijets_.emplace_back(ev.jets_.at(2) + ev.jets_.at(3));

      // free jet will be min CSV of the first four jets
      std::vector<float> csv_vector;
      for (std::size_t i=0; i < 4; i++) {
        csv_vector.emplace_back(ev.jets_.at(i).getDiscriminator("CSV"));
      }
      ev.free_is_.clear();
      ev.free_is_.emplace_back(std::distance(csv_vector.begin(),
                              std::min_element(csv_vector.begin(),csv_vector.end())));

      return true;
    }

    virtual bool output( TFile * tfile) {

      return true;

    }

};
