# Instructions for L1 Emulator
Done on lxplus in
```
/afs/cern.ch/work/g/gkopp/L1emulator
```

## Files and Edits for Layer-1, Layer-2 L1 Emulator
```
git cms-addpkg EventFilter/L1TRawToDigi
git cms-addpkg L1Trigger/L1TCaloLayer1
git cms-addpkg L1Trigger/L1TCalorimeter
```

Edits in following files:
```
EventFilter/L1TRawToDigi/plugins/implementations_stage2/CaloLayer1Packer.cc
EventFilter/L1TRawToDigi/plugins/implementations_stage2/UCTCTP7RawData.h
L1Trigger/L1TCaloLayer1/plugins/L1TCaloLayer1.cc
L1Trigger/L1TCalorimeter/src/firmware/Stage2Layer2JetAlgorithmFirmwareImp1.cc
```

Relies on updates to 
```
SimCalorimetry/HcalTrigPrimAlgos/src/HcalFinegrainBit.cc
SimCalorimetry/HcalTrigPrimAlgos/src/HcalTriggerPrimitiveAlgo.cc
```
From the following commits (all are prior to 12_3_0 so are included in the recent CMSSW branches):

[CMSSW PR#35599](https://github.com/cms-sw/cmssw/pull/35599): uHTR group 0 LUT bits12-15 for fine grain bits, fix fine grain bit assignment order

[CMSSW PR#34600](https://github.com/cms-sw/cmssw/pull/34600): Fine grain bits for timing and depth

[CMSSW PR#33050](https://github.com/cms-sw/cmssw/pull/33050): HB TDC packing and prompt - delayed boundaries

## Running
```
cd CMSSW_12_3_0_pre2/src
cmsDriver.py --python_filename DIGI_10events_TDCpacked_cfg.py --eventcontent FEVTDEBUGHLT --customise Configuration/DataProcessing/Utils.addMonitoring --datatier GEN-SIM-DIGI-RAW --fileout file:HTo2LongLivedTo4b_MH-125_MFF-50_CTau-3000mm_TuneCP5_13TeV_pythia8_cff-digi_noPU.root --pileup NoPileUp --conditions auto:phase1_2021_realistic --step DIGI,L1,DIGI2RAW,HLT:GRun --geometry DB:Extended --filein file:/eos/cms/store/group/dpg_hcal/comm_hcal/gillian/LLP_Run3/112X_TDC74pt8/HTo2LongLivedTo4b_MH-125_MFF-50_CTau-3000mm_TuneCP5_13TeV_pythia8_cff.root --era Run3 --no_exec --mc -n 10
cmsRun DIGI_10events_TDCpacked_cfg.py 
```

### Printouts used to verify
`EventFilter/L1TRawToDigi/plugins/implementations_stage2/UCTCTP7RawData.h` line 209:
```
if ((fb & 0b001111) != 0) {
          std::cout << "in UCTCTP7RawData, depth = " << depth << "; MIP bits = " << ((fb & 0b110000) >> 4)
                    << "; prompt = " << prompt << "; delayed 1,2 = " << delay1 << ", " << delay2
                    << ". And data in setFB (Accounting for shift by tower) = " << ((data >> tower) & 0b1)
                    << " with cType = " << cType << " at ieta = " << cEta << std::endl;
		    }
```

`L1Trigger/L1TCaloLayer1/plugins/L1TCaloLayer1.cc` line 230:
```
if ((fg == 1) | (fg2 == 1) | (fg3 == 1) | (fg4 == 1))
              std::cout << "in L1TCaloLayer1.cc featureBits = " << featureBits << " from depth = " << fg
                        << ", prompt = " << fg2 << ", and delayed1, delayed2 = " << fg3 << ", " << fg4
                        << " at ieta, iphi = " << caloEta << ", " << caloPhi << std::endl;
```

`L1Trigger/L1TCalorimeter/src/firmware/Stage2Layer2JetAlgorithmFirmwareImp1.cc` line 141:
```
if ((towDelay > 0) && (abs(ietaTest) < 29))
                std::cout << "in Stage2Layer2JetAlgorithmFirmwareImp towDelay from hwQual = " << towDelay
                          << " at ieta, iphi = " << ietaTest << ", " << iphiTest << std::endl;
```
line 163:
```
if (iDelay != 0 && abs(ieta) < 29)
            std::cout << "in Stage2Layer2JetAlgorithmFirmwareImp iDelay = " << iDelay << " at ieta, iphi = " << ieta
                      << ", " << iphi << std::endl;
```
line 256:
```
if (iDelay >= 2)
              std::cout << "Stage2Layer2JetAlgorithmFirmwareImp1 jet.setHwQual(1) = since iDelay = " << iDelay
                        << " at ieta, iphi = " << ieta << ", " << iphi << std::endl;
```

### Fake Data in HCAL Fine Grain Bits
Fake data patterns are added in the HCAL fine grain bits for testing purposes. This is done in `SimCalorimetry/HcalTrigPrimAlgos/src/HcalFinegrainBit.cc` line 141:
```
if (id.ieta() == 1 && id.iphi() == 1) {
    result[0] = true;
    result[1] = result[2] = result[3] = false;
  }
  if (id.ieta() == 3 && id.iphi() == 1) {
    result[1] = true;
    result[0] = result[2] = result[3] = false;
  }
  if (id.ieta() == 5 && id.iphi() == 1) {
    result[2] = true;
    result[1] = result[0] = result[3] = false;
  }
  if (id.ieta() == 7 && id.iphi() == 1) {
    result[3] = true;
    result[1] = result[2] = result[0] = false;
  }
  if (id.ieta() == 9 && id.iphi() == 1) {
    result[1] = result[2] = true;
    result[3] = result[0] = false;
  }
  if (id.ieta() == 11 && id.iphi() == 1) {
    result[1] = result[3] = true;
    result[2] = result[0] = false;
  }
  if (id.ieta() == 13 && id.iphi() == 1) {
    result[1] = result[2] = result[3] = true;
    result[0] = false;
  }
  if (id.ieta() == 15 && id.iphi() == 1) {
    result[0] = result[1] = result[2] = result[3] = true;
  }
```


# uGT emulator files from Rick
Untar the uGTemulator archive in the outer (non-CMSSW) directory.
```
tar -xvf uGTemulator.tar
```
Edit `my_cmssw_release=CMSSW_12_3_0_pre2`, `new_code_dir=/afs/cern.ch/work/g/gkopp/L1emulator` (this is the directory in which the archive file was untarred, and it has the modified files needed for the uGT emulator).

Either source the shell script: `source install-and-run.sh`, or run individual lines (the approach I took, easier for debugging). Make sure to point to the old/new UTM library (details in the comments of the shell script explain this in more detail). For example, I opened `$CMSSW_BASE/config/toolbox/slc7_amd64_gcc10/tools/selected/utm.xml` and copied the UTM version and location into the shell script. After compiling after adding the uGT emulator CMSSW packages, I had some warnings / notes, but no errors. `warning: 'EDProducer' is deprecated [-Wdeprecated-declarations]`.

Edit `runGlobalFakeInputProducer.py` to point to a DIGI file to run over. I used `file:/eos/cms/store/group/dpg_hcal/comm_hcal/gillian/LLP_Run3/112X_TDC74pt8_jetQUAL/HTo2LongLivedTo4b_MH-125_MFF-50_CTau-30000mm_TuneCP5_13TeV_pythia8_cff-digi_TDCunpacked2k.root' which is a DIGI MC LLP file produced with hwQual saved for each jet.

Following the L1 (uHTR, Layer-1, Layer-2) emulator, I used the output file of this `file:/afs/cern.ch/work/g/gkopp/L1emulator/CMSSW_12_3_0_pre2/src/HTo2LongLivedTo4b_MH-125_MFF-50_CTau-3000mm_TuneCP5_13TeV_pythia8_cff-digi_noPU.root` as the input to `runGlobalFakeInputProducer.py`. Edits:
```
nevents = 10 # line 17
"file:/afs/cern.ch/work/g/gkopp/L1emulator/CMSSW_12_3_0_pre2/src/HTo2LongLivedTo4b_MH-125_MFF-50_CTau-3000mm_TuneCP5_13TeV_pythia8_cff-digi_noPU.root" # line 82
```
And then run this file with `cmsRun runGlobalFakeInputProducer.py`, and this will produce four files: `TestVector_000.txt`, `test-vector-log.txt`, `l1t_debug.log`, `l1t_histos.root`. In `test-vector-log.txt` it shows how many events passed each L1 trigger:
```
=========================== Global Trigger Summary Report  ==================================
                       Algorithm Name                              Init     aBXM     Final
=============================================================================================
L1_AlwaysTrue							    10        10        10
L1_SingleJet35                                                      2         2		2
L1_SingleJet35_llp				 	   	    1         1         1
```

When the uGT emulator is added to the area that already has the L1 emulator, both printouts will show when the DIGI step of MC is run. But remember that the DIGI step sets jet hwQual values, and these are used in the uGT emulator, which relies on the produced DIGI file.

## Notes on cmsRun python config
With `cmsRun runGlobalFakeInputProducer.py`, the fake input to the uGT from `plugin/GenToInputProducer.cc` is used. This produces non-trivial inputs to uGT emulator for testing of firmware. Thus, sending in different DIGI files (ie one produced with fake fine grain bits on all iphi (ieta = 1,3,5,7,9,11,13,15) gives the same `test-vector-log.txt` as using a file produced with fake fine grain bits only on iphi=1). The `runGlobalFakeInputProducer` is not using the DIGI hwQual data to make the output.

Use `L1Trigger/L1TGlobal/test/l1tGlobalEmulation_FromDigi.py` to run on existing DIGIs. This is the config that runs emulation from existing digis. 

Helpful uGT emulation [twiki](https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideL1TGlobal).