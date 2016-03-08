
#ifndef CombinationSelector_h
#define CombinationSelector_h

#include <memory>

#include "BaseSelector.h"
#include "EventCounter.h"
#include "TriggerFilter.h"
#include "JetSelection.h"
#include "GenJetMatcher.h"
#include "CombinationWriter.h"


template <class EventClass> class CombinationSelector : public BaseSelector<EventClass> {

  public:


  CombinationSelector(TTree * /*tree*/ =0, std::vector<std::string> hlt_bits = {}, bool isHH = true,
      std::vector<std::string> hlt_bits_or = {}) :
    BaseSelector<EventClass>(0,hlt_bits, isHH)
    {
      std::size_t n_CSV = 0;
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new TriggerFilter<EventClass>(hlt_bits_or));
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new JetSelection<EventClass>(2.5, 20., 4));
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new BTagJetSelection<EventClass>("CSV", 0.890, n_CSV));
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new GenJetMatcher<EventClass>());
      this->addOperator(new AllGenBMatched<EventClass>());
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new CombinationWriter<EventClass>(true));
      this->addOperator(new EventCounter<EventClass>());


    }

  virtual ~CombinationSelector() {}

};

#endif


