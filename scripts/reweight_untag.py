#/usr/bin/env python 

import ROOT
from ROOT import ExtEvent as Event
from ROOT import TChain, BasicSelector,  vector
from ROOT import TH1, TFile

from di_higgs.hh2bbbb.samples_25ns import mc_samples

base_dir = "reweight_untag/"

max_events = -1000000
inEllipse = True 
freeJetTagged = False 
isMC = True

tfile = TFile("estimate_btag_prob/ratio.root")
h_ratio = tfile.Get("ratio")


TH1.AddDirectory(False)

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

hlt_paths = hlt_paths_mc if isMC else hlt_paths_data
hlt_paths_v = vector("string")()
for hlt_path in hlt_paths: hlt_paths_v.push_back(hlt_path)

hlt_paths_or = hlt_paths[0:1] +  hlt_paths[2:3] 
hlt_paths_or_v = vector("string")()
for hlt_path in hlt_paths_or: hlt_paths_or_v.push_back(hlt_path)

mc_names = mc_samples.keys()
#mc_names = ["HHTo4B_SM"]
for name in mc_names:
    isHH = False
    if "HH" in name: isHH = True
    selector = BasicSelector(Event)(0, hlt_paths_v, isHH,
                                    hlt_paths_or_v, inEllipse,
                                    freeJetTagged, h_ratio)
    tchain = TChain("tree")
    tchain.Add(mc_samples[name]["lustre_path"])
    print "processing {} sample".format(name)
    if max_events > 0:
        tchain.Process(selector, "ofile="+base_dir+name+".root", max_events)
    else:
        tchain.Process(selector, "ofile="+base_dir+name+".root")

#process_data = True
if (process_data):
    data_dir = "/lustre/cmswork/dallosso/hh2bbbb/non-resonant/analysis/13TeV/hh4bNores/data/Step0/" 
    data_name = "tree_Step0_V15_BTagCSVRun2015TopUp"
    isHH = False
    isMC = False
    hlt_paths = hlt_paths_mc if isMC else hlt_paths_data
    hlt_paths_v = vector("string")()
    for hlt_path in hlt_paths: hlt_paths_v.push_back(hlt_path)
    
    hlt_paths_or = hlt_paths[0:1] +  hlt_paths[2:3] 
    hlt_paths_or_v = vector("string")()
    for hlt_path in hlt_paths_or: hlt_paths_or_v.push_back(hlt_path)
    selector = BasicSelector(Event)(0, hlt_paths_v, isHH,
                                    hlt_paths_or_v, inEllipse,
                                    freeJetTagged, h_ratio)
    tchain = TChain("tree")
    tchain.Add(data_dir+data_name+".root")
    tchain.Process(selector, "ofile="+base_dir+data_name+".root")


