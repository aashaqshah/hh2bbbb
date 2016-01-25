
#pragma once

#include <algorithm>

#include "TH1.h"
#include "TH2.h"
#include "Math/GenVector/VectorUtil.h"

#include "BaseOperator.h"

template <class EventClass> class FreeJetPlotter : public BaseOperator<EventClass> {

  public:

    std::string disc_;
    std::vector<std::string> weights_;
    bool root_;
    std::string dir_;

    TH1D h_free_jet_size {"free_jet_size", "", 10, 0., 10.};
    TH1D h_free_jet_pt   {"free_jet_pt", "", 100, 0., 2000};
    TH1D h_free_jet_eta  {"free_jet_eta", "", 100, -10, 10};
    TH1D h_free_jet_csv  {"free_jet_csv", "", 100, -0.01, 1.01};
    TH1D h_free_jet_min_delta_phi_jet  {"free_jet_csv_min_delta_phi_jet", "", 100, 0.0, 4.0};
    TH1D h_free_jet_size_unc_sq {"free_jet_size_unc_sq", "", 10, 0., 10.};
    TH1D h_free_jet_pt_unc_sq   {"free_jet_pt_unc_sq", "", 100, 0., 2000};
    TH1D h_free_jet_eta_unc_sq  {"free_jet_eta_unc_sq", "", 100, -10, 10};
    TH1D h_free_jet_csv_unc_sq  {"free_jet_csv_unc_sq", "", 100, -0.01, 1.01};
    TH1D h_free_jet_min_delta_phi_jet_unc_sq  {"free_jet_csv_min_delta_phi_jet_unc_sq", "", 100, 0.0, 4.0};


    FreeJetPlotter( std::string disc, const std::vector<std::string> & weights = {},
                    bool root = false, std::string dir = ""  ) : 
      disc_(disc),
      weights_(weights),
      root_(root), 
      dir_(dir) {}
    virtual ~FreeJetPlotter() {}

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
      h_free_jet_size.SetDirectory(tdir);
      h_free_jet_pt.SetDirectory(tdir);
      h_free_jet_eta.SetDirectory(tdir);
      h_free_jet_csv.SetDirectory(tdir);
      h_free_jet_min_delta_phi_jet.SetDirectory(tdir); 
      h_free_jet_size.Sumw2();
      h_free_jet_pt.Sumw2();
      h_free_jet_eta.Sumw2();
      h_free_jet_csv.Sumw2();
      h_free_jet_min_delta_phi_jet.Sumw2(); 
      h_free_jet_size_unc_sq.SetDirectory(tdir);
      h_free_jet_pt_unc_sq.SetDirectory(tdir);
      h_free_jet_eta_unc_sq.SetDirectory(tdir);
      h_free_jet_csv_unc_sq.SetDirectory(tdir);
      h_free_jet_min_delta_phi_jet_unc_sq.SetDirectory(tdir); 
      h_free_jet_size_unc_sq.Sumw2();
      h_free_jet_pt_unc_sq.Sumw2();
      h_free_jet_eta_unc_sq.Sumw2();
      h_free_jet_csv_unc_sq.Sumw2();
      h_free_jet_min_delta_phi_jet_unc_sq.Sumw2(); 
   }

    virtual bool process( EventClass & ev ) {

      float w = 1.0;
      float w_unc_sq = 0.0;
      for ( const auto & weight :  weights_ ) {
        w*=ev.eventInfo_.getWeight(weight);
        if (ev.eventInfo_.hasWeight(weight + "_unc")) {
          auto w_unc = ev.eventInfo_.getWeight(weight + "_unc");
          w_unc_sq += w_unc*w_unc; 
        }
      }

      h_free_jet_size.Fill(ev.free_is_.size());
      for (const auto & i : ev.free_is_) {
        const auto & jet = ev.jets_.at(i);
        // for variables wrt rest of selected jets
        std::set<std::size_t> o_jets_is = {0,1,2,3};
        o_jets_is.erase(i);
        h_free_jet_pt.Fill(jet.pt(), w);
        h_free_jet_pt_unc_sq.Fill(jet.pt(), w_unc_sq);
        h_free_jet_eta.Fill(jet.eta(), w);
        h_free_jet_eta_unc_sq.Fill(jet.eta(), w_unc_sq);
        h_free_jet_csv.Fill(jet.getDiscriminator(disc_), w);
        h_free_jet_csv_unc_sq.Fill(jet.getDiscriminator(disc_), w_unc_sq);
        double min_delta_phi_jet = 4.0; // always smaller than pi 
        for (const auto & o_i : o_jets_is) {
          const auto & o_jet = ev.jets_.at(o_i);
          double delta_phi_jet = std::abs(ROOT::Math::VectorUtil::DeltaPhi(jet, o_jet));
          if (delta_phi_jet < min_delta_phi_jet) {
            min_delta_phi_jet = delta_phi_jet; 
          }
        }
        h_free_jet_min_delta_phi_jet.Fill(min_delta_phi_jet, w);
        h_free_jet_min_delta_phi_jet_unc_sq.Fill(min_delta_phi_jet, w_unc_sq);
      }

      return true;
    }

    virtual bool output( TFile * tfile) {

      return true;

    }

};
