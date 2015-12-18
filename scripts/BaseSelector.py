#!/usr/bin/env python 

import ROOT
from ROOT import ExtEvent as Event
from ROOT import TChain, BaseSelector,  vector
from ROOT import EventCounter, TriggerFilter, JetSelection, BTagJetSelection 
from ROOT import DiJetPairSelection, DiHiggsPlotter 

hlt_paths_data = ["HLT_BIT_HLT_QuadJet45_TripleBTagCSV0p67_v",
                  "HLT_BIT_HLT_QuadJet45_DoubleBTagCSV0p67_v",
                  "HLT_BIT_HLT_DoubleJet90_Double30_TripleBTagCSV0p67_v",
                  "HLT_BIT_HLT_DoubleJet90_Double30_DoubleBTagCSV0p67_v",
                  "HLT_HH4bAll"]

hlt_paths_mc = ["HLT_BIT_HLT_QuadJet45_TripleCSV0p5_v",
                "HLT_BIT_HLT_QuadJet45_DoubleCSV0p5_v",
                "HLT_BIT_HLT_DoubleJet90_Double30_TripleCSV0p5_v",
                "HLT_BIT_HLT_DoubleJet90_Double30_DoubleCSV0p5_v",
                "HLT_HH4bAll"]

hlt_paths = hlt_paths_mc
hlt_paths_v = vector("string")()
for hlt_path in hlt_paths: hlt_paths_v.push_back(hlt_path)

selector = BaseSelector(Event)(0, hlt_paths_v)
selector.addOperator(EventCounter(Event)())
selector.addOperator(TriggerFilter(Event)(vector("string")(1,"HLT_HH4bAll")))
selector.addOperator(EventCounter(Event)())
selector.addOperator(JetSelection(Event)(10., 20., 4))
selector.addOperator(EventCounter(Event)())
selector.addOperator(BTagJetSelection(Event)("CSV", 0.890, 3))
selector.addOperator(DiJetPairSelection(Event)())
selector.addOperator(DiHiggsPlotter(Event)())
selector.addOperator(EventCounter(Event)())
file_n = "/lustre/cmswork/dallosso/hh2bbbb/non-resonant/analysis/13TeV/hh4bNores/data/Step0/tree_Step0_V13_SM.root" 

tchain = TChain("tree")
tchain.Add(file_n)
tchain.Process(selector, "")

