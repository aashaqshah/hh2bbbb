#!/usr/bin/env python 

import ROOT
from ROOT import TChain, BaseSelector

selector = BaseSelector()
file_n = "/lustre/cmswork/dallosso/hh2bbbb/non-resonant/analysis/13TeV/hh4bNores/data/Step0/tree_Step0_V15_BTagCSVRun2015C-D.root"
tchain = TChain("tree")
tchain.Add(file_n)
tchain.Process(selector, "", 100000)

