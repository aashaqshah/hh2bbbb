
#include "di_higgs/hh2bbbb/interface/BaseSelector.h"
#include "di_higgs/hh2bbbb/interface/BasicSelector.h"
#include "di_higgs/hh2bbbb/interface/PairingSelector.h"
#include "di_higgs/hh2bbbb/interface/CombinationSelector.h"
#include "di_higgs/hh2bbbb/interface/MixingSelector.h"
#include "di_higgs/hh2bbbb/interface/SkimSelector.h"
#include "di_higgs/hh2bbbb/interface/Event.h"


namespace {
  struct hh4bbbb_ExtEvent {
    ExtEvent dummy0; 
    BaseSelector<ExtEvent> dummy1;
    BasicSelector<ExtEvent> dummy2;
    PairingSelector<ExtEvent> dummy3;
    CombinationSelector<ExtEvent> dummy4;
    MixingSelector<ExtEvent> dummy5;
    SkimSelector<ExtEvent> dummy6;
  };
}
