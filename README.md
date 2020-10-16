Introduction to CMSSW: http://cms-sw.github.io

## Location on lxplus
This is saved on `/afs/cern.ch/work/g/gkopp/MC_GenProduction/CMSSW_10_6_0/src`. Config files for LHE to miniaod in `CMSSW_10_6_0/src/LHE_to_miniaod/`.

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

## Config files for Multiple MC Outputs
This process fails if too many events are requested in a single MC (about 1000 events works consistently). To make 10k events, make 10 files of 1k events each. This is done by adding 

    skipEvents = cms.untracked.uint32(10,000)
    
in the process.source section of the step 0 configuration file. Then need to edit the name of the output root files to not overwrite others. In step 1 config file, edit the file names that are read in, output ones, and add 

    root://cms-xrd-global.cern.ch///store/relval
   
to the process.mix section. Then this can be submitted to Condor. 

# Instructions for Gen Matching
In afs/cern.ch/work/g/gkopp/MC_GenProduction/CMSSW_10_6_0/src/L1_TriggerWork/GenMatching/plugins there is GenProcessAnalyzer.cc and BuildFile.xml. The GenProcessAnalyer has requirements for gen matching - hard scattering process, PDG ID 5 for b quark - and creates histograms. Compile with 

    scram b -j 8
   
In GenMatching, there is ProcessAnalyzer.py, which uses GenProcessAnalyzer as the EDAnalyzer. Edit which MC files in EOS this directs to, and run with
    
    cmsRun ProcessAnalyzer.py
    
This will then output a ROOT file with histograms. Additionally, the parameter can be set to verbose (true, false) to choose if information about mother particles is printed out or not.
