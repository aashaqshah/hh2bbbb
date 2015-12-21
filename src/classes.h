
#include "di-higgs/hh2bbbb/interface/BaseSelector.h"
#include "di-higgs/hh2bbbb/interface/Event.h"
#include "di-higgs/hh2bbbb/interface/ExampleOperator.h"
#include "di-higgs/hh2bbbb/interface/TriggerFilter.h"
#include "di-higgs/hh2bbbb/interface/EventCounter.h"
#include "di-higgs/hh2bbbb/interface/JetSelection.h"
#include "di-higgs/hh2bbbb/interface/BTagJetSelection.h"
#include "di-higgs/hh2bbbb/interface/DiJetPairSelection.h"
#include "di-higgs/hh2bbbb/interface/DiHiggsPlotter.h"
#include "di-higgs/hh2bbbb/interface/GenJetMatcher.h"


namespace {
  struct hh4bbbb {
    Event dummy0; 
    BaseSelector<Event> dummy1;
    ExampleOperator<Event> dummy2;
    TriggerFilter<Event> dummy3;
    EventCounter<Event> dummy4;
    JetSelection<Event> dummy5;
    BTagJetSelection<Event> dummy6;
    ExtEvent dummy8; 
    BaseSelector<ExtEvent> dummy9;
    ExampleOperator<ExtEvent> dummy10;
    TriggerFilter<ExtEvent> dummy11;
    EventCounter<Event> dummy12;
    JetSelection<ExtEvent> dummy13;
    BTagJetSelection<ExtEvent> dummy14;
    DiJetPairSelection<ExtEvent> dummy15;
    DiHiggsPlotter<ExtEvent> dummy16;
    GenJetMatcher<ExtEvent> dummy17;
    HHJetsMatched<ExtEvent> dummy18;
  };
}
