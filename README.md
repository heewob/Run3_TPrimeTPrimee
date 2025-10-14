I use cmslpc-el9.fnal.gov and set up singularity using lines below

```source /cvmfs/cms.cern.ch/cmsset_default.sh
cmssw-el9 -p --bind /uscms_data --bind `readlink $HOME` --bind `readlink -f ${HOME}/nobackup/` --bind /cvmfs
cmsenv
voms-proxy-init --voms cms```

My CMSSW version is CMSSW_14_0_21
to run an example cfg, do `cmsRun example_cfg.py`. Settings are different between Run3 and Run2 cfgs.
