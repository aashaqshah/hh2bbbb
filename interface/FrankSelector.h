
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


  FrankSelector(TTree * /*tree*/ =0, std::vector<std::string> hlt_bits = {}, bool isHH = true, bool isData = false,
      std::vector<std::string> hlt_bits_or = {}) :
    BaseSelector<EventClass>(0,hlt_bits, isHH, isData)
    {


      this->addOperator(new ThrustAxisFinder<EventClass>());
      this->addOperator(new HemisphereProducer<EventClass>());
      std::string tfile_name = "/lustre/cmswork/dcastrom/projects/hh/CMSSW_8_0_3_patch1/src/di_higgs/hh2bbbb/scripts/mixing/BTagCSV.root";
      TFile * tfile =  new TFile(tfile_name.c_str());
      TTree * hem_tree = dynamic_cast<TTree *>(tfile->Get("hem_tree"));
      this->addOperator(new HemisphereMixer<EventClass>(hem_tree));
      this->addOperator(new FrankWriter<EventClass>());

    }

  virtual ~FrankSelector() {}

};

#endif


