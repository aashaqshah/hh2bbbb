#!/usr/bin/env python 

import ROOT
from ROOT import ExtEvent as Event
from ROOT import TChain, PairingSelector,  vector
from ROOT import TH1, TFile

from di_higgs.hh2bbbb.samples_25ns import mc_samples

base_dir = "pairing_selector/"

inEllipse = True 
freeJetTagged = True 
isMC = True

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

name = "HHTo4B_SM"
for pairing in [0,1]:
    isHH = False
    if "HH" in name: isHH = True
    selector = PairingSelector(Event)(0, hlt_paths_v, isHH,
                                    hlt_paths_or_v, pairing)
    tchain = TChain("tree")
    tchain.Add(mc_samples[name]["lustre_path"])
    print "processing {} sample".format(name)
    tchain.Process(selector, "ofile="+base_dir+name+"_{}.root".format(pairing))

