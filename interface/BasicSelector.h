
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
#include "FreeJetPlotter.h"
#include "NotOperator.h"
#include "FreeJetTagPdf.h"
#include "FreeJetTagged.h"
#include "FreeJetWeight.h"


template <class EventClass> class BasicSelector : public BaseSelector<EventClass> {

  public:


  BasicSelector(TTree * /*tree*/ =0, std::vector<std::string> hlt_bits = {}, bool isHH = false,
      std::vector<std::string> hlt_bits_or = {}, bool inEllipse = true, bool freeJetTagged = true,
      TH2D * ratio = nullptr) :
    BaseSelector<EventClass>(0,hlt_bits, isHH)
    {
      std::size_t n_CSV = 3;
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new TriggerFilter<EventClass>(hlt_bits_or));
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new JetSelection<EventClass>(2.5, 20., 4));
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new BTagJetSelection<EventClass>("CSV", 0.890, n_CSV));
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new BetterDiJetPairSelection<EventClass>("CSV", 0.890, n_CSV));
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new DiHiggsPlotter<EventClass>());
      this->addOperator(new FreeJetPlotter<EventClass>("CSV"));
      if (inEllipse) {
        this->addOperator(new WithinEllipse<EventClass>());
      } else { 
        this->addOperator(new NotOperator<WithinEllipse<EventClass>,EventClass>());
      }
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new DiHiggsPlotter<EventClass>());
      this->addOperator(new FreeJetPlotter<EventClass>("CSV"));
      this->addOperator(new FreeJetTagPdf<EventClass>("CSV",0.605));
      if (freeJetTagged) {
        this->addOperator(new FreeJetTagged<EventClass>("CSV",0.605));
        this->addOperator(new DiHiggsPlotter<EventClass>({}, true));
        this->addOperator(new FreeJetPlotter<EventClass>("CSV", {}, true));
      } else {
        this->addOperator(new NotOperator<FreeJetTagged<EventClass>, EventClass>("CSV",0.605));
        this->addOperator(new DiHiggsPlotter<EventClass>({}, true));
        this->addOperator(new FreeJetPlotter<EventClass>("CSV", {}, true));
        if ( ratio != nullptr) {
          this->addOperator(new FreeJetWeight2D<EventClass>("free_jet_weight", ratio));
          this->addOperator(new DiHiggsPlotter<EventClass>( {"free_jet_weight"}, true, "weighted"));
          this->addOperator(new FreeJetPlotter<EventClass>("CSV", {"free_jet_weight"}, true, "weighted"));
        } 
      }
      if (isHH) {
        this->addOperator(new GenJetMatcher<EventClass>());
        this->addOperator(new HHJetsMatched<EventClass>());
        this->addOperator(new EventCounter<EventClass>());
        this->addOperator(new DiHiggsPlotter<EventClass>());
        this->addOperator(new FreeJetPlotter<EventClass>("CSV"));
      }


    }

  virtual ~BasicSelector() {}

};

#endif


