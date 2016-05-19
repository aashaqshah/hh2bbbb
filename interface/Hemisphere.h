
#pragma once

#include "mut_framework/mut_dataformats/interface/EventInfo.h"
#include "mut_framework/mut_dataformats/interface/Jet.h"

class Hemisphere { 
  public:

    // read from TTree
    mut::EventInfo eventInfo_;
    std::vector<mut::Jet> jets_;

    Hemisphere() {}
    Hemisphere(const mut::EventInfo & eventInfo) : 
      eventInfo_(eventInfo) {}

    virtual ~Hemisphere() {}                   


};

