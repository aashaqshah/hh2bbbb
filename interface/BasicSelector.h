
#ifndef BasicSelector_h
#define BasicSelector_h

#include <memory>

#include "BaseSelector.h"
#include "EventCounter.h"
#include "TriggerFilter.h"
#include "JetSelection.h"
#include "BTagJetSelection.h"
#include "DiJetPairSelection.h"
#include "GenJetMatcher.h"
#include "DiHiggsPlotter.h"



template <class EventClass> class BasicSelector : public BaseSelector<EventClass> {

  public:

  BasicSelector(TTree * /*tree*/ =0, std::vector<std::string> hlt_bits = {}, bool isHH = false,
      std::vector<std::string> hlt_bits_or = {}) :
    BaseSelector<EventClass>(0,hlt_bits, isHH)
    {
      std::size_t n_CSV = 3;
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new TriggerFilter<EventClass>(hlt_bits_or));
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new JetSelection<EventClass>(10., 20., 4));
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new BTagJetSelection<EventClass>("CSV", 0.890, n_CSV));
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new BetterDiJetPairSelection<EventClass>("CSV", 0.890, n_CSV));
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new DiHiggsPlotter<EventClass>());
      if (isHH) {
        this->addOperator(new GenJetMatcher<EventClass>());
        this->addOperator(new HHJetsMatched<EventClass>());
        this->addOperator(new EventCounter<EventClass>());
        this->addOperator(new DiHiggsPlotter<EventClass>());
      }


    }

  virtual ~BasicSelector() {}

};

#endif


