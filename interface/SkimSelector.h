
#ifndef SkimSelector_h
#define SkimSelector_h

#include <memory>

#include "BaseSelector.h"
#include "EventCounter.h"
#include "TriggerFilter.h"
#include "JetSelection.h"
#include "FullWriter.h"
#include "FreeJetTagged.h"


template <class EventClass> class SkimSelector : public BaseSelector<EventClass> {

  public:


  SkimSelector(TTree * /*tree*/ =0, std::vector<std::string> hlt_bits = {}, bool isHH = true,
    std::vector<std::string> hlt_bits_or = {}) :
    BaseSelector<EventClass>(0,hlt_bits, isHH)
    {
      std::size_t n_CSV = 3;
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new TriggerFilter<EventClass>(hlt_bits_or));
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new JetSelection<EventClass>(2.5, 20., 4));
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new BTagJetSelection<EventClass>("CSV", 0.800, n_CSV));
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new DiJetPairSelection<EventClass>(4));
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new JetPlotter<EventClass>({}, false)); 
      this->addOperator(new FullWriter<EventClass>(true));
      this->addOperator(new FreeJetTagged<EventClass>("CSV",0.460));
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new JetPlotter<EventClass>({}, true)); 
    }

    virtual ~SkimSelector() {}

};

#endif


