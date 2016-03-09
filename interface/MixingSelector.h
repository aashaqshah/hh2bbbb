
#ifndef MixingSelector_h
#define MixingSelector_h

#include <memory>

#include "BaseSelector.h"
#include "EventCounter.h"
#include "TriggerFilter.h"
#include "JetSelection.h"
#include "MixingWriter.h"


template <class EventClass> class MixingSelector : public BaseSelector<EventClass> {

  public:


  MixingSelector(TTree * /*tree*/ =0, std::vector<std::string> hlt_bits = {}, bool isHH = true,
      std::vector<std::string> hlt_bits_or = {}) :
    BaseSelector<EventClass>(0,hlt_bits, isHH)
    {
      std::size_t n_CSV = 2;
      this->addOperator(new EventCounter<EventClass>());
//      this->addOperator(new TriggerFilter<EventClass>(hlt_bits_or));
//      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new JetSelection<EventClass>(2.5, 20., 4));
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new BTagJetSelection<EventClass>("CSV", 0.890, n_CSV));
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new MixingWriter<EventClass>(true));
    }

  virtual ~MixingSelector() {}

};

#endif


