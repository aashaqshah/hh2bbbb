
#pragma once

#include <TTreeReader.h>

#include "JetCollection.h"
#include "MET.h"
#include "GenParticleCollection.h"
#include "EventInfo.h"

class Event { 
  public:

    // read from TTree
    EventInfo eventInfo_;
    JetCollection jets_;
    MET met_;
    GenParticleCollection b_quarks_hs_;

    Event() {}

    Event(TTreeReader & reader, std::vector<std::string> hlt_bits, bool isHH = false) :
     eventInfo_(reader, hlt_bits),  
   	 jets_(reader),
     met_(reader),
     b_quarks_hs_(reader, "GenBQuarkFromH", isHH)
     {}                   

    virtual ~Event() {};

    virtual void update() {
      eventInfo_.update();
      jets_.update();
      met_.update();
      b_quarks_hs_.update();

    }

};

typedef std::vector<mut::Candidate> CandidateCollection;

class ExtEvent : public Event {
  public:

  CandidateCollection dijets_; 
  std::vector<std::set<std::size_t>> reco_jet_matchs_; 
  // indexes of jet chosen by min mass diff 
  std::vector<std::size_t> free_is_;

  // inherit constructors
  using Event::Event;
  virtual ~ExtEvent() {}

  virtual void update() {
    Event::update();
    dijets_.clear();
    reco_jet_matchs_.clear();
    free_is_.clear();
  }

};

