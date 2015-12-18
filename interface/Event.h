
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

    Event() {}

    Event(TTreeReader & reader, std::vector<std::string> hlt_bits) :
     eventInfo_(reader, hlt_bits),  
   	 jets_(reader),
     met_(reader)
     {}                   

    virtual ~Event() {};

    virtual void update() {
      eventInfo_.update();
      jets_.update();
      met_.update();

    }

};

typedef std::vector<mut::Candidate> CandidateCollection;

class ExtEvent : public Event {
  public:

  CandidateCollection dijets_; 

  // inherit constructors
  using Event::Event;
  virtual ~ExtEvent() {}

  virtual void update() {
    Event::update();
    dijets_.clear();
  }

};

