
#pragma once

#include <algorithm>
#include <math.h>

#include "BaseOperator.h"

#include "mut_framework/mut_dataformats/interface/Jet.h"
#include "mut_framework/mut_dataformats/interface/Candidate.h"

template <class EventClass> class MixingWriter : public BaseOperator<EventClass> {

  public:
 
    typedef ROOT::Math::PtEtaPhiEVector PtEtaPhiEVector;
    typedef std::vector<std::size_t>::iterator It;
    bool root_;
    std::string dir_;
    // variables to save in branches
    std::size_t event;
    std::vector<PtEtaPhiEVector> * jets_PtEtaPhiEVector = nullptr;
    std::vector<float> * jets_CSV = nullptr;
    PtEtaPhiEVector * met_PtEtaPhiEVector = nullptr;

    TTree mix_tree_{"mix_tree","Tree with jets and met for event mixing studies"};

     MixingWriter(bool root = false, std::string dir = "") :
      root_(root),
      dir_(dir) {}
    virtual ~MixingWriter() {}

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
      mix_tree_.Branch("event",&event, "event/I");
      mix_tree_.Branch("jets_PtEtaPhiEVector","std::vector<PtEtaPhiEVector>",
                        &jets_PtEtaPhiEVector, 64000, 1);
      mix_tree_.Branch("jets_CSV","std::vector<float>",
                        &jets_CSV, 64000, 1);
      mix_tree_.Branch("met_PtEtaPhiEVector","PtEtaPhiEVector",
                        &met_PtEtaPhiEVector, 64000, 1);


      mix_tree_.SetDirectory(tdir);
      mix_tree_.AutoSave();

   }


    virtual bool process( EventClass & ev ) {


      // to fill tree
      jets_PtEtaPhiEVector = new std::vector<PtEtaPhiEVector>();
      jets_CSV = new std::vector<float>();

      event = ev.eventInfo_.getEvent();

      // all jets
      for (std::size_t i = 0; i < ev.jets_.size(); i++ ) {     
        const auto & jet = ev.jets_.at(i);
        jets_PtEtaPhiEVector->emplace_back(jet); 
        jets_CSV->emplace_back(jet.getDiscriminator("CSV"));
      }

      // met 
      met_PtEtaPhiEVector = new PtEtaPhiEVector(ev.met_);
     

      mix_tree_.Fill();

      delete jets_PtEtaPhiEVector;
      delete jets_CSV;
      delete met_PtEtaPhiEVector;

      return true;
    }

    virtual bool output( TFile * tfile) {

      return true;

    }

};
