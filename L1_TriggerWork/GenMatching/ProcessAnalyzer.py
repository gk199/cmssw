import FWCore.ParameterSet.Config as cms

process = cms.Process("bsmAnalysis")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1

process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
process.load("Configuration.EventContent.EventContent_cff")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

#load run conditions
#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.Geometry.GeometryIdeal_cff')
#process.load('Configuration.Geometry.GeometryRecoDB_cff')
#process.load('Configuration.Geometry.GeometryReco_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')

process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('PhysicsTools.HepMCCandAlgos.genParticles_cfi')
#------ Declare the correct global tag ------#
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = '106X_upgrade2021_realistic_v4'

process.options   = cms.untracked.PSet(
   allowUnscheduled = cms.untracked.bool(True),
   SkipEvent = cms.untracked.vstring('ProductNotFound')
)

process.ProcessAnalyzer = cms.EDAnalyzer("GenProcessAnalyzer",
    GenParticleTag = cms.InputTag("genParticles"),
                                         verbose = cms.bool(False))

process.source = cms.Source("PoolSource",
#              fileNames = cms.untracked.vstring("file:/afs/cern.ch/work/y/yuanc/Analysis/CMSSW_9_3_15/src/HIG-RunIIFall17wmLHEGS-03808_wtFilter.root"),
#              fileNames = cms.untracked.vstring("file:/afs/cern.ch/work/y/yuanc/Analysis/CMSSW_9_3_15/src/HIG-RunIIFall17wmLHEGS-03808_woFilter.root"),
#              fileNames = cms.untracked.vstring("file:/afs/cern.ch/work/y/yuanc/Analysis/CMSSW_9_3_15/src/HIG-RunIIFall17wmLHEGS-03808_wtFiter_v2.root"),
               fileNames = cms.untracked.vstring("file:/eos/cms/store/group/dpg_hcal/comm_hcal/gillian/LLP_Run3/mh2000_mx975_pl10000_ev1000/ppTohToSS1SS2_SS1Tobb_SS2Toveve_1_withISR_step1.root"),
                            inputCommands=cms.untracked.vstring( 
        'keep *', 
        'drop *_ctppsLocalTrackLiteProducer_*_RECO'
#CTPPSPixelClusteredmDetSetVector_ctppsPixelClusters_*_RECO'
        )
)

process.TFileService = cms.Service("TFileService",
#			fileName = cms.string("filterAnalyzer_wtFilter.root")
#			fileName = cms.string("filterAnalyzer_woFilter.root")
			fileName = cms.string("GenProcessAnalyzer.root")
)

process.dump = cms.EDAnalyzer("EventContentAnalyzer")  

#process.p = cms.Path(process.dump)
process.p = cms.Path( 
#    process.genParticles*
#    process.dump*
    process.ProcessAnalyzer
    ) # * process.dump )	
