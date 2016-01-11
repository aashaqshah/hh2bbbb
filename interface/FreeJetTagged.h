
#pragma once

#include <algorithm>

#include "TH1.h"
#include "TH2.h"

#include "BaseOperator.h"

template <class EventClass> class FreeJetTagged : public BaseOperator<EventClass> {

  public:

    std::string disc_;
    double d_value_;
    std::size_t n_tagged_;

    FreeJetTagged( std::string disc, double d_value , std::size_t n_tagged = 1) : 
      disc_(disc), 
      d_value_(d_value),
      n_tagged_(n_tagged)  {}
    virtual ~FreeJetTagged() {}

    virtual bool process( EventClass & ev ) {

      std::size_t n_tagged{0};
      for (const auto & i : ev.free_is_) {
        const auto & jet = ev.jets_.at(i);
        if (jet.getDiscriminator(disc_) >= d_value_) {
          n_tagged++;
        }
       }
      
      return (n_tagged >= n_tagged_);
    }

   virtual std::string get_name() {
     std::string name{"free_jet_tagged"};
     return name;
   } 


};
