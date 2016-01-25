# /usr/bin/env python 

from di_higgs.hh2bbbb.samples_25ns import mc_samples
from ROOT import TH1, TFile


h_name = "free_jet_{}_pt_abs_eta"
mc_names = mc_samples.keys()
mc_names = ["TTJets"]

for i, name in enumerate(mc_names):
    tfile = TFile(name+".root") 
    h_tag = tfile.Get(h_name.format("tag"))
    h_untag = tfile.Get(h_name.format("untag"))
    if (i == 0):
        s_tag = h_tag.Clone("sum_"+h_name.format("tag"))
        s_untag = h_tag.Clone("sum_"+h_name.format("untag"))
        s_tag.Reset()
        s_untag.Reset()
    mc_sample = mc_samples[name]     
    factor = 1 
    s_tag.Add(h_tag,factor)    
    s_untag.Add(h_untag,factor)    

ratio = s_tag.Clone("ratio")
ratio.Divide(s_untag)
o_file = TFile("ratio.root", "RECREATE")
ratio.Write()


