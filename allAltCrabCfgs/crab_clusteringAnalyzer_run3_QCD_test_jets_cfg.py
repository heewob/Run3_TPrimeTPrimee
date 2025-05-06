from CRABClient.UserUtilities import config
config = config()
config.General.requestName = 'clustAlg_run3_test_QCD_nom_AltDatasets_jets'
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.JobType.allowUndistributedCMSSW = True
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = '../allCfgs/clusteringAnalyzer_run3_QCD_test_jets_cfg.py'
config.Data.inputDataset = '/QCD_Pt_1000to1400_TuneCP5_13p6TeV_pythia8/Run3Winter22MiniAOD-122X_mcRun3_2021_realistic_v9-v2/MINIAODSIM'
config.Data.publication = False
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.JobType.maxMemoryMB = 3000 # might be necessary for some of the QCD jobs
config.JobType.maxMemoryMB = 3000 # might be necessary for some of the TTjet jobs
config.Data.outputDatasetTag = 'clustAlg_run3_test_QCD_jets'
config.Data.outLFNDirBase = '/store/user/chungh/TprimeTprime_2025Feb'
config.Site.storageSite = 'T3_US_FNALLPC'
