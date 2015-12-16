
#pragma once

#include "BaseOperator.h"

template <class EventClass> class ExampleOperator : public BaseOperator<EventClass> {

  public:
    ExampleOperator() {}
    virtual ~ExampleOperator() {}

    virtual bool process( EventClass & ev ) {
      std::cout << ev.jets_.size() << std::endl;
      return true;
    }

};
