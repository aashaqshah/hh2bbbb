
#pragma once

#include <algorithm>

#include "TH1.h"
#include "TH2.h"

#include "BaseOperator.h"

template <class EventClass> class FreeJetTagPdf : public BaseOperator<EventClass> {

  public:

    std::string disc_;
    double d_value_;
    bool root_;

    std::vector<double> pt_bins {20., 50., 80., 120., 180., 300., 500. };
    std::vector<double> abs_eta_bins {0., 0.5, 1., 1.5, 2.5 };

    TH1D h_free_jet_tag_number {"free_jet_tag_number", "", 10, 0., 10.};
    TH1D h_free_jet_tag_pt     {"free_jet_tag_pt", "", 50, 0., 1000};
    TH1D h_free_jet_tag_abs_eta    {"free_jet_tag_abs_eta", "", 20, 0., 2.5};
    TH2D h_free_jet_tag_pt_abs_eta    {"free_jet_tag_pt_abs_eta", "", 
                                       int(pt_bins.size()-1), pt_bins.data(),
                                       int(abs_eta_bins.size()-1), abs_eta_bins.data()};
    TH1D h_free_jet_untag_number {"free_jet_untag_number", "", 10, 0., 10.};
    TH1D h_free_jet_untag_pt     {"free_jet_untag_pt", "", 50, 0., 1000};
    TH1D h_free_jet_untag_abs_eta    {"free_jet_untag_abs_eta", "", 20, 0.0, 2.5};
    TH2D h_free_jet_untag_pt_abs_eta    {"free_jet_untag_pt_abs_eta", "", 
                                       int(pt_bins.size()-1), pt_bins.data(),
                                       int(abs_eta_bins.size()-1), abs_eta_bins.data()};

    FreeJetTagPdf( std::string disc, double d_value, bool root = true) : 
      disc_(disc), 
      d_value_(d_value),
      root_(root)  {}
    virtual ~FreeJetTagPdf() {}

    virtual void init(TDirectory * tdir) {
      if (root_) tdir = tdir->GetFile();
      h_free_jet_tag_number.SetDirectory(tdir);
      h_free_jet_tag_pt.SetDirectory(tdir);
      h_free_jet_tag_abs_eta.SetDirectory(tdir);
      h_free_jet_tag_pt_abs_eta.SetDirectory(tdir);
      h_free_jet_untag_number.SetDirectory(tdir);
      h_free_jet_untag_pt.SetDirectory(tdir);
      h_free_jet_untag_abs_eta.SetDirectory(tdir);
      h_free_jet_untag_pt_abs_eta.SetDirectory(tdir);
      h_free_jet_tag_number.Sumw2();
      h_free_jet_tag_pt.Sumw2();
      h_free_jet_tag_abs_eta.Sumw2();
      h_free_jet_tag_pt_abs_eta.Sumw2();
      h_free_jet_untag_number.Sumw2();
      h_free_jet_untag_pt.Sumw2();
      h_free_jet_untag_abs_eta.Sumw2();
      h_free_jet_untag_pt_abs_eta.Sumw2();
    }

    virtual bool process( EventClass & ev ) {

        std::size_t n_tag = 0;
        std::size_t n_untag = 0;
        for (const auto & i : ev.free_is_) {
        const auto & jet = ev.jets_.at(i);
        if (jet.getDiscriminator(disc_) >= d_value_) {
          n_tag++;
          h_free_jet_tag_pt.Fill(jet.pt());
          h_free_jet_tag_abs_eta.Fill(std::abs(jet.eta()));
          h_free_jet_tag_pt_abs_eta.Fill(jet.pt(), std::abs(jet.eta()));
        } else {
          n_untag++;
          h_free_jet_untag_pt.Fill(jet.pt());
          h_free_jet_untag_abs_eta.Fill(std::abs(jet.eta()));
          h_free_jet_untag_pt_abs_eta.Fill(jet.pt(), std::abs(jet.eta()));
        }
       }
      
      h_free_jet_tag_number.Fill(n_tag);
      h_free_jet_untag_number.Fill(n_untag);  

      return true;
    }

    virtual bool output( TFile * tfile) {

      return true;

    }

};
