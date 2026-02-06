# Particle Flow 

Documentation for running MC, using the DQM offline for monitoring and plotting, making re-reco and ntuples for testing PF, and factorizing particle flow.

# Full Setup
Set up a recent CMSSW release, and get Jennifer's DQM plotting framework:
```
cmsrel CMSSW_15_0_6
cd CMSSW_15_0_6/src
cmsenv
git cms-addpkg DQMOffline/ParticleFlow

git config core.sparseCheckout true
echo "DQMOffline/ParticleFlow/*" > .git/info/sparse-checkout

git remote add user-repo https://github.com/jroloff/cmssw.git
git remote -v
git fetch user-repo pfclusters
git checkout -b PF_Project user-repo/pfclusters
git branch

scram b -j 8
```

## Git Commits
To avoid issues with sparse checkout (used to get the ParticleFlow area):
```
echo "PF_README.md" >> .git/info/sparse-checkout
```
This lets the .md file be committed. 

## To list files from DAS
```dasgoclient -query="file dataset=/QCD_Pt-15to7000_TuneCP5_Flat_13p6TeV_pythia8/Run3Winter24Reco-NoPU_133X_mcRun3_2024_realistic_v9-v2/AODSIM site=T2_US_MIT"```

# Monitoring and Plotting (DQMOffline)
Within CMSSW (using `CMSSW_15_0_6` currently), checkout the github branch `pfclusters` from [here](https://github.com/jroloff/cmssw/tree/pfclusters/DQMOffline/ParticleFlow) into DQMOffline/ParticleFlow. Make a `.log` file listing the ROOT files to analyze, and list this in `runBasic_cfg.py`. RECO files from MC can be used, or AODfull files from re-reco of data. AODfull files have trigger information included. From within the CMSSW area, run:
```
cmsenv
scram b -j 8
voms-proxy-init --rfc --voms cms --valid 48:00
cd DQMOffline/ParticleFlow/python
cmsRun runBasic_cfg.py
cmsRun runBasic_step2_cfg.py
```
The first step produces an output file `OUT_step1.root` that is used in the second step. Then use the PF plotting code from [here](https://gitlab.cern.ch/jroloff/pfmonitoringplots). This can be outside a CMSSW area. `sampleList.txt` lists the run number and the sample it came from. For example, `392251 2025*Era*C*JetMET` is from the ROOT file with run 392251 and 2025 Era C JetMET will be the legend label. Edit file name in `PlotDistributions.py`, and run:
```
python3 PlotDistributions.py --samples sampleList.txt --outDir Plots --varList varList_initial.txt --eventVarList eventVarList.txt
```
Note that the run number listed in eventVarList must correspond to the filename and run number in the ROOT tree. 

Note that for MC or the re-reco samples, the trigger and event selection might be too restrictive. To remove the trigger and event selection requirements, edit `runBasic_cfi.py`:
```
TriggerNames = cms.vstring(""),
eventSelection = cms.string("nocut"),
```

# MC Generation
For MC production of a single pion sample, `git-cms-addpkg Configuration/Generator` directory for reference. Start with the 10 GeV single pion from here, and then change the produced .py file for different energies, charged vs neutral, etc.

MC generations commands can be cross checked with the standard RunTheMatrix, such as `runTheMatrix.py -l 13034`. 
```
cmsDriver.py Configuration/Generator/python/SinglePiPt10_pythia8_cfi.py \
   --fileout file:SinglePionPt10_step0_GEN-SIM.root \
   --mc \
   --customise Configuration/DataProcessing/Utils.addMonitoring \
   --eventcontent RAWSIM --datatier GEN-SIM \
   --conditions auto:phase1_2024_realistic \
   --beamspot Realistic25ns13p6TeVEarly2022Collision \
   --step GEN,SIM --geometry DB:Extended \
   --era Run3_2024 --python_filename SinglePiPt10_cfg.py \
   -n 100 \
   --no_exec

cmsRun SinglePiPt10_cfg.py
```
The next steps (RAW and RECO) are:
```
cmsDriver.py step1 \
   --python_filename SinglePionPt10_step1_cfg.py \
   --filein file:SinglePionPt10_step0_GEN-SIM.root \
   --fileout file:SinglePiPt10_step1_GEN-SIM-RAW.root \
   --pileup NoPileUp \
   --customise Configuration/DataProcessing/Utils.addMonitoring \
   --eventcontent RAWSIM \
   --datatier GEN-SIM-RAW \
   --conditions auto:phase1_2024_realistic \
   --step DIGI,L1,DIGI2RAW \
   --geometry DB:Extended \
   --era Run3_2024 \
   --mc -n 100
```
Recommendations to add pileup:
```
--pileup Run3_Flat55To75_PoissonOOTPU 
--pileup_input das:/RelValMinBias_14TeV/CMSSW_14_1_0_pre7-140X_mcRun3_2024_realistic_v21_STD_RegeneratedGS_2024_noPU-v1/GEN-SIM
```
instead of `--pileup NoPileUp`. This is from the RunTheMatrix ttbar 2024 sample with 2024 PU conditions. 

```
cmsDriver.py step2 \
   --python_filename SinglePiPt10_RECO_cfg.py \
   --filein file:SinglePiPt10_step1_GEN-SIM-RAW.root \
   --fileout file:SinglePiPt10_step2_RECO.root \
   --customise Configuration/DataProcessing/Utils.addMonitoring \
   --eventcontent AODSIM \
   --datatier AODSIM \
   --conditions auto:phase1_2024_realistic \
   --step RAW2DIGI,L1Reco,RECO,RECOSIM,HLT:@relval2024 \
   --geometry DB:Extended \
   --era Run3_2024 \
   --mc -n 100 \
```

If the g4SimHits are needed, save the full event content:
```
cmsDriver.py step2test \
   --python_filename SinglePiPt10_FEVTDEBUGHLT_cfg.py \
   --filein file:SinglePiPt10_step1_GEN-SIM-RAW.root  \
   --fileout file:SinglePiPt10_step2_FEVTDEBUGHLT.root   \
   --customise Configuration/DataProcessing/Utils.addMonitoring    \
   --eventcontent FEVTDEBUGHLT    --datatier GEN-SIM-RECO    \
   --conditions auto:phase1_2024_realistic    \
   --step RAW2DIGI,L1Reco,RECO,RECOSIM,HLT:@relval2024  \  
   --geometry DB:Extended    --era Run3_2024    \
   --mc -n 10
```

# PF Configuration for data or MC
The full details and code for this setup are [here](https://github.com/gk199/PF-Reco-Analysis).

This is designed to re-run reconstruction on data or MC, from RAW samples, such that the full PF workflow is done. The PF clusters (HCAL and ECAL), blocks, and candidates are saved, along with HBHE rechits from raw. Multiple python configs are available, enabling the final file to either contain RECO, AOD, or AOD + trigger results.
```
cmsRun MyPFStudy_ReReco_RAW2DIGI_L1Reco_RECO.py
cmsRun MyPFStudy_ReRecoAOD_RAW2DIGI_L1Reco_RECO.py
cmsRun MyPFStudy_ReRecoAODfull_RAW2DIGI_L1Reco_RECO.py

edmDumpEventContent pf_only_reRecoAOD.root | grep particleFlow
```
For producing ntuples for plotting and further analysis:
```
cmsRun MyAnalyzer/PFObjectsNtupler/python/runPFObjectsNtupler_cfg.py
```

# Factorizing PF
Work in progress...

# Local Areas (Gillian)
MC production and data processing:
```
/afs/cern.ch/work/g/gkopp/2025_ParticleFlow/CMSSW_15_0_6/src
```
DQM ntupler:
```
/afs/cern.ch/work/g/gkopp/2025_ParticleFlow/CMSSW_15_0_6/src/DQMOffline/ParticleFlow/python
```
PF plots:
```
/afs/cern.ch/work/g/gkopp/2025_ParticleFlow/pfmonitoringplots
```

Github: `git push PF_Project PFdevelopment`.

[Link](https://github.com/gk199/cmssw/blob/PFdevelopment/PF_README.md) to github documentation.