
#pragma once

#include "BaseOperator.h"

template <class EventClass> class TriggerFilter : public BaseOperator<EventClass> {

  public:

    std::vector<std::string> or_paths_; 

    TriggerFilter(std::vector<std::string> or_paths) :
      or_paths_(or_paths) {}
    virtual ~TriggerFilter() {}

    virtual bool process( EventClass & ev ) {
      for (const auto & or_path : or_paths_) {
        if (ev.eventInfo_.getFilter(or_path)) return true;
      }
      return false;
    }

};
