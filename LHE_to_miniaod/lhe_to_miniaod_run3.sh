#!/bin/bash

#Format: bash lhe_to_miniaod_test.sh [step] [mh] [mx] [pl] [nev] [mode]
STEP=$1
MH=$2
MX=$3
RUN=run_mh${MH}_mx${MX}_ev${N_EV}
PL=$4
N_EV=$5
mode=$6
root_file_step0=root://cms-xrd-global.cern.ch//store//user/christiw/ppTohToSS1SS2_SS1Tobb_SS2Toveve_MC_prod/ppTohToSS1SS2_SS1Tobb_SS2Toveve_run_m50_w0p001_pl_10_step0/crab_CMSSW_7_6_3_ppTohToSS1SS2_SS1Tobb_SS2Toveve_run_m50_w0p001_pl_10_GENSIM_CaltechT2/190108_064437/0000/ppTohToSS1SS2_SS1Tobb_SS2Toveve_run_m50_w0p001_pl_10_step0_1.root
OUTPUT_FILE=ppTohToSS1SS2_SS1Tobb_SS2Toveve_${mode}_withISR
lhe_file=/eos/cms/store/user/mcitron/ppTohToSS1SS2_SS1Tobb_SS2Tobb_ggh_withISR/unweighted_events_mh${MH}_mx${MX}_pl${PL}_ev${N_EV}.lhe
LHE_OUTPUT=${OUTPUT_FILE}_mh${MH}_mx${MX}_pl${PL}_ev${N_EV}
#OUTPUT__DIR=~georgia/work/private/bsmhiggs/CMSSW_10_6_0/src
OUTPUT_DIR=/afs/cern.ch/work/g/gkopp/MC_GenProduction/CMSSW_10_6_0/src/
if [ $STEP -eq 0 ] 
then
#cd ~georgia/work/private/bsmhiggs/CMSSW_10_6_0/src
#/afs/cern.ch/work/g/gkopp/MC_GenProduction/CMSSW_10_6_0/src
cd /afs/cern.ch/work/g/gkopp/MC_GenProduction/CMSSW_10_6_0/src
eval `scramv1 runtime -sh`
cmsDriver.py Configuration/Generator/python/ppTohToSS1SS2_SS1Tobb_SS2Toveve.py --filein ${lhe_file} --fileout file:${OUTPUT_FILE}_step0.root --step GEN,SIM --conditions 106X_upgrade2021_realistic_v4 --no_exec --eventcontent RAWSIM -s GEN,SIM  --datatier GEN-SIM --beamspot Run3RoundOptics25ns13TeVHighSigmaZ --mc --era Run3 -n ${N_EV}  --python_filename ${OUTPUT_DIR}${LHE_OUTPUT}_step0_cfg.py
#cmsRun ${OUTPUT_DIR}${LHE_OUTPUT}_step0_cfg.py

echo "step 0 completed"

#step 1, from GENSIm to DIGI-RECO
elif [ $STEP -eq 1 ] 
then
cd /afs/cern.ch/work/g/gkopp/MC_GenProduction/CMSSW_10_6_0/src
#cd ~georgia/work/private/bsmhiggs/CMSSW_10_6_0/src  
eval `scramv1 runtime -sh`
cmsDriver.py step1 --filein file:${OUTPUT_DIR}${OUTPUT_FILE}_step0.root --fileout file:${OUTPUT_FILE}_step1.root  --pileup_input "das:/RelValMinBias_13/CMSSW_10_6_0_pre4-106X_upgrade2021_realistic_v4-v1/GEN-SIM" --mc --eventcontent FEVTDEBUGHLT --datatier GEN-SIM-DIGI-RAW --pileup AVE_50_BX_25ns --geometry DB:Extended --conditions 106X_upgrade2021_realistic_v4 --step DIGI:pdigi_valid,L1,DIGI2RAW,HLT:@relval2017 --nThreads 1 --era Run3 --python_filename ${OUTPUT_DIR}${OUTPUT_FILE}_step1_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n ${N_EV}
echo "step 1 completed"

#====== step 2, from DR to AODSIM
elif [ $STEP -eq 2 ]
then
cd /afs/cern.ch/work/g/gkopp/CMSSW_10_6_0/src
#cd /afs/cern.ch/work/c/christiw/public/releases/CMSSW_8_0_21/src
eval `scramv1 runtime -sh`
cmsDriver.py step2 --filein file:${OUTPUT_DIR}${OUTPUT_FILE}_step1.root --fileout file:${OUTPUT_FILE}_step2.root --mc --eventcontent AODSIM --runUnscheduled --datatier AODSIM --conditions 106X_upgrade2021_realistic_v4 --step RAW2DIGI,RECO,EI --nThreads 1 --era Run3 --python_filename ${OUTPUT_DIR}${OUTPUT_FILE}_step2_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n ${N_EV}
#sed -i "s/t.outputCommands/t.outputCommands + ['keep *_genParticles_xyz0_*', 'keep *_genParticles_t0_*',]/g" ${OUTPUT_DIR}${OUTPUT_FILE}_step2_cfg.py
cmsRun ${OUTPUT_DIR}${OUTPUT_FILE}_step2_cfg.py
#cd /afs/cern.ch/work/c/christiw/public/LLP/
#====== step 3, from AODSIM to MINIAODSIM
elif [ $STEP -eq 3 ]
then
cd /afs/cern.ch/work/g/gkopp/CMSSW_10_6_0/src
#cd /afs/cern.ch/work/c/christiw/public/releases/CMSSW_9_3_6/src
eval `scramv1 runtime -sh`
cmsDriver.py step3 --filein file:${OUTPUT_DIR}${OUTPUT_FILE}_step2.root --fileout file:${OUTPUT_DIR}${OUTPUT_FILE}_step3.root --mc --eventcontent MINIAODSIM --runUnscheduled --datatier MINIAODSIM --conditions 106X_upgrade2021_realistic_v4 --step PAT --nThreads 8 --era Run3 --python_filename ${OUTPUT_DIR}${OUTPUT_FILE}_step3_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n ${N_EV}
#cd /afs/cern.ch/work/c/christiw/public/LLP/
fi
#cmsRun ${OUTPUT_DIR}${OUTPUT_FILE}_step3_cfg.py
