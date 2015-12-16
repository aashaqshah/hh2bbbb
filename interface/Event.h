
#pragma once

#include <TTreeReader.h>

#include "JetCollection.h"
#include "MET.h"
#include "GenParticleCollection.h"
#include "EventInfo.h"

class Event { 
  public:

    EventInfo eventInfo_;
    JetCollection jets_;
    MET met_;

    Event(TTreeReader & reader, std::vector<std::string> hlt_bits) :
     eventInfo_(reader, hlt_bits),  
   	 jets_(reader),
     met_(reader)
     {}                   

    ~Event() {};

    void update() {
      eventInfo_.update();
      jets_.update();
      met_.update();
    }

};

