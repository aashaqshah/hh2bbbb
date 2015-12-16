#!/usr/bin/env python 

import ROOT
from ROOT import TChain, Event, BaseSelector, vector

hlt_paths = ["HLT_BIT_HLT_QuadJet45_TripleBTagCSV0p67_v",
                  "HLT_BIT_HLT_QuadJet45_DoubleBTagCSV0p67_v",
                  "HLT_BIT_HLT_DoubleJet90_Double30_TripleBTagCSV0p67_v",
                  "HLT_BIT_HLT_DoubleJet90_Double30_DoubleBTagCSV0p67_v",
                  "HLT_HH4bAll"]
hlt_paths_v = vector("string")()
for hlt_path in hlt_paths: hlt_paths_v.push_back(hlt_path)


selector = BaseSelector(Event)(0, hlt_paths_v)
file_n = "/lustre/cmswork/dallosso/hh2bbbb/non-resonant/analysis/13TeV/hh4bNores/data/Step0/tree_Step0_V15_BTagCSVRun2015C-D.root"
tchain = TChain("tree")
tchain.Add(file_n)
tchain.Process(selector, "", 100000)

