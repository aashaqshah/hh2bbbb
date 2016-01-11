
#pragma once

#include <algorithm>

#include "TH1.h"
#include "TH2.h"

#include "BaseOperator.h"

template <class EventClass> class FreeJetPlotter : public BaseOperator<EventClass> {

  public:

    std::string disc_;

    TH1D h_free_jet_size {"free_jet_size", "", 10, 0., 10.};
    TH1D h_free_jet_pt   {"free_jet_pt", "", 100, 0., 2000};
    TH1D h_free_jet_eta  {"free_jet_eta", "", 100, -10, 10};
    TH1D h_free_jet_csv  {"free_jet_csv", "", 100, -0.01, 1.01};

    FreeJetPlotter( std::string disc ) : disc_(disc)  {}
    virtual ~FreeJetPlotter() {}

    virtual void init(TDirectory * tdir) {
      h_free_jet_size.SetDirectory(tdir);
      h_free_jet_pt.SetDirectory(tdir);
      h_free_jet_eta.SetDirectory(tdir);
      h_free_jet_csv.SetDirectory(tdir);
    }

    virtual bool process( EventClass & ev ) {

      h_free_jet_size.Fill(ev.free_is_.size());
      for (const auto & i : ev.free_is_) {
        const auto & jet = ev.jets_.at(i);
        h_free_jet_pt.Fill(jet.pt());
        h_free_jet_eta.Fill(jet.eta());
        h_free_jet_csv.Fill(jet.getDiscriminator(disc_));
      }

      return true;
    }

    virtual bool output( TFile * tfile) {

      return true;

    }

};
