Introduction to CMSSW: http://cms-sw.github.io

# Directions for making MC files
Using lhe_to_miniaod_eos.sh, do

    voms-proxy-init --rfc --voms cms --valid 48:00
    ./lhe_to_miniaod_run3_eos.sh [step] [mh] [mx] [pl] [nev] [mode]

This will output a <filename>_step0_cfg.py file in the CMSSW_10_6_0 source directory. In "process.source", need to comment out dropDescendantsOfDroppedBranches and inputCommands. The output directory for the ROOT file is 

    /eos/cms/store/group/dpg_hcal/comm_hcal/gillian/LLP_Run3/mh${MH}_mx${MX}_pl${PL}_ev${N_EV}/

This file must exists to write the ROOT file. Then, in CMSSW_10_6_0/src
    
    cmsenv
    cmsRun <filename>_step0_cfg.py

This will output the <filename>_step0.root file, which will be used in Step 1.
    
    ./lhe_to_miniaod_run3_eos.sh [step] [mh] [mx] [pl] [nev] [mode]
    cd CMSSW_10_6_0/src/
    cmsenv
    cmsRun <filename>_step1_cfg.py
 
This also outputs the Step 1 ROOT file to the same EOS directory. The Step 1 ROOT file can now be used in the HCAL Trigger analyzer framework: https://github.com/gk199/cms-hcal-debug. Need to list the generated MC files in analyze_run3.py, arguments for run.C, and filepaths for plot_QCD_LLP.py.

# Instructions for Gen Matching
In afs/cern.ch/work/g/gkopp/MC_GenProduction/CMSSW_10_6_0/src/L1_TriggerWork/GenMatching/plugins there is GenProcessAnalyzer.cc and BuildFile.xml. Compile with 

    scram b -j 8
   
In GenMatching, there is ProcessAnalyzer.py. Edit which MC files in EOS this directs to, and run with
    
    cmsRun ProcessAnalyzer.py
