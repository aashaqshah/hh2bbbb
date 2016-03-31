
#pragma once

#include <algorithm>

#include "TH1.h"
#include "TH2.h"
#include "Math/GenVector/VectorUtil.h"

#include "BaseOperator.h"

template <class EventClass> class JetPlotter : public BaseOperator<EventClass> {

  public:

    std::vector<std::string> weights_;
    bool root_;
    std::string dir_;

    TH1D jet_multiplicity {"jet_multiplicity", "", 10, 0., 10.};
    TH1D jet_CSVM_multiplicity {"jet_CSVM_multiplicity", "", 10, 0., 10};


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
      jet_multiplicity.Sumw2();
      jet_CSVM_multiplicity.Sumw2();
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
        if ( i.getDiscriminator("CSV") > 0.890) n_tag_jets++; 
      }
      jet_CSVM_multiplicity.Fill(n_tag_jets, w);

      return true;
    }

    virtual bool output( TFile * tfile) {

      return true;

    }

};
