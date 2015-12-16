
#pragma once
#include <TFile.h>

template <class EventClass> class BaseOperator {

  public:
    BaseOperator() {}
    virtual ~BaseOperator() {}
    

    virtual bool process( EventClass & ev ) {
      return true;
    }

    virtual bool output( TFile & tfile ) {
      return false;
    }   

    virtual bool output( std::ostream & os ) {
      return false;
    } 

};
