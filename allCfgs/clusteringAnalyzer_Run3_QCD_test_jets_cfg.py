from PhysicsTools.PatAlgos.tools.helpers  import getPatAlgosToolsTask
import FWCore.ParameterSet.Config as cms
from Configuration.Eras.Era_Run3_cff import Run3
process = cms.Process('MINI', Run3)
from Configuration.AlCa.GlobalTag import GlobalTag
process.load('Configuration.StandardSequences.Services_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load('JetMETCorrections.Configuration.JetCorrectors_cff')
process.load('JetMETCorrections.Configuration.CorrectedJetProducers_cff')
process.load('JetMETCorrections.Configuration.CorrectedJetProducersDefault_cff')
#process.load("JetMETCorrections.Configuration.JetCorrectionServices_cff")
#process.load("JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff")
from PhysicsTools.PatAlgos.tools.jetTools import updateJetCollection
process.GlobalTag.globaltag = '130X_mcRun3_2022_realistic_postEE_v6'
process.options = cms.untracked.PSet( allowUnscheduled = cms.untracked.bool(True) )
isData = False
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
################# JEC ################
corrLabels = ['L2Relative']
if isData:
	corrLabels.append('L2L3Residual')
from PhysicsTools.PatAlgos.tools.jetTools import *
from RecoBTag.ONNXRuntime.pfDeepBoostedJet_cff import *
updateJetCollection(
 process,
 jetSource = cms.InputTag('slimmedJetsAK8'),
 labelName = 'AK8',
 jetCorrections = ('AK8PFPuppi', cms.vstring(corrLabels), 'None'), #previous corrections: 'L2Relative', 'L3Absolute', 'L2L3Residual'
 postfix = 'UpdatedJEC',
 printWarning = False
)
corrLabels_AK4 = ['L1FastJet','L2Relative']
if isData:
 corrLabels_AK4.append('L2L3Residual')
updateJetCollection(
 process,
 jetSource = cms.InputTag('slimmedJets'),
 labelName = 'AK4',
 jetCorrections = ('AK4PFchs', cms.vstring(corrLabels_AK4), 'None'),
 postfix = 'UpdatedJEC',
 printWarning = False
)  
################# Jet PU ID ################
from RecoJets.JetProducers.PileupJetID_cfi import pileupJetId
from RecoJets.JetProducers.PileupJetID_cfi import _chsalgos_106X_UL17
process.load("RecoJets.JetProducers.PileupJetID_cfi")
process.pileupJetIdUpdated = process.pileupJetId.clone( 
jets=cms.InputTag("selectedUpdatedPatJetsAK4UpdatedJEC"),  #should be the name of the post-JEC jet collection
inputIsCorrected=True,
applyJec=False,
vertexes=cms.InputTag("offlineSlimmedPrimaryVertices"),
algos = cms.VPSet(_chsalgos_106X_UL17),
)
process.patAlgosToolsTask.add(process.pileupJetIdUpdated)
updateJetCollection(    # running in unscheduled mode, need to manually update collection
 process,
 labelName = "PileupJetID",
 jetSource = cms.InputTag("selectedUpdatedPatJetsAK4UpdatedJEC"),
)
process.updatedPatJetsPileupJetID.userData.userInts.src = ["pileupJetIdUpdated:fullId"]
process.content = cms.EDAnalyzer("EventContentAnalyzer")
##############################################################################
process.leptonVeto = cms.EDFilter("leptonVeto",
 muonCollection= cms.InputTag("slimmedMuons"),
 electronCollection = cms.InputTag("slimmedElectrons"),
 metCollection = cms.InputTag("slimmedMETs"),
 tauCollection = cms.InputTag("slimmedTaus")
)
from PhysicsTools.PatUtils.l1PrefiringWeightProducer_cfi import l1PrefiringWeightProducer
process.prefiringweight = l1PrefiringWeightProducer.clone(
TheJets = cms.InputTag('selectedUpdatedPatJetsAK4UpdatedJEC'), #this should be the slimmedJets collection with up to date JECs 
DataEraECAL = cms.string('UL2017BtoF'), #Use 2016BtoH for 2016
DataEraMuon = cms.string('20172018'), #Use 2016 for 2016
UseJetEMPt = cms.bool(False),
PrefiringRateSystematicUnctyECAL = cms.double(0.2),
PrefiringRateSystematicUnctyMuon = cms.double(0.2)
)
process.selcetionStudy_Et50 = cms.EDAnalyzer("clusteringAnalyzerAll_process",
 runType = cms.string("QCDMC1000to1500"), #types: QCDMC1000to1500, QCDMC1500to2000, QCDMC2000toInf, TTbarMC, DataA, etc. , etc.
 genPartCollection = cms.InputTag("prunedGenParticles"),
  genParticles = cms.InputTag("genParticles"),
  packedGenParticles = cms.InputTag("packedGenParticles"),
 BESTname = cms.string('BESTGraph'),  BESTpath = cms.FileInPath('data/BEST_models/constantgraph_2017.pb'),
 BESTscale = cms.FileInPath('data/BESTScalerParameters_all_mass_2017.txt'),
 PUfile_path = cms.FileInPath('data/POG/LUM/2017_UL/puWeights.json'),
 bTagEff_path = cms.FileInPath('data/btaggingEffMaps/btag_efficiency_map_QCDMC_combined_2017.root'),
 bTagSF_path = cms.FileInPath('data/bTaggingSFs/2017_UL/btagging.json'),
 JECUncert_AK8_path = cms.FileInPath("data/JEC/2017_UL/MC/Summer19UL17_V5_MC_Uncertainty_AK8PFPuppi.txt"),
 JECUncert_AK4_path = cms.FileInPath("data/JEC/2017_UL/MC/Summer19UL17_V5_MC_Uncertainty_AK4PFchs.txt"),
 fatJetCollection = cms.InputTag("selectedUpdatedPatJetsAK8UpdatedJEC"),
 jetCollection = cms.InputTag("selectedUpdatedPatJetsPileupJetID"),
 muonCollection= cms.InputTag("slimmedMuons"),
 electronCollection = cms.InputTag("slimmedElectrons"),
 metCollection = cms.InputTag("slimmedMETs"),
 tauCollection = cms.InputTag("slimmedTaus"),
 pileupCollection = cms.InputTag("slimmedAddPileupInfo"),
 systematicType = cms.string("nom"),
 jetVetoMapFile = cms.FileInPath("data/jetVetoMaps/hotjets-UL17_v2.root"),
 jetVetoMapName = cms.string("h2hot_ul17_plus_hep17_plus_hbpw89"),
 includeAllBranches = cms.bool(False),
 slimmedSelection   = cms.bool(False),
 verbose            = cms.bool(False),
 runSideband            = cms.bool(False),
 year = cms.string("2017"), #types: 2015,2016,2017,2018
 genEventInfoTag=cms.InputTag("generator"),
 lheEventInfoTag=cms.InputTag("externalLHEProducer"),
 bits = cms.InputTag("TriggerResults", "", "HLT"),
 AK8_Et_cut = cms.double(50),
 triggers = cms.string("HLT_PFHT1050_v"),
 doPUID = cms.bool(True),
  doPDF = cms.bool(True)
)
process.selcetionStudy_Et100 = cms.EDAnalyzer("clusteringAnalyzerAll_process",
 runType = cms.string("QCDMC1000to1500"), #types: QCDMC1000to1500, QCDMC1500to2000, QCDMC2000toInf, TTbarMC, DataA, etc. , etc.
 genPartCollection = cms.InputTag("prunedGenParticles"),
  genParticles = cms.InputTag("genParticles"),
  packedGenParticles = cms.InputTag("packedGenParticles"),
 BESTname = cms.string('BESTGraph'),  BESTpath = cms.FileInPath('data/BEST_models/constantgraph_2017.pb'),
 BESTscale = cms.FileInPath('data/BESTScalerParameters_all_mass_2017.txt'),
 PUfile_path = cms.FileInPath('data/POG/LUM/2017_UL/puWeights.json'),
 bTagEff_path = cms.FileInPath('data/btaggingEffMaps/btag_efficiency_map_QCDMC_combined_2017.root'),
 bTagSF_path = cms.FileInPath('data/bTaggingSFs/2017_UL/btagging.json'),
 JECUncert_AK8_path = cms.FileInPath("data/JEC/2017_UL/MC/Summer19UL17_V5_MC_Uncertainty_AK8PFPuppi.txt"),
 JECUncert_AK4_path = cms.FileInPath("data/JEC/2017_UL/MC/Summer19UL17_V5_MC_Uncertainty_AK4PFchs.txt"),
 fatJetCollection = cms.InputTag("selectedUpdatedPatJetsAK8UpdatedJEC"),
 jetCollection = cms.InputTag("selectedUpdatedPatJetsPileupJetID"),
 muonCollection= cms.InputTag("slimmedMuons"),
 electronCollection = cms.InputTag("slimmedElectrons"),
 metCollection = cms.InputTag("slimmedMETs"),
 tauCollection = cms.InputTag("slimmedTaus"),
 pileupCollection = cms.InputTag("slimmedAddPileupInfo"),
 systematicType = cms.string("nom"),
 jetVetoMapFile = cms.FileInPath("data/jetVetoMaps/hotjets-UL17_v2.root"),
 jetVetoMapName = cms.string("h2hot_ul17_plus_hep17_plus_hbpw89"),
 includeAllBranches = cms.bool(False),
 slimmedSelection   = cms.bool(False),
 verbose            = cms.bool(False),
 runSideband            = cms.bool(False),
 year = cms.string("2017"), #types: 2015,2016,2017,2018
 genEventInfoTag=cms.InputTag("generator"),
 lheEventInfoTag=cms.InputTag("externalLHEProducer"),
 bits = cms.InputTag("TriggerResults", "", "HLT"),
 AK8_Et_cut = cms.double(100),
 triggers = cms.string("HLT_PFHT1050_v"),
 doPUID = cms.bool(True),
  doPDF = cms.bool(True)
)
process.selcetionStudy_Et150 = cms.EDAnalyzer("clusteringAnalyzerAll_process",
 runType = cms.string("QCDMC1000to1500"), #types: QCDMC1000to1500, QCDMC1500to2000, QCDMC2000toInf, TTbarMC, DataA, etc. , etc.
 genPartCollection = cms.InputTag("prunedGenParticles"),
  genParticles = cms.InputTag("genParticles"),
  packedGenParticles = cms.InputTag("packedGenParticles"),
 BESTname = cms.string('BESTGraph'),  BESTpath = cms.FileInPath('data/BEST_models/constantgraph_2017.pb'),
 BESTscale = cms.FileInPath('data/BESTScalerParameters_all_mass_2017.txt'),
 PUfile_path = cms.FileInPath('data/POG/LUM/2017_UL/puWeights.json'),
 bTagEff_path = cms.FileInPath('data/btaggingEffMaps/btag_efficiency_map_QCDMC_combined_2017.root'),
 bTagSF_path = cms.FileInPath('data/bTaggingSFs/2017_UL/btagging.json'),
 JECUncert_AK8_path = cms.FileInPath("data/JEC/2017_UL/MC/Summer19UL17_V5_MC_Uncertainty_AK8PFPuppi.txt"),
 JECUncert_AK4_path = cms.FileInPath("data/JEC/2017_UL/MC/Summer19UL17_V5_MC_Uncertainty_AK4PFchs.txt"),
 fatJetCollection = cms.InputTag("selectedUpdatedPatJetsAK8UpdatedJEC"),
 jetCollection = cms.InputTag("selectedUpdatedPatJetsPileupJetID"),
 muonCollection= cms.InputTag("slimmedMuons"),
 electronCollection = cms.InputTag("slimmedElectrons"),
 metCollection = cms.InputTag("slimmedMETs"),
 tauCollection = cms.InputTag("slimmedTaus"),
 pileupCollection = cms.InputTag("slimmedAddPileupInfo"),
 systematicType = cms.string("nom"),
 jetVetoMapFile = cms.FileInPath("data/jetVetoMaps/hotjets-UL17_v2.root"),
 jetVetoMapName = cms.string("h2hot_ul17_plus_hep17_plus_hbpw89"),
 includeAllBranches = cms.bool(False),
 slimmedSelection   = cms.bool(False),
 verbose            = cms.bool(False),
 runSideband            = cms.bool(False),
 year = cms.string("2017"), #types: 2015,2016,2017,2018
 genEventInfoTag=cms.InputTag("generator"),
 lheEventInfoTag=cms.InputTag("externalLHEProducer"),
 bits = cms.InputTag("TriggerResults", "", "HLT"),
 AK8_Et_cut = cms.double(150),
 triggers = cms.string("HLT_PFHT1050_v"),
 doPUID = cms.bool(True),
  doPDF = cms.bool(True)
)
process.selcetionStudy_Et200 = cms.EDAnalyzer("clusteringAnalyzerAll_process",
 runType = cms.string("QCDMC1000to1500"), #types: QCDMC1000to1500, QCDMC1500to2000, QCDMC2000toInf, TTbarMC, DataA, etc. , etc.
 genPartCollection = cms.InputTag("prunedGenParticles"),
  genParticles = cms.InputTag("genParticles"),
  packedGenParticles = cms.InputTag("packedGenParticles"),
 BESTname = cms.string('BESTGraph'),  BESTpath = cms.FileInPath('data/BEST_models/constantgraph_2017.pb'),
 BESTscale = cms.FileInPath('data/BESTScalerParameters_all_mass_2017.txt'),
 PUfile_path = cms.FileInPath('data/POG/LUM/2017_UL/puWeights.json'),
 bTagEff_path = cms.FileInPath('data/btaggingEffMaps/btag_efficiency_map_QCDMC_combined_2017.root'),
 bTagSF_path = cms.FileInPath('data/bTaggingSFs/2017_UL/btagging.json'),
 JECUncert_AK8_path = cms.FileInPath("data/JEC/2017_UL/MC/Summer19UL17_V5_MC_Uncertainty_AK8PFPuppi.txt"),
 JECUncert_AK4_path = cms.FileInPath("data/JEC/2017_UL/MC/Summer19UL17_V5_MC_Uncertainty_AK4PFchs.txt"),
 fatJetCollection = cms.InputTag("selectedUpdatedPatJetsAK8UpdatedJEC"),
 jetCollection = cms.InputTag("selectedUpdatedPatJetsPileupJetID"),
 muonCollection= cms.InputTag("slimmedMuons"),
 electronCollection = cms.InputTag("slimmedElectrons"),
 metCollection = cms.InputTag("slimmedMETs"),
 tauCollection = cms.InputTag("slimmedTaus"),
 pileupCollection = cms.InputTag("slimmedAddPileupInfo"),
 systematicType = cms.string("nom"),
 jetVetoMapFile = cms.FileInPath("data/jetVetoMaps/hotjets-UL17_v2.root"),
 jetVetoMapName = cms.string("h2hot_ul17_plus_hep17_plus_hbpw89"),
 includeAllBranches = cms.bool(False),
 slimmedSelection   = cms.bool(False),
 verbose            = cms.bool(False),
 runSideband            = cms.bool(False),
 year = cms.string("2017"), #types: 2015,2016,2017,2018
 genEventInfoTag=cms.InputTag("generator"),
 lheEventInfoTag=cms.InputTag("externalLHEProducer"),
 bits = cms.InputTag("TriggerResults", "", "HLT"),
 AK8_Et_cut = cms.double(200),
 triggers = cms.string("HLT_PFHT1050_v"),
 doPUID = cms.bool(True),
  doPDF = cms.bool(True)
)
process.selcetionStudy_Et250 = cms.EDAnalyzer("clusteringAnalyzerAll_process",
 runType = cms.string("QCDMC1000to1500"), #types: QCDMC1000to1500, QCDMC1500to2000, QCDMC2000toInf, TTbarMC, DataA, etc. , etc.
 genPartCollection = cms.InputTag("prunedGenParticles"),
  genParticles = cms.InputTag("genParticles"),
  packedGenParticles = cms.InputTag("packedGenParticles"),
 BESTname = cms.string('BESTGraph'),  BESTpath = cms.FileInPath('data/BEST_models/constantgraph_2017.pb'),
 BESTscale = cms.FileInPath('data/BESTScalerParameters_all_mass_2017.txt'),
 PUfile_path = cms.FileInPath('data/POG/LUM/2017_UL/puWeights.json'),
 bTagEff_path = cms.FileInPath('data/btaggingEffMaps/btag_efficiency_map_QCDMC_combined_2017.root'),
 bTagSF_path = cms.FileInPath('data/bTaggingSFs/2017_UL/btagging.json'),
 JECUncert_AK8_path = cms.FileInPath("data/JEC/2017_UL/MC/Summer19UL17_V5_MC_Uncertainty_AK8PFPuppi.txt"),
 JECUncert_AK4_path = cms.FileInPath("data/JEC/2017_UL/MC/Summer19UL17_V5_MC_Uncertainty_AK4PFchs.txt"),
 fatJetCollection = cms.InputTag("selectedUpdatedPatJetsAK8UpdatedJEC"),
 jetCollection = cms.InputTag("selectedUpdatedPatJetsPileupJetID"),
 muonCollection= cms.InputTag("slimmedMuons"),
 electronCollection = cms.InputTag("slimmedElectrons"),
 metCollection = cms.InputTag("slimmedMETs"),
 tauCollection = cms.InputTag("slimmedTaus"),
 pileupCollection = cms.InputTag("slimmedAddPileupInfo"),
 systematicType = cms.string("nom"),
 jetVetoMapFile = cms.FileInPath("data/jetVetoMaps/hotjets-UL17_v2.root"),
 jetVetoMapName = cms.string("h2hot_ul17_plus_hep17_plus_hbpw89"),
 includeAllBranches = cms.bool(False),
 slimmedSelection   = cms.bool(False),
 verbose            = cms.bool(False),
 runSideband            = cms.bool(False),
 year = cms.string("2017"), #types: 2015,2016,2017,2018
 genEventInfoTag=cms.InputTag("generator"),
 lheEventInfoTag=cms.InputTag("externalLHEProducer"),
 bits = cms.InputTag("TriggerResults", "", "HLT"),
 AK8_Et_cut = cms.double(250),
 triggers = cms.string("HLT_PFHT1050_v"),
 doPUID = cms.bool(True),
  doPDF = cms.bool(True)
)
process.selcetionStudy_Et300 = cms.EDAnalyzer("clusteringAnalyzerAll_process",
 runType = cms.string("QCDMC1000to1500"), #types: QCDMC1000to1500, QCDMC1500to2000, QCDMC2000toInf, TTbarMC, DataA, etc. , etc.
 genPartCollection = cms.InputTag("prunedGenParticles"),
  genParticles = cms.InputTag("genParticles"),
  packedGenParticles = cms.InputTag("packedGenParticles"),
 BESTname = cms.string('BESTGraph'),  BESTpath = cms.FileInPath('data/BEST_models/constantgraph_2017.pb'),
 BESTscale = cms.FileInPath('data/BESTScalerParameters_all_mass_2017.txt'),
 PUfile_path = cms.FileInPath('data/POG/LUM/2017_UL/puWeights.json'),
 bTagEff_path = cms.FileInPath('data/btaggingEffMaps/btag_efficiency_map_QCDMC_combined_2017.root'),
 bTagSF_path = cms.FileInPath('data/bTaggingSFs/2017_UL/btagging.json'),
 JECUncert_AK8_path = cms.FileInPath("data/JEC/2017_UL/MC/Summer19UL17_V5_MC_Uncertainty_AK8PFPuppi.txt"),
 JECUncert_AK4_path = cms.FileInPath("data/JEC/2017_UL/MC/Summer19UL17_V5_MC_Uncertainty_AK4PFchs.txt"),
 fatJetCollection = cms.InputTag("selectedUpdatedPatJetsAK8UpdatedJEC"),
 jetCollection = cms.InputTag("selectedUpdatedPatJetsPileupJetID"),
 muonCollection= cms.InputTag("slimmedMuons"),
 electronCollection = cms.InputTag("slimmedElectrons"),
 metCollection = cms.InputTag("slimmedMETs"),
 tauCollection = cms.InputTag("slimmedTaus"),
 pileupCollection = cms.InputTag("slimmedAddPileupInfo"),
 systematicType = cms.string("nom"),
 jetVetoMapFile = cms.FileInPath("data/jetVetoMaps/hotjets-UL17_v2.root"),
 jetVetoMapName = cms.string("h2hot_ul17_plus_hep17_plus_hbpw89"),
 includeAllBranches = cms.bool(False),
 slimmedSelection   = cms.bool(False),
 verbose            = cms.bool(False),
 runSideband            = cms.bool(False),
 year = cms.string("2017"), #types: 2015,2016,2017,2018
 genEventInfoTag=cms.InputTag("generator"),
 lheEventInfoTag=cms.InputTag("externalLHEProducer"),
 bits = cms.InputTag("TriggerResults", "", "HLT"),
 AK8_Et_cut = cms.double(300),
 triggers = cms.string("HLT_PFHT1050_v"),
 doPUID = cms.bool(True),
  doPDF = cms.bool(True)
)
process.source = cms.Source("PoolSource",
fileNames = cms.untracked.vstring("/store/mc/Run3Winter22MiniAOD/QCD_Pt_1000to1400_TuneCP5_13p6TeV_pythia8/MINIAODSIM/122X_mcRun3_2021_realistic_v9-v2/60000/020e26a2-b69c-4b53-b1f7-62ff69d2c924.root"
)
)
process.TFileService = cms.Service("TFileService",fileName = cms.string("clusteringAnalyzer_process_run3_test_QCD_output.root")
)
process.options = cms.untracked.PSet(
 wantSummary = cms.untracked.bool(True),
)
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.p = cms.Path(  process.pileupJetIdUpdated * process.leptonVeto * process.prefiringweight   * process.selcetionStudy_Et50
 * process.selcetionStudy_Et100
 * process.selcetionStudy_Et150
 * process.selcetionStudy_Et200
 * process.selcetionStudy_Et250
 * process.selcetionStudy_Et300
)
process.patAlgosToolsTask = getPatAlgosToolsTask(process)
process.pathRunPatAlgos = cms.Path(process.patAlgosToolsTask)
