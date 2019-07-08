# Auto generated configuration file
# using: 
# Revision: 1.19 
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v 
# with command line options: Configuration/Generator/python/ppTohToSS1SS2_SS1Tobb_SS2Toveve.py --filein root://cms-xrd-global.cern.ch//store/user/mcitron/ppTohToSS1SS2_SS1Tobb_SS2Tobb_ggh_withISR/unweighted_events_mh125_mx50_pl500_ev100000.lhe --fileout /eos/cms/store/group/dpg_hcal/comm_hcal/gillian/LLP_Run3/mh125_mx50_pl500_ev1000/ppTohToSS1SS2_SS1Tobb_SS2Toveve_1_withISR_step0.root --step GEN,SIM --conditions 106X_upgrade2021_realistic_v4 --no_exec --eventcontent RAWSIM -s GEN,SIM --datatier GEN-SIM --beamspot Run3RoundOptics25ns13TeVHighSigmaZ --mc --era Run3 -n 1000 --python_filename /afs/cern.ch/work/g/gkopp/MC_GenProduction/CMSSW_10_6_0/src/ppTohToSS1SS2_SS1Tobb_SS2Toveve_1_withISR_mh125_mx50_pl500_ev1000_step0_cfg.py
import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_cff import Run3

process = cms.Process('SIM',Run3)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.Geometry.GeometrySimDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRun3RoundOptics25ns13TeVHighSigmaZ_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

# Input source
process.source = cms.Source("LHESource",
   # dropDescendantsOfDroppedBranches = cms.untracked.bool(False),
    fileNames = cms.untracked.vstring('root://cms-xrd-global.cern.ch//store/user/mcitron/ppTohToSS1SS2_SS1Tobb_SS2Tobb_ggh_withISR/unweighted_events_mh125_mx50_pl500_ev100000.lhe')
    #inputCommands = cms.untracked.vstring(
     #   'keep *', 
      #  'drop LHEXMLStringProduct_*_*_*'
   # )
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('Configuration/Generator/python/ppTohToSS1SS2_SS1Tobb_SS2Toveve.py nevts:1000'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    ),
    compressionAlgorithm = cms.untracked.string('LZMA'),
    compressionLevel = cms.untracked.int32(1),
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM'),
        filterName = cms.untracked.string('')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(20971520),
    fileName = cms.untracked.string('/eos/cms/store/group/dpg_hcal/comm_hcal/gillian/LLP_Run3/mh125_mx50_pl500_ev1000/ppTohToSS1SS2_SS1Tobb_SS2Toveve_1_withISR_step0.root'),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

# Additional output definition

# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '106X_upgrade2021_realistic_v4', '')

process.generator = cms.EDFilter("Pythia8HadronizerFilter",
    PythiaParameters = cms.PSet(
        JetMatchingParameters = cms.vstring(
            'JetMatching:setMad = off', 
            'JetMatching:scheme = 1', 
            'JetMatching:merge = on', 
            'JetMatching:jetAlgorithm = 2', 
            'JetMatching:etaJetMax = 5.', 
            'JetMatching:coneRadius = 1.', 
            'JetMatching:slowJetPower = 1', 
            'JetMatching:qCut = 58', 
            'JetMatching:nQmatch = 5', 
            'JetMatching:nJetMax = 2', 
            'JetMatching:doShowerKt = off', 
            '25:m0 = 125.0'
        ),
        parameterSets = cms.vstring(
            'pythia8CommonSettings', 
            'pythia8CUEP8M1Settings', 
            'JetMatchingParameters'
        ),
        processParameters = cms.vstring(
            'ParticleDecays:limitTau0 = off', 
            'ParticleDecays:tau0Max = 10000000', 
            'SUSY:all on'
        ),
        pythia8CUEP8M1Settings = cms.vstring(
            'Tune:pp 14', 
            'Tune:ee 7', 
            'MultipartonInteractions:pT0Ref=2.4024', 
            'MultipartonInteractions:ecmPow=0.25208', 
            'MultipartonInteractions:expPow=1.6'
        ),
        pythia8CommonSettings = cms.vstring(
            'Tune:preferLHAPDF = 2', 
            'Main:timesAllowErrors = 10000', 
            'Check:epTolErr = 0.01', 
            'Beams:setProductionScalesFromLHEF = off', 
            'SLHA:keepSM = on', 
            'SLHA:minMassSM = 1000.', 
            'ParticleDecays:limitTau0 = on', 
            'ParticleDecays:tau0Max = 10', 
            'ParticleDecays:allowPhotonRadiation = on'
        )
    ),
    comEnergy = cms.double(13000.0),
    filterEfficiency = cms.untracked.double(1.0),
    maxEventsToPrint = cms.untracked.int32(1),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    pythiaPylistVerbosity = cms.untracked.int32(1)
)


# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.endjob_step,process.RAWSIMoutput_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)
# filter all path with the production filter sequence
for path in process.paths:
	getattr(process,path).insert(0, process.generator)


# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
