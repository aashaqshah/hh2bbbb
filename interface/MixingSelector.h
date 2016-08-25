
#ifndef MixingSelector_h
#define MixingSelector_h

#include <memory>

#include "BaseSelector.h"
#include "EventCounter.h"
#include "TriggerFilter.h"
#include "JetSelection.h"
#include "FullWriter.h"
#include "NotOperator.h"
#include "ThrustAxisFinder.h"
#include "HemisphereProducer.h"
#include "HemisphereWriter.h"


template <class EventClass> class MixingSelector : public BaseSelector<EventClass> {

  public:


  MixingSelector(TTree * /*tree*/ =0, std::vector<std::string> hlt_bits = {}, bool isHH = true, bool isData = false,
      std::vector<std::string> hlt_bits_or = {}, std::vector<std::string> corr_names = {}, double corr_factor = 1) :
    BaseSelector<EventClass>(0,hlt_bits, isHH, isData, corr_names, corr_factor)
    {
      std::size_t n_CSV = 3;
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new TriggerFilter<EventClass>(hlt_bits_or));
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new JetSelection<EventClass>(2.5, 30., 4));
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new BTagJetSelection<EventClass>("CSV", 0.800, n_CSV));
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new DiJetPairSelection<EventClass>(4));
      this->addOperator(new EventCounter<EventClass>());
//      this->addOperator(new NotOperator<WithinRectangle<EventClass>, EventClass>());
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new DiHiggsPlotter<EventClass>({}, true));
      this->addOperator(new ThrustAxisFinder<EventClass>());
      this->addOperator(new HemisphereProducer<EventClass>());
      this->addOperator(new FullWriter<EventClass>(true));
      this->addOperator(new HemisphereWriter<EventClass>(true));

    }

  virtual ~MixingSelector() {}

};

#endif


