# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: step1 --filein file:/eos/cms/store/group/dpg_hcal/comm_hcal/gillian/LLP_Run3/mh125_mx50_pl500_ev1000/ppTohToSS1SS2_SS1Tobb_SS2Toveve_1_withISR_step0.root --fileout /eos/cms/store/group/dpg_hcal/comm_hcal/gillian/LLP_Run3/mh125_mx50_pl500_ev1000/ppTohToSS1SS2_SS1Tobb_SS2Toveve_1_withISR_step1.root --pileup_input das:/RelValMinBias_13/CMSSW_10_6_0_pre4-106X_upgrade2021_realistic_v4-v1/GEN-SIM --mc --eventcontent FEVTDEBUGHLT --datatier GEN-SIM-DIGI-RAW --pileup AVE_50_BX_25ns --geometry DB:Extended --conditions 106X_upgrade2021_realistic_v4 --step DIGI:pdigi_valid,L1,DIGI2RAW,HLT:@relval2017 --nThreads 1 --era Run3 --python_filename /afs/cern.ch/work/g/gkopp/MC_GenProduction/CMSSW_10_6_0/src/ppTohToSS1SS2_SS1Tobb_SS2Toveve_1_withISR_step1_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n 1000
import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_cff import Run3

process = cms.Process('HLT',Run3)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mix_POISSON_average_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.DigiToRaw_cff')
process.load('HLTrigger.Configuration.HLT_Fake2_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

# Input source
process.source = cms.Source("PoolSource",
    dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
    fileNames = cms.untracked.vstring('file:/eos/cms/store/group/dpg_hcal/comm_hcal/gillian/LLP_Run3/mh125_mx50_pl500_ev1000/ppTohToSS1SS2_SS1Tobb_SS2Toveve_1_withISR_step0.root'),
    inputCommands = cms.untracked.vstring(
        'keep *', 
        'drop *_genParticles_*_*', 
        'drop *_genParticlesForJets_*_*', 
        'drop *_kt4GenJets_*_*', 
        'drop *_kt6GenJets_*_*', 
        'drop *_iterativeCone5GenJets_*_*', 
        'drop *_ak4GenJets_*_*', 
        'drop *_ak7GenJets_*_*', 
        'drop *_ak8GenJets_*_*', 
        'drop *_ak4GenJetsNoNu_*_*', 
        'drop *_ak8GenJetsNoNu_*_*', 
        'drop *_genCandidatesForMET_*_*', 
        'drop *_genParticlesForMETAllVisible_*_*', 
        'drop *_genMetCalo_*_*', 
        'drop *_genMetCaloAndNonPrompt_*_*', 
        'drop *_genMetTrue_*_*', 
        'drop *_genMetIC5GenJs_*_*'
    ),
    secondaryFileNames = cms.untracked.vstring()
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step1 nevts:1000'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.FEVTDEBUGHLToutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-DIGI-RAW'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('/eos/cms/store/group/dpg_hcal/comm_hcal/gillian/LLP_Run3/mh125_mx50_pl500_ev1000/ppTohToSS1SS2_SS1Tobb_SS2Toveve_1_withISR_step1.root'),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
process.mix.input.nbPileupEvents.averageNumber = cms.double(50.000000)
process.mix.bunchspace = cms.int32(25)
process.mix.minBunch = cms.int32(-12)
process.mix.maxBunch = cms.int32(3)
process.mix.input.fileNames = cms.untracked.vstring(['/store/relval/CMSSW_10_6_0_pre4/RelValMinBias_13/GEN-SIM/106X_upgrade2021_realistic_v4-v1/20000/F1F2FB5A-1ADB-464F-AD3F-220E0B8F19B4.root', '/store/relval/CMSSW_10_6_0_pre4/RelValMinBias_13/GEN-SIM/106X_upgrade2021_realistic_v4-v1/20000/B9AD8184-3A82-DB47-9C58-D760355B2BAD.root', '/store/relval/CMSSW_10_6_0_pre4/RelValMinBias_13/GEN-SIM/106X_upgrade2021_realistic_v4-v1/20000/0F774348-F4D9-0745-B45F-4E62009E85CF.root', '/store/relval/CMSSW_10_6_0_pre4/RelValMinBias_13/GEN-SIM/106X_upgrade2021_realistic_v4-v1/20000/9F2162DA-5078-B943-A0E0-9A0C44A62CA9.root', '/store/relval/CMSSW_10_6_0_pre4/RelValMinBias_13/GEN-SIM/106X_upgrade2021_realistic_v4-v1/20000/94E67F70-30D3-6B41-BC46-C7F101454E20.root', '/store/relval/CMSSW_10_6_0_pre4/RelValMinBias_13/GEN-SIM/106X_upgrade2021_realistic_v4-v1/20000/BFBEEBAD-303B-F248-897F-5BDF4E4F9CAF.root', '/store/relval/CMSSW_10_6_0_pre4/RelValMinBias_13/GEN-SIM/106X_upgrade2021_realistic_v4-v1/20000/12A13BC3-AD47-9C44-A4C0-B76D06C99826.root', '/store/relval/CMSSW_10_6_0_pre4/RelValMinBias_13/GEN-SIM/106X_upgrade2021_realistic_v4-v1/20000/4529A24C-B455-0A48-9014-F477FCAF2E29.root'])
process.mix.digitizers = cms.PSet(process.theDigitizersValid)
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '106X_upgrade2021_realistic_v4', '')

# Path and EndPath definitions
process.digitisation_step = cms.Path(process.pdigi_valid)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.digi2raw_step = cms.Path(process.DigiToRaw)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGHLToutput_step = cms.EndPath(process.FEVTDEBUGHLToutput)

# Schedule definition
process.schedule = cms.Schedule(process.digitisation_step,process.L1simulation_step,process.digi2raw_step)
process.schedule.extend(process.HLTSchedule)
process.schedule.extend([process.endjob_step,process.FEVTDEBUGHLToutput_step])
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)

# customisation of the process.

# Automatic addition of the customisation function from Configuration.DataProcessing.Utils
from Configuration.DataProcessing.Utils import addMonitoring 

#call to customisation function addMonitoring imported from Configuration.DataProcessing.Utils
process = addMonitoring(process)

# Automatic addition of the customisation function from HLTrigger.Configuration.customizeHLTforMC
from HLTrigger.Configuration.customizeHLTforMC import customizeHLTforMC 

#call to customisation function customizeHLTforMC imported from HLTrigger.Configuration.customizeHLTforMC
process = customizeHLTforMC(process)

# End of customisation functions

# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
