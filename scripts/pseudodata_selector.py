#!/usr/bin/env python 

from datetime import datetime
import ROOT
from ROOT import ThinEvent, ExtEvent
from ROOT import TChain, FrankSelector,  vector
from ROOT import TH1, TFile, TEventList

from di_higgs.hh2bbbb.samples_25ns import mc_samples
from di_higgs.hh2bbbb.event_list_creator import EventListCreator


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


samples = mc_samples
sub_strs = ['QCD_HT500','QCD_HT700','QCD_HT1000',
            'QCD_HT1500','QCD_HT2000','HH', 'TTJets']
sam_names = samples.keys()
sam_names=[n for n in sam_names if any(s in n for s in sub_strs)]
f_signal = 100.
sam_factors = {k : f_signal for k in sam_names if 'HH' in k}

mult =  5

input_path = "mixing/"
output_path = "mixed/"
# construct output file base name
out_bn = datetime.now().strftime("%y%m%d_%H%M%S")


# create event list
el_creator = EventListCreator()
for sam_name in sam_names:
    sam_info = samples[sam_name]
    sam_factor = sam_factors[sam_name] if sam_name in sam_factors else 1.0
    el_creator.add_sample(input_path+sam_name+".root", factor=sam_factor,
                          **{k:sam_info[k] for k in ['xs', 'gen_event']})
el = el_creator.get_event_list()    

# setup hemisphere tchain
tc_hm = TChain("hem_tree")
for sam_name in sam_names: tc_hm.Add(input_path+sam_name+".root")

tc_hm.SetEventList(el)
selector = FrankSelector(ExtEvent(ThinEvent))(0, hlt_paths_v, isHH,
                                              isData, hlt_paths_or_v,
                                              tc_hm, mult)

# setup event tchain and set event list
ev_tc = TChain("tree")
for sam_name in sam_names: ev_tc.Add(input_path+sam_name+".root")
ev_tc.SetEventList(el)
ev_tc.Process(selector, "ofile="+output_path+ "pseudodata_"+out_bn+".root")

