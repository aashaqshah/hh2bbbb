
#pragma once

#include <algorithm>

#include "TH1.h"
#include "TH2.h"
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"

#include "BaseOperator.h"

template <class EventClass> class JetPlotter : public BaseOperator<EventClass> {

  public:

    typedef ROOT::Math::PtEtaPhiEVector PtEtaPhiEVector;

    std::vector<std::string> weights_;
    bool root_;
    std::string dir_;

    TH1D jet_multiplicity {"jet_multiplicity", "", 10, 0., 10.};
    TH1D jet_CSVM_multiplicity {"jet_CSVM_multiplicity", "", 10, 0., 10};
    TH2D h_n_mass_bcount {"h_n_mass_bcount","", 300, 0., 1200., 10 , 0., 10.};  


    JetPlotter( const std::vector<std::string> & weights = {},
                bool root = false, std::string dir = ""  ) : 
      weights_(weights),
      root_(root), 
      dir_(dir) {}
    virtual ~JetPlotter() {}

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
      jet_multiplicity.SetDirectory(tdir);
      jet_CSVM_multiplicity.SetDirectory(tdir);
      h_n_mass_bcount.SetDirectory(tdir);
      jet_multiplicity.Sumw2();
      jet_CSVM_multiplicity.Sumw2();
      h_n_mass_bcount.Sumw2();
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

      jet_multiplicity.Fill(ev.jets_.size(), w);
      std::size_t n_tag_jets = 0;
      for (const auto & i : ev.jets_) {
        if ( i.getDiscriminator("CSV") > 0.800) n_tag_jets++; 
      }
      jet_CSVM_multiplicity.Fill(n_tag_jets, w);
      
      // number of jets to consider 
      std::size_t n_use= 4;
      std::size_t n_use_bflav = 0;
      // to keep sum of n-vectors
      PtEtaPhiEVector jet_sum_vector{ev.jets_.at(0)};
      if (ev.jets_.at(0).getHadronFlavour() == 5) n_use_bflav++; 
      for (std::size_t i=1; i < n_use; i++ ) {
        jet_sum_vector += ev.jets_.at(i);
        if (ev.jets_.at(i).getHadronFlavour() == 5) n_use_bflav++; 
      }
      h_n_mass_bcount.Fill(jet_sum_vector.M(), n_use_bflav);


      return true;
    }

    virtual bool output( TFile * tfile) {

      return true;

    }

};
