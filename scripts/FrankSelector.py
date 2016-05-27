#!/usr/bin/env python 

import ROOT
from ROOT import ThinEvent, ExtEvent
from ROOT import TChain, FrankSelector,  vector
from ROOT import TH1, TFile
from ROOT import gInterpreter

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

base_dir = "mixed/"
thinEventPath = "mixing/"
name = "BTagCSV"
isHH = False
isData = True 
mult = 5


tc_hm = TChain("hem_tree")
tc_hm.Add(thinEventPath+name+".root")
selector = FrankSelector(ExtEvent(ThinEvent))(0, hlt_paths_v, isHH,
                                              isData, hlt_paths_or_v, tc_hm, mult)

tchain = TChain("tree")
n_added = tchain.Add(thinEventPath+name+".root")
tchain.Process(selector, "ofile="+base_dir+name+".root", max_events)

