
#pragma once

#include <TTreeReader.h>

#include "JetCollection.h"
#include "MET.h"
#include "GenParticleCollection.h"
#include "EventInfo.h"
#include "Hemisphere.h"

#include "mut_framework/mut_dataformats/interface/Reader.h"

class VHBBEvent { 
  public:

    // read from TTree
    EventInfo eventInfo_;
    JetCollection jets_;
    MET met_;
    GenParticleCollection b_quarks_hs_;

    VHBBEvent() {}

    VHBBEvent(TTreeReader & reader, std::vector<std::string> hlt_bits, bool isHH = false, bool isData = false) :
     eventInfo_(reader, hlt_bits),  
   	 jets_(reader, isData),
     met_(reader),
     b_quarks_hs_(reader, "GenBQuarkFromH", isHH)
     {}                   

    virtual ~VHBBEvent() {};

    virtual void update() {
      eventInfo_.update();
      jets_.update();
      met_.update();
      b_quarks_hs_.update();

    }

};

class ThinEvent { 
  public:

    // read from TTree
    mut::Reader<mut::EventInfo> eventInfo_;
    mut::Reader<std::vector<mut::Jet>> jets_;
    mut::Reader<mut::MET> met_;
    mut::Reader<std::vector<mut::Candidate>> b_quarks_hs_;

    ThinEvent() {}

    ThinEvent(TTreeReader & reader, std::vector<std::string> hlt_bits, bool isHH = false, bool isData = false) :
     eventInfo_(reader, "eventInfo" ),  
   	 jets_(reader, "pfjets"),
     met_(reader, "pfmet"),
     b_quarks_hs_(reader, "GenBQuarkFromH")
     {}                   

    virtual ~ThinEvent() {};

    virtual void update() {
      eventInfo_.update();
      jets_.update();
      met_.update();
      b_quarks_hs_.update();

    }

};

typedef std::vector<mut::Candidate> CandidateCollection;

template <class EventBase> class ExtEvent : public EventBase {
  public:

  CandidateCollection dijets_; 
  std::vector<std::set<std::size_t>> reco_jet_matchs_; 
  // indexes of jet chosen by min mass diff 
  std::vector<std::size_t> free_is_;
  // tranverse thrust phi
  double thrust_phi_ = -1.;
  // hemispheres (rotated and pz positive)
  std::vector<Hemisphere> hems_; 

  // inherit constructors
  using EventBase::EventBase;
  virtual ~ExtEvent() {}

  virtual void update() {
    EventBase::update();
    dijets_.clear();
    reco_jet_matchs_.clear();
    free_is_.clear();
    thrust_phi_ = -1.;
    hems_.clear();
  }

};

