

mc_samples = {}

mc_samples["HHTo4B_SM"] = {
  "das_name" : "/GluGluToHHTo4B_node_SM_13TeV-madgraph/RunIISpring15DR74-Asympt25ns_MCRUN2_74_V9-v1/MINIAODSIM",     
  "lustre_path" : "/lustre/cmswork/dallosso/hh2bbbb/non-resonant/analysis/13TeV/hh4bNores/data/Step0/tree_Step0_V13_SM.root",
  "xs" : 34.3*0.58*0.58,
  "gen_events" : 276432.  # should be 300k 
}

mc_samples["QCD_HT100to200"] = {
  "das_name" : "/QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",     
  "lustre_path" : "/lustre/cmswork/hh/VHBBHeppyV14/QCD_HT100to200_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/tree_*.root",
  "xs" : 27990000000.,
  "gen_events" : 2868066 # DAS 81637494.
}

mc_samples["QCD_HT200to300"] = {
  "das_name" : "/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",     
  "lustre_path" : "/lustre/cmswork/hh/VHBBHeppyV14/QCD_HT200to300_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/tree_*.root",
  "xs" : 1712000000.,
  "gen_events" : 11168549 # DAS 18718905.
}

mc_samples["QCD_HT300to500"] = {
  "das_name" : "/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",     
  "lustre_path" : "/lustre/cmswork/hh/VHBBHeppyV14/QCD_HT300to500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/tree_*.root",
  "xs" : 347700000.,
  "gen_events" :  17721815 # DAS 19826197.
}

mc_samples["QCD_HT500to700"] = {
  "das_name" : "/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",     
  "lustre_path" : "/lustre/cmswork/hh/VHBBHeppyV14/QCD_HT500to700_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/tree_*.root",
  "xs" : 32100000.,
  "gen_events" :  19186729 # DAS 19664159.
}

mc_samples["QCD_HT700to1000"] = {
  "das_name" : "/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",     
  "lustre_path" : "/lustre/cmswork/hh/VHBBHeppyV14/QCD_HT700to1000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/tree_*.root",
 "xs" : 6831000.,
  "gen_events" :  15150703 # DAS 15356448.
}

mc_samples["QCD_HT1000to1500"] = {
  "das_name" : "/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",     
  "lustre_path" : "/lustre/cmswork/hh/VHBBHeppyV14/QCD_HT1000to1500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/tree_*.root",
  "xs" : 1207000.,
  "gen_events" :  4954504 # DAS 4963895.
}

mc_samples["QCD_HT1500to2000"] = {
  "das_name" : "/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",     
  "lustre_path" : "/lustre/cmswork/hh/VHBBHeppyV14/QCD_HT1500to2000_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/tree_*.root",
  "xs" : 119900.,
  "gen_events" : 3689928 # DAS  3868886.
}

mc_samples["QCD_HT2000toInf"] = {
  "das_name" : "/QCD_HT2000to2500_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",     
  "lustre_path" : "/lustre/cmswork/hh/VHBBHeppyV14/QCD_HT2000toInf_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/tree_*.root",
  "xs" : 25240.,
  "gen_events" :  1912191 # DAS 1912529.
}

mc_samples["TTJets"] = {
  "das_name" : "/TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/RunIISpring15MiniAODv2-74X_mcRun2_asymptotic_v2-v1/MINIAODSIM",
  "lustre_path" : "/lustre/cmswork/dallosso/hh2bbbb/non-resonant/analysis/13TeV/hh4bNores/data/Step0/tree_Step0_V14_TTJets.root",
  "xs" : 831760.,
  "gen_events" : 11344206.
}
