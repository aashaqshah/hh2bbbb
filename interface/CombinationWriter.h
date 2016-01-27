
#pragma once

#include <algorithm>
#include <math.h>

#include "BaseOperator.h"

#include "mut_framework/mut_dataformats/interface/Jet.h"
#include "mut_framework/mut_dataformats/interface/Candidate.h"

#include "mut_framework/mut_utils/interface/combinations.h"
#include "mut_framework/mut_utils/interface/prettyprint.hpp"


template <class EventClass> class CombinationWriter : public BaseOperator<EventClass> {

  public:
 
    typedef std::vector<std::size_t>::iterator It;
    bool root_;
    std::string dir_;

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

    std::cout << "matchs - " << matchs << std::endl;
    std::cout << "matched_is - " << matched_is << std::endl;


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
          bool is_good_comb = std::equal(matched_is.begin(), matched_is.end(), jet_is.begin());
          std::cout << jet_is << is_good_comb << std::endl;                   

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
