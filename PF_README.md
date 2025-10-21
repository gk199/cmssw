# Particle Flow 

# PF Configuration for data or MC
```
cmsRun MyPFStudy_ReRecoAOD_RAW2DIGI_L1Reco_RECO.py
edmDumpEventContent pf_only_reRecoAOD.root | grep particleFlow
```
For producing ntuples:
```
cmsRun MyAnalyzer/PFObjectsNtupler/python/runPFObjectsNtupler_cfg.py
```

## Git Commits
To avoid issues with sparse checkout (used to get the ParticleFlow area):
```
echo "PF_README.md" >> .git/info/sparse-checkout
```
This lets the .md file be committed. 

## To list files from DAS
```dasgoclient -query="file dataset=/QCD_Pt-15to7000_TuneCP5_Flat_13p6TeV_pythia8/Run3Winter24Reco-NoPU_133X_mcRun3_2024_realistic_v9-v2/AODSIM site=T2_US_MIT"```

# MC Generation
For MC production of a single pion sample, `git-cms-addpkg` the `Configuration/Generator` directory for reference. Start with the 10 GeV single pion from here, and then change the produced .py file for different energies, charged vs neutral, etc.
```
cmsDriver.py Configuration/Generator/python/SinglePiPt10_pythia8_cfi.py \
   --fileout file:SinglePionPt10_step0_GEN-SIM.root \
   --mc \
   --customise Configuration/DataProcessing/Utils.addMonitoring \
   --eventcontent RAWSIM --datatier GEN-SIM \
   --conditions auto:phase1_2024_realistic \
   --beamspot Realistic25ns13p6TeVEarly2022Collision \
   --step GEN,SIM --geometry DB:Extended \
   --era Run3 --python_filename SinglePiPt10_cfg.py \
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

cmsDriver.py step2 \
   --python_filename SinglePiPt10_RECO_cfg.py \
   --filein file:SinglePiPt10_step1_GEN-SIM-RAW.root \
   --fileout file:SinglePiPt10_step2_RECO.root \
   --customise Configuration/DataProcessing/Utils.addMonitoring \
   --eventcontent AODSIM \
   --datatier AODSIM \
   --conditions auto:phase1_2024_realistic \
   --step RAW2DIGI,L1Reco,RECO,RECOSIM \
   --geometry DB:Extended \
   --era Run3_2024 \
   --mc -n 100 \
```

# Monitoring and Plotting (DQMOffline)
Within CMSSW (using `CMSSW_15_0_6` currently), checkout the github branch `pfclusters` from [here](https://github.com/jroloff/cmssw/tree/pfclusters/DQMOffline/ParticleFlow) into DQMOffline/ParticleFlow. Make a `.log` file listing the ROOT files to analyze, and list this in `runBasic_cfg.py`. From within the CMSSW area, run:
```
cmsenv
scram b -j 8
cd DQMOffline/ParticleFlow/python
cmsRun runBasic_cfg.py
cmsRun runBasic_step2_cfg.py
```
Then use the PF plotting code from [here](https://gitlab.cern.ch/jroloff/pfmonitoringplots). This can be outside a CMSSW area. `sampleList.txt` lists the run number and the sample it came from. For example, `392251 2025*Era*C*JetMET` is from the ROOT file with run 392251 and 2025 Era C JetMET will be the legend label. Edit file name in `PlotDistributions.py`, and run:
```
python3 PlotDistributions.py --samples sampleList.txt --outDir Plots --varList varList_initial.txt --eventVarList eventVarList.txt
```

Note that for MC or the re-reco samples, the trigger and event selection might be too restrictive. To remove the trigger and event selection requirements, edit `runBasic_cfi.py`:
```
TriggerNames = cms.vstring(""),
eventSelection = cms.string("nocut"),
```

# Factorizing PF


# Local Areas (Gillian)
MC production and data processing:
```/afs/cern.ch/work/g/gkopp/2025_ParticleFlow/CMSSW_15_0_6/src
```
DQM ntupler:
```/afs/cern.ch/work/g/gkopp/2025_ParticleFlow/CMSSW_15_0_6/src/DQMOffline/ParticleFlow/python
```
PF plots:
```/afs/cern.ch/work/g/gkopp/2025_ParticleFlow/pfmonitoringplots
```