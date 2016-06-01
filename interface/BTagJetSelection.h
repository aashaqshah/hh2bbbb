
#pragma once

#include <algorithm>

#include "BaseOperator.h"
#include "mut_framework/mut_dataformats/interface/Jet.h"


inline void order_jets_by_disc(std::vector<mut::Jet> & jets, std::string & disc) {
      // sort in discriminator order 
      auto comparator = [&](mut::Jet a, mut::Jet b){ 
        return a.getDiscriminator(disc) > b.getDiscriminator(disc); };
      std::sort(jets.begin(), jets.end(), comparator );
}


template <class EventClass> class BTagJetSelection : public BaseOperator<EventClass> {

  public:

    std::string disc_;
    double d_value_;
    std::size_t min_number_;


    BTagJetSelection( std::string disc, double d_value, std::size_t min_number ) :
    disc_(disc),
    d_value_(d_value),
    min_number_(min_number) {}
    virtual ~BTagJetSelection() {}

    virtual bool process( EventClass & ev ) {

      // order by discriminator
      order_jets_by_disc(ev.jets_, disc_);

      // check that min_number jet pass d_value    
      for (std::size_t i=0; i < min_number_; i++) {
        if (ev.jets_.at(i).getDiscriminator(disc_) < d_value_) return false;
      }

      // compute event btag weights
      std::map<std::string, float> ev_bw_m;
      for (const auto & jet : ev.jets_) {
        for (const auto & pair : jet.getJetIDPairs()) {
          if (pair.first.find("bTagW") != std::string::npos) {
            if( ev_bw_m.count(pair.first) == 0) { 
              ev_bw_m[pair.first] = pair.second;
            } else {
              ev_bw_m[pair.first] *= pair.second;
            }
          }
        }
      }

      for (const auto & ev_bw : ev_bw_m) {
        ev.eventInfo_.emplaceWeight( ev_bw.first, ev_bw.second);
      }

      return true;
    }

    virtual std::string get_name() {
      auto name = std::string{"sort_jets_in"+disc_};
      name += "and_min_"+std::to_string(min_number_)+">" + std::to_string(d_value_); 
      return name;
    } 

};
