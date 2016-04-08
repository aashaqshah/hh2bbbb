
#include "di_higgs/hh2bbbb/interface/BaseSelector.h"
#include "di_higgs/hh2bbbb/interface/BasicSelector.h"
#include "di_higgs/hh2bbbb/interface/PairingSelector.h"
#include "di_higgs/hh2bbbb/interface/CombinationSelector.h"
#include "di_higgs/hh2bbbb/interface/MixingSelector.h"
#include "di_higgs/hh2bbbb/interface/SkimSelector.h"
#include "di_higgs/hh2bbbb/interface/Event.h"


namespace {
  struct hh4bbbb_ExtEvent {
    VHBBEvent dummya; 
    ExtEvent<VHBBEvent> dummya0; 
    BaseSelector<ExtEvent<VHBBEvent>> dummya1;
    BasicSelector<ExtEvent<VHBBEvent>> dummya2;
    PairingSelector<ExtEvent<VHBBEvent>> dummya3;
    CombinationSelector<ExtEvent<VHBBEvent>> dummya4;
    MixingSelector<ExtEvent<VHBBEvent>> dummya5;
    SkimSelector<ExtEvent<VHBBEvent>> dummya6;
    ThinEvent dummyb; 
    ExtEvent<ThinEvent> dummyb0; 
    BaseSelector<ExtEvent<ThinEvent>> dummyb1;
    BasicSelector<ExtEvent<ThinEvent>> dummyb2;
    PairingSelector<ExtEvent<ThinEvent>> dummyb3;
    CombinationSelector<ExtEvent<ThinEvent>> dummyb4;
    MixingSelector<ExtEvent<ThinEvent>> dummyb5;
    SkimSelector<ExtEvent<ThinEvent>> dummyb6;
 
  };
}
