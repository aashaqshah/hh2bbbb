
#ifndef FrankSelector_h
#define FrankSelector_h

#include <memory>

#include "BaseSelector.h"
#include "EventCounter.h"
#include "FullWriter.h"
#include "ThrustAxisFinder.h"
#include "HemisphereMixer.h"
#include "FrankWriter.h"


template <class EventClass> class FrankSelector : public BaseSelector<EventClass> {

  public:


  FrankSelector(TTree * /*tree*/ =0, std::vector<std::string> hlt_bits = {},
                bool isHH = true, bool isData = false,
                std::vector<std::string> hlt_bits_or = {},
                TChain * tc_hm = nullptr) :
    BaseSelector<EventClass>(0,hlt_bits, isHH, isData)
    {

      this->addOperator(new FullWriter<EventClass>(true));
      this->addOperator(new ThrustAxisFinder<EventClass>());
      this->addOperator(new HemisphereProducer<EventClass>());
      this->addOperator(new HemisphereMixer<EventClass>(tc_hm));
      this->addOperator(new FrankWriter<EventClass>(true));

    }

  virtual ~FrankSelector() {}

};

#endif


