
#pragma once

#include "BaseOperator.h"

template <class Operator, class EventClass> class NotOperator : public Operator {

  public:
    // inhertit constructor
    using Operator::Operator;
    virtual ~NotOperator() {}

    virtual bool process( EventClass & ev ) {
      return !Operator::process(ev);
    }

    virtual std::string get_name() {
      return "not_" + Operator::get_name();
    }

};
