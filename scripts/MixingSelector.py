#!/usr/bin/env python 

import ROOT
from ROOT import ExtEvent, VHBBEvent 
from ROOT import TChain, MixingSelector,  vector
from ROOT import TH1

from di_higgs.hh2bbbb.samples_25ns import mc_samples

max_events = -10000

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

mc_names = mc_samples.keys()
mc_names = []
for name in mc_names:
    isHH = False
    isData = False 
    if "HH" in name: isHH = True
    selector = MixingSelector(ExtEvent(VHBBEvent))(0, hlt_paths_v, isHH, isData,
                                          hlt_paths_or_v)
    tchain = TChain("tree")
    tchain.Add(mc_samples[name]["lustre_path"])
    print "processing {} sample".format(name)
    if max_events > 0:
        tchain.Process(selector, "ofile="+"./mixing/"+name+".root", max_events)
    else:
        tchain.Process(selector, "ofile="+"./mixing/"+name+".root")

process_data = True 
if process_data:
    sample_name = "/lustre/cmswork/hh/VHBBHeppyV21/BTagCSV/tree_*.root"
    name = "BTagCSV"
    isHH = False
    isData = True
    selector = MixingSelector(ExtEvent(VHBBEvent))(0, hlt_paths_v, isHH, isData,
                                          hlt_paths_or_v)
    tchain = TChain("tree")
    tchain.Add(sample_name)
    print "processing {} sample".format(name)
    if max_events > 0:
        tchain.Process(selector, "ofile="+"./mixing/"+name+".root", max_events)
    else:
        tchain.Process(selector, "ofile="+"./mixing/"+name+".root")

