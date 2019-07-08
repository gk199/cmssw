Introduction to CMSSW: http://cms-sw.github.io


# Setup

Install with:

    git clone git@github.com:cms-hcal-trigger/cms-hcal-debug.git Debug/HcalDebug
    scram b -j 8

# Examples

# Directions for making MC files
Using lhe_to_miniaod_eos.sh, do
    voms-proxy-init --rfc --voms cms --valid 48:00
    ./lhe_to_miniaod_run3_eos.sh [step] [mh] [mx] [pl] [nev] [mode]

This will output a <filename>_step0_cfg.py file in the source directory. Then go into CMSSW_10_6_0/src
  cmsenv
  cmsRun <filename>_step0_cfg.py
