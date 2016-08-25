#!/usr/bin/env python 

import ROOT
from ROOT import ThinEvent, ExtEvent
from ROOT import TChain, FrankSelector,  vector
from ROOT import TH1, TFile
from ROOT import TEventList 

from di_higgs.hh2bbbb.samples_25ns import mc_samples


max_events = 100000000

TH1.AddDirectory(False)

hlt_paths = ["HLT_BIT_HLT_QuadJet45_TripleBTagCSV0p67_v",
             "HLT_BIT_HLT_QuadJet45_DoubleBTagCSV0p67_v",
             "HLT_BIT_HLT_DoubleJet90_Double30_TripleBTagCSV0p67_v",
             "HLT_BIT_HLT_DoubleJet90_Double30_DoubleBTagCSV0p67_v",
             "HLT_HH4bAll"]

hlt_paths_v = vector("string")()
for hlt_path in hlt_paths: hlt_paths_v.push_back(hlt_path)

hlt_paths_or = hlt_paths[0:1] +  hlt_paths[2:3] 
hlt_paths_or_v = vector("string")()
for hlt_path in hlt_paths_or: hlt_paths_or_v.push_back(hlt_path)

base_dir = "mixed_pt30_3CSVM/"
thinEventPath = "mixing_pt30_3CSVM/"
name = "BTagCSV"
isHH = False
isData = True 
mult = 5


# process 4+ events
tc_hm = TChain("hem_tree")
tc_hm.Add(thinEventPath+name+".root")
tchain = TChain("tree")
tchain.Add(thinEventPath+name+".root")

el = TEventList("el","el")
tchain.Draw(">>el","(eventInfo.getFilterC(\"json\")==1) && (Sum$(pfjets.getDiscriminatorC(\"CSV\") > 0.8) > 3)")
tc_hm.SetEventList(el)
tchain.SetEventList(el)
selector = FrankSelector(ExtEvent(ThinEvent))(0, hlt_paths_v, isHH,
                                              isData, hlt_paths_or_v, tc_hm, mult)
tchain.Process(selector, "ofile="+base_dir+name+"_4+CSVv2M.root", max_events)

# process 3 tag events
tc_hm_3 = TChain("hem_tree")
tc_hm_3.Add(thinEventPath+name+".root")
tchain_3 = TChain("tree")
tchain_3.Add(thinEventPath+name+".root")

el_3 = TEventList("el","el")
tchain_3.Draw(">>el","(eventInfo.getFilterC(\"json\")==1) && (Sum$(pfjets.getDiscriminatorC(\"CSV\") > 0.8) == 3)" )

tc_hm_3.SetEventList(el)
tchain_3.SetEventList(el)
selector_3 = FrankSelector(ExtEvent(ThinEvent))(0, hlt_paths_v, isHH,
                                              isData, hlt_paths_or_v, tc_hm, mult)
tchain_3.Process(selector_3, "ofile="+base_dir+name+"_3CSVv2M.root", max_events)
