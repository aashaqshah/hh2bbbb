
#pragma once

#include <algorithm>

#include "BaseOperator.h"
#include "mut_framework/mut_utils/interface/combinations.h"
#include "mut_framework/mut_utils/interface/prettyprint.hpp"


template <class EventClass> class DiJetPairSelection : public BaseOperator<EventClass> {

  public:

    typedef std::vector<int>::iterator It;
    std::size_t n_fix_jets_;


    DiJetPairSelection( std::size_t n_fix_jets = 3  ) :
    n_fix_jets_(n_fix_jets) {}
    virtual ~DiJetPairSelection() {}

    virtual bool process( EventClass & ev ) {

    std::vector<int> jet_is(ev.jets_.size());
    std::iota(jet_is.begin(), jet_is.end(), 0);

    std::vector<int> min_is(jet_is.begin(), jet_is.end());
    double min_v = 100000 ;

    // iterate over all jet combinations for the non fixed jet 
    for_each_combination(jet_is.begin()+n_fix_jets_, jet_is.begin()+4,
                         jet_is.end(), [&](It fo, It lo) -> bool {
      // iterate over all dijet pickings 
    	for_each_combination(jet_is.begin(), jet_is.begin()+2, 
                           jet_is.begin()+3, [&](It fi, It li) -> bool {
        // this could be done faster with VectorUtil function                   
				double mass_one = (ev.jets_.at(*jet_is.begin())+ ev.jets_.at(*(jet_is.begin()+1))).M();
				double mass_two = (ev.jets_.at(*(jet_is.begin()+2))+ ev.jets_.at(*(jet_is.begin()+3))).M();
        double mass_diff = std::abs(mass_one-mass_two);
        if ( mass_diff < min_v) {
          min_v = mass_diff; 
          min_is.clear();
          if (mass_one > mass_two) {
            min_is.insert(min_is.begin(), jet_is.begin(), jet_is.end());
          } else {
            min_is.insert(min_is.begin(), jet_is.begin()+2, jet_is.begin()+4);
            min_is.insert(min_is.begin()+2, jet_is.begin(), jet_is.begin()+2);
            min_is.insert(min_is.begin()+4, jet_is.begin()+4, jet_is.end());
          }
        }
        return false;
    	});
      return false;
		});

      // the fist pair of elements of the min_is variale are
      // the indexes of the fist pair and the folowign two
      // are indexes for the second pair 
      ev.dijets_.emplace_back(ev.jets_.at(0) + ev.jets_.at(1));
      ev.dijets_.emplace_back(ev.jets_.at(2) + ev.jets_.at(3));

      return true;
    }

};
