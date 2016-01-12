
#pragma once

#include <algorithm>

#include "TH1.h"
#include "TH2.h"

#include "BaseOperator.h"

template <class EventClass> class DiHiggsPlotter : public BaseOperator<EventClass> {

  public:
 
    std::vector<std::string> weights_;

    TH1D h_H0_mass{"h_H0_mass", "", 300, 0., 900.};
    TH1D h_H1_mass {"h_H1_mass", "", 300, 0., 900.};
    TH2D h_H0_H1_mass {"h_H0_H1_mass","", 300, 0., 600., 300, 0., 600.};  

    DiHiggsPlotter(const std::vector<std::string> & weights = {}) :
      weights_(weights) {}
    virtual ~DiHiggsPlotter() {}

    virtual void init(TDirectory * tdir) {
      h_H0_mass.SetDirectory(tdir);
      h_H1_mass.SetDirectory(tdir);
      h_H0_H1_mass.SetDirectory(tdir);
    }

    virtual bool process( EventClass & ev ) {

      float w = 1.0;
      for ( const auto & weight :  weights_ ) w*=ev.eventInfo_.getWeight(weight);

      h_H0_mass.Fill(ev.dijets_.at(0).mass(), w);
      h_H1_mass.Fill(ev.dijets_.at(1).mass(), w);
      h_H0_H1_mass.Fill(ev.dijets_.at(0).mass(), ev.dijets_.at(1).mass(), w);

      return true;
    }

    virtual bool output( TFile * tfile) {

      return true;

    }

};
