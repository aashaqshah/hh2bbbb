
#pragma once

#include "BaseOperator.h"

template <class EventClass> class EventCounter : public BaseOperator<EventClass> {

  public:

    long n_sel_ev_; 

    EventCounter() :
      n_sel_ev_(0) {}
    virtual ~EventCounter() {}

    virtual bool process( EventClass & ev ) {
      n_sel_ev_++;
      return true;
    }

    virtual bool output ( std::ostream & os) {
      os << " n_sel_ev " << n_sel_ev_ << std::endl ;
      return true;
    }

};
