
#pragma once

#include <algorithm>

#include "TH1.h"
#include "TH2.h"

#include "BaseOperator.h"

template <class EventClass> class DiHiggsPlotter : public BaseOperator<EventClass> {

  public:
 
    std::vector<std::string> weights_;
    bool root_;
    std::string dir_;

    TH1D h_H0_mass {"h_H0_mass", "", 300, 0., 900.};
    TH1D h_H0_mass_unc_sq {"h_H0_mass_unc_sq", "", 300, 0., 900.};
    TH1D h_H1_mass {"h_H1_mass", "", 300, 0., 900.};
    TH1D h_H1_mass_unc_sq {"h_H1_mass_unc_sq", "", 300, 0., 900.};
    TH2D h_H0_H1_mass {"h_H0_H1_mass","", 300, 0., 600., 300, 0., 600.};  
    TH2D h_H0_H1_mass_unc_sq {"h_H0_H1_mass_unc_sq","", 300, 0., 600., 300, 0., 600.};  
    TH1D h_H0H1_mass {"h_H0H0_mass", "", 300, 0., 900.};
    TH1D h_H0H1_mass_unc_sq {"h_H0H0_mass_unc_sq", "", 300, 0., 900.};

     DiHiggsPlotter(const std::vector<std::string> & weights = {}, bool root = false, std::string dir = ""  ) :
      weights_(weights),
      root_(root),
      dir_(dir) {}
    virtual ~DiHiggsPlotter() {}

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
      h_H0_mass.SetDirectory(tdir);
      h_H1_mass.SetDirectory(tdir);
      h_H0_H1_mass.SetDirectory(tdir);
      h_H0H1_mass.SetDirectory(tdir);
      h_H0_mass_unc_sq.SetDirectory(tdir);
      h_H1_mass_unc_sq.SetDirectory(tdir);
      h_H0_H1_mass_unc_sq.SetDirectory(tdir);
      h_H0H1_mass_unc_sq.SetDirectory(tdir);
      h_H0_mass.Sumw2();
      h_H1_mass.Sumw2();
      h_H0_H1_mass.Sumw2();
      h_H0H1_mass.Sumw2();
      h_H0_mass_unc_sq.Sumw2();
      h_H1_mass_unc_sq.Sumw2();
      h_H0_H1_mass_unc_sq.Sumw2();
      h_H0H1_mass_unc_sq.Sumw2();
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

      h_H0_mass.Fill(ev.dijets_.at(0).mass(), w);
      h_H1_mass.Fill(ev.dijets_.at(1).mass(), w);
      h_H0_H1_mass.Fill(ev.dijets_.at(0).mass(), ev.dijets_.at(1).mass(), w);
      h_H0H1_mass.Fill((ev.dijets_.at(0)+ev.dijets_.at(1)).mass(), w);

      h_H0_mass_unc_sq.Fill(ev.dijets_.at(0).mass(), w_unc_sq);
      h_H1_mass_unc_sq.Fill(ev.dijets_.at(1).mass(), w_unc_sq);
      h_H0_H1_mass_unc_sq.Fill(ev.dijets_.at(0).mass(), ev.dijets_.at(1).mass(), w_unc_sq);
      h_H0H1_mass_unc_sq.Fill((ev.dijets_.at(0)+ev.dijets_.at(1)).mass(), w_unc_sq);

      return true;
    }

    virtual bool output( TFile * tfile) {

      return true;

    }

};
