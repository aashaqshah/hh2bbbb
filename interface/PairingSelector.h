
#ifndef PairingSelector_h
#define PairingSelector_h

#include <memory>

#include "BaseSelector.h"
#include "EventCounter.h"
#include "TriggerFilter.h"
#include "JetSelection.h"
#include "BTagJetSelection.h"
#include "DiJetPairSelection.h"
#include "MVAPairSelection.h"
#include "GenJetMatcher.h"
#include "DiHiggsPlotter.h"
#include "FreeJetPlotter.h"
#include "NotOperator.h"
#include "FreeJetTagPdf.h"
#include "FreeJetTagged.h"
#include "FreeJetWeight.h"


template <class EventClass> class PairingSelector : public BaseSelector<EventClass> {

  public:


  PairingSelector(TTree * /*tree*/ =0, std::vector<std::string> hlt_bits = {}, bool isHH = false,
      std::vector<std::string> hlt_bits_or = {}, std::size_t pair_method = 2) :
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
      if (pair_method == 0) {
        this->addOperator(new DiJetPairSelection<EventClass>(4));
      } else if (pair_method == 1) {
        this->addOperator(new DiJetPairSelection<EventClass>(3));
      } else if (pair_method == 2) {
        std::string base_path = "/lustre/cmswork/hh/combinatorics/dataset_06022016/no_spectators/";
        std::vector<std::string> mva_xmls;
        for (std::size_t i=4; i<9; i++) 
          mva_xmls.emplace_back(base_path + std::to_string(i) +
          "jets_5var_block_massDiffTMVAClassification_kFisher.weights.xml");
        this->addOperator(new MVAPairSelection<EventClass>(mva_xmls));
      } else { 
        this->addOperator(new BetterDiJetPairSelection<EventClass>("CSV", 0.890, n_CSV));
      }
      this->addOperator(new EventCounter<EventClass>());
      this->addOperator(new DiHiggsPlotter<EventClass>({}, true));
      this->addOperator(new FreeJetPlotter<EventClass>("CSV", {}, true));
      if (isHH) {
        this->addOperator(new GenJetMatcher<EventClass>());
        this->addOperator(new HHJetsMatched<EventClass>());
        this->addOperator(new EventCounter<EventClass>());
        this->addOperator(new DiHiggsPlotter<EventClass>({}, true, "matched"));
        this->addOperator(new FreeJetPlotter<EventClass>("CSV", {}, true, "matched"));
      }

    }

  virtual ~PairingSelector() {}

};

#endif


