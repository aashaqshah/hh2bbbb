# hh2bbbb

alternative analysis code for 13TeV hh->bbbb analysis

## setup workspace

```
cmsrel CMSSW_7_6_3_patch2
cd CMSSW_7_6_3_patch2/src
cmsenv
git clone https://github.com/pablodecm/mut_dataformats.git mut_framework/mut_dataformats
git clone https://github.com/pablodecm/mut_utils.git mut_framework/mut_utils
git clone https://github.com/pablodecm/hh2bbbb.git di_higgs/hh2bbbb
scram b -j 8
```

## run example analysis

```
cd di_higgs/hh2bbbb/scripts/
python signal_region.py
```
