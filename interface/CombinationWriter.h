
#pragma once

#include <algorithm>
#include <math.h>

#include "BaseOperator.h"

#include "mut_framework/mut_dataformats/interface/Jet.h"
#include "mut_framework/mut_dataformats/interface/Candidate.h"

#include "mut_framework/mut_utils/interface/combinations.h"
#include "mut_framework/mut_utils/interface/prettyprint.hpp"


class Combination {
  public:
    std::size_t event;
    std::size_t n_jets; 
    bool well_matched; 
};


template <class EventClass> class CombinationWriter : public BaseOperator<EventClass> {

  public:
 
    typedef ROOT::Math::PtEtaPhiEVector PtEtaPhiEVector;
    typedef std::vector<std::size_t>::iterator It;
    bool root_;
    std::string dir_;
    // variables to save in branches
    std::size_t event;
    std::size_t n_jets; 
    bool well_matched;
    std::vector<PtEtaPhiEVector> * comb_jets_PtEtaPhiEVector = nullptr;
    std::vector<float> * comb_jets_CSV = nullptr;
    std::vector<PtEtaPhiEVector> * dijets_PtEtaPhiEVector = nullptr;
    PtEtaPhiEVector * fourjet_PtEtaPhiEVector = nullptr;

    TTree comb_tree_{"comb_tree","Tree with jet combinations"};

     CombinationWriter(bool root = false, std::string dir = ""  ) :
      root_(root),
      dir_(dir) {}
    virtual ~CombinationWriter() {}

    virtual void init(TDirectory * tdir) {
      if (root_) {
        tdir = tdir->GetFile();
        auto ndir = tdir->mkdir(dir_.c_str());
        if (ndir == 0) {
          tdir = tdir->GetDirectory(dir_.c_str());
        } else {
          tdir = ndir;
        }
      }
      comb_tree_.Branch("event",&event, "event/I");
      comb_tree_.Branch("n_jets",&n_jets, "n_jets/I");
      comb_tree_.Branch("well_matched",&well_matched, "well_matched/O");
      comb_tree_.Branch("comb_jets_PtEtaPhiEVector","std::vector<PtEtaPhiEVector>",
                        &comb_jets_PtEtaPhiEVector, 64000, 1);
      comb_tree_.Branch("comb_jets_CSV","std::vector<float>",
                        &comb_jets_CSV, 64000, 1);
      comb_tree_.Branch("dijets_PtEtaPhiEVector","std::vector<PtEtaPhiEVector>",
                        &dijets_PtEtaPhiEVector, 64000, 1);
      comb_tree_.Branch("fourjet_PtEtaPhiEVector","PtEtaPhiEVector",
                        &fourjet_PtEtaPhiEVector, 64000, 1);


      comb_tree_.SetDirectory(tdir);
      comb_tree_.AutoSave();

   }


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

    // choose fist dijet pair -> first H candidate 
    for_each_combination(jet_is.begin(), jet_is.begin()+2,
                         jet_is.end(), [&](It fo, It lo) -> bool {
      // choose second dijet pair -> second H candidate 
    	for_each_combination(jet_is.begin()+2, jet_is.begin()+4, 
                           jet_is.end(), [&](It fi, It li) -> bool {
        // neat trick to skip permutations                   
        if ( *(jet_is.begin()+0) <  *(jet_is.begin()+2)) {
          std::cout << jet_is << std::endl;
          bool is_good_comb = std::equal(matched_is.begin(), matched_is.end(), jet_is.begin());

          // to fill tree
          comb_jets_PtEtaPhiEVector = new std::vector<PtEtaPhiEVector>();
          comb_jets_CSV = new std::vector<float>();
          dijets_PtEtaPhiEVector = new std::vector<PtEtaPhiEVector>();

          event = ev.eventInfo_.getEvent();
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
          
          

          comb_tree_.Fill();

          delete comb_jets_PtEtaPhiEVector;
          delete comb_jets_CSV;
          delete dijets_PtEtaPhiEVector;
          delete fourjet_PtEtaPhiEVector;
          

        } 
        return false;
    	});
      return false;
		});

      return true;
    }

    virtual bool output( TFile * tfile) {

      return true;

    }

};
