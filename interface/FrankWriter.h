
#pragma once

#include <algorithm>
#include <math.h>

#include "BaseOperator.h"
#include "BTagJetSelection.h"
#include "DiJetPairSelection.h"

#include "mut_framework/mut_dataformats/interface/EventInfo.h"
#include "mut_framework/mut_dataformats/interface/Jet.h"
#include "mut_framework/mut_dataformats/interface/MET.h"


template <class EventClass> class FrankWriter : public BaseOperator<EventClass> {

  public:
 
    // variables to save in branches
    mut::EventInfo * eventInfo = nullptr;
    std::vector<mut::Jet> mix_jets_;
    // a pointer to avoid undecalred label
    std::vector<mut::Jet> * mix_jets_ptr_;
    mut::MET * pfmet = nullptr;
    std::vector<mut::Candidate> * b_quarks_hs = nullptr;

    // hemisphere combinations to save
    std::size_t n_h_mix_;
    std::size_t n_h_skip_;

    // to order jets after mixing
    std::string disc_ = "CSV";
    std::size_t n_fix_jets_ = 4;

    bool root_;
    std::string dir_;


    TTree tree_{"mix_tree","Tree wth mixed events"};

     FrankWriter(std::size_t n_h_mix = 1, std::size_t n_h_skip = 1,
                bool root = false, std::string dir = "") :
      mix_jets_ptr_(&mix_jets_),  
      n_h_mix_(n_h_mix),
      n_h_skip_(n_h_skip), 
      root_(root),
      dir_(dir) {}
    virtual ~FrankWriter() {}

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
      tree_.Branch("eventInfo","mut::EventInfo",
                   &eventInfo, 64000, 1);
      tree_.Branch("pfjets","std::vector<mut::Jet>",
                   &mix_jets_ptr_, 64000, 1);
      tree_.Branch("pfmet","mut::MET",
                   &pfmet, 64000, 1);
      tree_.Branch("GenBQuarkFromH","std::vector<mut::Candidate>",
                   &b_quarks_hs, 64000, 1);


      tree_.SetDirectory(tdir);
      tree_.AutoSave();

   }


    virtual bool process( EventClass & ev ) {


      // most stuff from original event
      eventInfo = dynamic_cast<mut::EventInfo *>(&ev.eventInfo_);
      pfmet = dynamic_cast<mut::MET *>(&ev.met_);
      b_quarks_hs = dynamic_cast<std::vector<mut::Candidate> * >(&ev.b_quarks_hs_);

      const auto & bm_hems = ev.best_match_hems_;

      // clear jet collection
      mix_jets_.clear();

      // for each hemisphere i
      for (std::size_t h_i=n_h_skip_; h_i<(n_h_skip_+n_h_mix_); h_i++) {
        // for each hemisphere j
        for (std::size_t h_j=n_h_skip_; h_j<(n_h_skip_+n_h_mix_); h_j++) {

          // references for easy access
          const auto jets_i = bm_hems.at(0).at(h_i).jets_;
          const auto jets_j = bm_hems.at(1).at(h_j).jets_;
          mix_jets_.insert(mix_jets_.end(), jets_i.begin(), jets_i.end());
          mix_jets_.insert(mix_jets_.end(), jets_j.begin(), jets_j.end());


          // order by b-tagging disc
          order_jets_by_disc(mix_jets_, disc_);
          // pair with min mass
          dijet_pairing_simple(mix_jets_, n_fix_jets_);        

          // fill tree with combination
          tree_.Fill();
        }
      }



      return true;
    }

    virtual bool output( TFile * tfile) {

      return true;

    }

};
