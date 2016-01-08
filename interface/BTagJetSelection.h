
#pragma once

#include <algorithm>

#include "BaseOperator.h"
#include "mut_framework/mut_dataformats/interface/Jet.h"

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

      // sort in discriminator order 
      auto comparator = [&](mut::Jet a, mut::Jet b){ 
        return a.getDiscriminator(disc_) > b.getDiscriminator(disc_); };
      std::sort(ev.jets_.begin(), ev.jets_.end(), comparator );

      // check that min_number jet pass d_value    
      for (std::size_t i=0; i < min_number_; i++) {
        if (ev.jets_.at(i).getDiscriminator(disc_) < d_value_) return false;
      }

      return true;
    }

    virtual std::string get_name() {
      auto name = std::string{"sort_jets_in"+disc_};
      name += "and_min_"+std::to_string(min_number_)+">" + std::to_string(d_value_); 
      return name;
    } 

};
