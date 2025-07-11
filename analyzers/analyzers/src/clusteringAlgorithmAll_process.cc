////////////////////////////HELP////////////////////////////////
//////////////Uses new clustering algorithm to capture heavy resonance jet substructure//////////////
////////////////Last updated 17 Jul 2024 ////////////////////////////////////////////////////////////

//_top_
// system include files
#include <fastjet/JetDefinition.hh>
#include <fastjet/GhostedAreaSpec.hh>
#include <fastjet/PseudoJet.hh>
#include <fastjet/tools/Filter.hh>
#include <fastjet/ClusterSequence.hh>
//#include <fastjet/ActiveAreaSpec.hh>
#include <fastjet/ClusterSequenceArea.hh>
#include "FWCore/Framework/interface/EventSetup.h"
#include <memory>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <math.h>
#include "TH2.h"
#include<TRandom3.h>

#include "correction.h"
#include "ROOT/RDataFrame.hxx"
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
//#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CondFormats/DataRecord/interface/JetResolutionRcd.h"
#include "CondFormats/DataRecord/interface/JetResolutionScaleFactorRcd.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/StreamID.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "PhysicsTools/CandUtils/interface/EventShapeVariables.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
// new includes
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/Math/interface/PtEtaPhiMass.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "PhysicsTools/CandUtils/interface/Thrust.h"
//#include "Thrust.h"
#include <TTree.h>
#include <cmath>
#include "TLorentzVector.h"
#include "TVector3.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include <algorithm>   
#include "DataFormats/PatCandidates/interface/MET.h"

#include "TTree.h"
#include "TFile.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "JetMETCorrections/JetCorrector/interface/JetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetResolutionObject.h"
#include "JetMETCorrections/Modules/interface/JetResolution.h"
#include "PhysicsTools/PatUtils/interface/SmearedJetProducerT.h"

//#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <string>
#include <complex>
#include "sortJets.h"
//#include "BESTtoolbox.h"
//#include "CacheHandler.h"
//#include "BESTEvaluation.h"

#include "LHAPDF/LHAPDF.h"
#include "LHAPDF/Reweighting.h"

//run3 include
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/stream/EDAnalyzer.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

//LUND
#include "fastjet/contrib/LundGenerator.hh"
#include "fastjet/ClusterSequence.hh"

//#include "AnalysisDataFormats/TopObjects/interface/TtGenEvent.h"
//#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
using namespace reco;
using namespace correction;
typedef math::XYZTLorentzVector LorentzVector;
typedef math::XYZVector Vector;
using correction::CorrectionSet;
using namespace LHAPDF;
class clusteringAnalyzerAll_process : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
   explicit clusteringAnalyzerAll_process(const edm::ParameterSet&);
private:

   virtual void analyze(const edm::Event&, const edm::EventSetup&);
   double calcMPP(TLorentzVector superJetTLV ); 
   bool isMatchedtoSJ(std::vector<TLorentzVector> superJetTLVs, TLorentzVector candJet); 
   bool fillSJVars(std::map<std::string, float> &treeVars, std::vector<fastjet::PseudoJet> iSJ, int nSuperJets);
   bool isgoodjet(const float eta, const float NHF,const float NEMF, const size_t NumConst,const float CHF,const int CHM, const float MUF, const float CEMF, bool jetPUid, const float iJet_pt);
   bool isgoodjet(const float eta, const float NHF,const float NEMF, const size_t NumConst,const float CHF,const int CHM, const float MUF, const float CEMF, int nfatjets);
   bool isHEM(const float jet_eta, const float jet_phi);
   double top_pt_SF(double top_pt);
   double calcAlphas(double q2);
   double calcRenormWeight(double q2, int up_or_dn, int nQCD);
   double calcFactorizWeight(LHAPDF::PDF* pdf, double id1, double id2, double x1, double x2, double q2, int up_or_dn);
   std::string returnJECFile(std::string year, std::string systematicType, std::string jet_type, std::string runType);
   double getJECUncertaintyFromSources(std::string jet_type, double pt, double eta);
   const reco::Candidate* parse_chain(const reco::Candidate* cand);
   bool applyJERSource(std::string uncertainty_source, double eta);
   std::map<std::string, std::map<std::string, std::string>> file_map;

   //const reco::Candidate* parse_chain(const reco::Candidate*);

   //init all inpaths, tokens, instrings
   edm::EDGetTokenT<std::vector<pat::Jet>> fatJetToken_;
   edm::EDGetTokenT<std::vector<reco::GenParticle>> genParticleToken_; 
   edm::EDGetTokenT<std::vector<reco::GenParticle>> genPartToken_; 
   edm::EDGetTokenT<std::vector<reco::GenParticle>> packedGenParticleToken_; 
   //edm::EDGetTokenT<LHEEventProduct> lheEventProductToken_;
   edm::EDGetTokenT<std::vector<pat::Jet>> jetToken_;
   edm::EDGetTokenT<std::vector<pat::MET>> metToken_;
   edm::EDGetTokenT<std::vector<PileupSummaryInfo> > puSummaryToken_;
   edm::EDGetTokenT<GenEventInfoProduct> GeneratorToken_;
   edm::EDGetTokenT<std::vector<float>> pdfWeightToken_;
   edm::EDGetTokenT<std::vector<float>> scaleWeightToken_;
   edm::EDGetTokenT< double > prefweight_token;
   edm::EDGetTokenT< double > prefweightup_token;
   edm::EDGetTokenT< double > prefweightdown_token;
   edm::EDGetTokenT<edm::TriggerResults> triggerBits_;
   edm::EDGetTokenT<double> m_rho_token;

   /*
   //Run3 CMSSW14 new API for JER resolution
   edm::ESGetToken<JME::JetResolution, JetResolutionRcd> AK4JER_token_;
   edm::ESGetToken<JME::JetResolutionScaleFactor, JetResolutionScaleFactorRcd> AK4JERSF_token_;
   edm::ESGetToken<JME::JetResolution, JetResolutionRcd> AK8JER_token_;
   edm::ESGetToken<JME::JetResolutionScaleFactor, JetResolutionScaleFactorRcd> AK8JERSF_token_;
   */


   TTree * tree;
   //BESTEvaluation* BEST_;
   //const CacheHandler* cache_;
   edm::FileInPath path_;
   edm::FileInPath bTagSF_path;
   edm::FileInPath bTagEff_path;
   edm::FileInPath JECUncert_AK8_path;
   edm::FileInPath JECUncert_AK4_path;

   edm::FileInPath PUfile_path;
   std::string runType;
   std::string systematicType;
   std::string year;
   std::string lumiTag;

   std::string jetVetoMapName;
   edm::FileInPath jetVetoMapFile;

   std::vector<std::string> triggers;

   bool doPUID = false;
   bool doPDF = false;
   int eventnum = 0;
   int nAK4 = 0;
   int nfatjets = 0;
   int raw_nfatjets;
   int tot_nAK4_50 =0,tot_nAK4_70 = 0;
   int tot_mpp_AK4 = 0;
   //std::map<std::string, float> BESTmap;

   //event process variables
   int nWbHt = 0;
   int nWbZt = 0;
   int nWbWb = 0;
   int nHtZt = 0;
   int nHtHt = 0;
   int nZtZt = 0;

   int jetMultiplicity;
   int process;


   //init event variables
   bool doJEC              = true;
   bool doJER              = false;  // will be set to true for MC
   bool doBtagSF           = false;  // will be set to true for MC
   bool doPUSF             = false;  // will be set to true for MC
   bool doTopPtReweight    = false;  // will be set to true for ttbar MC
   bool doPDFWeights       = false;  // will be set to true for MC
   bool doPrefiringWeight  = true;   
   bool includeAllBranches = false;  // don't include some branches that are not often used
   bool slimmedSelection   = false;  // apply a more strict selection to save space, set through cfg
   bool _verbose           = false;  // do printouts of each section for debugging, set through cfg
   bool debug              = false;  // print out some other debug stuff, set through cfg
   bool runSideband        = false;  // change the HT region, set through cfg
   int SJ_nAK4_50[100],SJ_nAK4_70[100],SJ_nAK4_100[100],SJ_nAK4_125[100],SJ_nAK4_150[100],SJ_nAK4_200[100],SJ_nAK4_300[100],SJ_nAK4_400[100],SJ_nAK4_600[100],SJ_nAK4_800[100],SJ_nAK4_1000[100];
   double jet_pt[100], jet_eta[100], jet_mass[100], jet_dr[100], raw_jet_mass[100],raw_jet_pt[100],raw_jet_phi[100];
   double SJ_mass_50[100], SJ_mass_70[100],SJ_mass_100[100],superJet_mass[100],SJ_AK4_50_mass[100],SJ_AK4_70_mass[100];
   double posSJ_mass,negSJ_mass;
   double SJ_mass_125[100], SJ_mass_150[100], SJ_mass_200[100],SJ_mass_300[100],SJ_mass_400[100],SJ_mass_600[100],SJ_mass_800[100],SJ_mass_1000[100];   
   double AK4_mass[100], AK4_E[500], leadAK8_mass[10];
   double AK8_Et_cut;
   double event_reco_pt;
   double diSuperJet_mass,diSuperJet_mass_100;
   double top_pt_weight;
   double totHT = 0;
   double dijetMassOne, dijetMassTwo;
   int nfatjet_pre = 0;
   int nSuperJets = 0;
   double AK4_bdisc[100], AK4_DeepJet_disc[100];
   int jet_ndaughters[100], jet_nAK4[100],jet_nAK4_20[100],jet_nAK4_30[100],jet_nAK4_50[100],jet_nAK4_70[100],jet_nAK4_100[100],jet_nAK4_150[100];
   double totMET;
   int lab_nAK4 = 0;
   double lab_AK4_pt[100];
   //double btag_score_uncut[100];
   int nAK4_uncut = 0;
   int nGenBJets_AK4[100],AK4_hadronFlavour[100], AK4_partonFlavour[100];
   int nCategories = 3;
   double AK4_ptot[100], AK4_eta[100], AK4_phi[100];
   int SJ1_decision,SJ2_decision;
   //double SJ1_BEST_scores[3],SJ2_BEST_scores[3];
   double bTag_eventWeight_T_nom, bTag_eventWeight_T_up, bTag_eventWeight_T_down, bTag_eventWeight_bc_T_up, bTag_eventWeight_bc_T_down, bTag_eventWeight_light_T_up, bTag_eventWeight_light_T_down;
   double bTag_eventWeight_M_nom, bTag_eventWeight_M_up, bTag_eventWeight_M_down, bTag_eventWeight_bc_M_up, bTag_eventWeight_bc_M_down, bTag_eventWeight_light_M_up, bTag_eventWeight_light_M_down;
   bool _htWb,_htZt,_ZtWb,_WbWb,_htht,_ZtZt;
   //BES variables
   double AK4_m1[2], AK4_m2[2],AK4_m3[2],AK4_m4[2], AK41_E_tree[2],AK42_E_tree[2],AK43_E_tree[2],AK44_E_tree[2], AK4_m12[2],AK4_m13[2],AK4_m14[2],AK4_m23[2],AK4_m24[2],AK4_m34[2];
   double AK4_m123[2],AK4_m124[2],AK4_m134[2],AK4_m234[2], AK4_m1234[2],AK4_theta12[2],AK4_theta13[2],AK4_theta14[2],AK4_theta23[2],AK4_theta24[2],AK4_theta34[2];
   int AK41_ndaughters[2], AK41_nsubjets[2];
   double AK41_thrust[2],AK41_sphericity[2],AK41_asymmetry[2],AK41_isotropy[2],AK41_aplanarity[2],AK41_FW1[2],AK41_FW2[2],AK41_FW3[2],AK41_FW4[2];
   int AK42_ndaughters[2], AK42_nsubjets[2]; 
   double AK42_thrust[2],AK42_sphericity[2],AK42_asymmetry[2],AK42_isotropy[2],AK42_aplanarity[2],AK42_FW1[2],AK42_FW2[2],AK42_FW3[2],AK42_FW4[2];
   int AK43_ndaughters[2], AK43_nsubjets[2]; 
   double AK43_thrust[2],AK43_sphericity[2],AK43_asymmetry[2],AK43_isotropy[2],AK43_aplanarity[2],AK43_FW1[2],AK43_FW2[2],AK43_FW3[2],AK43_FW4[2];
   double SJ_thrust[2],SJ_sphericity[2],SJ_asymmetry[2],SJ_isotropy[2],SJ_aplanarity[2],SJ_FW1[2],SJ_FW2[2],SJ_FW3[2],SJ_FW4[2];

   double JEC_uncert_AK8[100],JEC_uncert_AK4[100];
   //MP superjet AK4 jet momenta and energies, tedious,but means we can do some SJ calculations locally, these will also become BEST variables
   double SJ1_AK4_px[20], SJ1_AK4_py[20], SJ1_AK4_pz[20], SJ1_AK4_E[20];
   double SJ2_AK4_px[20], SJ2_AK4_py[20], SJ2_AK4_pz, SJ2_AK4_E[20];

   double AK8_hadronFlavour[100], AK8_partonFlavour[100];
   int AK8_SJ_assignment[100], AK4_SJ_assignment[100];
   bool passesPFHT = false, passesPFJet = false;
   double jet_phi[100];
   int ntrueInt;
   double diAK8Jet_mass[100];
   double fourAK8JetMass;
   int nAK8diJets = 2;

   double PU_eventWeight_up, PU_eventWeight_nom, PU_eventWeight_down;
   double deepJet_wp_loose, deepJet_wp_med, deepjet_wp_tight;

   double AK8_JER[25], AK8_JEC[25];
   double weightFacUp,weightFacDn,weightRenUp,weightRenUpxweightFacUp,weightRenUpxweightFacDn;
   double weightRenDn,weightRenDnxweightFacUp,weightRenDnxweightFacDn;

   double jet_bTagSF_b_T[100], jet_bTagSF_c_T[100], jet_bTagSF_light_T[100];
   double jet_bTagSF_b_M[100], jet_bTagSF_c_M[100], jet_bTagSF_light_M[100];

   int nTrue_b_AK4, nTrue_c_AK4, nTrue_light_AK4;
   double pdf_weights[200];
   double x1,x2,q2;
   int id1,id2;
   int eventNumber;

   int lab_AK4_AK8_parent[100]; // the nfatjet index of of the AK8 jet in which selected AK4 jets reside
   bool AK8_is_near_highE_CA4[100], AK4_is_near_highE_CA4[100];

   bool fatjet_isHEM[100], jet_pre_isHEM[100], jet_isHEM[100];
   TH2F *truebjet_eff,*truecjet_eff, *lightjet_eff;
   TH2F *truebjet_eff_med,*truecjet_eff_med, *lightjet_eff_med;

   TH2F * jetVetoMap;
   //double prefiringWeight_nom, prefiringWeight_up, prefiringWeight_down;

   double bTagEffMap_PtRange, bTagEffMap_Eta_high, bTagEffMap_Eta_low;
   int bTagEffMap_nPtBins, bTagEffMap_nEtaBins;

   bool AK8_fails_veto_map[100], AK4_fails_veto_map[100];
   // PDF weight stuff
   int nPDFWeights, nScaleWeights;
   float pdfWeights[200], scaleWeights[200];
   double PDFWeights_alphas, PDFWeights_varWeightsRMS,PDFWeights_varWeightsErr;
   double PDFWeights_renormWeights[2],PDFWeights_factWeightsRMSs[2];
   int PDFWeights_nVars;
   double alphas;
   int nVars; 

   double factWeightsRMSs[2]; // Up, down
   double varWeightsRMS;
   double varWeightsErr;
   double renormWeights[2]; //Up, down
   double scale_envelope[10];

   int LHAPDF_NOM;
   int LHAPDF_VAR_LOW;
   int LHAPDF_VAR_HIGH;
   PDF* nomPDF;
   PDF* varPDFs[102];
   double PDFWeights_factWeightsRMS_up, PDFWeights_factWeightsRMS_down;
   double PDFWeights_renormWeight_up, PDFWeights_renormWeight_down;
   double PDFWeightUp, PDFWeightDown;  // BEST-style PDF weights
   TRandom3 *randomNum = new TRandom3(); // for JERs



   int nHeavyAK8_pt400_M10 = 0, nHeavyAK8_pt400_M20 = 0, nHeavyAK8_pt400_M30 = 0;
   int nHeavyAK8_pt300_M10 = 0, nHeavyAK8_pt300_M20 = 0, nHeavyAK8_pt300_M30 = 0; 
   int nHeavyAK8_pt200_M10 = 0, nHeavyAK8_pt200_M20 = 0, nHeavyAK8_pt200_M30 = 0; 

   /*
   int nLowMassJetCR_M30 = 0;
   int nLowMassJetCR_M20 = 0;
   int nEventsHighMass = 0;
   int nLowMassJetCR_M45 = 0;
   */
   // btag scale factor stuff
   std::unique_ptr<CorrectionSet> cset;
   Correction::Ref cset_corrector_bc;
   Correction::Ref cset_corrector_light; 

   // Jet correction uncertainty classes
   JetCorrectionUncertainty *jecUnc_AK4;
   JetCorrectionUncertainty *jecUnc_AK8;
   std::unique_ptr<CorrectionSet> PUjson;
   Correction::Ref PUjson_year;

   // jet veto map stuff
   double jetVetoMap_XRange, jetVetoMap_YRange, jetVetoMap_Xmin, jetVetoMap_Ymin;
   int jetVetoMap_nBinsX, jetVetoMap_nBinsY;

   //double QCDFactorization_up_BEST, QCDFactorization_down_BEST, QCDRenormalization_up_BEST, QCDRenormalization_down_BEST;

   std::vector<std::string> uncertainty_sources;
   std::map<std::string, std::unique_ptr<JetCorrectionUncertainty>> JEC_map_AK4;   // contains the correctors for each uncertainty source
   std::map<std::string, std::unique_ptr<JetCorrectionUncertainty>> JEC_map_AK8;   // contains the correctors for each uncertainty source

    //LUND
   std::vector<std::vector<float>> jet_lund_dR;
   std::vector<std::vector<float>> jet_lund_kT;


};

//_constructor_
clusteringAnalyzerAll_process::clusteringAnalyzerAll_process(const edm::ParameterSet& iConfig):
   path_ (iConfig.getParameter<edm::FileInPath>("BESTpath"))

{
   usesResource("TFileService");

   // import parameters from cfg, set variables needed to analyze the event
   runType        = iConfig.getParameter<std::string>("runType");
   systematicType = iConfig.getParameter<std::string>("systematicType");
   year           = iConfig.getParameter<std::string>("year");

   // necessary for importing NN file
   //cache_ = new CacheHandler(path_);
   //BEST_  = new BESTEvaluation(cache_);
   //BEST_->configure(iConfig);

   includeAllBranches = iConfig.getParameter<bool>("includeAllBranches");
   slimmedSelection   = iConfig.getParameter<bool>("slimmedSelection");
   _verbose           = iConfig.getParameter<bool>("verbose");
   runSideband        = iConfig.getParameter<bool>("runSideband");

   jetVetoMapName = iConfig.getParameter<std::string>("jetVetoMapName");
   jetVetoMapFile = iConfig.getParameter<edm::FileInPath>("jetVetoMapFile");

   doPDF = iConfig.getParameter<bool>("doPDF");
   AK8_Et_cut = iConfig.getParameter<double>("AK8_Et_cut");

   // "pdfWeights" "NNPDF31"

   //triggers     = iConfig.getParameter<std::string>("triggers"); // not currently set up: am setting triggers manually in this file
   triggerBits_ = consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("bits"));

   if( (runType.find("MC") != std::string::npos) || (runType.find("Tprime") != std::string::npos))    //don't want these variables for data
   {
      // these will only be done for MC
      doBtagSF = true;
      doJER    = true;
      doPUSF   = true;
      if(doPDF)doPDFWeights = true; 
      else {doPDFWeights = false;}

      if ((runType.find("TTTo") != std::string::npos) || (runType.find("TTJets") != std::string::npos)) doTopPtReweight = true;  

      bTagSF_path    = iConfig.getParameter<edm::FileInPath>("bTagSF_path");
      bTagEff_path   = iConfig.getParameter<edm::FileInPath>("bTagEff_path");

      /*if(doPDFWeights)
      {

         GeneratorToken_       = consumes<GenEventInfoProduct>(iConfig.getParameter<edm::InputTag>("genEventInfoTag")); //generator
         //lheEventProductToken_ = consumes<LHEEventProduct> (edm::InputTag("externalLHEProducer", "", "GEN"));
         //std::cout << "Getting the PDF weight tokens." << std::endl;
         //pdfWeightToken_ = consumes<std::vector<float>>(edm::InputTag("PDFweights")); // calculated with PDFRecalculator
         //scaleWeightToken_ = consumes<std::vector<float>>(edm::InputTag("ScaleWeights")); // calculated with PDFRecalculator
         //std::cout << "Got the PDF weight tokens." << std::endl;
         //pdfWeightToken_ = consumes<std::vector<float>>(edm::InputTag("PDFRecalculator:PDFweights"));
         //scaleWeightToken_ = consumes<std::vector<float>>(edm::InputTag("PDFRecalculator:ScaleWeights"));

         if((runType.find("QCD") != std::string::npos) || ((runType.find("TTJets") != std::string::npos)))
         {
            LHAPDF_NOM = 325300;
            LHAPDF_VAR_LOW = 325301;
            LHAPDF_VAR_HIGH = 325402;
         }
         else if(runType.find("Tprime") != std::string::npos)
         {
            LHAPDF_NOM = 325500;
            LHAPDF_VAR_LOW = 325501;
            LHAPDF_VAR_HIGH = 325600;
         }
         else   // NOT correct for TTbar
         {
            LHAPDF_NOM = 325300;
            LHAPDF_VAR_LOW = 325301;
            LHAPDF_VAR_HIGH = 325402;
         }

         nVars = LHAPDF_VAR_HIGH - LHAPDF_VAR_LOW + 1;
         std::cout << "The PDF sets are LHAPDF_VAR_LOW-LHAPDF_VAR_HIGH: " << LHAPDF_VAR_LOW << "/" << LHAPDF_VAR_HIGH << std::endl;
      } */
      
   }
   PUfile_path        = iConfig.getParameter<edm::FileInPath>("PUfile_path");
   JECUncert_AK8_path = iConfig.getParameter<edm::FileInPath>("JECUncert_AK8_path");
   JECUncert_AK4_path = iConfig.getParameter<edm::FileInPath>("JECUncert_AK4_path");

   // prefiring weights
   //prefweight_token     = consumes< double >(edm::InputTag("prefiringweight:nonPrefiringProb"));
   //prefweightup_token   = consumes< double >(edm::InputTag("prefiringweight:nonPrefiringProbUp"));
   //prefweightdown_token = consumes< double >(edm::InputTag("prefiringweight:nonPrefiringProbDown"));

   doPUID = iConfig.getParameter<bool>("doPUID");

   /*
   //JER tokens added for Run3
   AK4JER_token_ = esConsumes<JME::JetResolution, JetResolutionRcd>();
   AK4JERSF_token_ = esConsumes<JME::JetResolutionScaleFactor, JetResolutionScaleFactorRcd>();
   AK8JER_token_ = esConsumes<JME::JetResolution, JetResolutionRcd>();
   AK8JERSF_token_ = esConsumes<JME::JetResolutionScaleFactor, JetResolutionScaleFactorRcd>();
   */

   ///////////////////////////////////////
   /////// JEC Uncertainty stuff /////////
   ///////////////////////////////////////


   //initialize JEC text files (should be possible to open these here )

   /////////////////////////////////////////////////////////////////////////
   //////////////// create file maps for the JEC sources ///////////////////

   file_map["2015"]["MC"] = "Summer19UL16APV_V9_MC";
   file_map["2016"]["MC"] = "Summer19UL16_V9_MC";
   file_map["2017"]["MC"] = "Summer19UL17_V6_MC";
   file_map["2018"]["MC"] = "Summer19UL18_V5_MC";

   file_map["2015"]["dataBCD"] = "Summer19UL16APV_RunBCD_V7_DATA";
   file_map["2015"]["dataEF"]  = "Summer19UL16APV_RunEF_V7_DATA";

   file_map["2016"]["dataFGH"] = "Summer19UL16_RunFGH_V7_DATA";

   file_map["2017"]["dataB"] = "Summer19UL17_RunB_V6_DATA";
   file_map["2017"]["dataC"] = "Summer19UL17_RunC_V6_DATA";
   file_map["2017"]["dataD"] = "Summer19UL17_RunD_V6_DATA";
   file_map["2017"]["dataE"] = "Summer19UL17_RunE_V6_DATA";
   file_map["2017"]["dataF"] = "Summer19UL17_RunF_V6_DATA";

   file_map["2018"]["dataA"] = "Summer19UL18_RunA_V6_DATA";
   file_map["2018"]["dataB"] = "Summer19UL18_RunB_V6_DATA";
   file_map["2018"]["dataC"] = "Summer19UL18_RunC_V6_DATA";
   file_map["2018"]["dataD"] = "Summer19UL18_RunD_V6_DATA";


   if(_verbose)std::cout << "Getting text file names." << std::endl;
   edm::FileInPath  JEC_source_text_AK4 = (edm::FileInPath )returnJECFile(year, systematicType, "AK4", runType );
   edm::FileInPath  JEC_source_text_AK8 = (edm::FileInPath )returnJECFile(year, systematicType, "AK8", runType );

   if(_verbose)std::cout << "AK4/AK8 text files are " << JEC_source_text_AK4 << "--------" << JEC_source_text_AK8 << std::endl;
   // OLD way of doing JEC uncertainties, these are paths to the total uncertainties
   //jecUnc_AK4 = new JetCorrectionUncertainty(JECUncert_AK4_path.fullPath().c_str());
   //jecUnc_AK8 = new JetCorrectionUncertainty(JECUncert_AK8_path.fullPath().c_str());

   if(_verbose)std::cout << "Finished getting JetCorrectionUncertainty objects from text files." << std::endl;

   fatJetToken_ = consumes<std::vector<pat::Jet>>(iConfig.getParameter<edm::InputTag>("fatJetCollection"));
   jetToken_    = consumes<std::vector<pat::Jet>>(iConfig.getParameter<edm::InputTag>("jetCollection"));

   m_rho_token  = consumes<double>(edm::InputTag("fixedGridRhoAll", "", "RECO"));
   edm::Service<TFileService> fs;     
   std::string Et_str = std::to_string(AK8_Et_cut); 
   tree = fs->make<TTree>(  ("tree_"+systematicType+"_Et"+Et_str).c_str(), ("tree_"+systematicType+"_Et"+Et_str).c_str());
   TFile *bTagEff_file;
   TFile *jetVetoMap_file;

   if(year == "2018")
   {
      //deepJet_wp_loose = 0.0490;
      deepJet_wp_med   = 0.2783;
      deepjet_wp_tight = 0.7100;
      lumiTag = "Collisions18_UltraLegacy_goldenJSON";
      triggers = {"HLT_PFJet500_v", "HLT_PFHT1050_v"};
   }
   else if(year == "2017")
   {
      //deepJet_wp_loose = 0.0532;
      deepJet_wp_med   = 0.3040;
      deepjet_wp_tight = 0.7476;
      lumiTag = "Collisions17_UltraLegacy_goldenJSON";
      triggers = {"HLT_PFJet500_v", "HLT_PFHT1050_v"};
   }
   else if(year == "2016")
   {
      //deepJet_wp_loose = 0.0480;
      deepJet_wp_med   = 0.2489;
      deepjet_wp_tight = 0.6377;
      lumiTag = "Collisions16_UltraLegacy_goldenJSON";
      triggers  = {"HLT_PFJet450_v","HLT_PFHT900_v"};
   }
   else if(year == "2015")
   {
      //deepJet_wp_loose = 0.0508;
      deepJet_wp_med   = 0.2598;
      deepjet_wp_tight = 0.6502;
      lumiTag = "Collisions16_UltraLegacy_goldenJSON";
      triggers = {"HLT_PFJet450_v","HLT_PFHT900_v"};
   }
   else
   {
      std::cout << "Incorrect year: " << year << std::endl;
      return; // return cut, input year is incorrect
   }


   // load in the JetCorrectors for each uncertainty source for the given reduced uncertainty source
   // systematics = [ "JEC_FlavorQCD", "JEC_RelativeBal", "JEC_HF", "JEC_BBEC1", "JEC_EC2", "JEC_Absolute", "JEC_BBEC1_year", "JEC_EC2_year", "JEC_Absolute_year", "JEC_HF_year", "JEC_RelativeSample_year", "JER_eta193", "JER_193eta25",  "JEC","JER","nom" ]

   if ((systematicType == "JEC_up" )|| (systematicType == "JEC_down")  )   // total uncertainty 
   {
      uncertainty_sources = {"Total"};
   }

   else if ((systematicType.find("BBEC1_year") != std::string::npos) )   uncertainty_sources = {"RelativeJEREC1","RelativePtEC1","RelativeStatEC"};
   else if ((systematicType.find("EC2_year") != std::string::npos) )     uncertainty_sources = {"RelativeJEREC2","RelativePtEC2"};
   else if ((systematicType.find("Absolute_year") != std::string::npos) )  uncertainty_sources = {"AbsoluteStat","RelativeStatFSR","TimePtEta"};
   else if ((systematicType.find("HF_year") != std::string::npos) )      uncertainty_sources = {"RelativeStatHF"};
   else if ((systematicType.find("RelativeSample_year") != std::string::npos))  uncertainty_sources = {"RelativeSample"};
   else if ((systematicType.find("FlavorQCD") != std::string::npos) )    uncertainty_sources = {"FlavorQCD"};
   else if ((systematicType.find("RelativeBal") != std::string::npos))  uncertainty_sources = {"RelativeBal"};
   else if ((systematicType.find("HF") != std::string::npos) )           uncertainty_sources = {"PileUpPtHF", "RelativeJERHF","RelativePtHF"};
   else if ((systematicType.find("BBEC1") != std::string::npos) )        uncertainty_sources = {"PileUpPtBB", "PileUpPtEC1", "RelativePtBB"};
   else if ((systematicType.find("EC2") != std::string::npos) )          uncertainty_sources = {"PileUpPtEC2"};
   else if ((systematicType.find("Absolute") != std::string::npos) )     uncertainty_sources = {"AbsoluteMPFBias", "AbsoluteScale", "Fragmentation","PileUpDataMC","PileUpPtRef","RelativeFSR","SinglePionECAL","SinglePionHCAL"};
   else {std::cout << "Running uncertainty " << systematicType << std::endl;}
   // get jet corrector maps   
   if(_verbose)std::cout << "Getting JEC correction maps for AK4 and AK8 jets." << std::endl;

   for(const auto &uncertainty_source: uncertainty_sources)
   {

      if(_verbose)std::cout <<  "----- AK4 jets: -----" << std::endl;
      if(_verbose)std::cout <<  "uncertainty_source: " << uncertainty_source << std::endl;
      JetCorrectorParameters source_parameters_reduced_AK4(JEC_source_text_AK4.fullPath().c_str(), uncertainty_source);
      if(_verbose)std::cout << "AK4: JEC_source_text_AK4.fullPath().c_str() is " << JEC_source_text_AK4.fullPath().c_str() << std::endl;
      if(_verbose)std::cout <<  "AK4: creating the pointer for JetCorrectionUncertainty" << std::endl;
      std::unique_ptr<JetCorrectionUncertainty> source_uncertainty_reduced_AK4(new JetCorrectionUncertainty(source_parameters_reduced_AK4));
      if(_verbose)std::cout <<  "AK4: adding the uncertainty object to the JEC map" << std::endl;
      JEC_map_AK4.emplace(uncertainty_source, std::move(source_uncertainty_reduced_AK4));
      if(_verbose)std::cout <<  "AK4: added the uncertainty object to the JEC map" << std::endl;


      if(_verbose)std::cout <<  "----- AK8 jets: -----" << std::endl;
      if(_verbose)std::cout << "AK8: JEC_source_text_AK8.fullPath().c_str() is " << JEC_source_text_AK8.fullPath().c_str() << std::endl;
      if(_verbose)std::cout << "uncertainty_source is " << uncertainty_source << std::endl;
      JetCorrectorParameters source_parameters_reduced_AK8(JEC_source_text_AK8.fullPath().c_str(), uncertainty_source);
      if(_verbose)std::cout <<  "AK8: creating the pointer for JetCorrectionUncertainty" << std::endl;
      std::unique_ptr<JetCorrectionUncertainty> source_uncertainty_reduced_AK8(new JetCorrectionUncertainty(source_parameters_reduced_AK8));
      if(_verbose)std::cout <<  "AK8: adding the uncertainty object to the JEC map" << std::endl;
      JEC_map_AK8.emplace(uncertainty_source, std::move(source_uncertainty_reduced_AK8));
      if(_verbose)std::cout <<  "AK8: added the uncertainty object to the JEC map" << std::endl;

   }


   if(_verbose)
   {
      // list the uncertainty sources listed in the map
      for (const auto& pair : JEC_map_AK4) 
      {
        std::cout << "AK4 Key: " << pair.first << std::endl;
      }
      for (const auto& pair : JEC_map_AK8) 
      {
        std::cout << "AK8 Key: " << pair.first << std::endl;
      }
   }


   if(_verbose)std::cout << "Got JEC correction maps for AK4 and AK8 jets." << std::endl;


   ///////////////////////////////////////////////////////////////
   //////////////////////// B-tagging stuff //////////////////////
   ///////////////////////////////////////////////////////////////

   if( (runType.find("MC") != std::string::npos) || (runType.find("Tprime") != std::string::npos))    //don't want these variables for data
   {

      if(_verbose)std::cout << "Setting up b-tag efficiency maps" << std::endl;
      bTagEff_file = new TFile( bTagEff_path.fullPath().c_str() );   //this path might need to be updated

      truebjet_eff = (TH2F*)bTagEff_file->Get("h_effbJets_tight");   
      truecjet_eff = (TH2F*)bTagEff_file->Get("h_effcJets_tight");  
      lightjet_eff = (TH2F*)bTagEff_file->Get("h_effLightJets_tight"); 

      truebjet_eff_med = (TH2F*)bTagEff_file->Get("h_effbJets_med"); 
      truecjet_eff_med = (TH2F*)bTagEff_file->Get("h_effcJets_med"); 
      lightjet_eff_med = (TH2F*)bTagEff_file->Get("h_effLightJets_med"); 


      bTagEffMap_PtRange = lightjet_eff->GetXaxis()->GetXmax();
      bTagEffMap_Eta_high = lightjet_eff->GetYaxis()->GetXmax();
      bTagEffMap_Eta_low  = lightjet_eff->GetYaxis()->GetXmin();

      bTagEffMap_nPtBins = lightjet_eff->GetNbinsX();
      bTagEffMap_nEtaBins = lightjet_eff->GetNbinsY();

      if(_verbose)std::cout << "Setting up b-tag correctionlib corrector" << std::endl;

      cset = CorrectionSet::from_file(bTagSF_path.fullPath());
      cset_corrector_bc    = cset->at("deepJet_mujets");   //deepJet_comb,    // deepJet_mujets -> what we were using originally, deepJet_ttbar -> what BTV suggested we could use, UPDATE: deepJet_ttbar is not recognized!!!
      if(_verbose)std::cout << "Got bc b-tag correctionlib corrector" << std::endl;
      cset_corrector_light = cset->at("deepJet_incl");   //deepJet_incl
      //get corrections from PU file
      PUjson = CorrectionSet::from_file(PUfile_path.fullPath());
      PUjson_year = PUjson->at(lumiTag);
      if(_verbose)std::cout << "Set up pileup correctionlib corrector" << std::endl;
   }

   if(_verbose)std::cout << "Setting up jet veto map files and ranges" << std::endl;


   //////////////////////////////////////
   //////// Jet veto map stuff //////////
   ////////////////////////////////////// 

   jetVetoMap_file = new TFile( jetVetoMapFile.fullPath().c_str() ); 
   jetVetoMap = (TH2F*)jetVetoMap_file->Get(jetVetoMapName.c_str());
   jetVetoMap_XRange = jetVetoMap->GetXaxis()->GetXmax() - jetVetoMap->GetXaxis()->GetXmin();
   jetVetoMap_YRange = jetVetoMap->GetYaxis()->GetXmax() - jetVetoMap->GetYaxis()->GetXmin();

   jetVetoMap_Xmin = jetVetoMap->GetXaxis()->GetXmin();
   jetVetoMap_Ymin = jetVetoMap->GetYaxis()->GetXmin();
   jetVetoMap_nBinsX = jetVetoMap->GetNbinsX();
   jetVetoMap_nBinsY = jetVetoMap->GetNbinsY();


   if(_verbose)std::cout << "Initializing TTree variables." << std::endl;

   ///////////////////////////////////////
   ////////// init tree branches /////////
   ///////////////////////////////////////



   tree->Branch("passesPFHT", &passesPFHT  , "passesPFHT/O");
   tree->Branch("passesPFJet", &passesPFJet  , "passesPFJet/O");

   tree->Branch("nfatjets", &nfatjets, "nfatjets/I");

   tree->Branch("nAK4", &nAK4, "nAK4/I");
   tree->Branch("AK4_DeepJet_disc", AK4_DeepJet_disc, "AK4_DeepJet_disc[nAK4]/D");

   tree->Branch("nSuperJets", &nSuperJets, "nSuperJets/I");
   tree->Branch("tot_nAK4_50", &tot_nAK4_50, "tot_nAK4_50/I");             //total #AK4 jets (E>50 GeV) for BOTH superjets
   tree->Branch("tot_nAK4_70", &tot_nAK4_70, "tot_nAK4_70/I");
   tree->Branch("diSuperJet_mass",&diSuperJet_mass, "diSuperJet_mass/D");
   tree->Branch("diSuperJet_mass_100",&diSuperJet_mass_100, "diSuperJet_mass_100/D");
   tree->Branch("nfatjet_pre",&nfatjet_pre, "nfatjet_pre/I");
   tree->Branch("totHT",&totHT, "totHT/D");
   
   tree->Branch("jet_pt", jet_pt, "jet_pt[nfatjets]/D");
   tree->Branch("jet_eta", jet_eta, "jet_eta[nfatjets]/D");
   tree->Branch("jet_phi", jet_phi, "jet_phi[nfatjets]/D");
   tree->Branch("AK8_fails_veto_map", AK8_fails_veto_map, "AK8_fails_veto_map[nfatjets]/O");

   tree->Branch("fatjet_isHEM", fatjet_isHEM, "fatjet_isHEM[nfatjets]/O");
   tree->Branch("jet_pre_isHEM", jet_pre_isHEM, "jet_pre_isHEM[nfatjet_pre]/O");

   tree->Branch("jet_isHEM", jet_isHEM, "jet_isHEM[nAK4]/O");

   tree->Branch("lab_nAK4", &lab_nAK4, "lab_nAK4/I");
   tree->Branch("lab_AK4_pt", lab_AK4_pt, "lab_AK4_pt[lab_nAK4]/D");
   tree->Branch("AK4_eta", AK4_eta  , "AK4_eta[lab_nAK4]/D");
   tree->Branch("AK4_phi", AK4_phi  , "AK4_phi[lab_nAK4]/D");
   tree->Branch("AK4_fails_veto_map", AK4_fails_veto_map  , "AK4_fails_veto_map[lab_nAK4]/O");

   tree->Branch("jet_mass", jet_mass, "jet_mass[nfatjets]/D");
   tree->Branch("AK4_mass", AK4_mass, "AK4_mass[nAK4]/D");

   tree->Branch("dijetMassOne", &dijetMassOne, "dijetMassOne/D");
   tree->Branch("dijetMassTwo", &dijetMassTwo, "dijetMassTwo/D");

   tree->Branch("SJ_nAK4_50", SJ_nAK4_50, "SJ_nAK4_50[nSuperJets]/I");
   tree->Branch("SJ_nAK4_70", SJ_nAK4_70, "SJ_nAK4_70[nSuperJets]/I");
   tree->Branch("SJ_nAK4_100", SJ_nAK4_100, "SJ_nAK4_100[nSuperJets]/I");
   tree->Branch("SJ_nAK4_125", SJ_nAK4_125, "SJ_nAK4_125[nSuperJets]/I");
   tree->Branch("SJ_nAK4_150", SJ_nAK4_150, "SJ_nAK4_150[nSuperJets]/I");
   tree->Branch("SJ_nAK4_200", SJ_nAK4_200, "SJ_nAK4_200[nSuperJets]/I");
   tree->Branch("SJ_nAK4_300", SJ_nAK4_300, "SJ_nAK4_300[nSuperJets]/I");
   tree->Branch("SJ_nAK4_400", SJ_nAK4_400, "SJ_nAK4_400[nSuperJets]/I");

   tree->Branch("nHeavyAK8_pt400_M10",  &nHeavyAK8_pt400_M10, "nHeavyAK8_pt400_M10/I");
   tree->Branch("nHeavyAK8_pt400_M20",  &nHeavyAK8_pt400_M20, "nHeavyAK8_pt400_M20/I");
   tree->Branch("nHeavyAK8_pt400_M30",  &nHeavyAK8_pt400_M30, "nHeavyAK8_pt400_M30/I");
   tree->Branch("nHeavyAK8_pt300_M10",  &nHeavyAK8_pt300_M10, "nHeavyAK8_pt300_M10/I");
   tree->Branch("nHeavyAK8_pt300_M20",  &nHeavyAK8_pt300_M20, "nHeavyAK8_pt300_M20/I");
   tree->Branch("nHeavyAK8_pt300_M30",  &nHeavyAK8_pt300_M30, "nHeavyAK8_pt300_M30/I");
   tree->Branch("nHeavyAK8_pt200_M10",  &nHeavyAK8_pt200_M10, "nHeavyAK8_pt200_M10/I");
   tree->Branch("nHeavyAK8_pt200_M20",  &nHeavyAK8_pt200_M20, "nHeavyAK8_pt200_M20/I");
   tree->Branch("nHeavyAK8_pt200_M30",  &nHeavyAK8_pt200_M30, "nHeavyAK8_pt200_M30/I");

   tree->Branch("AK8_SJ_assignment", AK8_SJ_assignment  , "AK8_SJ_assignment[nfatjets]/I");   // which superjet (1 or 0) was an AK8 jet assigned to
   tree->Branch("AK4_SJ_assignment", AK4_SJ_assignment  , "AK4_SJ_assignment[nAK4]/I");   // which superjet (1 or 0) was an AK4 jet assigned to (based off the AK8 jet it was inside)

   tree->Branch("AK8_is_near_highE_CA4", AK8_is_near_highE_CA4  , "AK8_is_near_highE_CA4[nfatjets]/O");   // which superjet (1 or 0) was an AK8 jet assigned to
   tree->Branch("lab_AK4_AK8_parent", lab_AK4_AK8_parent  , "lab_AK4_AK8_parent[nAK4]/I");       //  the nfatjet index of of the AK8 jet in which selected AK4 jets reside
   tree->Branch("AK4_is_near_highE_CA4", AK4_is_near_highE_CA4  , "AK4_is_near_highE_CA4[nAK4]/O");   // which superjet (1 or 0) was an AK8 jet assigned to

   //tree->Branch("prefiringWeight_nom",  &prefiringWeight_nom, "prefiringWeight_nom/D");

   tree->Branch("JEC_uncert_AK4",  &JEC_uncert_AK4, "JEC_uncert_AK4[nAK4]/D");
   tree->Branch("JEC_uncert_AK8",  &JEC_uncert_AK8, "JEC_uncert_AK4[nfatjets]/D");

   //LUND
   tree->Branch("jet_lund_dR", &jet_lund_dR);
   tree->Branch("jet_lund_kT", &jet_lund_kT);

   if(includeAllBranches) // EXTRA tree branches to be included if needed
   {
      if(_verbose)std::cout << "Setting up extra TTree variables" << std::endl;

      //tree->Branch("AK4_bdisc", AK4_bdisc, "AK4_bdisc[nAK4]/D");
      tree->Branch("totMET",&totMET, "totMET/D");

      tree->Branch("SJ_nAK4_600", SJ_nAK4_600, "SJ_nAK4_600[nSuperJets]/I");
      tree->Branch("SJ_nAK4_800", SJ_nAK4_800, "SJ_nAK4_800[nSuperJets]/I");
      tree->Branch("SJ_nAK4_1000", SJ_nAK4_1000, "SJ_nAK4_1000[nSuperJets]/I");
      tree->Branch("SJ_mass_600", SJ_mass_600, "SJ_mass_600[nSuperJets]/D");
      tree->Branch("SJ_mass_800", SJ_mass_800, "SJ_mass_800[nSuperJets]/D");
      tree->Branch("SJ_mass_1000", SJ_mass_1000, "SJ_mass_1000[nSuperJets]/D");
      tree->Branch("leadAK8_mass",leadAK8_mass, "leadAK8_mass[nfatjets]/D");
      tree->Branch("event_reco_pt", &event_reco_pt, "event_reco_pt/D");

      //////// BES variables (not actually necessary to save here)

      tree->Branch("AK4_E", AK4_E, "AK4_E[tot_mpp_AK4]/D");
      tree->Branch("AK41_E_tree",AK41_E_tree , "AK41_E_tree[nSuperJets]/D");
      tree->Branch("AK42_E_tree",AK42_E_tree , "AK42_E_tree[nSuperJets]/D");
      tree->Branch("AK43_E_tree",AK43_E_tree , "AK43_E_tree[nSuperJets]/D");
      tree->Branch("AK44_E_tree",AK44_E_tree , "AK44_E_tree[nSuperJets]/D");
      tree->Branch("AK4_m12",AK4_m12 , "AK4_m12[nSuperJets]/D");
      tree->Branch("AK4_m13",AK4_m13 , "AK4_m13[nSuperJets]/D");
      tree->Branch("AK4_m14",AK4_m14 , "AK4_m14[nSuperJets]/D");
      tree->Branch("AK4_m23",AK4_m23 , "AK4_m23[nSuperJets]/D");
      tree->Branch("AK4_m24",AK4_m24 , "AK4_m24[nSuperJets]/D");
      tree->Branch("AK4_m34",AK4_m34 , "AK4_m34[nSuperJets]/D");
      tree->Branch("AK4_m123", AK4_m123, "AK4_m123[nSuperJets]/D");
      tree->Branch("AK4_m124", AK4_m124, "AK4_m124[nSuperJets]/D");
      tree->Branch("AK4_m134",AK4_m134 , "AK4_m134[nSuperJets]/D");
      tree->Branch("AK4_m234",AK4_m234 , "AK4_m234[nSuperJets]/D");
      tree->Branch("AK4_m1234",AK4_m1234 , "AK4_m1234[nSuperJets]/D");
      tree->Branch("AK4_theta12",AK4_theta12 , "AK4_theta12[nSuperJets]/D");
      tree->Branch("AK4_theta13",AK4_theta13,  "AK4_theta13[nSuperJets]/D");
      tree->Branch("AK4_theta14", AK4_theta14, "AK4_theta14[nSuperJets]/D");
      tree->Branch("AK4_theta23",AK4_theta23 , "AK4_theta23[nSuperJets]/D");
      tree->Branch("AK4_theta24",AK4_theta24 , "AK4_theta24[nSuperJets]/D");
      tree->Branch("AK4_theta34", AK4_theta34, "AK4_theta34[nSuperJets]/D");


      tree->Branch("AK41_ndaughters",AK41_ndaughters , "AK41_ndaughters[nSuperJets]/I");
      tree->Branch("AK41_nsubjets", AK41_nsubjets, "AK41_nsubjets[nSuperJets]/I");
      tree->Branch("AK41_thrust", AK41_thrust, "AK41_thrust[nSuperJets]/D");
      tree->Branch("AK41_sphericity", AK41_sphericity, "AK41_sphericity[nSuperJets]/D");
      tree->Branch("AK41_asymmetry",AK41_asymmetry , "AK41_asymmetry[nSuperJets]/D");
      tree->Branch("AK41_isotropy",AK41_isotropy , "AK41_isotropy[nSuperJets]/D");
      tree->Branch("AK41_aplanarity", AK41_aplanarity, "AK41_aplanarity[nSuperJets]/D");
      tree->Branch("AK41_FW1", AK41_FW1, "AK41_FW1[nSuperJets]/D");
      tree->Branch("AK41_FW2", AK41_FW2, "AK41_FW2[nSuperJets]/D");
      tree->Branch("AK41_FW3", AK41_FW3, "AK41_FW3[nSuperJets]/D");
      tree->Branch("AK41_FW4", AK41_FW4, "AK41_FW4[nSuperJets]/D");

      tree->Branch("SJ_thrust", SJ_thrust, "SJ_thrust[nSuperJets]/D");
      tree->Branch("SJ_sphericity", SJ_sphericity, "SJ_sphericity[nSuperJets]/D");
      tree->Branch("SJ_asymmetry", SJ_asymmetry, "SJ_asymmetry[nSuperJets]/D");
      tree->Branch("SJ_isotropy", SJ_isotropy, "SJ_isotropy[nSuperJets]/D");
      tree->Branch("SJ_aplanarity", SJ_aplanarity, "SJ_aplanarity[nSuperJets]/D");
      tree->Branch("SJ_FW1", SJ_FW1, "SJ_FW1[nSuperJets]/D");
      tree->Branch("SJ_FW2", SJ_FW2, "SJ_FW2[nSuperJets]/D");
      tree->Branch("SJ_FW3", SJ_FW3, "SJ_FW3[nSuperJets]/D");
      tree->Branch("SJ_FW4", SJ_FW4, "SJ_FW4[nSuperJets]/D");
      tree->Branch("AK42_ndaughters",AK42_ndaughters , "AK42_ndaughters[nSuperJets]/I");
      tree->Branch("AK42_nsubjets", AK42_nsubjets, "AK42_nsubjets[nSuperJets]/I");

      tree->Branch("nAK4_uncut", &nAK4_uncut, "nAK4_uncut/I");
      //tree->Branch("btag_score_uncut", btag_score_uncut, "btag_score_uncut[nAK4_uncut]/D");
      tree->Branch("AK4_ptot", AK4_ptot  , "AK4_ptot[lab_nAK4]/D");


   }
   tree->Branch("SJ_mass_50", SJ_mass_50, "SJ_mass_50[nSuperJets]/D");
   tree->Branch("SJ_mass_70", SJ_mass_70, "SJ_mass_70[nSuperJets]/D");
   tree->Branch("SJ_mass_100", SJ_mass_100, "SJ_mass_100[nSuperJets]/D");
   tree->Branch("SJ_mass_125", SJ_mass_125, "SJ_mass_125[nSuperJets]/D");
   tree->Branch("SJ_mass_150", SJ_mass_150, "SJ_mass_150[nSuperJets]/D");
   tree->Branch("SJ_mass_200", SJ_mass_200, "SJ_mass_200[nSuperJets]/D");
   tree->Branch("SJ_mass_300", SJ_mass_300, "SJ_mass_300[nSuperJets]/D");
   tree->Branch("SJ_mass_400", SJ_mass_400, "SJ_mass_400[nSuperJets]/D");


   tree->Branch("superJet_mass", superJet_mass, "superJet_mass[nSuperJets]/D");
   tree->Branch("posSJ_mass", &posSJ_mass, "posSJ_mass/D");
   tree->Branch("negSJ_mass", &negSJ_mass, "negSJ_mass/D");
   tree->Branch("SJ_AK4_50_mass", SJ_AK4_50_mass, "SJ_AK4_50_mass[tot_nAK4_50]/D");    //mass of individual reclustered AK4 jets
   tree->Branch("SJ_AK4_70_mass", SJ_AK4_70_mass, "SJ_AK4_70_mass[tot_nAK4_70]/D");


   tree->Branch("AK4_m1",AK4_m1 , "AK4_m1[nSuperJets]/D");
   tree->Branch("AK4_m2",AK4_m2 , "AK4_m2[nSuperJets]/D");
   tree->Branch("AK4_m3",AK4_m3 , "AK4_m3[nSuperJets]/D");
   tree->Branch("AK4_m4",AK4_m4 , "AK4_m4[nSuperJets]/D");

   tree->Branch("nCategories", &nCategories, "nCategories/I");
   //tree->Branch("SJ1_BEST_scores", SJ1_BEST_scores, "SJ1_BEST_scores[nCategories]/D");
   //tree->Branch("SJ2_BEST_scores", SJ2_BEST_scores, "SJ2_BEST_scores[nCategories]/D");
   tree->Branch("SJ1_decision", &SJ1_decision, "SJ1_decision/I");
   tree->Branch("SJ2_decision", &SJ2_decision, "SJ2_decision/I");

   ////////// standard technique plots
   tree->Branch("nAK8diJets", &nAK8diJets  , "nAK8diJets/I");
   tree->Branch("diAK8Jet_mass", diAK8Jet_mass  , "diAK8Jet_mass[nAK8diJets]/D");
   tree->Branch("fourAK8JetMass", &fourAK8JetMass  , "fourAK8JetMass/D");

   tree->Branch("eventNumber", &eventNumber  , "eventNumber/I");

   //////////signal event process type & jet multiplicity
   tree->Branch("process", &process, "process/I");
   tree->Branch("jetMultiplicity", &jetMultiplicity, "jetMultiplicity/I");



   if( (runType.find("MC") != std::string::npos) || (runType.find("Tprime") != std::string::npos))    //don't want these variables for data
   {

      if(_verbose)std::cout << "Setting up MC-specific stuff (genParts tokens, MC-specific TTree vars)" << std::endl;

      genPartToken_           = consumes<std::vector<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("genPartCollection"));
      packedGenParticleToken_ = consumes<std::vector<reco::GenParticle>>(iConfig.getParameter<edm::InputTag>("packedGenParticles"));
      genParticleToken_       = consumes<std::vector<reco::GenParticle> >(iConfig.getParameter<edm::InputTag>("genParticles"));
      puSummaryToken_         = consumes<std::vector<PileupSummaryInfo>>(iConfig.getParameter<edm::InputTag>("pileupCollection"));

      if(doTopPtReweight) tree->Branch("top_pt_weight", &top_pt_weight, "top_pt_weight/D");
      tree->Branch("AK8_JER", AK8_JER  , "AK8_JER[nfatjets]/D");


      tree->Branch("nTrue_b_AK4", &nTrue_b_AK4  , "nTrue_b_AK4/I");
      tree->Branch("nTrue_c_AK4", &nTrue_c_AK4  , "nTrue_c_AK4/I");
      tree->Branch("nTrue_light_AK4", &nTrue_light_AK4  , "nTrue_light_AK4/I");

      tree->Branch("jet_bTagSF_b_T", jet_bTagSF_b_T  , "jet_bTagSF_b_T[nTrue_b_AK4]/D");
      tree->Branch("jet_bTagSF_c_T", jet_bTagSF_c_T  , "jet_bTagSF_c_T[nTrue_c_AK4]/D");
      tree->Branch("jet_bTagSF_light_T", jet_bTagSF_light_T  , "jet_bTagSF_light_T[nTrue_light_AK4]/D");

      tree->Branch("jet_bTagSF_b_M", jet_bTagSF_b_M  , "jet_bTagSF_b_M[nTrue_b_AK4]/D");
      tree->Branch("jet_bTagSF_c_M", jet_bTagSF_c_M  , "jet_bTagSF_c_M[nTrue_c_AK4]/D");
      tree->Branch("jet_bTagSF_light_M", jet_bTagSF_light_M  , "jet_bTagSF_light_M[nTrue_light_AK4]/D");

      tree->Branch("bTag_eventWeight_T_nom", &bTag_eventWeight_T_nom  , "bTag_eventWeight_T_nom/D");  /// tight WP event weight
      tree->Branch("bTag_eventWeight_M_nom", &bTag_eventWeight_M_nom  , "bTag_eventWeight_M_nom/D");  /// med WP event weight

      if(doPUSF)  tree->Branch("PU_eventWeight_nom", &PU_eventWeight_nom, "PU_eventWeight_nom/D");

      if (systematicType == "nom") // don't need these saved for other systematic types
      {
         if(_verbose)std::cout << "Setting up nom uncertainty-specific variables (event weights)" << std::endl;

         if(doPUSF)
         {
            tree->Branch("PU_eventWeight_up", &PU_eventWeight_up, "PU_eventWeight_up/D");
            tree->Branch("PU_eventWeight_down", &PU_eventWeight_down, "PU_eventWeight_down/D");

         }
         if(doBtagSF)
         {
            // tight WP event weight
            tree->Branch("bTag_eventWeight_T_up", &bTag_eventWeight_T_up  , "bTag_eventWeight_T_up/D");
            tree->Branch("bTag_eventWeight_T_down", &bTag_eventWeight_T_down  , "bTag_eventWeight_T_down/D");

            tree->Branch("bTag_eventWeight_bc_T_up", &bTag_eventWeight_bc_T_up  , "bTag_eventWeight_bc_T_up/D");
            tree->Branch("bTag_eventWeight_bc_T_down", &bTag_eventWeight_bc_T_down  , "bTag_eventWeight_bc_T_down/D");
            tree->Branch("bTag_eventWeight_light_T_up", &bTag_eventWeight_light_T_up  , "bTag_eventWeight_light_T_up/D");
            tree->Branch("bTag_eventWeight_light_T_down", &bTag_eventWeight_light_T_down  , "bTag_eventWeight_light_T_down/D");

            // med WP event weight
            tree->Branch("bTag_eventWeight_M_up", &bTag_eventWeight_M_up  , "bTag_eventWeight_M_up/D");
            tree->Branch("bTag_eventWeight_M_down", &bTag_eventWeight_M_down  , "bTag_eventWeight_M_down/D");

            tree->Branch("bTag_eventWeight_bc_M_up", &bTag_eventWeight_bc_M_up  , "bTag_eventWeight_bc_M_up/D");
            tree->Branch("bTag_eventWeight_bc_M_down", &bTag_eventWeight_bc_M_down  , "bTag_eventWeight_bc_M_down/D");
            tree->Branch("bTag_eventWeight_light_M_up", &bTag_eventWeight_light_M_up  , "bTag_eventWeight_light_M_up/D");
            tree->Branch("bTag_eventWeight_light_M_down", &bTag_eventWeight_light_M_down  , "bTag_eventWeight_light_M_down/D");

         }
         /*if(doPDFWeights)
         {
            tree->Branch("PDFWeights_renormWeight_up",     &PDFWeights_renormWeight_up, "PDFWeights_renormWeight_up/D");
            tree->Branch("PDFWeights_renormWeight_down",   &PDFWeights_renormWeight_down, "PDFWeights_renormWeight_down/D");
            tree->Branch("PDFWeights_factWeightsRMS_up",   &PDFWeights_factWeightsRMS_up, "PDFWeights_factWeightsRMS_up/D");
            tree->Branch("PDFWeights_factWeightsRMS_down", &PDFWeights_factWeightsRMS_down, "PDFWeights_factWeightsRMS_down/D");
            
            //tree->Branch("PDFWeightUp_BEST", &PDFWeightUp, "PDFWeightUp_BEST/D");
            //tree->Branch("PDFWeightDown_BEST", &PDFWeightDown, "PDFWeightDown_BEST/D");


            // alternative calculations as BEST does them

            //tree->Branch("QCDFactorization_up_BEST", &QCDFactorization_up_BEST, "QCDFactorization_up_BEST/D");
            //tree->Branch("QCDFactorization_down_BEST", &QCDFactorization_down_BEST, "QCDFactorization_down_BEST/D");

            //tree->Branch("QCDRenormalization_up_BEST", &QCDRenormalization_up_BEST, "QCDRenormalization_up_BEST/D");
            //tree->Branch("QCDRenormalization_down_BEST", &QCDRenormalization_down_BEST, "QCDRenormalization_down_BEST/D");

            tree->Branch("scale_envelope", scale_envelope, "scale_envelope[10]/D");
            // above: scale_envelope = all the envelope variations from the pdf as written in https://twiki.cern.ch/twiki/bin/viewauth/CMS/TopSystematics#Factorization_and_renormalizatio
            // index 0: muF = 2.0, muR = 1.0
            // index 1: muF = 0.5, muR = 1.0
            // index 2: muF = 2.0, muR = 2.0
            // index 3: muF = 1.0, muR = 2.0
            // index 4: muF = 0.5, muR = 0.5
            // index 5: muF = 1.0, muR = 0.5
            // index 6: muF = 2.0, muR = 0.5 // this shouldn't be needed
            // index 7: muF = 0.5, muR = 2.0 // this shouldn't be needed
            // index 8: muF = 1.0, muR = 1.0 // this might be needed for normalization
            // index 9: nominal pdf weight
         }*/

         /*if(doPrefiringWeight)
         {
            tree->Branch("prefiringWeight_up",   &prefiringWeight_up, "prefiringWeight_up/D");
            tree->Branch("prefiringWeight_down", &prefiringWeight_down, "prefiringWeight_down/D");
         }*/
      }

      tree->Branch("nGenBJets_AK4", nGenBJets_AK4, "nGenBJets_AK4[lab_nAK4]/I");
      tree->Branch("AK4_partonFlavour", AK4_partonFlavour, "AK4_partonFlavour[lab_nAK4]/I");
      tree->Branch("AK4_hadronFlavour", AK4_hadronFlavour, "AK4_hadronFlavour[lab_nAK4]/I");

      tree->Branch("AK8_hadronFlavour", AK8_hadronFlavour, "AK8_hadronFlavour[nfatjets]/D");
      tree->Branch("AK8_partonFlavour", AK8_partonFlavour, "AK8_partonFlavour[nfatjets]/D");

      if(doPUSF)tree->Branch("ntrueInt", &ntrueInt  , "ntrueInt/I");

      // generator/pdf variables
      /*if(doPDFWeights)
      {
         if(_verbose)std::cout << "Importing PDF weights and setting up PDF weight variables" << std::endl;

         nomPDF = LHAPDF::mkPDF(LHAPDF_NOM); //Nominal PDF
         // get PDF weights
         for (int i = 0; i< nVars; i++)
         {
            varPDFs[i] = LHAPDF::mkPDF(LHAPDF_VAR_LOW + i);
         } 
         tree->Branch("id1", &id1, "id1/I");
         tree->Branch("id2", &id2, "id2/I");
         tree->Branch("x1", &x1, "x1/D");
         tree->Branch("x2", &x2, "x2/D");
         tree->Branch("q2", &q2, "q2/D");   
         tree->Branch("PDFWeights_alphas", &alphas, "PDFWeights_alphas/D");
         tree->Branch("PDFWeights_nVars", &nVars, "PDFWeights_nVars/i");
         tree->Branch("PDFWeights_varWeightsRMS", &varWeightsRMS, "PDFWeights_varWeightsRMS/D");
         tree->Branch("PDFWeights_varWeightsErr", &varWeightsErr, "PDFWeights_varWeightsErr/D");
      }*/

   }
   else if(runType == "Data")
   {
         std::cout <<"Running as data ..." << std::endl;
   }
   if(_verbose)std::cout << "------- End of Constructor ------ " << std::endl;

}


//recursively returns status-change copies of generated particles until you get to new decays
const reco::Candidate* clusteringAnalyzerAll_process::parse_chain(const reco::Candidate* cand)
{  
   for (unsigned int iii=0; iii<cand->numberOfDaughters(); iii++)
   {
      if(cand->daughter(iii)->pdgId() == cand->pdgId()) return parse_chain(cand->daughter(iii));
   }
   return cand;
}


//// return back the JEC file for a given systematic, year, and jet type
std::string clusteringAnalyzerAll_process::returnJECFile(std::string year, std::string systematicType, std::string jet_type, std::string runType)
{
   std::string data_type = "MC";
   std::string jet_str  = "AK8PFPuppi";
   if (jet_type == "AK4"){ jet_str = "AK4PFchs";}
   if ((runType.find("data") != std::string::npos) )
   {
      if(year == "2015")
      {
         if( (runType.find("dataE") != std::string::npos) || (runType.find("dataF") != std::string::npos)  )
         {
            data_type = "dataEF";
         }
         else 
         { 
            data_type = "dataBCD";
         }
      }
      if(year == "2016")
      {
         data_type = "dataFGH";
      } 
      else if (year == "2017")
      { 
         data_type = runType;
      }
      else if (year == "2018")
      { 
         data_type = runType;
      }
   }  
   // Summer19UL16APV_V9_MC/RegroupedV2_Summer19UL16APV_V9_MC_UncertaintySources_AK4PFchs.txt
   if (data_type.find("data") != std::string::npos ) return  ("data/JEC_uncertainty_sources/" + file_map[year][data_type] + "/" + file_map[year][data_type] + "_UncertaintySources_" +jet_str  + ".txt" ).c_str();
   else { return  ("data/JEC_uncertainty_sources/" + file_map[year][data_type] + "/" + file_map[year][data_type] + "_UncertaintySources_" +jet_str  + ".txt" ).c_str(); }

}

/// returns the JEC uncertainty scale factor (from a specific, reduced source) for a given jet with pt, eta
double clusteringAnalyzerAll_process::getJECUncertaintyFromSources(std::string jet_type, double pt, double eta)
{  

   if(_verbose)std::cout << "----------------------------- new jet --------------------------------- " << std::endl;
   if(_verbose)std::cout << "Reduced uncertainty is " << systematicType << std::endl;

   if(_verbose)std::cout << "There are " << uncertainty_sources.size() << " uncertainties." << std::endl;
   double uncert = 0.0;
   int uncertainty_number = 0;
   std::string uncertainty_names = "";
   for(auto uncert_source = uncertainty_sources.begin(); uncert_source!=uncertainty_sources.end();uncert_source++)
   {
      /// get the uncertainty from the JetCorrector object
      double AK4_JEC_uncertainty;
      if(_verbose)std::cout << "Uncertainty number " << uncertainty_number << ". The uncertainty source is " << *uncert_source << std::endl;

      if( jet_type == "AK4")
      {  

         if(_verbose)std::cout << "For AK8: Trying to access source " << *uncert_source << std::endl;
         JEC_map_AK4[*uncert_source]->setJetEta(eta );
         JEC_map_AK4[*uncert_source]->setJetPt( pt );
         AK4_JEC_uncertainty = fabs(JEC_map_AK4[*uncert_source]->getUncertainty(true));
         if(_verbose)std::cout << "Source: " << *uncert_source << ", value:  " << AK4_JEC_uncertainty << std::endl;
         uncert+=AK4_JEC_uncertainty;
         uncertainty_names += (*uncert_source + " ");
      }
      else if(jet_type == "AK8")
      {

         if(_verbose)std::cout << "For AK8: Trying to access source " << *uncert_source << std::endl;
         JEC_map_AK8[*uncert_source]->setJetEta( eta );
         JEC_map_AK8[*uncert_source]->setJetPt( pt );
         double AK8_JEC_uncertainty = fabs(JEC_map_AK8[*uncert_source]->getUncertainty(true));
         if(_verbose)std::cout << "Source: " << *uncert_source << ", value:  " << AK8_JEC_uncertainty << std::endl;
         uncert+=AK8_JEC_uncertainty;
         uncertainty_names += (*uncert_source + ", ");
      }
      uncertainty_number++;
   }
   if(_verbose)std::cout << "Total uncertainty: " << uncert << " from uncertainties: " << uncertainty_names << "(" << uncertainty_number << " total sources)."  << std::endl;

   return uncert;
}  

// return the JER scale factor for a given uncertainty source and jet eta
bool clusteringAnalyzerAll_process::applyJERSource(std::string uncertainty_source, double eta)
{
   if((uncertainty_source == "JER_up") || (uncertainty_source == "JER_down"))return true; // all jets are corrected for the "total" JER uncertainty
   else if(  (uncertainty_source.find("JER_eta193") != std::string::npos) && (abs(eta) < 1.93  ))return true;
   else if(  (uncertainty_source.find("JER_193eta25") != std::string::npos) && ( abs(eta) >=1.93  ) && (abs(eta) < 2.5  ))return true;
   else {return false;}
}

// returns bool if jet (or more generally, object) is within the HEM region
bool clusteringAnalyzerAll_process::isHEM(const float jet_eta, const float jet_phi)
{
   if(year != "2018") return false; // HEM is only relevant for 2018

   if( (jet_phi >  -1.57)&&( jet_phi < -0.87) )
   {
      if( (jet_eta > -3.0)&&(jet_eta < -1.3))return true;

   }
   return false;
}

// bool corresponding to if AK4 jet passes tight ID
bool clusteringAnalyzerAll_process::isgoodjet(const float eta, const float NHF,const float NEMF, const size_t NumConst,const float CHF,const int CHM, const float MUF, const float CEMF,bool jetPUid, const float iJet_pt)
{
   if( (abs(eta) > 2.4)) return false;

   // only apply to AK4 jets
   // apply the MEDIUM PU jet id https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupJetIDUL
   if( (!jetPUid) && (iJet_pt < 50.0)) return false;

   if ((NHF>0.9) || (NEMF>0.9) || (NumConst<1) || (CHF<0.) || (CHM<0) || (MUF > 0.8) || (CEMF > 0.8)) 
      {
         return false;
      }
   else{ return true;}

}
// checks AK8 jet (tight) ID
bool clusteringAnalyzerAll_process::isgoodjet(const float eta, const float NHF,const float NEMF, const size_t NumConst,const float CHF,const int CHM, const float MUF, const float CEMF, int nfatjets)
{
   if ( (nfatjets < 2) && (abs(eta) > 2.4) ) return false;
   else if ( (nfatjets >= 2) && (abs(eta) > 1.4) ) return false;

   if ((NHF>0.9) || (NEMF>0.9) || (NumConst<1) || (CHF<0.) || (CHM<0) || (MUF > 0.8) || (CEMF > 0.8)) 
      {
         return false;
      }
   else{ return true;}

}
// calculates the MPP frame given a TLorentzVector of SJ particles
double clusteringAnalyzerAll_process::calcMPP(TLorentzVector superJetTLV )  
{
   Vector jet_axis(superJetTLV.Px()/superJetTLV.P(),superJetTLV.Py()/superJetTLV.P(),superJetTLV.Pz()/superJetTLV.P());
   double min_pp = 99999999999999.;
   double min_boost = 0.;

   for(int iii=0;iii<10000;iii++)
   {
      TLorentzVector superJetTLV_ = superJetTLV;
      double beta_cand = iii/10000.;
      superJetTLV_.Boost(-beta_cand*jet_axis.X(),-beta_cand*jet_axis.Y(),-beta_cand*jet_axis.Z());
      if(abs( ( superJetTLV_.Px()*superJetTLV.Px()+superJetTLV_.Py()*superJetTLV.Py() +superJetTLV_.Pz()*superJetTLV.Pz() )/superJetTLV.P() ) < min_pp) 
         {
            min_boost = beta_cand; 
            min_pp = abs( ( superJetTLV_.Px()*superJetTLV.Px()+superJetTLV_.Py()*superJetTLV.Py() +superJetTLV_.Pz()*superJetTLV.Pz() )/superJetTLV.P() ) ;
         }
      }
      return min_boost;
}


// returns the top pt scale factor as detailed here - https://twiki.cern.ch/twiki/bin/view/CMS/TopPtReweighting#Run_1_strategy_Obsolete
double clusteringAnalyzerAll_process::top_pt_SF(double top_pt)
{

   if (top_pt > 500.) top_pt = 500.;
   //$SF(p_T)=e^{0.0615-0.0005\cdot p_T}$ for data/POWHEG+Pythia8
   //return 0.103*exp(-0.0118*top_pt) -0.000134*top_pt+ 0.973;
   return exp(0.0615-0.0005*top_pt);  // this is the scale factor based on data aka data-NLO and data-NNLO weights
}
// tells you if a TLorentzVector is associated with a candidate jet (via delta R matching)
bool clusteringAnalyzerAll_process::isMatchedtoSJ(std::vector<TLorentzVector> superJetTLVs, TLorentzVector candJet)
{
   for(auto iJet = superJetTLVs.begin(); iJet!=superJetTLVs.end(); iJet++)
   {
      if( abs(candJet.Angle(iJet->Vect())) < 0.001) return true;  
   }
   return false;
}
// for QCD scale uncertainty
double clusteringAnalyzerAll_process::calcAlphas(double q2) 
{ 
    double mZ = 91.2; //Z boson mass in the NNPDF31_nnlo_as_0118 docs (http://lhapdfsets.web.cern.ch/lhapdfsets/current/NNPDF31_nnlo_as_0118/NNPDF31_nnlo_as_0118.info )
    double alphas_mZ = 0.118; //alpha_s evaluated at Z boson mass, based on the NNPDF31_nnlo_as_0118 docs (http://lhapdfsets.web.cern.ch/lhapdfsets/current/NNPDF31_nnlo_as_0118/NNPDF31_nnlo_as_0118.info )
    int nFlavors = 5; //effective number of flavors
    double b0 = (33 - 2.0 * nFlavors) / (12 * M_PI); 
    return alphas_mZ / (1 + alphas_mZ * b0 * std::log(q2 / std::pow(mZ,2))); // alphas evolution
}
// for QCD factorization uncertainty
double clusteringAnalyzerAll_process::calcFactorizWeight(LHAPDF::PDF* pdf, double id1, double id2, double x1, double x2, double q2, int up_or_dn) 
{
    double k2;
    if ( up_or_dn ==  1 )
        k2 = 4; // 2*q ==> 4*q2
    else if ( up_or_dn == -1 )
        k2 = 0.25; // 0.5*q ==> 0.25*q2
    else {
        throw std::invalid_argument("up_or_dn must be -1 or 1");
    }

    double pdf1old = pdf->xfxQ2(id1,x1,q2);
    double pdf2old = pdf->xfxQ2(id2,x2,q2);
    double pdf1new = pdf->xfxQ2(id1,x1,k2*q2);
    double pdf2new = pdf->xfxQ2(id2,x2,k2*q2);
    double weight = (pdf1new * pdf2new) / (pdf1old * pdf2old);

    return weight;
}

double clusteringAnalyzerAll_process::calcRenormWeight(double q2, int up_or_dn, int nQCD) 
{ 
    if (nQCD == 0) //Time saving check since we will exponentiate by nQCD as the last step
        return 1;

    double k2;
    if ( up_or_dn ==  1 )
        k2 = 4; // 2*q ==> 4*q2
    else if ( up_or_dn == -1 )
        k2 = 0.25; // 0.5*q ==> 0.25*q2
    else {
      throw std::invalid_argument("up_or_dn must be -1 or 1");
    }
    double alphas_old = calcAlphas(q2);
    double alphas_new = calcAlphas(k2*q2);
 
    return std::pow(alphas_new / alphas_old, nQCD);
}

// does filling of the superjet variables to give the NN
bool clusteringAnalyzerAll_process::fillSJVars(std::map<std::string, float> &treeVars, std::vector<fastjet::PseudoJet> iSJ, int nSuperJets )
{
   bool testNewVars = true; // include new SJ vars to give to NN

   double superJetpx=0,superJetpy=0,superJetpz=0,superJetE=0;
   for(auto iP = iSJ.begin();iP != iSJ.end();iP++)
   {
      superJetpx+=iP->px();superJetpy+=iP->py();superJetpz+=iP->pz();superJetE +=iP->E();
   }

   TLorentzVector superJetTLV(superJetpx,superJetpy,superJetpz,superJetE);    //Lorentz vector representing jet axis -> now minimize the parallel momentum
      
   //boost COM
   std::vector<fastjet::PseudoJet> boostedSuperJetPart;

   //boost particles in SuperJet to COM frame
   for(auto iP = iSJ.begin();iP != iSJ.end();iP++)
   {
      TLorentzVector iP_(iP->px(),iP->py(),iP->pz(),iP->E());
      iP_.Boost(-superJetTLV.Px()/superJetTLV.E(),-superJetTLV.Py()/superJetTLV.E(),-superJetTLV.Pz()/superJetTLV.E());
      boostedSuperJetPart.push_back(fastjet::PseudoJet(iP_.Px(),iP_.Py(),iP_.Pz(),iP_.E()));
   }

   ///////////get a bunch of versions of SJ particle collection to calclate BES variables
   std::vector<TLorentzVector> boostedSuperJetPart_TLV;
   std::vector<reco::LeafCandidate> boostedSuperJetPart_LC;
   std::vector<math::XYZVector> boostedSuperJetPart_XYZ;
   double sumPz = 0, sumP = 0;
   for(auto iP_= boostedSuperJetPart.begin(); iP_ !=boostedSuperJetPart.end(); iP_++)
   {   
      boostedSuperJetPart_TLV.push_back(TLorentzVector(iP_->px(),iP_->py(),iP_->pz(),iP_->E()));
      boostedSuperJetPart_LC.push_back(reco::LeafCandidate(+1, reco::Candidate::LorentzVector(iP_->px(),iP_->py(),iP_->pz(),iP_->E())));
      boostedSuperJetPart_XYZ.push_back(math::XYZVector( iP_->px(),iP_->py(),iP_->pz() ));
      sumPz += abs(iP_->pz());
      sumP += abs(sqrt(pow(iP_->pz(),2) + pow(iP_->px(),2)+ pow(iP_->py(),2)));
   }

   ///reclustering SuperJet that is now boosted into the SJ COM frame
   double R = 0.4;

   //fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, R);
   fastjet::JetDefinition jet_def(fastjet::cambridge_algorithm, R);
   fastjet::ClusterSequence cs_jet(boostedSuperJetPart, jet_def); 
   std::vector<fastjet::PseudoJet> jetsFJ_jet = fastjet::sorted_by_E(cs_jet.inclusive_jets(0.0));

   double SJ_25_px = 0, SJ_25_py=0,SJ_25_pz=0,SJ_25_E=0;
   double SJ_50_px = 0, SJ_50_py=0,SJ_50_pz=0,SJ_50_E=0;
   double SJ_75_px = 0, SJ_75_py=0,SJ_75_pz=0,SJ_75_E=0;
   double SJ_100_px = 0, SJ_100_py=0,SJ_100_pz=0,SJ_100_E=0;
   double SJ_150_px = 0, SJ_150_py=0,SJ_150_pz=0,SJ_150_E=0;
   double SJ_200_px = 0, SJ_200_py=0,SJ_200_pz=0,SJ_200_E=0;
   double SJ_300_px = 0, SJ_300_py=0,SJ_300_pz=0,SJ_300_E=0;
   double SJ_400_px = 0, SJ_400_py=0,SJ_400_pz=0,SJ_400_E=0;
   double SJ_500_px = 0, SJ_500_py=0,SJ_500_pz=0,SJ_500_E=0;
   double SJ_800_px = 0, SJ_800_py=0,SJ_800_pz=0,SJ_800_E=0;
   double SJ_1000_px = 0, SJ_1000_py=0,SJ_1000_pz=0,SJ_1000_E=0;

   int SJ_nAK4_1 = 0, SJ_nAK4_5 = 0, SJ_nAK4_10_ = 0;
   int SJ_nAK4_25_ = 0;
   int SJ_nAK4_50_ = 0, SJ_nAK4_75_ = 0, SJ_nAK4_100_ = 0,SJ_nAK4_150_ = 0,SJ_nAK4_200_ = 0,SJ_nAK4_300_ = 0;
   int SJ_nAK4_400_ = 0,SJ_nAK4_500_ = 0,SJ_nAK4_800_ = 0,SJ_nAK4_1000_ = 0;


   std::vector<TLorentzVector> AK41_parts;
   std::vector<TLorentzVector> AK42_parts;
   std::vector<TLorentzVector> AK43_parts;
   std::vector<TLorentzVector> AK44_parts;

   double AK41_px = 0, AK41_py=0,AK41_pz = 0, AK41_E = 0;
   double AK42_px = 0, AK42_py=0,AK42_pz = 0, AK42_E = 0;
   double AK43_px = 0, AK43_py=0,AK43_pz = 0, AK43_E = 0;
   double AK44_px = 0, AK44_py=0,AK44_pz = 0, AK44_E = 0;


   
   if (jetsFJ_jet.size() < 2) // fewer than 2 reclustered CA4 jets in superjet, this shouldn't reasonably happen ...
   {
      return false; // RETURN CUT
      std::cout << "A pseudojet vector has a size smaller than 2 - not reclustering enough jets from pool of particles - " << jetsFJ_jet.size() << std::endl;
      treeVars["AK44_px"] = 0;
      treeVars["AK44_py"] = 0;
      treeVars["AK44_pz"] = 0;
      treeVars["AK44_E"] = 0;    
   }
   //else if (jetsFJ_jet.size() < 3) { jetsFJ_jet.push_back( fastjet::PseudoJet(0,0,0,0)); jetsFJ_jet.push_back( fastjet::PseudoJet(0,0,0,0));      }       // error avoidance
   //else if (jetsFJ_jet.size() < 4) { jetsFJ_jet.push_back( fastjet::PseudoJet(0,0,0,0));}                                                                 // error avoidance
   if (jetsFJ_jet.size() < 4) {   return false;} // RETURN CUT, tried to prevent errors in calculating SJ vars, but did not succeed (will get seg fault without this)

   int pseudoJetNum = 0;
   for (auto iPJ=jetsFJ_jet.begin(); iPJ<jetsFJ_jet.end(); iPJ++)                             
   {

      // do calculations of AK4 btagged particle ratios for leading 4 AK4 jets
      std::vector<fastjet::PseudoJet> iPJ_daughters = iPJ->constituents();
            
      if(pseudoJetNum == 0)
      {   
         treeVars["AK41_px"] = iPJ->px();
         treeVars["AK41_py"] = iPJ->py();
         treeVars["AK41_pz"] = iPJ->pz();
         treeVars["AK41_E"] = iPJ->E();
         for(auto iPart = iPJ_daughters.begin(); iPart != iPJ_daughters.end(); iPart++)
         {
            AK41_parts.push_back(TLorentzVector(iPart->px(),iPart->py(),iPart->pz(),iPart->E()));
            AK41_px+=iPart->px();AK41_py+=iPart->py();AK41_pz+=iPart->pz();AK41_E+=iPart->E();
         }

      }
      else if(pseudoJetNum == 1)
      {
         treeVars["AK42_px"] = iPJ->px();
         treeVars["AK42_py"] = iPJ->py();
         treeVars["AK42_pz"] = iPJ->pz();
         treeVars["AK42_E"] = iPJ->E();
         for(auto iPart = iPJ_daughters.begin(); iPart != iPJ_daughters.end(); iPart++)
         {
            AK42_parts.push_back(TLorentzVector(iPart->px(),iPart->py(),iPart->pz(),iPart->E()));
            AK42_px+=iPart->px();AK42_py+=iPart->py();AK42_pz+=iPart->pz();AK42_E+=iPart->E();
         }
      }
      else if(pseudoJetNum == 2)
      {
         treeVars["AK43_px"] = iPJ->px();
         treeVars["AK43_py"] = iPJ->py();
         treeVars["AK43_pz"] = iPJ->pz();
         treeVars["AK43_E"] = iPJ->E();
         for(auto iPart = iPJ_daughters.begin(); iPart != iPJ_daughters.end(); iPart++)
         {
            AK43_parts.push_back(TLorentzVector(iPart->px(),iPart->py(),iPart->pz(),iPart->E()));
            AK43_px+=iPart->px();AK43_py+=iPart->py();AK43_pz+=iPart->pz();AK43_E+=iPart->E();
         }
      }
      else if(pseudoJetNum == 3)
      {
         treeVars["AK44_px"] = iPJ->px();
         treeVars["AK44_py"] = iPJ->py();
         treeVars["AK44_pz"] = iPJ->pz();
         treeVars["AK44_E"] = iPJ->E();      
         for(auto iPart = iPJ_daughters.begin(); iPart != iPJ_daughters.end(); iPart++)
         {
            AK44_parts.push_back(TLorentzVector(iPart->px(),iPart->py(),iPart->pz(),iPart->E()));
            AK44_px+=iPart->px();AK44_py+=iPart->py();AK44_pz+=iPart->pz();AK44_E+=iPart->E();
         }
      }

      if(iPJ->E()>1.)  SJ_nAK4_1++;   // this is just used for a fraction calculation
      if(iPJ->E()>5.)  SJ_nAK4_5++;   // this is just used for a fraction calculation
      if(iPJ->E()>10.) SJ_nAK4_10_++;

      if(iPJ->E()>25.)
      {
         SJ_25_px+=iPJ->px();SJ_25_py+=iPJ->py();SJ_25_pz+=iPJ->pz();SJ_25_E+=iPJ->E();
         SJ_nAK4_25_++;
      }

      if(iPJ->E()>50.)
      {
         SJ_50_px+=iPJ->px();SJ_50_py+=iPJ->py();SJ_50_pz+=iPJ->pz();SJ_50_E+=iPJ->E();
         SJ_nAK4_50_++;
      }
      if(iPJ->E()>75.)
      {
         SJ_75_px+=iPJ->px();SJ_75_py+=iPJ->py();SJ_75_pz+=iPJ->pz();SJ_75_E+=iPJ->E();
         SJ_nAK4_75_++;
      }
      if(iPJ->E()>100)
      {
         SJ_100_px+=iPJ->px();SJ_100_py+=iPJ->py();SJ_100_pz+=iPJ->pz();SJ_100_E+=iPJ->E();
         SJ_nAK4_100_++; 
      }

      if(iPJ->E()>150)
      {
         SJ_150_px+=iPJ->px();SJ_150_py+=iPJ->py();SJ_150_pz+=iPJ->pz();SJ_150_E+=iPJ->E();
         SJ_nAK4_150_++; 
      }

      if(iPJ->E()>200)
      {
         SJ_200_px+=iPJ->px();SJ_200_py+=iPJ->py();SJ_200_pz+=iPJ->pz();SJ_200_E+=iPJ->E();
         SJ_nAK4_200_++; 
      }
      if(iPJ->E()>300)
      {
         SJ_300_px+=iPJ->px();SJ_300_py+=iPJ->py();SJ_300_pz+=iPJ->pz();SJ_300_E+=iPJ->E();
         SJ_nAK4_300_++; 
      }
      if(iPJ->E()>400)
      {
         SJ_400_px+=iPJ->px();SJ_400_py+=iPJ->py();SJ_400_pz+=iPJ->pz();SJ_400_E+=iPJ->E();
         SJ_nAK4_400_++; 
      }
      if(iPJ->E()>500)
      {
         SJ_500_px+=iPJ->px();SJ_500_py+=iPJ->py();SJ_500_pz+=iPJ->pz();SJ_500_E+=iPJ->E();
         SJ_nAK4_500_++; 
      }
      if(iPJ->E()>800)
      {
         SJ_800_px+=iPJ->px();SJ_800_py+=iPJ->py();SJ_800_pz+=iPJ->pz();SJ_800_E+=iPJ->E();
         SJ_nAK4_800_++; 
      }
      if(iPJ->E()>1000)
      {
         SJ_1000_px+=iPJ->px();SJ_1000_py+=iPJ->py();SJ_1000_pz+=iPJ->pz();SJ_1000_E+=iPJ->E();
         SJ_nAK4_1000_++; 
      }
      pseudoJetNum++;
   }

   boostedSuperJetPart.clear();   //shouldn't be needed, just in case

   /////annoying process of getting the BES information for the reclustered AK4 jets

   TVector3 AK41_boost(AK41_px/AK41_E,AK41_py/AK41_E,AK41_pz/AK41_E);
   TVector3 AK42_boost(AK42_px/AK42_E,AK42_py/AK42_E,AK42_pz/AK42_E);
   TVector3 AK43_boost(AK43_px/AK43_E,AK43_py/AK43_E,AK43_pz/AK43_E);
   TVector3 AK44_boost(AK44_px/AK44_E,AK44_py/AK44_E,AK44_pz/AK44_E);

   std::vector<TLorentzVector> boostedAK41_Part_TLV;
   std::vector<reco::LeafCandidate>  boostedAK41_Part_LC;
   std::vector<math::XYZVector>  boostedAK41_Part_XYZ;

   std::vector<TLorentzVector> boostedAK42_Part_TLV;
   std::vector<reco::LeafCandidate>  boostedAK42_Part_LC;
   std::vector<math::XYZVector>  boostedAK42_Part_XYZ;

   std::vector<TLorentzVector> boostedAK43_Part_TLV;
   std::vector<reco::LeafCandidate>  boostedAK43_Part_LC;
   std::vector<math::XYZVector>  boostedAK43_Part_XYZ;

   std::vector<TLorentzVector> boostedAK44_Part_TLV;
   std::vector<reco::LeafCandidate>  boostedAK44_Part_LC;
   std::vector<math::XYZVector>  boostedAK44_Part_XYZ;

   double sumPz_AK41 =0,sumPz_AK42 = 0,sumPz_AK43 = 0,sumPz_AK44 = 0;
   double sumP_AK41 = 0,sumP_AK42 = 0,sumP_AK43 = 0, sumP_AK44 = 0;

   int n_AK41_parts_1 = 0, n_AK41_parts_5 = 0, n_AK41_parts_10 =0; // n_AK41_parts_20=0, n_AK41_parts_40=0, n_AK41_parts_50=0, n_AK41_parts_75=0, n_AK41_parts_100=0;
   int n_AK41_parts_0p1 = 0, n_AK41_parts_0p5 = 0, n_AK41_parts_2 =0, n_AK41_parts_7p5=0, n_AK41_parts_15=0;
   int n_AK42_parts_1 = 0, n_AK42_parts_5 = 0, n_AK42_parts_10 =0; 
   int n_AK42_parts_0p1 = 0, n_AK42_parts_0p5 = 0, n_AK42_parts_2 =0, n_AK42_parts_7p5=0, n_AK42_parts_15=0;
   int n_AK43_parts_1 = 0, n_AK43_parts_5 = 0, n_AK43_parts_10 =0; 
   int n_AK43_parts_0p1 = 0, n_AK43_parts_0p5 = 0, n_AK43_parts_2 =0, n_AK43_parts_7p5=0, n_AK43_parts_15=0;
   int n_AK44_parts_1 = 0, n_AK44_parts_5 = 0, n_AK44_parts_10 =0; 
   int n_AK44_parts_0p1 = 0, n_AK44_parts_0p5 = 0, n_AK44_parts_2 =0, n_AK44_parts_7p5=0, n_AK44_parts_15=0;



   // reclustered AK4 jets where only particles with energy greater than X are considered 
   TLorentzVector AK41_0p1(0,0,0,0); TLorentzVector AK41_0p5(0,0,0,0); TLorentzVector AK41_1(0,0,0,0); 
   TLorentzVector AK41_2(0,0,0,0); TLorentzVector AK41_5(0,0,0,0); TLorentzVector AK41_7p5(0,0,0,0);
   TLorentzVector AK41_10(0,0,0,0); TLorentzVector AK41_15(0,0,0,0);
   TLorentzVector AK41_20(0,0,0,0); //TLorentzVector AK41_40(0,0,0,0);
   //TLorentzVector AK41_50(0,0,0,0); TLorentzVector AK41_75(0,0,0,0); TLorentzVector AK41_100(0,0,0,0);
   TLorentzVector AK42_0p1(0,0,0,0); TLorentzVector AK42_0p5(0,0,0,0); TLorentzVector AK42_1(0,0,0,0); 
   TLorentzVector AK42_2(0,0,0,0); TLorentzVector AK42_5(0,0,0,0); TLorentzVector AK42_7p5(0,0,0,0);
   TLorentzVector AK42_10(0,0,0,0); TLorentzVector AK42_15(0,0,0,0);
   TLorentzVector AK42_20(0,0,0,0); 
   TLorentzVector AK43_0p1(0,0,0,0); TLorentzVector AK43_0p5(0,0,0,0); TLorentzVector AK43_1(0,0,0,0); 
   TLorentzVector AK43_2(0,0,0,0); TLorentzVector AK43_5(0,0,0,0); TLorentzVector AK43_7p5(0,0,0,0);
   TLorentzVector AK43_10(0,0,0,0); TLorentzVector AK43_15(0,0,0,0);
   TLorentzVector AK43_20(0,0,0,0); 
   TLorentzVector AK44_0p1(0,0,0,0); TLorentzVector AK44_0p5(0,0,0,0); TLorentzVector AK44_1(0,0,0,0); 
   TLorentzVector AK44_2(0,0,0,0); TLorentzVector AK44_5(0,0,0,0); TLorentzVector AK44_7p5(0,0,0,0);
   TLorentzVector AK44_10(0,0,0,0); TLorentzVector AK44_15(0,0,0,0);
   TLorentzVector AK44_20(0,0,0,0); 

   for(auto iP = AK41_parts.begin(); iP != AK41_parts.end(); iP++)
   {
      iP->Boost(-AK41_boost.X(),-AK41_boost.Y(), -AK41_boost.Z());
      boostedAK41_Part_TLV.push_back(TLorentzVector(iP->Px(),iP->Py(),iP->Pz(),iP->E()));
      boostedAK41_Part_LC.push_back(reco::LeafCandidate(+1, reco::Candidate::LorentzVector(iP->Px(),iP->Py(),iP->Pz(),iP->E())));
      boostedAK41_Part_XYZ.push_back(math::XYZVector( iP->Px(),iP->Py(),iP->Pz() ));
      if(iP->E() > 0.1)
      {
         AK41_0p1+= *iP;
         n_AK41_parts_0p1++;
      }
      if(iP->E() > 0.5)
      {
         AK41_0p5+= *iP;
         n_AK41_parts_0p5++;
      }
      if(iP->E() > 1)
      {
         AK41_1+= *iP;
         n_AK41_parts_1++;
      }
      if(iP->E() > 2)
      {
         AK41_2+= *iP;
         n_AK41_parts_2++;
      }
      if(iP->E() > 5)
      {
         AK41_5+= *iP;
         n_AK41_parts_5++;
      }
      if(iP->E() > 7.5)
      {
         AK41_7p5+= *iP;
         n_AK41_parts_7p5++;
      }
      if(iP->E() > 10)
      {
         AK41_10+= *iP;
         n_AK41_parts_10++;
      }
      if(iP->E() > 15)
      {
         AK41_15+= *iP;
         n_AK41_parts_15++;
      }
      sumPz_AK41+= abs(iP->Pz()); sumP_AK41+= abs(iP->P());
   }
   for(auto iP = AK42_parts.begin(); iP != AK42_parts.end(); iP++)
   {
      iP->Boost(-AK42_boost.X(),-AK42_boost.Y(), -AK42_boost.Z());
      boostedAK42_Part_TLV.push_back(TLorentzVector(iP->Px(),iP->Py(),iP->Pz(),iP->E()));
      boostedAK42_Part_LC.push_back(reco::LeafCandidate(+1, reco::Candidate::LorentzVector(iP->Px(),iP->Py(),iP->Pz(),iP->E())));
      boostedAK42_Part_XYZ.push_back(math::XYZVector( iP->Px(),iP->Py(),iP->Pz() ));  
      if(iP->E() > 0.1)
      {
         AK42_0p1+= *iP;
         n_AK42_parts_0p1++;
      }
      if(iP->E() > 0.5)
      {
         AK42_0p5+= *iP;
         n_AK42_parts_0p5++;
      }
      if(iP->E() > 1)
      {
         AK42_1+= *iP;
         n_AK42_parts_1++;
      }
      if(iP->E() > 2)
      {
         AK42_2+= *iP;
         n_AK42_parts_2++;
      }
      if(iP->E() > 5)
      {
         AK42_5+= *iP;
         n_AK42_parts_5++;
      }
      if(iP->E() > 7.5)
      {
         AK42_7p5+= *iP;
         n_AK42_parts_7p5++;
      }
      if(iP->E() > 10)
      {
         AK42_10+= *iP;
         n_AK42_parts_10++;
      }
      if(iP->E() > 15)
      {
         AK42_15+= *iP;
         n_AK42_parts_15++;
      }
      sumPz_AK42+= abs(iP->Pz()); sumP_AK42+= abs(iP->P());
   }
   for(auto iP = AK43_parts.begin(); iP != AK43_parts.end(); iP++)
   {
      iP->Boost(-AK43_boost.X(),-AK43_boost.Y(), -AK43_boost.Z());
      boostedAK43_Part_TLV.push_back(TLorentzVector(iP->Px(),iP->Py(),iP->Pz(),iP->E()));
      boostedAK43_Part_LC.push_back(reco::LeafCandidate(+1, reco::Candidate::LorentzVector(iP->Px(),iP->Py(),iP->Pz(),iP->E())));
      boostedAK43_Part_XYZ.push_back(math::XYZVector( iP->Px(),iP->Py(),iP->Pz() )); 
      if(iP->E() > 0.1)
      {
         AK43_0p1+= *iP;
         n_AK43_parts_0p1++;
      }
      if(iP->E() > 0.5)
      {
         AK43_0p5+= *iP;
         n_AK43_parts_0p5++;
      }
      if(iP->E() > 1)
      {
         AK43_1+= *iP;
         n_AK43_parts_1++;
      }
      if(iP->E() > 2)
      {
         AK43_2+= *iP;
         n_AK43_parts_2++;
      }
      if(iP->E() > 5)
      {
         AK43_5+= *iP;
         n_AK43_parts_5++;
      }
      if(iP->E() > 7.5)
      {
         AK43_7p5+= *iP;
         n_AK43_parts_7p5++;
      }
      if(iP->E() > 10)
      {
         AK43_10+= *iP;
         n_AK43_parts_10++;
      }
      if(iP->E() > 15)
      {
         AK43_15+= *iP;
         n_AK43_parts_15++;
      }
      sumPz_AK43 += abs(iP->Pz()); sumP_AK43 += abs(iP->P());   
   }
   for(auto iP = AK44_parts.begin(); iP != AK44_parts.end(); iP++)
   {
      iP->Boost(-AK44_boost.X(),-AK44_boost.Y(), -AK44_boost.Z());
      boostedAK44_Part_TLV.push_back(TLorentzVector(iP->Px(),iP->Py(),iP->Pz(),iP->E()));
      boostedAK44_Part_LC.push_back(reco::LeafCandidate(+1, reco::Candidate::LorentzVector(iP->Px(),iP->Py(),iP->Pz(),iP->E())));
      boostedAK44_Part_XYZ.push_back(math::XYZVector( iP->Px(),iP->Py(),iP->Pz() )); 
      if(iP->E() > 0.1)
      {
         AK44_0p1+= *iP;
         n_AK44_parts_0p1++;
      }
      if(iP->E() > 0.5)
      {
         AK44_0p5+= *iP;
         n_AK44_parts_0p5++;
      }
      if(iP->E() > 1)
      {
         AK44_1+= *iP;
         n_AK44_parts_1++;
      }
      if(iP->E() > 2)
      {
         AK44_2+= *iP;
         n_AK44_parts_2++;
      }
      if(iP->E() > 5)
      {
         AK44_5+= *iP;
         n_AK44_parts_5++;
      }
      if(iP->E() > 7.5)
      {
         AK44_7p5+= *iP;
         n_AK44_parts_7p5++;
      }
      if(iP->E() > 10)
      {
         AK44_10+= *iP;
         n_AK44_parts_10++;
      }
      if(iP->E() > 15)
      {
         AK44_15+= *iP;
         n_AK44_parts_15++;
      }
      sumPz_AK44 += abs(iP->Pz()); sumP_AK44 += abs(iP->P());   
   }

   ////vectors to get angles
        
   TVector3 AK4_jet1(jetsFJ_jet[0].px(),jetsFJ_jet[0].py(),jetsFJ_jet[0].pz());
   TVector3 AK4_jet2(jetsFJ_jet[1].px(),jetsFJ_jet[1].py(),jetsFJ_jet[1].pz());
   TVector3 AK4_jet3(jetsFJ_jet[2].px(),jetsFJ_jet[2].py(),jetsFJ_jet[2].pz());
   TVector3 AK4_jet4(jetsFJ_jet[3].px(),jetsFJ_jet[3].py(),jetsFJ_jet[3].pz());

   //fill superjet variables here ...
   //SJ mass variables

   treeVars["SJ_mass"]    = sqrt(pow(superJetE,2)-pow(superJetpx,2)-pow(superJetpy,2)-pow(superJetpz,2)); 
   treeVars["SJ_mass_25"] = sqrt(pow(SJ_25_E,2)-pow(SJ_25_px,2)-pow(SJ_25_py,2)-pow(SJ_25_pz,2)); 
   treeVars["SJ_mass_50"] = sqrt(pow(SJ_50_E,2)-pow(SJ_50_px,2)-pow(SJ_50_py,2)-pow(SJ_50_pz,2)); 
   treeVars["SJ_mass_100"] = sqrt(pow(SJ_100_E,2)-pow(SJ_100_px,2)-pow(SJ_100_py,2)-pow(SJ_100_pz,2)); 
   treeVars["SJ_mass_150"] = sqrt(pow(SJ_150_E,2)-pow(SJ_150_px,2)-pow(SJ_150_py,2)-pow(SJ_150_pz,2)); 
   treeVars["SJ_mass_200"] = sqrt(pow(SJ_200_E,2)-pow(SJ_200_px,2)-pow(SJ_200_py,2)-pow(SJ_200_pz,2)); 
   treeVars["SJ_mass_300"] = sqrt(pow(SJ_300_E,2)-pow(SJ_300_px,2)-pow(SJ_300_py,2)-pow(SJ_300_pz,2)); 

   treeVars["SJ_mass_400"] = sqrt(pow(SJ_400_E,2)-pow(SJ_400_px,2)-pow(SJ_400_py,2)-pow(SJ_400_pz,2)); 
   treeVars["SJ_mass_500"] = sqrt(pow(SJ_500_E,2)-pow(SJ_500_px,2)-pow(SJ_500_py,2)-pow(SJ_500_pz,2)); 
   treeVars["SJ_mass_800"] = sqrt(pow(SJ_800_E,2)-pow(SJ_800_px,2)-pow(SJ_800_py,2)-pow(SJ_800_pz,2)); 
   treeVars["SJ_mass_1000"] = sqrt(pow(SJ_1000_E,2)-pow(SJ_1000_px,2)-pow(SJ_1000_py,2)-pow(SJ_1000_pz,2)); 

   double offsetInts = 0.5;

   //SJ nAK4 variables
   treeVars["SJ_nAK4_25"] = SJ_nAK4_25_ + offsetInts; 
   treeVars["SJ_nAK4_50"] = SJ_nAK4_50_ + offsetInts; 
   treeVars["SJ_nAK4_100"] = SJ_nAK4_100_ + offsetInts; 
   treeVars["SJ_nAK4_150"] = SJ_nAK4_150_ + offsetInts; 
   treeVars["SJ_nAK4_200"] = SJ_nAK4_200_ + offsetInts; 
   treeVars["SJ_nAK4_300"] = SJ_nAK4_300_ + offsetInts; 
   treeVars["SJ_nAK4_400"] = SJ_nAK4_400_ + offsetInts; 
   treeVars["SJ_nAK4_500"] = SJ_nAK4_500_ + offsetInts; 
   treeVars["SJ_nAK4_800"] = SJ_nAK4_800_ + offsetInts; 
   treeVars["SJ_nAK4_1000"] = SJ_nAK4_1000_ + offsetInts; 


   treeVars["AK41_nDaughters"] = jetsFJ_jet[0].constituents().size() + offsetInts; 
   treeVars["AK42_nDaughters"] = jetsFJ_jet[1].constituents().size() + offsetInts; 
   treeVars["AK43_nDaughters"] = jetsFJ_jet[2].constituents().size() + offsetInts; 
   treeVars["AK44_nDaughters"] = jetsFJ_jet[3].constituents().size() + offsetInts; 


   //AK4 jet mass combinations
   AK4_m1[nSuperJets] = jetsFJ_jet[0].m();   
   AK4_m2[nSuperJets] = jetsFJ_jet[1].m(); 
   AK4_m3[nSuperJets] = jetsFJ_jet[2].m(); 
   AK4_m4[nSuperJets] = jetsFJ_jet[3].m(); 

   AK41_E_tree[nSuperJets] = jetsFJ_jet[0].E(); 
   AK42_E_tree[nSuperJets] = jetsFJ_jet[1].E(); 
   AK43_E_tree[nSuperJets] = jetsFJ_jet[2].E(); 
   AK44_E_tree[nSuperJets] = jetsFJ_jet[3].E(); 


   AK4_m12[nSuperJets] = sqrt( pow(jetsFJ_jet[0].E() + jetsFJ_jet[1].E() ,2) - pow(jetsFJ_jet[0].px() + jetsFJ_jet[1].px(),2) - pow(jetsFJ_jet[0].py() + jetsFJ_jet[1].py(),2)- pow(jetsFJ_jet[0].pz() + jetsFJ_jet[1].pz(),2));   
   AK4_m13[nSuperJets] = sqrt( pow(jetsFJ_jet[0].E() + jetsFJ_jet[2].E() ,2) - pow(jetsFJ_jet[0].px() + jetsFJ_jet[2].px(),2) - pow(jetsFJ_jet[0].py() + jetsFJ_jet[2].py(),2)- pow(jetsFJ_jet[0].pz() + jetsFJ_jet[2].pz(),2));   
   AK4_m14[nSuperJets] = sqrt( pow(jetsFJ_jet[0].E() + jetsFJ_jet[3].E() ,2) - pow(jetsFJ_jet[0].px() + jetsFJ_jet[3].px(),2) - pow(jetsFJ_jet[0].py() + jetsFJ_jet[3].py(),2)- pow(jetsFJ_jet[0].pz() + jetsFJ_jet[3].pz(),2));   
   AK4_m23[nSuperJets] = sqrt( pow(jetsFJ_jet[2].E() + jetsFJ_jet[1].E() ,2) - pow(jetsFJ_jet[2].px() + jetsFJ_jet[1].px(),2) - pow(jetsFJ_jet[2].py() + jetsFJ_jet[1].py(),2)- pow(jetsFJ_jet[2].pz() + jetsFJ_jet[1].pz(),2));   
   AK4_m24[nSuperJets] = sqrt( pow(jetsFJ_jet[3].E() + jetsFJ_jet[1].E() ,2) - pow(jetsFJ_jet[3].px() + jetsFJ_jet[1].px(),2) - pow(jetsFJ_jet[3].py() + jetsFJ_jet[1].py(),2)- pow(jetsFJ_jet[3].pz() + jetsFJ_jet[1].pz(),2));   
   AK4_m34[nSuperJets] = sqrt( pow(jetsFJ_jet[2].E() + jetsFJ_jet[3].E() ,2) - pow(jetsFJ_jet[2].px() + jetsFJ_jet[3].px(),2) - pow(jetsFJ_jet[2].py() + jetsFJ_jet[3].py(),2)- pow(jetsFJ_jet[2].pz() + jetsFJ_jet[3].pz(),2));   

   AK4_m123[nSuperJets] =sqrt( pow(jetsFJ_jet[0].E() + jetsFJ_jet[1].E() + jetsFJ_jet[2].E() ,2) - pow(jetsFJ_jet[0].px() + jetsFJ_jet[1].px() + jetsFJ_jet[2].px(),2) - pow(jetsFJ_jet[0].py() + jetsFJ_jet[1].py() + jetsFJ_jet[2].py(),2)- pow(jetsFJ_jet[0].pz() + jetsFJ_jet[1].pz() + jetsFJ_jet[2].pz(),2));   
   AK4_m124[nSuperJets] =sqrt( pow(jetsFJ_jet[0].E() + jetsFJ_jet[1].E() + jetsFJ_jet[3].E() ,2) - pow(jetsFJ_jet[0].px() + jetsFJ_jet[1].px() + jetsFJ_jet[3].px(),2) - pow(jetsFJ_jet[0].py() + jetsFJ_jet[1].py() + jetsFJ_jet[3].py(),2)- pow(jetsFJ_jet[0].pz() + jetsFJ_jet[1].pz() + jetsFJ_jet[3].pz(),2));   
   AK4_m134[nSuperJets] =sqrt( pow(jetsFJ_jet[0].E() + jetsFJ_jet[2].E() + jetsFJ_jet[3].E() ,2) - pow(jetsFJ_jet[0].px() + jetsFJ_jet[2].px() + jetsFJ_jet[3].px(),2) - pow(jetsFJ_jet[0].py() + jetsFJ_jet[2].py() + jetsFJ_jet[3].py(),2)- pow(jetsFJ_jet[0].pz() + jetsFJ_jet[2].pz() + jetsFJ_jet[3].pz(),2));   
   AK4_m234[nSuperJets] =sqrt( pow(jetsFJ_jet[1].E() + jetsFJ_jet[2].E() + jetsFJ_jet[3].E() ,2) - pow(jetsFJ_jet[1].px() + jetsFJ_jet[2].px() + jetsFJ_jet[3].px(),2) - pow(jetsFJ_jet[1].py() + jetsFJ_jet[2].py() + jetsFJ_jet[3].py(),2)- pow(jetsFJ_jet[1].pz() + jetsFJ_jet[2].pz() + jetsFJ_jet[3].pz(),2));   

   AK4_m1234[nSuperJets] =sqrt( pow(jetsFJ_jet[0].E() + jetsFJ_jet[1].E() + jetsFJ_jet[2].E()+jetsFJ_jet[3].E() ,2) - pow(jetsFJ_jet[0].px() + jetsFJ_jet[1].px() + jetsFJ_jet[2].px()+jetsFJ_jet[3].px(),2) - pow(jetsFJ_jet[0].py() + jetsFJ_jet[1].py() + jetsFJ_jet[2].py()+jetsFJ_jet[3].py(),2)- pow(jetsFJ_jet[0].pz() + jetsFJ_jet[1].pz() + jetsFJ_jet[2].pz()+jetsFJ_jet[3].pz(),2));   

   AK4_theta12[nSuperJets] = cos(abs(AK4_jet1.Angle(AK4_jet2)));   
   AK4_theta13[nSuperJets] = cos(abs(AK4_jet1.Angle(AK4_jet3)));
   AK4_theta14[nSuperJets] = cos(abs(AK4_jet1.Angle(AK4_jet4)));
   AK4_theta23[nSuperJets] = cos(abs(AK4_jet2.Angle(AK4_jet3)));
   AK4_theta24[nSuperJets] = cos(abs(AK4_jet2.Angle(AK4_jet4)));
   AK4_theta34[nSuperJets] = cos(abs(AK4_jet3.Angle(AK4_jet4)));

   EventShapeVariables eventShapesAK41( boostedAK41_Part_XYZ );
   Thrust thrustCalculatorAK41( boostedAK41_Part_LC.begin(), boostedAK41_Part_LC.end() );
   double fwmAK41[5] = { 0.0, 0.0 ,0.0 ,0.0,0.0};
   //FWMoments( boostedAK41_Part_TLV, fwmAK41); 

   EventShapeVariables eventShapesAK42( boostedAK42_Part_XYZ );
   Thrust thrustCalculatorAK42( boostedAK42_Part_LC.begin(), boostedAK42_Part_LC.end() );
   double fwmAK42[5] = { 0.0, 0.0 ,0.0 ,0.0,0.0};
   //FWMoments( boostedAK42_Part_TLV, fwmAK42); 

   EventShapeVariables eventShapesAK43( boostedAK43_Part_XYZ );
   Thrust thrustCalculatorAK43( boostedAK43_Part_LC.begin(), boostedAK43_Part_LC.end() );
   double fwmAK43[5] = { 0.0, 0.0 ,0.0 ,0.0,0.0};
   //FWMoments( boostedAK43_Part_TLV, fwmAK43); 

   EventShapeVariables eventShapesAK44( boostedAK44_Part_XYZ );
   Thrust thrustCalculatorAK44( boostedAK44_Part_LC.begin(), boostedAK44_Part_LC.end() );
   double fwmAK44[5] = { 0.0, 0.0 ,0.0 ,0.0,0.0};
   //FWMoments( boostedAK44_Part_TLV, fwmAK44); 



   //AK4 jet boosted information - boost reclustered AK4 jets into their COM and look at BES variables, ndaughters, nsubjettiness
   AK41_ndaughters[nSuperJets] = jetsFJ_jet[0].constituents().size();  
   AK41_nsubjets[nSuperJets] = jetsFJ_jet[0].n_exclusive_subjets(0.2); 
   AK41_thrust[nSuperJets] = thrustCalculatorAK41.thrust();
   AK41_sphericity[nSuperJets] = eventShapesAK41.sphericity();
   AK41_asymmetry[nSuperJets] = sumPz_AK41/sumP_AK41; 
   AK41_isotropy[nSuperJets] = eventShapesAK41.isotropy();
   AK41_aplanarity[nSuperJets] = eventShapesAK41.aplanarity();
   AK41_FW1[nSuperJets] = fwmAK41[1]; 
   AK41_FW2[nSuperJets] = fwmAK41[2]; 
   AK41_FW3[nSuperJets] = fwmAK41[3]; 
   AK41_FW4[nSuperJets] = fwmAK41[4]; 

   AK42_ndaughters[nSuperJets] = jetsFJ_jet[1].constituents().size(); 
   AK42_nsubjets[nSuperJets] = jetsFJ_jet[1].exclusive_subjets(0.2).size();
   AK42_thrust[nSuperJets] = thrustCalculatorAK42.thrust(); 
   AK42_sphericity[nSuperJets] = eventShapesAK42.sphericity();
   AK42_asymmetry[nSuperJets] = sumPz_AK42/sumP_AK42; 
   AK42_isotropy[nSuperJets] = eventShapesAK42.isotropy();
   AK42_aplanarity[nSuperJets] = eventShapesAK42.aplanarity();
   AK42_FW1[nSuperJets] = fwmAK42[1]; 
   AK42_FW2[nSuperJets] = fwmAK42[2]; 
   AK42_FW3[nSuperJets] = fwmAK42[3]; 
   AK42_FW4[nSuperJets] = fwmAK42[4]; 

   AK43_ndaughters[nSuperJets] = jetsFJ_jet[2].constituents().size(); 
   AK43_nsubjets[nSuperJets] = jetsFJ_jet[2].exclusive_subjets(0.2).size();
   AK43_thrust[nSuperJets] = thrustCalculatorAK43.thrust();
   AK43_sphericity[nSuperJets] = eventShapesAK43.sphericity();
   AK43_asymmetry[nSuperJets] = sumPz_AK43/sumP_AK43; 
   AK43_isotropy[nSuperJets] = eventShapesAK43.isotropy();
   AK43_aplanarity[nSuperJets] = eventShapesAK43.aplanarity();
   AK43_FW1[nSuperJets] = fwmAK43[1]; 
   AK43_FW2[nSuperJets] = fwmAK43[2]; 
   AK43_FW3[nSuperJets] = fwmAK43[3]; 
   AK43_FW4[nSuperJets] = fwmAK43[4]; 

   EventShapeVariables eventShapes( boostedSuperJetPart_XYZ );
   Thrust thrustCalculator( boostedSuperJetPart_LC.begin(), boostedSuperJetPart_LC.end() );
   double fwm[5] = { 0.0, 0.0 ,0.0 ,0.0,0.0};
   //FWMoments( boostedSuperJetPart_TLV, fwm); 


   SJ_thrust[nSuperJets] = thrustCalculator.thrust();
   SJ_sphericity[nSuperJets] = eventShapes.sphericity();
   SJ_asymmetry[nSuperJets] = sumPz/sumP; 
   SJ_isotropy[nSuperJets] = eventShapes.isotropy();
   SJ_aplanarity[nSuperJets] = eventShapes.aplanarity();
   SJ_FW1[nSuperJets] = fwm[1]; 
   SJ_FW2[nSuperJets] = fwm[2]; 
   SJ_FW3[nSuperJets] = fwm[3]; 
   SJ_FW4[nSuperJets] = fwm[4];


   //AK4 jet mass combinations
   treeVars["AK4_m1"] = AK4_m1[nSuperJets]; 
   treeVars["AK4_m2"] = AK4_m2[nSuperJets]; 
   treeVars["AK4_m3"] = AK4_m3[nSuperJets]; 
   treeVars["AK4_m4"] = AK4_m4[nSuperJets]; 

   treeVars["AK41_E"] = AK41_E_tree[nSuperJets]; 
   treeVars["AK42_E"] = AK42_E_tree[nSuperJets]; 
   treeVars["AK43_E"] = AK43_E_tree[nSuperJets]; 
   treeVars["AK44_E"] = AK44_E_tree[nSuperJets]; 


   treeVars["AK4_m12"] = AK4_m12[nSuperJets];   
   treeVars["AK4_m13"] = AK4_m13[nSuperJets];   
   treeVars["AK4_m14"] = AK4_m14[nSuperJets];   
   treeVars["AK4_m23"] = AK4_m23[nSuperJets];   
   treeVars["AK4_m24"] = AK4_m24[nSuperJets];   
   treeVars["AK4_m34"] = AK4_m34[nSuperJets];   

   treeVars["AK4_m123"] = AK4_m123[nSuperJets];   
   treeVars["AK4_m124"] =AK4_m124[nSuperJets];   
   treeVars["AK4_m134"] =AK4_m134[nSuperJets];   
   treeVars["AK4_m234"] = AK4_m234[nSuperJets];   

   treeVars["AK4_m1234"] = AK4_m1234[nSuperJets];   
   

   //AK4 jet angles 
   treeVars["AK4_theta12"] = AK4_theta12[nSuperJets];   
   treeVars["AK4_theta13"] = AK4_theta13[nSuperJets];
   treeVars["AK4_theta14"] = AK4_theta14[nSuperJets];
   treeVars["AK4_theta23"] = AK4_theta23[nSuperJets];
   treeVars["AK4_theta24"] = AK4_theta24[nSuperJets];
   treeVars["AK4_theta34"] = AK4_theta34[nSuperJets];

   //AK4 jet boosted information - boost reclustered AK4 jets into their COM and look at BES variables, ndaughters, nsubjettiness
   treeVars["AK41_nsubjets"] = jetsFJ_jet[0].n_exclusive_subjets(0.2) + offsetInts; 
   treeVars["AK41_thrust"] = thrustCalculatorAK41.thrust();
   treeVars["AK41_sphericity"] = eventShapesAK41.sphericity();
   treeVars["AK41_asymmetry"] = sumPz_AK41/sumP_AK41; 
   treeVars["AK41_isotropy"] = eventShapesAK41.isotropy();
   treeVars["AK41_aplanarity"] = eventShapesAK41.aplanarity();
   treeVars["AK41_FW1"] = fwmAK41[1]; 
   treeVars["AK41_FW2"] = fwmAK41[2]; 
   treeVars["AK41_FW3"] = fwmAK41[3]; 
   treeVars["AK41_FW4"] = fwmAK41[4]; 

   treeVars["AK42_nsubjets"] = jetsFJ_jet[1].exclusive_subjets(0.2).size() + offsetInts;
   treeVars["AK42_thrust"] = thrustCalculatorAK42.thrust(); 
   treeVars["AK42_sphericity"] = eventShapesAK42.sphericity();
   treeVars["AK42_asymmetry"] = sumPz_AK42/sumP_AK42; 
   treeVars["AK42_isotropy"] = eventShapesAK42.isotropy();
   treeVars["AK42_aplanarity"] = eventShapesAK42.aplanarity();
   treeVars["AK42_FW1"] = fwmAK42[1]; 
   treeVars["AK42_FW2"] = fwmAK42[2]; 
   treeVars["AK42_FW3"] = fwmAK42[3]; 
   treeVars["AK42_FW4"] = fwmAK42[4]; 

   treeVars["AK43_nsubjets"] = jetsFJ_jet[2].exclusive_subjets(0.2).size() + offsetInts;
   treeVars["AK43_thrust"] = thrustCalculatorAK43.thrust();
   treeVars["AK43_sphericity"] = eventShapesAK43.sphericity();
   treeVars["AK43_asymmetry"] = sumPz_AK43/sumP_AK43; 
   treeVars["AK43_isotropy"] = eventShapesAK43.isotropy();
   treeVars["AK43_aplanarity"] = eventShapesAK43.aplanarity();
   treeVars["AK43_FW1"] = fwmAK43[1]; 
   treeVars["AK43_FW2"] = fwmAK43[2]; 
   treeVars["AK43_FW3"] = fwmAK43[3]; 
   treeVars["AK43_FW4"] = fwmAK43[4]; 

   treeVars["AK44_nsubjets"] = jetsFJ_jet[3].exclusive_subjets(0.2).size() + offsetInts;
   treeVars["AK44_thrust"] = thrustCalculatorAK44.thrust();
   treeVars["AK44_sphericity"] = eventShapesAK44.sphericity();
   treeVars["AK44_asymmetry"] = sumPz_AK44/sumP_AK44; 
   treeVars["AK44_isotropy"] = eventShapesAK44.isotropy();
   treeVars["AK44_aplanarity"] = eventShapesAK44.aplanarity();
   treeVars["AK44_FW1"] = fwmAK44[1]; 
   treeVars["AK44_FW2"] = fwmAK44[2]; 
   treeVars["AK44_FW3"] = fwmAK44[3]; 
   treeVars["AK44_FW4"] = fwmAK44[4]; 

   //Full SJ BES variablesf

   treeVars["SJ_thrust"] = thrustCalculator.thrust();
   treeVars["SJ_sphericity"] = eventShapes.sphericity();
   treeVars["SJ_asymmetry"] = sumPz/sumP; 
   treeVars["SJ_isotropy"] = eventShapes.isotropy();
   treeVars["SJ_aplanarity"] = eventShapes.aplanarity();
   treeVars["SJ_FW1"] = fwm[1]; 
   treeVars["SJ_FW2"] = fwm[2]; 
   treeVars["SJ_FW3"] = fwm[3]; 
   treeVars["SJ_FW4"] = fwm[4]; 

  if(testNewVars)
  {

     treeVars["SJ_AK4_frac_10"] = 1.0*SJ_nAK4_10_ / SJ_nAK4_1;
     treeVars["SJ_AK4_frac_25"] = 1.0*SJ_nAK4_25_ / SJ_nAK4_1;
     treeVars["SJ_AK4_frac_50"] = 1.0*SJ_nAK4_50_ / SJ_nAK4_1;
     treeVars["SJ_AK4_frac_75"] = 1.0*SJ_nAK4_75_ / SJ_nAK4_1;
     treeVars["SJ_AK4_frac_100"] = 1.0*SJ_nAK4_100_/ SJ_nAK4_1;
     treeVars["SJ_AK4_frac_200"] = 1.0*SJ_nAK4_200_/ SJ_nAK4_1;
     treeVars["SJ_AK4_frac_300"] = 1.0*SJ_nAK4_300_/ SJ_nAK4_1;
     treeVars["SJ_AK4_frac_500"] = 1.0*SJ_nAK4_500_ / SJ_nAK4_1;
     treeVars["SJ_AK4_frac_800"] = 1.0*SJ_nAK4_800_/ SJ_nAK4_1;

     treeVars["AK41_daughters_frac_0p1"] = 1.0*n_AK41_parts_0p1  /AK41_parts.size();   
     treeVars["AK41_daughters_frac_0p5"] = 1.0*n_AK41_parts_0p5  /AK41_parts.size();   
     treeVars["AK41_daughters_frac_1"] = 1.0*n_AK41_parts_1  /AK41_parts.size();   
     treeVars["AK41_daughters_frac_2"] = 1.0*n_AK41_parts_2  /AK41_parts.size();   
     treeVars["AK41_daughters_frac_5"] = 1.0*n_AK41_parts_5  /AK41_parts.size(); 
     treeVars["AK41_daughters_frac_7p5"] = 1.0*n_AK41_parts_7p5 /AK41_parts.size();     
     treeVars["AK41_daughters_frac_10"] = 1.0*n_AK41_parts_10  /AK41_parts.size();   
     treeVars["AK41_daughters_frac_15"] = 1.0*n_AK41_parts_15 /AK41_parts.size();   

     treeVars["AK41_mass_0p1"] = AK41_0p1.M();
     treeVars["AK41_mass_0p5"] = AK41_0p5.M();
     treeVars["AK41_mass_1"] = AK41_1.M();
     treeVars["AK41_mass_2"] = AK41_2.M();
     //treeVars["AK41_mass_5"] = AK41_5.M();
     treeVars["AK41_mass_7p5"] = AK41_7p5.M();
     treeVars["AK41_mass_10"] = AK41_10.M();
     treeVars["AK41_mass_15"] = AK41_15.M();

     treeVars["AK42_daughters_frac_0p1"] = 1.0*n_AK42_parts_0p1  /AK42_parts.size();   
     treeVars["AK42_daughters_frac_0p5"] = 1.0*n_AK42_parts_0p5  /AK42_parts.size();   
     treeVars["AK42_daughters_frac_1"] = 1.0*n_AK42_parts_1  /AK42_parts.size();   
     treeVars["AK42_daughters_frac_2"] = 1.0*n_AK42_parts_2  /AK42_parts.size();   
     treeVars["AK42_daughters_frac_5"] = 1.0*n_AK42_parts_5  /AK42_parts.size(); 
     treeVars["AK42_daughters_frac_7p5"] = 1.0*n_AK42_parts_7p5 /AK42_parts.size();     
     treeVars["AK42_daughters_frac_10"] = 1.0*n_AK42_parts_10  /AK42_parts.size();   
     treeVars["AK42_daughters_frac_15"] = 1.0*n_AK42_parts_15 /AK42_parts.size();   

     treeVars["AK42_mass_0p1"] = AK42_0p1.M();
     treeVars["AK42_mass_0p5"] = AK42_0p5.M();
     treeVars["AK42_mass_1"] = AK42_1.M();
     treeVars["AK42_mass_2"] = AK42_2.M();
     //treeVars["AK42_mass_5"] = AK42_5.M();
     treeVars["AK42_mass_7p5"] = AK42_7p5.M();
     treeVars["AK42_mass_10"] = AK42_10.M();
     treeVars["AK42_mass_15"] = AK42_15.M();

     treeVars["AK43_daughters_frac_0p1"] = 1.0*n_AK43_parts_0p1  /AK43_parts.size();   
     treeVars["AK43_daughters_frac_0p5"] = 1.0*n_AK43_parts_0p5  /AK43_parts.size();   
     treeVars["AK43_daughters_frac_1"] = 1.0*n_AK43_parts_1  /AK43_parts.size();   
     treeVars["AK43_daughters_frac_2"] = 1.0*n_AK43_parts_2  /AK43_parts.size();   
     treeVars["AK43_daughters_frac_5"] = 1.0*n_AK43_parts_5  /AK43_parts.size(); 
     treeVars["AK43_daughters_frac_7p5"] = 1.0*n_AK43_parts_7p5 /AK43_parts.size();     
     treeVars["AK43_daughters_frac_10"] = 1.0*n_AK43_parts_10  /AK43_parts.size();   
     treeVars["AK43_daughters_frac_15"] = 1.0*n_AK43_parts_15 /AK43_parts.size();   

     treeVars["AK43_mass_0p1"] = AK43_0p1.M();
     treeVars["AK43_mass_0p5"] = AK43_0p5.M();
     treeVars["AK43_mass_1"] = AK43_1.M();
     treeVars["AK43_mass_2"] = AK43_2.M();
     //treeVars["AK43_mass_5"] = AK43_5.M();
     treeVars["AK43_mass_7p5"] = AK43_7p5.M();
     treeVars["AK43_mass_10"] = AK43_10.M();
     treeVars["AK43_mass_15"] = AK43_15.M();

     treeVars["AK44_daughters_frac_0p1"] = 1.0*n_AK44_parts_0p1  /AK44_parts.size();   
     treeVars["AK44_daughters_frac_0p5"] = 1.0*n_AK44_parts_0p5  /AK44_parts.size();   
     treeVars["AK44_daughters_frac_1"] = 1.0*n_AK44_parts_1  /AK44_parts.size();   
     treeVars["AK44_daughters_frac_2"] = 1.0*n_AK44_parts_2  /AK44_parts.size();   
     treeVars["AK44_daughters_frac_5"] = 1.0*n_AK44_parts_5  /AK44_parts.size(); 
     treeVars["AK44_daughters_frac_7p5"] = 1.0*n_AK44_parts_7p5 /AK44_parts.size();     
     treeVars["AK44_daughters_frac_10"] = 1.0*n_AK44_parts_10  /AK44_parts.size();   
     treeVars["AK44_daughters_frac_15"] = 1.0*n_AK44_parts_15 /AK44_parts.size();   

     treeVars["AK44_mass_0p1"] = AK44_0p1.M();
     treeVars["AK44_mass_0p5"] = AK44_0p5.M();
     treeVars["AK44_mass_1"] = AK44_1.M();
     treeVars["AK44_mass_2"] = AK44_2.M();
     //treeVars["AK44_mass_5"] = AK44_5.M();
     treeVars["AK44_mass_7p5"] = AK44_7p5.M();
     treeVars["AK44_mass_10"] = AK44_10.M();
     treeVars["AK44_mass_15"] = AK44_15.M();

  }



   ///////////////////// BESvars for tree /////////////
   /////////// fill input variables to NN /////////////
   for (auto i = treeVars.begin(); i!=treeVars.end(); i++)
   {
      //std::cout<< i->first << " "  << i->second << std::endl;
      if (  (i->second  != i->second ) || ( isinf(i->second   )  ) )
      {
           i->second = 0;      //return false; // RETURN cut - event variables are NaN or inf
      }
      else if ( abs( i->second+999.99 ) < 1.0e-10 ) return false;
   }
   return true;

}


// main analyzer function: pre-select events, recluster superjets, calculate all variables from superjets
void clusteringAnalyzerAll_process::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   if(_verbose)std::cout << "-------- Beginning of Analyzer -------- " << std::endl;


   //////////////////////////////////
   //////// Event information ///////
   //////////////////////////////////

   const edm::EventAuxiliary& aux = iEvent.eventAuxiliary();
   //runNum       = aux.run();
   //lumiBlockNum = aux.luminosityBlock();
   eventNumber       = aux.event();

   if(_verbose)std::cout << " -----------------Starting Event ----------------- " << std::endl;
   eventnum++;

   edm::Handle< std::vector<reco::GenParticle> > genPartCollection; // this will be used several times throughout the code


   ////////////////////////////////// 
   ///////// Apply Triggers /////////
   //////////////////////////////////


   edm::Handle<edm::TriggerResults> triggerBits;
   iEvent.getByToken(triggerBits_, triggerBits);

   const edm::TriggerNames &names = iEvent.triggerNames(*triggerBits);
   
   passesPFJet = false;
   passesPFHT  = false;

   for(auto iT = triggers.begin(); iT != triggers.end(); iT++)
   {
      std::string trigname = *iT;
      if(debug)std::cout << "Looking for the " << trigname << " trigger." << std::endl; 

      //bool pass = false;
      for (unsigned int i = 0; i < triggerBits->size(); ++i) 
      {
         const std::string name = names.triggerName(i);
         const bool accept = triggerBits->accept(i);
         if ((name.find(trigname) != std::string::npos) &&(accept))
         {

            //std::cout << "trigname is " << trigname << std::endl;
            if( ( trigname == "HLT_PFJet500_v") || (trigname == "HLT_PFJet450_v") ) passesPFJet = true;
            else if( ( trigname == "HLT_PFHT900_v") || (trigname == "HLT_PFHT1050_v") ) passesPFHT = true;

            if(debug)std::cout << "Found the " << *iT << " trigger." << std::endl;
            //pass =true;
         }
      } 
   }



   if(_verbose)std::cout << "In analyze" << std::endl;


   /////////////////////////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////
   ///////////////////////////////////   _Background MC area_ //////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////////////


   if ((runType.find("MC") != std::string::npos) || (runType.find("Tprime")!=std::string::npos ) )
   {

      if(_verbose)std::cout << "before pileup" << std::endl;


      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////_pileup_//////////////////////////////////////////////////////////////
      ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      //                                              get the pileup weight for this event
      if(doPUSF)
      {
         edm::Handle<std::vector<PileupSummaryInfo> > PupInfo;
         iEvent.getByToken(puSummaryToken_, PupInfo);

         for (auto const& v : *PupInfo)
         {
            ntrueInt = v.getTrueNumInteractions();
            PU_eventWeight_nom  = PUjson_year->evaluate( {std::real(ntrueInt),"nominal"});
            PU_eventWeight_up  = PUjson_year->evaluate({std::real(ntrueInt),"up"});
            PU_eventWeight_down = PUjson_year->evaluate({std::real(ntrueInt),"down"}); 

            if ((PU_eventWeight_nom != PU_eventWeight_nom) || (std::isinf(PU_eventWeight_nom)))
            {
               std::cout << "Found bad PUSF: val/ntrueInt = " << PU_eventWeight_nom << "/" << ntrueInt <<std::endl;
            }
         }  
      }

   }  

   if(_verbose)std::cout << "before AK4 jets" << std::endl;
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////////////_AK4 jets_/////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   totHT = 0;
   lab_nAK4 = 0;
   nAK4 = 0;
   nAK4_uncut = 0;   
   std::vector<TLorentzVector> leadAK4Jets;

   std::vector<TLorentzVector> selectedAK4_TLV; // will be used to study superjet taggging flavour dependence 

   //Run3 commented out
   //JME::JetResolution resolution_AK4               = JME::JetResolution::get(iSetup, "AK4PFchs_pt");                          //load JER stuff from global tag
   //JME::JetResolutionScaleFactor resolution_sf_AK4 = JME::JetResolutionScaleFactor::get(iSetup, "AK4PFchs");
   
   /*
   //Run3 added
   auto const& resolution_AK4 = iSetup.getData(AK4JER_token_);
   auto const& resolution_sf_AK4 = iSetup.getData(AK4JERSF_token_);
   */
 

   edm::Handle<std::vector<pat::Jet>> smallJets;
   iEvent.getByToken(jetToken_, smallJets);

   edm::Handle<double> rho;
   iEvent.getByToken(m_rho_token, rho);


   //// tight b-tag WP variables
   double MC_tagged = 1.0,  MC_notTagged = 1.0, data_tagged = 1.0, data_notTagged = 1.0; //these are for the bTagging SFs
   double data_tagged_up = 1.0, data_notTagged_up = 1.0; //these are for the bTagging SFs
   double data_tagged_down = 1.0, data_notTagged_down = 1.0; //these are for the bTagging SFs

   double data_tagged_bc_up = 1.0, data_notTagged_bc_up = 1.0; //these are for the bTagging SFs
   double data_tagged_bc_down = 1.0, data_notTagged_bc_down = 1.0; //these are for the bTagging SFs

   double data_tagged_light_up = 1.0, data_notTagged_light_up = 1.0; //these are for the bTagging SFs
   double data_tagged_light_down = 1.0, data_notTagged_light_down = 1.0; //these are for the bTagging SFs

   //// med b-tag WP variables
   double MC_tagged_med = 1.0,  MC_notTagged_med = 1.0, data_tagged_med = 1.0, data_notTagged_med = 1.0; //these are for the bTagging SFs
   double data_tagged_up_med = 1.0, data_notTagged_up_med = 1.0; //these are for the bTagging SFs
   double data_tagged_down_med = 1.0, data_notTagged_down_med = 1.0; //these are for the bTagging SFs

   double data_tagged_bc_up_med = 1.0, data_notTagged_bc_up_med = 1.0; //these are for the bTagging SFs
   double data_tagged_bc_down_med = 1.0, data_notTagged_bc_down_med = 1.0; //these are for the bTagging SFs

   double data_tagged_light_up_med = 1.0, data_notTagged_light_up_med = 1.0; //these are for the bTagging SFs
   double data_tagged_light_down_med = 1.0, data_notTagged_light_down_med = 1.0; //these are for the bTagging SFs

   nTrue_light_AK4 = 0;
   nTrue_b_AK4 = 0;
   nTrue_c_AK4 = 0;


   // loop over AK4 jets: correct with JECs and JERs, get information for selection, save kinematic information, and calculate b-tagging event weights
   for(auto iJet = smallJets->begin(); iJet != smallJets->end(); iJet++) 
   {

      double AK4_sf_total = 1.0;
      double AK4_JEC_corr_factor = 1.0;

      if( (iJet->pt() < 10. ) || (!(iJet->isPFJet())) || ( abs(iJet->eta()) > 2.5 )) continue;   //don't even bother with these jets, lost causes

      ////////////////////////////////////////////////////////////////////////////////////////////////////////////
      ///////// _JET ENERGY CORRECTION UNCERTAINTY STUFF, AK4 jets should already be corrected in the cfg ////////
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////

      if(doJEC)
      {

         double AK4_JEC_uncertainty = getJECUncertaintyFromSources("AK4",  iJet->pt(), iJet->eta());

         if(systematicType.find("_up") != std::string::npos) 
         {
            AK4_JEC_corr_factor = 1 + AK4_JEC_uncertainty;
         }
         else if(systematicType.find("_down") != std::string::npos) 
         {
            AK4_JEC_corr_factor = 1 - AK4_JEC_uncertainty;
         }   
         AK4_sf_total*= AK4_JEC_corr_factor;

      }


/*      ////////////////////////////////////////////////////////////////////////////////////////////////////////////
      //////////   _JET ENERGY RESOLUTION STUFF //////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////

      if(doJER)
      {
         if ((runType.find("MC") != std::string::npos) || (runType.find("Tprime") != std::string::npos) )
         {
            double AK4_JER_corr_factor = 1.0; // this won't be touched for data

            if(_verbose)std::cout << "doing JER" << std::endl;

            double sJER     = -9999.;    //JER scale factor
            double sigmaJER = -9999.;    //this is the "resolution" you get from the scale factors 

            
            //these are for getting the JER scale factors
            //JME::JetParameters parameters = {{JME::Binning::JetEta, AK8_JEC_corr_factor*iJet->eta()}, {JME::Binning::Rho, rho}};

            JME::JetParameters parameters_1;
            parameters_1.setJetPt(iJet->pt());
            parameters_1.setJetEta(iJet->eta());
            parameters_1.setRho(*rho);
            sigmaJER = resolution_AK4.getResolution(parameters_1);   //pT resolution

            //JME::JetParameters
            JME::JetParameters parameters;
            parameters.setJetPt(iJet->pt());
            parameters.setJetEta(iJet->eta());
            sJER =  resolution_sf_AK4.getScaleFactor(parameters  );  //{{JME::Binning::JetEta, iJet->eta()}});
            
            if( applyJERSource(systematicType, iJet->eta())  )
            {
               if( systematicType.find("_up") != std::string::npos ) 
               {
                  sJER = resolution_sf_AK4.getScaleFactor(parameters, Variation::UP);    // SF + 1 sigma uncertainty
               }
               else if( systematicType.find("_down") != std::string::npos ) 
               {
                  sJER = resolution_sf_AK4.getScaleFactor(parameters, Variation::DOWN);  // SF - 1 sigma uncertainty
               }
            }

            const reco::GenJet *genJet = iJet->genJet();
            if( genJet)   // try the first technique
            {
               AK4_JER_corr_factor = 1 + (sJER - 1)*(iJet->pt()-genJet->pt())/iJet->pt();
            }
            else   // if no gen jet is matched, try the second technique
            {
               randomNum->SetSeed( abs(static_cast<int>(iJet->phi()*1e4)) );
               double JERrand = randomNum->Gaus(0.0, sigmaJER);
               //double JERrand = 1.0 + sigmaJER;
               AK4_JER_corr_factor = max(0., 1 + JERrand*sqrt(max(pow(sJER,2)-1,0.)));   //want to make sure this is truncated at 0
            }
            if(_verbose)std::cout << "finished with JER" << std::endl;
            AK4_sf_total*= AK4_JER_corr_factor;

         }

      }

*/
      // create corrected (scaled) jet object that will be used for cuts 
      pat::Jet corrJet(*iJet);
      LorentzVector corrJetP4(AK4_sf_total*iJet->px(),AK4_sf_total*iJet->py(),AK4_sf_total*iJet->pz(),AK4_sf_total*iJet->energy());
      corrJet.setP4(corrJetP4);
      if(_verbose)std::cout << "The PU id bool is " << bool(corrJet.userInt("pileupJetIdUpdated:fullId") & (1 << 1) )<< std::endl;

      nAK4_uncut++;

      //measure event HT
      if((corrJet.pt() > 30.)&&(abs(corrJet.eta()) < 2.5)  )totHT+= abs(corrJet.pt() );
      
      // apply AK4 jet selection (post JEC and JER)
      bool PUID = true;  // assumed true if not applying this
      if(doPUID)
      {
         PUID = false;
         PUID = bool( (corrJet.userInt("pileupJetIdUpdated:fullId") & (1 << 1)) || (corrJet.pt() > 50.) );
      }
      if( (corrJet.pt()  <30.) || (!(corrJet.isPFJet())) || (!isgoodjet(corrJet.eta(),corrJet.neutralHadronEnergyFraction(), corrJet.neutralEmEnergyFraction(),corrJet.numberOfDaughters(),corrJet.chargedHadronEnergyFraction(),corrJet.chargedMultiplicity(),corrJet.muonEnergyFraction(),corrJet.chargedEmEnergyFraction(),PUID, corrJet.pt() )) ) continue;
      

      double deepJetScore = corrJet.bDiscriminator("pfDeepFlavourJetTags:probb") + corrJet.bDiscriminator("pfDeepFlavourJetTags:probbb")+ corrJet.bDiscriminator("pfDeepFlavourJetTags:problepb");
      JEC_uncert_AK4[nAK4] = AK4_JEC_corr_factor;
      if(_verbose)std::cout << "the year is " << year << " with working point value of " << deepjet_wp_tight << ". The hadronFlavour is " << corrJet.hadronFlavour() <<  " and deepjet score " << deepJetScore<< std::endl;
      


      //////////////////////////////////////////////////////////////////////////////////////////////////////////////
      ////////////////////////////////////// b tagging study stuff /////////////////////////////////////////////////
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if ((runType.find("MC") != std::string::npos) || (runType.find("Tprime") != std::string::npos) )
      {
         iEvent.getByToken(genPartToken_, genPartCollection);
         // jet is assumed to be not tagged 
         nGenBJets_AK4[nAK4] = 0;
         AK4_partonFlavour[nAK4] = corrJet.partonFlavour();
         AK4_hadronFlavour[nAK4] = corrJet.hadronFlavour();

         for (auto iG = genPartCollection->begin(); iG != genPartCollection->end(); iG++) 
         {
            if( (abs(iG->pdgId()) != 5) || (iG->pt() < 10.)|| (!(iG->isLastCopy())) ) continue;  //only b jets that are the last copy
            if(  sqrt(pow(corrJet.phi()-iG->phi(),2)+ pow(corrJet.eta()-iG->eta(),2))   < 0.4     )
            {
               nGenBJets_AK4[nAK4]++;
            } 
         }
      }

      //////////////////////////////////////////////////////////////////////////////////////////////////////////////
      ////////////////////////////////////// b tagging event weight stuff //////////////////////////////////////////
      //////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if(doBtagSF)
      {
         // calculate the b-tag scale factor for the event using the efficiency maps that were loaded in the constructor and correctionlib
         if ( ( runType.find("MC") != std::string::npos  ) || (runType.find("Tprime")!= std::string::npos ) )    //only run this on MC (we won't have the hadron flavor stuff for data )
         {

            //these are the bins of the eff histogram you need to draw from
            // histogram size:  63, 0, 6000, 18, -2.4, 2.4   

            // should import the histogram size here so this dosn't have to be manually changed
            int xbin = (int)(corrJet.pt()*bTagEffMap_nPtBins/bTagEffMap_PtRange) + 1;
            int ybin = (int)((corrJet.eta()-bTagEffMap_Eta_low)*bTagEffMap_nEtaBins/(bTagEffMap_Eta_high-bTagEffMap_Eta_low)) +1;

            if(debug) std::cout << "bTagEffMap_nPtBins/bTagEffMap_PtRange/bTagEffMap_Eta_low/bTagEffMap_nEtaBins/bTagEffMap_Eta_high:" <<bTagEffMap_nPtBins << "/"  <<bTagEffMap_PtRange << "/" <<bTagEffMap_Eta_low << "/" <<bTagEffMap_nEtaBins << "/" <<bTagEffMap_Eta_high << std::endl;
            double SF, SF_up, SF_down;
            double SF_med, SF_up_med, SF_down_med;
            double bTag_eff_value, bTag_eff_value_med;

            if(corrJet.hadronFlavour() == 0)   //light jets
            {

               bTag_eff_value = lightjet_eff->GetBinContent(xbin,ybin);
               bTag_eff_value_med = lightjet_eff_med->GetBinContent(xbin,ybin);

               SF = cset_corrector_light->evaluate({"central", "T", 0, std::abs(corrJet.eta()), corrJet.pt()}); /// tight WP SF
               SF_med = cset_corrector_light->evaluate({"central", "M", 0, std::abs(corrJet.eta()), corrJet.pt()});

               jet_bTagSF_light_T[nTrue_light_AK4] = SF;
               jet_bTagSF_light_M[nTrue_light_AK4] = SF_med;

               nTrue_light_AK4++;

               // tight WP
               SF_up   = cset_corrector_light->evaluate({"up_correlated", "T", 0, std::abs(corrJet.eta()), corrJet.pt()});
               SF_down = cset_corrector_light->evaluate({"down_correlated", "T", 0, std::abs(corrJet.eta()), corrJet.pt()});
               // med WP
               SF_up_med   = cset_corrector_light->evaluate({"up_correlated", "M", 0, std::abs(corrJet.eta()), corrJet.pt()});
               SF_down_med = cset_corrector_light->evaluate({"down_correlated", "M", 0, std::abs(corrJet.eta()), corrJet.pt()});


               //////// MED WP SCALE FACTOR ////////
               if( deepJetScore > deepJet_wp_med)
               {
                  // med WP
                  MC_tagged_med *= bTag_eff_value_med;
                  data_tagged_med *= SF_med*bTag_eff_value_med;
                  data_tagged_up_med *= SF_up_med*bTag_eff_value_med;
                  data_tagged_down_med *= SF_down_med*bTag_eff_value_med;

                  data_tagged_light_up_med   *= SF_up_med*bTag_eff_value_med;   // this is a light jet, shift by uncertainty
                  data_tagged_light_down_med *= SF_down_med*bTag_eff_value_med; // this is a light jet, shift by uncertainty
                  data_tagged_bc_up_med   *= SF_med*bTag_eff_value_med;   // this is a light jet, leave bc at nom
                  data_tagged_bc_down_med *= SF_med*bTag_eff_value_med;   // this is a light jet, leave bc at nom

                  if(_verbose)std::cout << "tagged light jet: MC_tagged/data_tagged: " << bTag_eff_value << ":" << SF*bTag_eff_value << std::endl;
               }
               else
               {
                  // med WP

                  MC_notTagged_med *= (1 - bTag_eff_value_med);
                  data_notTagged_med *= (1 - SF_med*bTag_eff_value_med);
                  data_notTagged_up_med *= (1 - SF_up_med*bTag_eff_value_med);
                  data_notTagged_down_med *= (1 - SF_down_med*bTag_eff_value_med);

                  data_notTagged_light_up_med   *= (1 - SF_up_med*bTag_eff_value_med); // this is a light jet, shift by uncertainty
                  data_notTagged_light_down_med *= (1 - SF_down_med*bTag_eff_value_med); // this is a light jet, shift by uncertainty
                  data_notTagged_bc_up_med      *= (1 - SF_med*bTag_eff_value_med); // this is a light jet, leave bc at nom
                  data_notTagged_bc_down_med    *= (1 - SF_med*bTag_eff_value_med); // this is a light jet, leave bc at nom
               }

               //////// TIGHT WP SCALE FACTOR ///////
               if(_verbose)std::cout << "Scale factor is " << SF << std::endl;
               if(deepJetScore > deepjet_wp_tight)
               {

                  // tight WP
                  MC_tagged *= bTag_eff_value;
                  data_tagged *= SF*bTag_eff_value;
                  data_tagged_up *= SF_up*bTag_eff_value;
                  data_tagged_down *= SF_down*bTag_eff_value;

                  data_tagged_light_up   *= SF_up*bTag_eff_value;   // this is a light jet, shift by uncertainty
                  data_tagged_light_down *= SF_down*bTag_eff_value; // this is a light jet, shift by uncertainty
                  data_tagged_bc_up   *= SF*bTag_eff_value;   // this is a light jet, leave bc at nom
                  data_tagged_bc_down *= SF*bTag_eff_value;   // this is a light jet, leave bc at nom
               }

               else
               {

                  // tight WP
                  MC_notTagged *= (1 - bTag_eff_value);
                  data_notTagged *= (1 - SF*bTag_eff_value);
                  data_notTagged_up *= (1 - SF_up*bTag_eff_value);
                  data_notTagged_down *= (1 - SF_down*bTag_eff_value);

                  data_notTagged_light_up   *= (1 - SF_up*bTag_eff_value); // this is a light jet, shift by uncertainty
                  data_notTagged_light_down *= (1 - SF_down*bTag_eff_value); // this is a light jet, shift by uncertainty
                  data_notTagged_bc_up      *= (1 - SF*bTag_eff_value); // this is a light jet, leave bc at nom
                  data_notTagged_bc_down    *= (1 - SF*bTag_eff_value); // this is a light jet, leave bc at nom


                  if(_verbose)std::cout << "untagged light jet: MC_notTagged/data_notTagged: " << (1- bTag_eff_value) << ":" << (1- SF*bTag_eff_value) << std::endl;
               }
            }
            else if(corrJet.hadronFlavour() == 4) //charm jets
            {
               bTag_eff_value = truecjet_eff->GetBinContent(xbin,ybin);
               bTag_eff_value_med = truecjet_eff_med->GetBinContent(xbin,ybin);

               SF = cset_corrector_bc->evaluate(   {"central", "T", 4, std::abs(corrJet.eta()), corrJet.pt()});
               SF_med = cset_corrector_bc->evaluate(   {"central", "M", 4, std::abs(corrJet.eta()), corrJet.pt()});

               jet_bTagSF_c_T[nTrue_c_AK4] = SF;
               jet_bTagSF_c_M[nTrue_c_AK4] = SF_med;

               nTrue_c_AK4++;
               // tight WP
               SF_up = cset_corrector_bc->evaluate({"up_correlated", "T", 4, std::abs(corrJet.eta()), corrJet.pt()});
               SF_down = cset_corrector_bc->evaluate(   {"down_correlated", "T", 4, std::abs(corrJet.eta()), corrJet.pt()});

               // med WP
               SF_up_med = cset_corrector_bc->evaluate({"up_correlated", "M", 4, std::abs(corrJet.eta()), corrJet.pt()});
               SF_down_med = cset_corrector_bc->evaluate(   {"down_correlated", "M", 4, std::abs(corrJet.eta()), corrJet.pt()});
               if(_verbose)std::cout << "Scale factor is " << SF << std::endl;

               //////// MED WP SCALE FACTOR ////////
               if(deepJetScore > deepJet_wp_med)
               {
                  // med WP
                  MC_tagged_med  *= bTag_eff_value_med;
                  data_tagged_med *= SF_med*bTag_eff_value_med;
                  data_tagged_up_med *= SF_up_med*bTag_eff_value_med;
                  data_tagged_down_med *= SF_down_med*bTag_eff_value_med;

                  data_tagged_light_up_med    *= SF_med*bTag_eff_value_med;      // this is a c jet, so leave light jet SF as nom
                  data_tagged_light_down_med  *= SF_med*bTag_eff_value_med;      // this is a c jet, so leave light jet SF as nom
                  data_tagged_bc_up_med       *= SF_up_med*bTag_eff_value_med;   //this is a c jet, shift bc uncertainty by sigma
                  data_tagged_bc_down_med     *= SF_down_med*bTag_eff_value_med; //this is a c jet, shift bc uncertainty by sigma
               }
               else
               {

                  // med WP
                  MC_notTagged_med *= (1 - bTag_eff_value_med);
                  data_notTagged_med *= (1 - SF_med*bTag_eff_value_med);
                  data_notTagged_up_med *= (1 - SF_up_med*bTag_eff_value_med);
                  data_notTagged_down_med *= (1 - SF_down_med*bTag_eff_value_med);

                  data_notTagged_light_up_med     *= (1 - SF_med*bTag_eff_value_med); // this is a c jet, so leave light jet SF as nom
                  data_notTagged_light_down_med   *= (1 - SF_med*bTag_eff_value_med); // this is a c jet, so leave light jet SF as nom
                  data_notTagged_bc_up_med        *= (1 - SF_up_med*bTag_eff_value_med); //this is a c jet, shift bc uncertainty by sigma
                  data_notTagged_bc_down_med      *= (1 - SF_down_med*bTag_eff_value_med); //this is a c jet, shift bc uncertainty by sigma
               }

               //////// TIGHT WP SCALE FACTOR ////////
               if(deepJetScore > deepjet_wp_tight)
               {
                  // tight WP
                  MC_tagged  *= bTag_eff_value;
                  data_tagged *= SF*bTag_eff_value;
                  data_tagged_up *= SF_up*bTag_eff_value;
                  data_tagged_down *= SF_down*bTag_eff_value;

                  data_tagged_light_up    *= SF*bTag_eff_value;      // this is a c jet, so leave light jet SF as nom
                  data_tagged_light_down  *= SF*bTag_eff_value;      // this is a c jet, so leave light jet SF as nom
                  data_tagged_bc_up       *= SF_up*bTag_eff_value;   //this is a c jet, shift bc uncertainty by sigma
                  data_tagged_bc_down     *= SF_down*bTag_eff_value; //this is a c jet, shift bc uncertainty by sigma

                  if(_verbose)std::cout << "tagged c jet: MC_tagged/data_tagged: " << bTag_eff_value << ":" << SF*bTag_eff_value << std::endl;
               }
               else
               {

                  // tight WP
                  MC_notTagged *= (1 - bTag_eff_value);
                  data_notTagged *= (1 - SF*bTag_eff_value);
                  data_notTagged_up *= (1 - SF_up*bTag_eff_value);
                  data_notTagged_down *= (1 - SF_down*bTag_eff_value);

                  data_notTagged_light_up     *= (1 - SF*bTag_eff_value); // this is a c jet, so leave light jet SF as nom
                  data_notTagged_light_down   *= (1 - SF*bTag_eff_value); // this is a c jet, so leave light jet SF as nom
                  data_notTagged_bc_up        *= (1 - SF_up*bTag_eff_value); //this is a c jet, shift bc uncertainty by sigma
                  data_notTagged_bc_down      *= (1 - SF_down*bTag_eff_value); //this is a c jet, shift bc uncertainty by sigma

                  if(_verbose)std::cout << "untagged c jet: MC_notTagged/data_notTagged: " << (1- bTag_eff_value) << ":" << (1- SF*bTag_eff_value) << std::endl;
               }
            }
            else if(corrJet.hadronFlavour() == 5) // b jets
            {
               bTag_eff_value = truebjet_eff->GetBinContent(xbin,ybin);
               bTag_eff_value_med = truebjet_eff_med->GetBinContent(xbin,ybin);

               SF             = cset_corrector_bc->evaluate(   {"central", "T", 5, std::abs(corrJet.eta()), corrJet.pt()}); // tight WP
               SF_med         = cset_corrector_bc->evaluate(   {"central", "M", 5, std::abs(corrJet.eta()), corrJet.pt()}); // med WP

               jet_bTagSF_b_T[nTrue_b_AK4] = SF;
               jet_bTagSF_b_M[nTrue_b_AK4] = SF_med;

               nTrue_b_AK4++;

               // tight WP
               SF_up = cset_corrector_bc->evaluate({"up_correlated", "T", 5, std::abs(corrJet.eta()), corrJet.pt()});
               SF_down = cset_corrector_bc->evaluate(   {"down_correlated", "T", 5, std::abs(corrJet.eta()), corrJet.pt()});

               // med WP
               SF_up_med = cset_corrector_bc->evaluate({"up_correlated", "M", 5, std::abs(corrJet.eta()), corrJet.pt()});
               SF_down_med = cset_corrector_bc->evaluate(   {"down_correlated", "M", 5, std::abs(corrJet.eta()), corrJet.pt()});

               if(_verbose)std::cout << "Scale factor is " << SF << std::endl;

               //////// MED WP SCALE FACTOR ////////
               if(deepJetScore > deepJet_wp_med)
               {
                  // med WP
                  MC_tagged_med  *= bTag_eff_value_med;
                  data_tagged_med *= SF_med*bTag_eff_value_med;
                  data_tagged_up_med *= SF_up_med*bTag_eff_value_med;
                  data_tagged_down_med *= SF_down_med*bTag_eff_value_med;

                  data_tagged_light_up_med    *= SF_med*bTag_eff_value_med;      // this is a b jet, so leave light jet SF as nom
                  data_tagged_light_down_med  *= SF_med*bTag_eff_value_med;      // this is a b jet, so leave light jet SF as nom
                  data_tagged_bc_up_med       *= SF_up_med*bTag_eff_value_med;   //this is a b jet, shift bc uncertainty by sigma
                  data_tagged_bc_down_med     *= SF_down_med*bTag_eff_value_med; //this is a b jet, shift bc uncertainty by sigma
               }
               else
               {

                  // med WP
                  MC_notTagged_med *= (1 - bTag_eff_value_med);
                  data_notTagged_med *= (1 - SF_med*bTag_eff_value_med);
                  data_notTagged_up_med *= (1 - SF_up_med*bTag_eff_value_med);
                  data_notTagged_down_med *= (1 - SF_down_med*bTag_eff_value_med);

                  data_notTagged_light_up_med     *= (1 - SF_med*bTag_eff_value_med); // this is a c jet, so leave light jet SF as nom
                  data_notTagged_light_down_med   *= (1 - SF_med*bTag_eff_value_med); // this is a c jet, so leave light jet SF as nom
                  data_notTagged_bc_up_med        *= (1 - SF_up_med*bTag_eff_value_med); //this is a c jet, shift bc uncertainty by sigma
                  data_notTagged_bc_down_med      *= (1 - SF_down_med*bTag_eff_value_med); //this is a c jet, shift bc uncertainty by sigma
               }

               //////// TIGHT WP SCALE FACTOR ////////
               if(deepJetScore > deepjet_wp_tight)
               {
                  // tight WP
                  MC_tagged  *= bTag_eff_value;
                  data_tagged *= SF*bTag_eff_value;
                  data_tagged_up *= SF_up*bTag_eff_value;
                  data_tagged_down *= SF_down*bTag_eff_value;

                  data_tagged_light_up    *= SF*bTag_eff_value;      // this is a b jet, so leave light jet SF as nom
                  data_tagged_light_down  *= SF*bTag_eff_value;      // this is a b jet, so leave light jet SF as nom
                  data_tagged_bc_up       *= SF_up*bTag_eff_value;   //this is a b jet, shift bc uncertainty by sigma
                  data_tagged_bc_down     *= SF_down*bTag_eff_value; //this is a b jet, shift bc uncertainty by sigma
                  if(_verbose)std::cout << "tagged b jet: MC_tagged/data_tagged: " << bTag_eff_value << ":" << SF*bTag_eff_value << std::endl;
               }
               else
               {  

                  // tight WP
                  MC_notTagged *= (1 - bTag_eff_value);
                  data_notTagged *= (1 - SF*bTag_eff_value);
                  data_notTagged_up *= (1 - SF_up*bTag_eff_value);
                  data_notTagged_down *= (1 - SF_down*bTag_eff_value);

                  data_notTagged_light_up     *= (1 - SF*bTag_eff_value); // this is a c jet, so leave light jet SF as nom
                  data_notTagged_light_down   *= (1 - SF*bTag_eff_value); // this is a c jet, so leave light jet SF as nom
                  data_notTagged_bc_up        *= (1 - SF_up*bTag_eff_value); //this is a c jet, shift bc uncertainty by sigma
                  data_notTagged_bc_down      *= (1 - SF_down*bTag_eff_value); //this is a c jet, shift bc uncertainty by sigma
                  if(_verbose)std::cout << "untagged b jet: MC_notTagged/data_notTagged: " << (1- bTag_eff_value) << ":" << (1- SF*bTag_eff_value) << std::endl;
               }
            }
            else
            {
               std::cout << "ERROR: invalid AK4 hadron flavour. " << std::endl;
            }
            double epsilon = 1e-12;
            if ( (abs(MC_tagged)<epsilon )||(abs(MC_tagged)<epsilon )||(abs(MC_tagged)<epsilon )||(abs(MC_tagged)<epsilon ))
            {
                  if(_verbose)std::cout << "bTag_eff_value/SF/MC_tagged/MC_notTagged/data_tagged/data_notTagged: " << bTag_eff_value<< " / " << SF<< " / " << MC_tagged<< " / " << MC_notTagged << " / " <<data_tagged << " / " << data_notTagged << std::endl;
            }
         }
      }

      int vetoMap_xbin = (int)((corrJet.eta() - jetVetoMap_Xmin)*jetVetoMap_nBinsX/jetVetoMap_XRange ) +1;
      int vetoMap_ybin = (int)((corrJet.phi() - jetVetoMap_Ymin)*jetVetoMap_nBinsY/jetVetoMap_YRange ) +1;       // pt(b) = b*(range / # bins) + b0 -> (pt(b) - b0)*(# bins / range)
      AK4_fails_veto_map[nAK4] = false;
      if(jetVetoMap->GetBinContent(vetoMap_xbin,vetoMap_ybin) > 0) AK4_fails_veto_map[nAK4] = true;

      lab_AK4_pt[nAK4] = corrJet.pt();
      AK4_mass[nAK4] = corrJet.mass();
      AK4_ptot[nAK4] = corrJet.p();
      AK4_eta[nAK4] = corrJet.eta();
      AK4_phi[nAK4] = corrJet.phi();
      AK4_bdisc[nAK4] = corrJet.bDiscriminator("pfDeepCSVJetTags:probb") + corrJet.bDiscriminator("pfDeepCSVJetTags:probbb");
      AK4_DeepJet_disc[nAK4] = deepJetScore;
      selectedAK4_TLV.push_back(TLorentzVector(corrJet.px(),corrJet.py(),corrJet.pz(),corrJet.energy()));
      AK4_is_near_highE_CA4[nAK4] = false; // initialize this to false
      lab_AK4_AK8_parent[nAK4] = -1;  // init this to identify AK4 jets that aren't inside any AK8 jets
      AK4_SJ_assignment[nAK4] = -999; // this will correspond to AK4 jets that are not assigned to a superjet (because they're not in an AK8 jet)

      if(nAK4 < 4)
      {
         leadAK4Jets.push_back(TLorentzVector(corrJet.px(),corrJet.py(),corrJet.pz(),corrJet.energy()));
      }
      nAK4++;

   }

   ///////////////////// calculate b tag event weights //////////////////////
   if(doBtagSF)
   {
      // tight WP
      bTag_eventWeight_T_nom =  (data_tagged*data_notTagged) / (MC_tagged*MC_notTagged);
      bTag_eventWeight_T_up  =  (data_tagged_up*data_notTagged_up) / (MC_tagged*MC_notTagged);    // MC portion doesn't contain any scale factor portion, so there is no "up" or "down"
      bTag_eventWeight_T_down = (data_tagged_down*data_notTagged_down) / (MC_tagged*MC_notTagged);   

      bTag_eventWeight_light_T_up  =  (data_tagged_light_up*data_notTagged_light_up) / (MC_tagged*MC_notTagged);    // MC portion doesn't contain any scale factor portion, so there is no "up" or "down"
      bTag_eventWeight_light_T_down = (data_tagged_light_down*data_notTagged_light_down) / (MC_tagged*MC_notTagged);   

      bTag_eventWeight_bc_T_up  =  (data_tagged_bc_up*data_notTagged_bc_up) / (MC_tagged*MC_notTagged);    // MC portion doesn't contain any scale factor portion, so there is no "up" or "down"
      bTag_eventWeight_bc_T_down = (data_tagged_bc_down*data_notTagged_bc_down) / (MC_tagged*MC_notTagged);   

      // med WP
      bTag_eventWeight_M_nom =  (data_tagged_med*data_notTagged_med) / (MC_tagged_med*MC_notTagged_med);
      bTag_eventWeight_M_up  =  (data_tagged_up_med*data_notTagged_up_med) / (MC_tagged_med*MC_notTagged_med);    // MC portion doesn't contain any scale factor portion, so there is no "up" or "down"
      bTag_eventWeight_M_down = (data_tagged_down_med*data_notTagged_down_med) / (MC_tagged_med*MC_notTagged_med);   

      bTag_eventWeight_light_M_up  =  (data_tagged_light_up_med*data_notTagged_light_up_med) / (MC_tagged_med*MC_notTagged_med);    // MC portion doesn't contain any scale factor portion, so there is no "up" or "down"
      bTag_eventWeight_light_M_down = (data_tagged_light_down_med*data_notTagged_light_down_med) / (MC_tagged_med*MC_notTagged_med);   

      bTag_eventWeight_bc_M_up  =  (data_tagged_bc_up_med*data_notTagged_bc_up_med) / (MC_tagged_med*MC_notTagged_med);    // MC portion doesn't contain any scale factor portion, so there is no "up" or "down"
      bTag_eventWeight_bc_M_down = (data_tagged_bc_down_med*data_notTagged_bc_down_med) / (MC_tagged_med*MC_notTagged_med);   

      if ((bTag_eventWeight_T_nom != bTag_eventWeight_T_nom) || (std::isinf(bTag_eventWeight_T_nom)) || (bTag_eventWeight_T_nom < 0.))
      {
         //std::cout << "BAD BTAG SF: " << bTag_eventWeight_T_nom << std::endl;
         if(_verbose)std::cout << "data_tagged_up/data_notTagged_up/MC_tagged/MC_notTagged: " <<data_tagged << "/" <<data_notTagged << "/" << MC_notTagged<< "/" << MC_notTagged<<  std::endl;
      } 


      if ((bTag_eventWeight_M_nom != bTag_eventWeight_M_nom) || (std::isinf(bTag_eventWeight_M_nom)) || (bTag_eventWeight_M_nom < 0.))
      {
         //std::cout << "BAD BTAG SF: " << bTag_eventWeight_T_nom << std::endl;
         if(_verbose)std::cout << "data_tagged_med/data_notTagged__med/MC_tagged__med/MC_notTagged_med: " <<data_tagged_med << "/" <<data_notTagged_med << "/" << MC_notTagged_med<< "/" << MC_notTagged_med<<  std::endl;
      } 



   }

   lab_nAK4 = nAK4;

   if(nAK4 <2)return;   // RETURN cut




   /////////////////////////////////
   // Apply HT selection criteria //
   /////////////////////////////////

   //std::cout << "WARNING: cuts are disabled (for testing)." << std::endl;
   


//   if( ( runSideband) )
//   {  
//      if( (totHT > 1600) ||  (totHT  < 1200)  ) return; // only want to lower HT region, RETURN cut
//   }
//   else // MAIN-BAND
//   {
//      if(slimmedSelection) // slimmed selection to save space
//      {
//         if( totHT < 1600 ) return; // // RETURN cut
//      }
//      else // normal selection 
//      {
//         if( totHT < 1400 ) return; // RETURN cut
//      }
//   }




   ///////////////////////////////////
   ////// Calculate dijet masses /////
   ///////////////////////////////////


   // error avoidance
   if (nAK4 < 3) 
   {
      leadAK4Jets.push_back(TLorentzVector(0,0,0,0)); 
      leadAK4Jets.push_back(TLorentzVector(0,0,0,0));  
   }
   else if(nAK4 < 4) leadAK4Jets.push_back(TLorentzVector(0,0,0,0));

   // calculate the candidate dijet delta R values
   double minDeltaRDisc12 = sqrt( pow(leadAK4Jets[0].DeltaR(leadAK4Jets[1]),2) + pow(leadAK4Jets[2].DeltaR(leadAK4Jets[3]),2));    // dijet one always has j1 in it
   double minDeltaRDisc13 = sqrt( pow(leadAK4Jets[0].DeltaR(leadAK4Jets[2]),2) + pow(leadAK4Jets[1].DeltaR(leadAK4Jets[3]),2));
   double minDeltaRDisc14 = sqrt( pow(leadAK4Jets[0].DeltaR(leadAK4Jets[3]),2) + pow(leadAK4Jets[1].DeltaR(leadAK4Jets[2]),2));

   if (  abs(min(minDeltaRDisc12, min(minDeltaRDisc13,minDeltaRDisc14)) -minDeltaRDisc12)<1e-8 ) 
   {
      //set dijet masses
      dijetMassOne = (leadAK4Jets[0] +leadAK4Jets[1]).M();
      dijetMassTwo = (leadAK4Jets[2] +leadAK4Jets[3]).M();
   }
   else if (  abs(min(minDeltaRDisc12, min(minDeltaRDisc13,minDeltaRDisc14)) -minDeltaRDisc13)<1e-8 ) 
   {
      // set dijet masses
      dijetMassOne = (leadAK4Jets[0] +leadAK4Jets[2]).M();
      dijetMassTwo = (leadAK4Jets[1] +leadAK4Jets[3]).M();
   }
   else if (  abs(min(minDeltaRDisc12, min(minDeltaRDisc13,minDeltaRDisc14)) -minDeltaRDisc14)<1e-8 ) 
   {
      //set dijet masses
      dijetMassOne = (leadAK4Jets[0] +leadAK4Jets[3]).M();
      dijetMassTwo = (leadAK4Jets[1] +leadAK4Jets[2]).M();
   }

   if(_verbose)std::cout << "before MET loop" << std::endl;


   /*
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////_MET_///////////////////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

   edm::Handle<std::vector<pat::MET> > _totMET;
   iEvent.getByToken(metToken_, _totMET);
   for(auto iMET = _totMET->begin(); iMET != _totMET->end(); iMET++)
   {
      if(iMET->isPFMET())
      {
         totMET = iMET->pt();
      }
   }
   */

   if(_verbose)std::cout << "before AK8 jets" << std::endl;

   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////_AK8 Jets_/////////////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   std::vector<reco::LeafCandidate> candsBoosted;   
   std::vector<reco::LeafCandidate> candsUnboosted;
   std::vector<fastjet::PseudoJet> superJetOne;        //jets for dot prduct #cos(theta) > 0
   std::vector<fastjet::PseudoJet> superJetTwo;       //jets for dot product #cos(theta) < 0
   std::vector<TLorentzVector> superJetOneLV;        //workaround for weird fastjet bug
   std::vector<TLorentzVector> superJetTwoLV;        //workaround for weird fastjet bug

   std::vector<TLorentzVector> selectedAK8_TLV;
   edm::Handle<std::vector<pat::Jet> > fatJets;
   iEvent.getByToken(fatJetToken_, fatJets);

   //Run3 comment out
   //JME::JetResolution resolution = JME::JetResolution::get(iSetup, "AK8PF_pt");                          //load JER stuff from global tag
   //JME::JetResolutionScaleFactor resolution_sf = JME::JetResolutionScaleFactor::get(iSetup, "AK8PF");
   
   /*
   //Run3 added
   auto const& resolution = iSetup.getData(AK8JER_token_);
   auto const& resolution_sf = iSetup.getData(AK8JERSF_token_);
   */


   nHeavyAK8_pt400_M10 = 0; nHeavyAK8_pt400_M20 = 0; nHeavyAK8_pt400_M30 = 0;
   nHeavyAK8_pt300_M10 = 0; nHeavyAK8_pt300_M20 = 0; nHeavyAK8_pt300_M30 = 0; 
   nHeavyAK8_pt200_M10 = 0; nHeavyAK8_pt200_M20 = 0; nHeavyAK8_pt200_M30 = 0; 


   nfatjets = 0;
   nfatjet_pre = 0;
   TLorentzVector allAK8(0,0,0,0);
   std::vector<TLorentzVector> leadAK8Jets;
   //diAK8Jet_mass
   double tot_jet_px=0,tot_jet_py=0,tot_jet_pz=0, tot_jet_E=0;


   // loop over AK8 jets, save information for event selection, grab particles to create superjets
   for(auto iJet = fatJets->begin(); iJet != fatJets->end(); iJet++)    
   {


      if( (sqrt(pow(iJet->mass(),2)+pow(iJet->pt(),2)) < 25.) || (!(iJet->isPFJet())) || ( abs(iJet->eta()) > 2.4 )) continue;   //don't even bother with these jets

      double AK8_sf_total = 1.0;     // this scales jet/particle 4-vectors, compounds all scale factors

      double AK8_JEC_corr_factor = 1.0;

      ////////////////////////////////////////////////////////////////////////////////////////////////////////////
      ///////// _JET ENERGY CORRECTION UNCERTAINTY STUFF, AK8 jets should already be corrected in the cfg ////////
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////

      if(doJEC)
      {

         //jecUnc_AK8->setJetEta( iJet->eta() );
         //jecUnc_AK8->setJetPt( iJet->pt() );
         // JECs are already applied in cfg with updateJetCollection, this is for getting the uncertainties 
         //double AK8_JEC_uncertainty = jecUnc_AK8->getUncertainty(true);
        double  AK8_JEC_uncertainty = getJECUncertaintyFromSources("AK8",  iJet->pt(), iJet->eta());

         if (systematicType.find("_up") != std::string::npos)      
         {
            AK8_JEC_corr_factor = 1 + AK8_JEC_uncertainty;
         }
         else if (systematicType.find("_down") != std::string::npos)      
         {
            AK8_JEC_corr_factor = 1 - AK8_JEC_uncertainty;
         }

         AK8_JEC[nfatjets] = AK8_JEC_corr_factor;
         AK8_sf_total*=AK8_JEC_corr_factor;

      }

  /*    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
      //////////   _JET ENERGY RESOLUTION STUFF //////////////////////////////////////////////////////////////////
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////
      if(doJER)
      {
         double AK8_JER_corr_factor = 1.0; // this won't be touched for data
         
         if( (runType.find("MC") != std::string::npos)|| (runType.find("Tprime")!= std::string::npos ) ) //if((runType == "SigMC") || (runType == "QCDMC") || (runType == "TTbarMC") ) 
         {
            if(_verbose)std::cout << "doing JER" << std::endl;

            double sJER     = -9999.;    //JER scale factor
            double sigmaJER = -9999.;    //this is the "resolution" you get from the scale factors 
            
            //these are for getting the JER scale factors
            JME::JetParameters parameters_1;
            parameters_1.setJetPt(iJet->pt());
            parameters_1.setJetEta(iJet->eta());
            parameters_1.setRho(*rho);
            sigmaJER = resolution.getResolution(parameters_1);   //pT resolution

            //JME::JetParameters
            JME::JetParameters parameters;
            parameters.setJetPt(iJet->pt());
            parameters.setJetEta(iJet->eta());
            sJER =  resolution_sf.getScaleFactor(parameters  );  //{{JME::Binning::JetEta, iJet->eta()}});

            if( applyJERSource(systematicType, iJet->eta())  )
            {
               if( systematicType.find("_up") != std::string::npos ) 
               {
                  sJER = resolution_sf_AK4.getScaleFactor(parameters, Variation::UP);    // SF + 1 sigma uncertainty
               }
               else if( systematicType.find("_down") != std::string::npos ) 
               {
                  sJER = resolution_sf_AK4.getScaleFactor(parameters, Variation::DOWN);  // SF - 1 sigma uncertainty
               }
            }

            const reco::GenJet *genJet = iJet->genJet();
            if( genJet)   // try the first technique
            {
               AK8_JER_corr_factor = 1 + (sJER - 1)*(iJet->pt()-genJet->pt())/iJet->pt();
            }
            else   // if no gen jet is matched, try the second technique
            {
               randomNum->SetSeed( abs(static_cast<int>(iJet->phi()*1e4)) );
               double JERrand = randomNum->Gaus(0.0, sigmaJER);
               //double JERrand = 1.0 + sigmaJER;
               AK8_JER_corr_factor = max(0., 1 + JERrand*sqrt(max(pow(sJER,2)-1,0.)));   //want to make sure this is truncated at 0
            }
            AK8_JER[nfatjets] = AK8_JER_corr_factor;

            if(_verbose)std::cout << "finished with JER" << std::endl;
         }
         AK8_sf_total*= AK8_JER_corr_factor;
      }
      //JME::JetParameters parameters = {{JME::Binning::JetEta, AK8_JEC_corr_factor*iJet->eta()}, {JME::Binning::Rho, rho}};


      /////////////////////////////////////////////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////

*/

      // create corrected (scaled) jet object that will be used for cuts 
      pat::Jet corrJet(*iJet);
      LorentzVector corrJetP4(AK8_sf_total*iJet->px(),AK8_sf_total*iJet->py(),AK8_sf_total*iJet->pz(),AK8_sf_total*iJet->energy());
      corrJet.setP4(corrJetP4);

      if(_verbose)  std::cout << "nominal p4: " << iJet->px()<< "," <<iJet->py() << "," << iJet->pz()<< "," << iJet->energy()<< std::endl;
      if(_verbose)  std::cout << "corrected p4: " << corrJet.px()<< "," <<corrJet.py() << "," << corrJet.pz()<< "," << corrJet.energy()<< std::endl;

      if((corrJet.pt() > 200.) && ((corrJet.isPFJet())) && (isgoodjet(corrJet.eta(),corrJet.neutralHadronEnergyFraction(), corrJet.neutralEmEnergyFraction(),corrJet.numberOfDaughters(),corrJet.chargedHadronEnergyFraction(),corrJet.chargedMultiplicity(),corrJet.muonEnergyFraction(),corrJet.chargedEmEnergyFraction(),nfatjets) ) && (corrJet.userFloat("ak8PFJetsPuppiSoftDropMass") > 45.))//this is heavy AK8 jet cut, I changed 500 to 200 
      {
         if(isHEM(corrJet.eta(),corrJet.phi())) jet_pre_isHEM[nfatjet_pre] = true;
         else{ jet_pre_isHEM[nfatjet_pre] = false;}

         nfatjet_pre++;
      }

      if((sqrt(pow(corrJet.mass(),2)+pow(corrJet.pt(),2)) < AK8_Et_cut) || (!(corrJet.isPFJet())) || (!isgoodjet(corrJet.eta(),corrJet.neutralHadronEnergyFraction(), corrJet.neutralEmEnergyFraction(),corrJet.numberOfDaughters(),corrJet.chargedHadronEnergyFraction(),corrJet.chargedMultiplicity(),corrJet.muonEnergyFraction(),corrJet.chargedEmEnergyFraction(),nfatjets )) || (corrJet.mass()< 0.)) continue; //userFloat("ak8PFJetsPuppiSoftDropMass")//this is normal AK8 jet cut, I changed E cut 200 to 50
      


      if((corrJet.pt() > 400.) && ((corrJet.isPFJet())) && (isgoodjet(corrJet.eta(),corrJet.neutralHadronEnergyFraction(), corrJet.neutralEmEnergyFraction(),corrJet.numberOfDaughters(),corrJet.chargedHadronEnergyFraction(),corrJet.chargedMultiplicity(),corrJet.muonEnergyFraction(),corrJet.chargedEmEnergyFraction(),nfatjets) ) && (corrJet.userFloat("ak8PFJetsPuppiSoftDropMass") > 10.) && (!isHEM(corrJet.eta(),corrJet.phi()))) nHeavyAK8_pt400_M10++;
      if((corrJet.pt() > 400.) && ((corrJet.isPFJet())) && (isgoodjet(corrJet.eta(),corrJet.neutralHadronEnergyFraction(), corrJet.neutralEmEnergyFraction(),corrJet.numberOfDaughters(),corrJet.chargedHadronEnergyFraction(),corrJet.chargedMultiplicity(),corrJet.muonEnergyFraction(),corrJet.chargedEmEnergyFraction(),nfatjets) ) && (corrJet.userFloat("ak8PFJetsPuppiSoftDropMass") > 20.) && (!isHEM(corrJet.eta(),corrJet.phi()))) nHeavyAK8_pt400_M20++;
      if((corrJet.pt() > 400.) && ((corrJet.isPFJet())) && (isgoodjet(corrJet.eta(),corrJet.neutralHadronEnergyFraction(), corrJet.neutralEmEnergyFraction(),corrJet.numberOfDaughters(),corrJet.chargedHadronEnergyFraction(),corrJet.chargedMultiplicity(),corrJet.muonEnergyFraction(),corrJet.chargedEmEnergyFraction(),nfatjets) ) && (corrJet.userFloat("ak8PFJetsPuppiSoftDropMass") > 30.) && (!isHEM(corrJet.eta(),corrJet.phi()))) nHeavyAK8_pt400_M30++;


      if((corrJet.pt() > 300.) && ((corrJet.isPFJet())) && (isgoodjet(corrJet.eta(),corrJet.neutralHadronEnergyFraction(), corrJet.neutralEmEnergyFraction(),corrJet.numberOfDaughters(),corrJet.chargedHadronEnergyFraction(),corrJet.chargedMultiplicity(),corrJet.muonEnergyFraction(),corrJet.chargedEmEnergyFraction(),nfatjets) ) && (corrJet.userFloat("ak8PFJetsPuppiSoftDropMass") > 10.) && (!isHEM(corrJet.eta(),corrJet.phi()))) nHeavyAK8_pt300_M10++;
      if((corrJet.pt() > 300.) && ((corrJet.isPFJet())) && (isgoodjet(corrJet.eta(),corrJet.neutralHadronEnergyFraction(), corrJet.neutralEmEnergyFraction(),corrJet.numberOfDaughters(),corrJet.chargedHadronEnergyFraction(),corrJet.chargedMultiplicity(),corrJet.muonEnergyFraction(),corrJet.chargedEmEnergyFraction(),nfatjets) ) && (corrJet.userFloat("ak8PFJetsPuppiSoftDropMass") > 20.) && (!isHEM(corrJet.eta(),corrJet.phi()))) nHeavyAK8_pt300_M20++;
      if((corrJet.pt() > 300.) && ((corrJet.isPFJet())) && (isgoodjet(corrJet.eta(),corrJet.neutralHadronEnergyFraction(), corrJet.neutralEmEnergyFraction(),corrJet.numberOfDaughters(),corrJet.chargedHadronEnergyFraction(),corrJet.chargedMultiplicity(),corrJet.muonEnergyFraction(),corrJet.chargedEmEnergyFraction(),nfatjets) ) && (corrJet.userFloat("ak8PFJetsPuppiSoftDropMass") > 30.) && (!isHEM(corrJet.eta(),corrJet.phi()))) nHeavyAK8_pt300_M30++;


      if((corrJet.pt() > 200.) && ((corrJet.isPFJet())) && (isgoodjet(corrJet.eta(),corrJet.neutralHadronEnergyFraction(), corrJet.neutralEmEnergyFraction(),corrJet.numberOfDaughters(),corrJet.chargedHadronEnergyFraction(),corrJet.chargedMultiplicity(),corrJet.muonEnergyFraction(),corrJet.chargedEmEnergyFraction(),nfatjets) ) && (corrJet.userFloat("ak8PFJetsPuppiSoftDropMass") > 10.) && (!isHEM(corrJet.eta(),corrJet.phi()))) nHeavyAK8_pt200_M10++;
      if((corrJet.pt() > 200.) && ((corrJet.isPFJet())) && (isgoodjet(corrJet.eta(),corrJet.neutralHadronEnergyFraction(), corrJet.neutralEmEnergyFraction(),corrJet.numberOfDaughters(),corrJet.chargedHadronEnergyFraction(),corrJet.chargedMultiplicity(),corrJet.muonEnergyFraction(),corrJet.chargedEmEnergyFraction(),nfatjets) ) && (corrJet.userFloat("ak8PFJetsPuppiSoftDropMass") > 20.) && (!isHEM(corrJet.eta(),corrJet.phi()))) nHeavyAK8_pt200_M20++;
      if((corrJet.pt() > 200.) && ((corrJet.isPFJet())) && (isgoodjet(corrJet.eta(),corrJet.neutralHadronEnergyFraction(), corrJet.neutralEmEnergyFraction(),corrJet.numberOfDaughters(),corrJet.chargedHadronEnergyFraction(),corrJet.chargedMultiplicity(),corrJet.muonEnergyFraction(),corrJet.chargedEmEnergyFraction(),nfatjets) ) && (corrJet.userFloat("ak8PFJetsPuppiSoftDropMass") > 30.) && (!isHEM(corrJet.eta(),corrJet.phi()))) nHeavyAK8_pt200_M30++;



      if(isHEM(corrJet.eta(),corrJet.phi()))
      {
         fatjet_isHEM[nfatjets] = true;
      } 
      else{fatjet_isHEM[nfatjets] = false;}

      JEC_uncert_AK8[nfatjets] = AK8_JEC_corr_factor;

      int vetoMap_xbin = (int)((corrJet.eta() - jetVetoMap_Xmin)*jetVetoMap_nBinsX/jetVetoMap_XRange ) +1;
      int vetoMap_ybin = (int)((corrJet.phi() - jetVetoMap_Ymin)*jetVetoMap_nBinsY/jetVetoMap_YRange ) +1;       // pt(b) = b*(range / # bins) + b0 -> (pt(b) - b0)*(# bins / range)
      AK8_fails_veto_map[nfatjets] = false;
      if(jetVetoMap->GetBinContent(vetoMap_xbin,vetoMap_ybin) > 0.) AK8_fails_veto_map[nfatjets] = true; // saved for future vetoing

      if(nfatjets < 2) leadAK8_mass[nfatjets] = corrJet.userFloat("ak8PFJetsPuppiSoftDropMass");
      jet_pt[nfatjets] = corrJet.pt();
      jet_phi[nfatjets] = corrJet.phi();  
      jet_eta[nfatjets] = corrJet.eta();
      jet_mass[nfatjets] = corrJet.mass();


      // save selected AK8 jet particles for use in calculating the thrust axis
      for (unsigned int iii=0; iii<iJet->numberOfDaughters();iii++)   // get all jet particles
      {
         const reco::Candidate* iJ = iJet->daughter(iii);
         const pat::PackedCandidate* candJetbegin = (pat::PackedCandidate*) iJ;
         double puppiweight = candJetbegin->puppiWeight();
         candsUnboosted.push_back(LeafCandidate(iJet->daughter(iii)->charge(), Particle::LorentzVector(AK8_sf_total*puppiweight*iJ->px(), AK8_sf_total*puppiweight*iJ->py(), AK8_sf_total*puppiweight*iJ->pz(), AK8_sf_total*puppiweight*iJ->energy())));
         tot_jet_px+=AK8_sf_total*puppiweight*candJetbegin->px();tot_jet_py+=AK8_sf_total*puppiweight*candJetbegin->py();tot_jet_pz+=AK8_sf_total*puppiweight*candJetbegin->pz();tot_jet_E+=AK8_sf_total*puppiweight*candJetbegin->energy();
      }
      if(nfatjets < 4)
      {
         leadAK8Jets.push_back(TLorentzVector(corrJet.px(),corrJet.py(),corrJet.pz(),corrJet.energy()));
      }

      selectedAK8_TLV.push_back(TLorentzVector(corrJet.px(),corrJet.py(),corrJet.pz(),corrJet.energy()));

      // match up AK4 jets to AK8 jets (leave indices as -1 if not matched)
      int AK4_counter = 0;
      for(auto iAK4 = selectedAK4_TLV.begin();iAK4 != selectedAK4_TLV.end(); iAK4++ )
      {  
         if( sqrt(pow(iAK4->Eta()-corrJet.eta(),2)+ pow(iAK4->Phi()-corrJet.phi(),2)) < 0.8 )lab_AK4_AK8_parent[AK4_counter] = nfatjets;
         AK4_counter++;
      }

      AK8_is_near_highE_CA4[nfatjets] = false; // initialize this to false

      AK8_hadronFlavour[nfatjets] = (double)iJet->hadronFlavour();  // why are these showing up as non integers??
      AK8_partonFlavour[nfatjets] = (double)iJet->partonFlavour();
      if((AK8_hadronFlavour[nfatjets]  > 5) || (AK8_hadronFlavour[nfatjets]  < 0))std::cout << "hadron flavour found to be large or small: " << AK8_hadronFlavour[nfatjets]  << std::endl;

      nfatjets++;
   }
   event_reco_pt = sqrt(pow(tot_jet_px,2) + pow(tot_jet_py,2));

   diAK8Jet_mass[0] = 0; diAK8Jet_mass[1] = 0;
   fourAK8JetMass = 0;
   if(nfatjets >3)
   {

      fourAK8JetMass = (leadAK8Jets[0] + leadAK8Jets[1] +  leadAK8Jets[2] + leadAK8Jets[3]).M();
      double minDeltaRDisc12_AK8 = sqrt( pow(leadAK8Jets[0].DeltaR(leadAK8Jets[1]),2) + pow(leadAK8Jets[2].DeltaR(leadAK8Jets[3]),2));    // dijet one always has j1 in it
      double minDeltaRDisc13_AK8 = sqrt( pow(leadAK8Jets[0].DeltaR(leadAK8Jets[2]),2) + pow(leadAK8Jets[1].DeltaR(leadAK8Jets[3]),2));
      double minDeltaRDisc14_AK8 = sqrt( pow(leadAK8Jets[0].DeltaR(leadAK8Jets[3]),2) + pow(leadAK8Jets[1].DeltaR(leadAK8Jets[2]),2));

      if (  abs(min(minDeltaRDisc12_AK8, min(minDeltaRDisc13_AK8,minDeltaRDisc14_AK8)) - minDeltaRDisc12_AK8)<1e-8 ) 
      {
         //set dijet masses
         diAK8Jet_mass[0] = (leadAK8Jets[0] +leadAK8Jets[1]).M();
         diAK8Jet_mass[1] = (leadAK8Jets[2] +leadAK8Jets[3]).M();
      }
      else if (  abs(min(minDeltaRDisc12_AK8, min(minDeltaRDisc13_AK8,minDeltaRDisc14_AK8)) -minDeltaRDisc13_AK8)<1e-8 ) 
      {
         // set dijet masses
         diAK8Jet_mass[0] = (leadAK8Jets[0] +leadAK8Jets[2]).M();
         diAK8Jet_mass[1] = (leadAK8Jets[1] +leadAK8Jets[3]).M();
      }
      else if (  abs(min(minDeltaRDisc12_AK8, min(minDeltaRDisc13_AK8,minDeltaRDisc14_AK8)) -minDeltaRDisc14_AK8)<1e-8 ) 
      {
         //set dijet masses
         diAK8Jet_mass[0] = (leadAK8Jets[0] +leadAK8Jets[3]).M();
         diAK8Jet_mass[1] = (leadAK8Jets[1] +leadAK8Jets[2]).M();
      }
   }



//   if( !(runType.find("Tprime") != std::string::npos) || ( (systematicType.find("JEC") != std::string::npos ) ) )  // FOR NOM SIGNAL, SAVE MORE EVENTS FOR STUDYING
//   {
//      if(slimmedSelection)
//      {
//         if (  (nfatjets < 3) ||   ((nfatjet_pre < 2) && ((dijetMassOne < 1000.) || (dijetMassTwo < 1000.)  )  )  )  return;  // RETURN cut
//      }

//      else if ((nfatjets < 2) || (nfatjet_pre < 1) )return; // RETURN cut

//   }
//commented out

   /////////////////////////////////////////////////
   /////// Get L1 prefiring weight for event////////
   /////////////////////////////////////////////////

   /*if(doPrefiringWeight)
   {
      edm::Handle< double > theprefweight;
      iEvent.getByToken(prefweight_token, theprefweight ) ;
      prefiringWeight_nom =(*theprefweight);

      edm::Handle< double > theprefweightup;
      iEvent.getByToken(prefweightup_token, theprefweightup ) ;
      prefiringWeight_up =(*theprefweightup);

      edm::Handle< double > theprefweightdown;
      iEvent.getByToken(prefweightdown_token, theprefweightdown ) ;
      prefiringWeight_down =(*theprefweightdown);
   }*/



////////////////////////////////////////////////////////////
/////////////// calculate _top pt weights //////////////////   
////////////////////////////////////////////////////////////

   if( doTopPtReweight) // will only be true for TTbar samples
   {

   ////////////////////// Calculate top pt weight /////////////////
      iEvent.getByToken(genPartToken_, genPartCollection);
      reco::GenParticle * top_one = nullptr;
      reco::GenParticle * top_two = nullptr;
      int top_pdgid = 6;
      for( auto iG = genPartCollection->begin(); iG != genPartCollection->end(); iG++)
      {

         if( ( abs(iG->pdgId()) == top_pdgid ) && (  iG->isLastCopy() ))
         {
            if(top_one == NULL) top_one = iG->clone();
            else if (top_two == NULL) top_two = iG->clone();
            else
            {
               std::cout << "ERROR: There are somehow 3 tops in the event ..." << std::endl;
               return;  // RETURN cut
            }

         }
      }
      if ((top_one == NULL) || (top_two == NULL))
      {
         std::cout << "ERROR: Fewer than 2 tops found." << std::endl;
         return;  // RETURN cut
      }

      double genTopQuarkOne_pt=top_one->pt();
      double genTopQuarkTwo_pt=top_two->pt();
      top_pt_weight=sqrt(top_pt_SF(genTopQuarkOne_pt)*top_pt_SF(genTopQuarkTwo_pt));

   }

   /////////////////////////////////////////////////////////////
   /////////////// calculate _pdf weights //////////////////////
   /////////////////////////////////////////////////////////////

   /*if(doPDFWeights)
   {
      if(_verbose)std::cout << "Setting PDF variables";
      
      edm::Handle<GenEventInfoProduct> evt_info;
      iEvent.getByToken(GeneratorToken_, evt_info);

      nPDFWeights   = 0;
      nScaleWeights = 0;
      
      if( (runType.find("QCD") != std::string::npos) || (runType.find("TTJets") != std::string::npos)|| (runType.find("Tprime") != std::string::npos) )
      {

         double factorizWeights_up[nVars];
         double factorizWeights_down[nVars];
         double weightsForVar[nVars]; 


         //// calculate the up and down pdf wights the "BEST" bay
         double PDFWeightAvg = 0, PDFWeightStdDev= 0;
         double PDFwgt, PDFWeightNom, PDFWeightFrac;

         //edm::Handle<LHEEventProduct> lheEventProduct;
         //iEvent.getByToken(lheEventProductToken_, lheEventProduct);


         // PDF weight information: 
         // -------------------------------------------------------------

         // FOR QCD MC
         //PDF nom == 325300
         //Scale: ID=1002-1045, [MUR,MU=0.5/1/1.5, DynScale=1-4]
         //Renomalization Scale: ID = 1006 (MU_R=2.0), 1011 (MU_R=0.5) 
         // Factorization Scale: ID = 1016 (MU_F=2.0), 1021 (MU_F=0.5) 
         // variations 325301-325402 = PDF: ID=1047-1148


         // For TTJets MC:
         //PDF nom = 325300 = ID 1048
         // variations 325501-325600 = PDF, ID=1049-1148

         // For signal: 
         // PDF nom = 325500
         // Scale: ID=1002-1045
         //Renormalization scale: ID = 1006 (MU_R=2.0), 1011 (MU_R=0.5)
         // Factorization Scale: ID = 1016 (MU_F=2.0), 1026 (MU_F=0.5) 
         // variations 325301-325402 = PDF: ID=1049-1148

         // ---------------------------------------------------------------

         unsigned int PDFstart, PDFend;
         if(runType.find("QCD") != std::string::npos)
         {
            PDFstart = 46; // corresponding to <weight MUF="1.0" MUR="1.0" PDF="325301" id="1047"> PDF=325300 MemberID=1 </weight>
            PDFend = 147;  // cooresponding to <weight MUF="1.0" MUR="1.0" PDF="325402" id="1148"> PDF=325300 MemberID=102 </weight>
         }
         else if( runType.find("Tprime") != std::string::npos )
         {
            PDFstart = 48;
            PDFend = 147;
         }
         else if( runType.find("TTJets"))
         {     
             PDFstart =  48;  // <weight MUF="1.0" MUR="1.0" PDF="325301" id="1049"> PDF=325300 MemberID=1 </weight>
             PDFend   =  149; // <weight MUF="1.0" MUR="1.0" PDF="325402" id="1150"> PDF=325300 MemberID=102 </weight>
         }
         else   // generic (won't work for powheg samples like TTTo)
         {     
             PDFstart =  48;  // <weight MUF="1.0" MUR="1.0" PDF="325301" id="1049"> PDF=325300 MemberID=1 </weight>
             PDFend   =  149; // <weight MUF="1.0" MUR="1.0" PDF="325402" id="1150"> PDF=325300 MemberID=102 </weight>
         }
            
         //PDFWeightNom = lheEventProduct->originalXWGTUP();

         for(unsigned int iii = 0;  iii < lheEventProduct->weights().size();  iii++) {
            //std::cout<<"Systematic "<<iii<<" ID: "<<lheEventProduct->weights()[iii].id<<", value "<<lheEventProduct->weights()[iii].wgt<<", ratio "<<lheEventProduct->weights()[iii].wgt/PDFWeightNom<<std::endl;
            if( iii >= PDFstart && iii < PDFend) 
            {
               PDFwgt = lheEventProduct->weights()[iii].wgt;
               PDFWeightFrac = PDFwgt/PDFWeightNom;
               PDFWeightAvg += PDFWeightFrac;
            }
         }

         PDFWeightAvg = PDFWeightAvg/(PDFend - PDFstart);
         for (unsigned int i=PDFstart; i < PDFend; ++i) 
         {
            PDFwgt = lheEventProduct->weights()[i].wgt;
            PDFWeightFrac = PDFwgt/(PDFWeightNom);
            PDFWeightStdDev += std::pow((PDFWeightFrac - PDFWeightAvg),2);
         }
         PDFWeightStdDev = sqrt(PDFWeightStdDev/(PDFend - PDFstart - 1)); 
         PDFWeightUp = 1.0 + PDFWeightStdDev;
         PDFWeightDown = 1.0 - PDFWeightStdDev;

         //// calculate the renormalization and factorizations the "Ben" way

         // This is the way BEST is doing it
         //QCDRenormalization_up_BEST      = lheEventProduct->weights()[5].wgt /PDFWeightNom;   // muF = 1.0, muR = 2.0
         //QCDRenormalization_down_BEST    = lheEventProduct->weights()[10].wgt/PDFWeightNom;  // muF = 1.0, muR = 0.5
         //QCDFactorization_up_BEST        = lheEventProduct->weights()[15].wgt/PDFWeightNom;  // muF = 2.0, muR = 1.0
         //QCDFactorization_down_BEST      = lheEventProduct->weights()[30].wgt/PDFWeightNom;  // muF = 0.5, muR = 1.0

         // envelope indices are the same for QCD and TTbarMC   
         
         scale_envelope[0] = lheEventProduct->weights()[15].wgt;  // index 0: muF = 2.0, muR = 1.0 : <weight MUF="2.0" MUR="1.0" PDF="325300" id="1016"> MUF=2.0  </weight>
         scale_envelope[1] = lheEventProduct->weights()[30].wgt;  // index 1: muF = 0.5, muR = 1.0 : <weight MUF="0.5" MUR="1.0" PDF="325300" id="1031"> MUF=0.5  </weight>
         scale_envelope[2] = lheEventProduct->weights()[20].wgt;  // index 2: muF = 2.0, muR = 2.0 : <weight MUF="2.0" MUR="2.0" PDF="325300" id="1021"> MUR=2.0 MUF=2.0  </weight>
         scale_envelope[3] = lheEventProduct->weights()[5].wgt;   // index 3: muF = 1.0, muR = 2.0 : <weight MUF="1.0" MUR="2.0" PDF="325300" id="1006"> MUR=2.0  </weight>
         scale_envelope[4] = lheEventProduct->weights()[40].wgt;  // index 4: muF = 0.5, muR = 0.5 : <weight MUF="0.5" MUR="0.5" PDF="325300" id="1041"> MUR=0.5 MUF=0.5  </weight>
         scale_envelope[5] = lheEventProduct->weights()[10].wgt;  // index 5: muF = 1.0, muR = 0.5 : <weight MUF="1.0" MUR="0.5" PDF="325300" id="1011"> MUR=0.5  </weight>
         scale_envelope[6] = lheEventProduct->weights()[25].wgt;  // index 6: muF = 2.0, muR = 0.5 : <weight MUF="2.0" MUR="0.5" PDF="325300" id="1026"> MUR=0.5 MUF=2.0  </weight>   : this shouldn't be needed
         scale_envelope[7] = lheEventProduct->weights()[35].wgt;  // index 7: muF = 0.5, muR = 2.0 : <weight MUF="0.5" MUR="2.0" PDF="325300" id="1036"> MUR=2.0 MUF=0.5  </weight> this shouldn't be needed
         scale_envelope[8] = lheEventProduct->weights()[0].wgt;    // index 8L muF = 1.0, muR = 1.0 : <weight MUF="1.0" MUR="1.0" PDF="325300" id="1001">  </weight> NOMINAL: this might be needed for normalization
         scale_envelope[9] = lheEventProduct->originalXWGTUP();    // nominal (central?) PDF weight



         const int VAR_UP = 1;
         const int VAR_DOWN = -1;
         int nQCD = 0;

         // init stuff

         PDFWeights_factWeightsRMS_up = 0.;
         PDFWeights_factWeightsRMS_down = 0.;
         varWeightsRMS = 0;

         edm::Handle<GenEventInfoProduct> evt_info;
         iEvent.getByToken(GeneratorToken_, evt_info);

         double scalePDF = evt_info->pdf()->scalePDF; //scalePDF seems to return scale q -> this was previously called q2
         id1 = evt_info->pdf()->id.first; //particle 1 id 
         id2 = evt_info->pdf()->id.second; //particle 2 id
         x1 = evt_info->pdf()->x.first;
         x2 = evt_info->pdf()->x.second;
         q2 = pow(scalePDF,2);
         if(_verbose)std::cout << "id1/id2/x1/x2/scalePDF are" << id1<< "/" <<id2 << "/" << x1<< "/" << x2<< "/" << scalePDF<< std::endl;

         alphas           = calcAlphas(scalePDF);
         PDFWeights_renormWeight_up = calcRenormWeight(scalePDF, VAR_UP, nQCD); 
         PDFWeights_renormWeight_down = calcRenormWeight(scalePDF, VAR_DOWN, nQCD); 

         // what are my PDF weights related to what he has here? 

         for (int varN = 0; varN < nVars; varN++) 
         {
            factorizWeights_up[varN]   = calcFactorizWeight(varPDFs[varN], id1, id2, x1, x2, scalePDF, VAR_UP);   
            factorizWeights_down[varN] = calcFactorizWeight(varPDFs[varN], id1, id2, x1, x2, scalePDF, VAR_DOWN); 

            if(_verbose)std::cout << "varN is " << varN << ", the up/down factorizWeights is " << factorizWeights_up[varN]<< "/" <<factorizWeights_down[varN] << std::endl;

            PDFWeights_factWeightsRMS_up += (factorizWeights_up[varN] * factorizWeights_up[varN]);
            PDFWeights_factWeightsRMS_down += (factorizWeights_down[varN] * factorizWeights_down[varN]);
            // weight using https://lhapdf.hepforge.org/group__reweight__double.html, one per replica.
            weightsForVar[varN] = LHAPDF::weightxxQ(id1, id2, x1, x2, scalePDF, nomPDF, varPDFs[varN]); 
            if(_verbose)std::cout << "weightsForVar[varN] is " << weightsForVar[varN] << std::endl;
            varWeightsRMS += (weightsForVar[varN] * weightsForVar[varN]);
            if(_verbose)std::cout << "varWeightsRMS increased by " << varWeightsRMS << std::endl;
         }

         //Calculate the RMS's
         PDFWeights_factWeightsRMS_up /= (1.0*nVars);
         PDFWeights_factWeightsRMS_down /= (1.0*nVars); 

         PDFWeights_factWeightsRMS_up = sqrt(PDFWeights_factWeightsRMS_up);

         PDFWeights_factWeightsRMS_down = sqrt(PDFWeights_factWeightsRMS_down);

         if(_verbose)std::cout << "PDFWeights_factWeightsRMS_up/PDFWeights_factWeightsRMS_down" << PDFWeights_factWeightsRMS_up<< "/" <<PDFWeights_factWeightsRMS_down << std::endl;

         varWeightsRMS /= nVars;
         varWeightsRMS = sqrt(varWeightsRMS);


         //Calculated the error on the varWeightsRMS according to eqn 6.4 from https://arxiv.org/pdf/2203.05506.pdf
         //Need the values in sorted order
         int arrSize = sizeof(weightsForVar) / sizeof(weightsForVar[0]);
         sort(weightsForVar, weightsForVar + arrSize);
         double weight16 = weightsForVar[15]; // what's the deal with these indices?
         double weight84 = weightsForVar[83];
         varWeightsErr = (weight84 - weight16) / 2.0;
         if (varWeightsErr < 0) varWeightsErr = 0;
 
         if(_verbose)std::cout << " The PDFWeights_alphas is " << alphas << "." << std::endl;
         if(_verbose)std::cout << " The renormWeights up/down is " <<renormWeights[0] << "/" << renormWeights[1] << "." << std::endl;
         if(_verbose)std::cout << " The nVars is " <<nVars << "." << std::endl;
         if(_verbose)std::cout << " The varWeightsRMS is " <<varWeightsRMS << "." << std::endl;
         if(_verbose)std::cout << " The varWeightsErr is " <<varWeightsErr << "." << std::endl;
      }
      
   }*/






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////_process_&_jet multiplicity_/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
if( (runType.find("MC") != std::string::npos) || (runType.find("Tprime") != std::string::npos)) {
   //gen particles
   int Tprime_pdgid = 6000006; //Tprime
   //int nSuu = 0;
   int nChi = 0;
   int nW   = 0;
   int ntop = 0;
   int nH   = 0;
   int nhq  = 0;
   int nSuub = 0;
   int nSuu_W = 0;
   int nZ    = 0;
   int nZq   = 0;
   int nTopb = 0;
   int nWq   = 0;
   int nhGlu = 0;
   int nhWq = 0;
   int nhZq = 0;

   std::vector<TLorentzVector> genChiZt;
   std::vector<TLorentzVector> genChiHt;
   std::vector<TLorentzVector> genChiWb;


   std::vector<TLorentzVector> Zquarks;
   std::vector<TLorentzVector> Wquarks;
   std::vector<TLorentzVector> Hquarks;
   std::vector<TLorentzVector> TopWquarks;
   std::vector<TLorentzVector> Topb;
   std::vector<TLorentzVector> Suub;

iEvent.getByToken(genPartToken_, genPartCollection);

for (auto iG = genPartCollection->begin(); iG != genPartCollection->end(); iG++)
    {

if ((abs(iG->pdgId())==24)&&((abs(iG->mother()->pdgId())==Tprime_pdgid)) )
      {
         genChiWb.push_back( TLorentzVector(iG->mother()->px(),iG->mother()->py(),iG->mother()->pz(),iG->mother()->energy())  );
         const reco::Candidate* W_final = parse_chain(iG->clone());
         for (unsigned int iii=0; iii<W_final->numberOfDaughters(); iii++)
         {
            const reco::Candidate* W_daughter = W_final->daughter(iii);
          
            if(abs(W_daughter->pdgId()) < 6)
            {
               Wquarks.push_back(TLorentzVector(W_daughter->px(),W_daughter->py(),W_daughter->pz(),W_daughter->energy()));
               nWq++;
            }
         }

         nSuu_W++;
      }
      else if ( (abs(iG->pdgId()) == 5) && (abs(iG->mother()->pdgId()) == Tprime_pdgid)  )
      {
         Suub.push_back(TLorentzVector(iG->px(),iG->py(),iG->pz(),iG->energy()));
         nSuub++;
      }

      else if ( (abs(iG->pdgId()) == 6) && ((abs(iG->mother()->pdgId()) == Tprime_pdgid)) )
      {
         const reco::Candidate* t_final = parse_chain(iG->clone());

         for (unsigned int iii=0; iii<t_final->numberOfDaughters(); iii++)
         {
            const reco::Candidate* t_daughter = t_final->daughter(iii);
            if (abs(t_daughter->pdgId())==24)
            {
               const reco::Candidate* W_final = parse_chain(t_daughter->clone());
               for (unsigned int jjj=0; jjj<W_final->numberOfDaughters(); jjj++)
               {
                  const reco::Candidate* W_daughter = W_final->daughter(jjj);
                  if(abs(W_daughter->pdgId()) < 6)
                  {
                     TopWquarks.push_back(TLorentzVector(W_daughter->px(),W_daughter->py(),W_daughter->pz(),W_daughter->energy()));
                     nWq++;
                  }
               }
               nW++;
            }
            else if(abs(t_daughter->pdgId())==5)
            {
               Topb.push_back(TLorentzVector(t_daughter->px(),t_daughter->py(),t_daughter->pz(),t_daughter->energy()));
               nTopb++;
            }
         }
         ntop++;
      }
      else if ( (abs(iG->pdgId()) == 25) && ((abs(iG->mother()->pdgId()) == Tprime_pdgid)) )
      {
         genChiHt.push_back( TLorentzVector(iG->mother()->px(),iG->mother()->py(),iG->mother()->pz(),iG->mother()->energy())  );
         const reco::Candidate* h_final = parse_chain(iG->clone());
         for (unsigned int iii=0; iii<h_final->numberOfDaughters(); iii++)
         {
            const reco::Candidate* h_daughter = h_final->daughter(iii);
            if (abs(h_daughter->pdgId())<6)
            {
               Hquarks.push_back(TLorentzVector(h_daughter->px(),h_daughter->py(),h_daughter->pz(),h_daughter->energy()));
               nhq++;
            }
            else if (abs(h_daughter->pdgId()) ==24)
            {
               const reco::Candidate* higgsW_final = parse_chain(h_daughter->clone());
               for (unsigned int jjj=0; jjj<higgsW_final->numberOfDaughters(); jjj++)
               {
                  const reco::Candidate* higgsW_daughter = higgsW_final->daughter(jjj);
                  if( (abs(higgsW_daughter->pdgId()) < 6))
                  {
                     Hquarks.push_back(TLorentzVector(h_daughter->px(),h_daughter->py(),h_daughter->pz(),h_daughter->energy()));
                     nhWq++;
                  }
               }
            }
            else if (abs(h_daughter->pdgId()) ==23)
            {
               const reco::Candidate* higgsZ_final = parse_chain(h_daughter->clone());
               for (unsigned int jjj=0; jjj<higgsZ_final->numberOfDaughters(); jjj++)
               {
                  const reco::Candidate* higgsZ_daughter = higgsZ_final->daughter(jjj);
                  if( (abs(higgsZ_daughter->pdgId()) < 6))
                  {
                     Hquarks.push_back(TLorentzVector(h_daughter->px(),h_daughter->py(),h_daughter->pz(),h_daughter->energy()));
                     nhZq++;
                  }
               }
            }
            else if (abs(h_daughter->pdgId()) ==21)
            {
                Hquarks.push_back(TLorentzVector(h_daughter->px(),h_daughter->py(),h_daughter->pz(),h_daughter->energy()));
                nhGlu++;
            }
         }
         nH++;
      }
      else if ( (abs(iG->pdgId()) == 23) && ((abs(iG->mother()->pdgId()) == Tprime_pdgid)) )
      {
         genChiZt.push_back( TLorentzVector(iG->mother()->px(),iG->mother()->py(),iG->mother()->pz(),iG->mother()->energy())  );

         const reco::Candidate* Z_final = parse_chain(iG->clone());
         for (unsigned int jjj=0; jjj<Z_final->numberOfDaughters(); jjj++)
         {
            const reco::Candidate* Z_daughter = Z_final->daughter(jjj);
            if(abs(Z_daughter->pdgId()) < 6)
            {

               Zquarks.push_back(TLorentzVector(Z_daughter->px(),Z_daughter->py(),Z_daughter->pz(),Z_daughter->energy()));
               nZq++;
            }
         }
         nZ++;

      }
      else if ((abs(iG->pdgId()) == Tprime_pdgid) && (iG->isLastCopy()))
      {
          nChi++;
      }

    //processes
   bool _htWb = false;
   bool _htZt = false;
   bool _ZtWb = false;
   bool _WbWb = false;
   bool _htht = false;
   bool _ZtZt = false;

   if      ((nH == 2) && (nSuub == 0) && (nTopb >= 2) && ( (nhWq+nhZq == 8) || (nhq == 4) || (nhGlu ==4) || (nhWq+nhZq == 4 && nhq ==2) || (nhWq+nhZq == 4 && nhGlu ==2) || (nhq == 2 && nhGlu == 2)) && (nWq == 4) && (nZq == 0) && (ntop == 2) && (nW == 2)  && (nZ == 0))
   {
      _htht = true;
      nHtHt++;
   }
   else if ((nH == 0) && (nSuub == 0) && (nTopb >= 2) && ( (nhq+nhGlu+nhWq+nhZq) == 0) && (nWq == 4) && (nZq == 4) && (ntop == 2) && (nW == 2)  && (nZ == 2))
   {
      _ZtZt = true;
      nZtZt++;
   }
   else if ((nH == 0) && (nSuub == 2) && (nTopb >= 0) && ( (nhq+nhGlu+nhWq+nhZq) == 0) && (nWq == 4) && (nZq == 0) && (ntop == 0) && (nSuu_W == 2)  && (nZ == 0))
   {
      _WbWb = true;
      nWbWb++;
  }
   else if ((nH == 1) && (nSuub == 1) && (nTopb >= 1) && ( (nhWq+nhZq == 4) || (nhq == 2) || (nhGlu ==2) ) && (nWq == 4) && (nZq == 0) && (ntop == 1) && (nW == 1) && (nSuu_W == 1) && (nZ == 0))
   {
      _htWb = true;
      nWbHt++;
   }
   else if ((nH == 0) && (nSuub == 1) && (nTopb >= 1) && ( (nhq+nhGlu+nhWq+nhZq) == 0) && (nWq == 4) && (nZq == 2) && (ntop == 1) && (nW == 1) && (nSuu_W == 1)  && (nZ == 1))
   {
      _ZtWb = true;
      nWbZt++;
   }
   else if ((nH == 1) && (nSuub == 0) && (nTopb >= 2) && ( (nhWq+nhZq == 4) || (nhq == 2) || (nhGlu ==2) ) && (nWq == 4) && (nZq == 2) && (ntop == 2) && (nW == 2)  && (nZ == 1))
   {
      _htZt = true;
      nHtZt++;
   }

  //10 jets = HtHt, ZtZt, HtZt
  //8 jets = WbHt, WbZt 
  //6 jets = WbWb
  
      if (_htht || _ZtZt || _htZt) {jetMultiplicity = 10;}
      else if (_ZtWb || _htWb) {jetMultiplicity = 8;}
      else if (_WbWb) {jetMultiplicity = 6;}

      if (_htht) {process = 0;}
      else if (_ZtZt) {process = 1;}
      else if (_htZt) {process = 2;}
      else if (_ZtWb) {process = 3;}
      else if (_htWb) {process = 4;}
      else if (_WbWb) {process = 5;}
}
}

//LUND clear vectors before looping over jets
   jet_lund_dR.clear();
   jet_lund_kT.clear();

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////_LUND_/////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//This is when LUND variables will be calculated
for (auto iJet = fatJets->begin(); iJet != fatJets->end(); iJet++) {
	//std::cout<< "new jet start" <<std::endl;
        //get PF constituents
        std::vector<fastjet::PseudoJet> fj_particles;

        for (unsigned int i = 0; i < iJet->numberOfDaughters(); ++i) {
		const auto* daughter = iJet->daughter(i);
		if (daughter) {
                fj_particles.emplace_back(daughter->px(), daughter->py(), daughter->pz(), daughter->energy());
		//std::cout << "daughter found, Event number:" << eventnum << std::endl;
		}
        }

	if (fj_particles.empty()) {
		edm::LogWarning("LundPlane") << "skipping jet: no PF constituents, Event number:" << eventnum;
		continue;
	}

        //cluster consitituents with C/A (LUND requites C/A)
        fastjet::JetDefinition jet_def(fastjet::cambridge_algorithm, 1.0); //R=1.0 but not sure if this is good radius
        fastjet::ClusterSequence cs_jet(fj_particles, jet_def);
	std::vector<fastjet::PseudoJet> inclusive_jets = fastjet::sorted_by_E(cs_jet.inclusive_jets(0.)); //return a vector of jets sorted into decreasing energy with an energy cut of 0
        //fastjet::PseudoJet fj_jet = fastjet::sorted_by_E(cs_jet.inclusive_jets(0.)).at(0);
	//std::cout<< "how many jets are in AK8 jet particle after CA clustering: " <<inclusive_jets.size() <<std::endl;

	if (inclusive_jets.empty()) {
                edm::LogWarning("LundPlane") << "no clustered jets found from pF constituents";
                continue;
        }

	fastjet::PseudoJet fj_jet =inclusive_jets.at(0);

	if (!fj_jet.has_constituents()) {
                edm::LogWarning("LundPlane") << "clustered jet has no constituents";
                continue;
	}

	//printing out the first CA jet (sorted into decrasing energy, therefore the largest E CA jet)
	std::vector<fastjet::PseudoJet> constituents = fj_jet.constituents();
	//std::cout << "in event " << eventnum << ", the largest energy CA jet has " << constituents.size() << "constituents" << std::endl;
	//for (size_t i = 0; i < constituents.size(); ++i) {
	//	const fastjet::PseudoJet& p = constituents[i];
	//	std::cout << "constituent " << i << ": px = " << p.px() << ", py = " << p.py() << "y pz = " << p.pz() << ", E = " << p.E() << ", pt = " << p.pt() << ", eta = " << p.eta() << ", phi = " << p.phi_std() << std::endl;
	//}

        //Run LUND Generator
        fastjet::contrib::LundGenerator lundGen;
	std::vector<fastjet::contrib::LundDeclustering> lund_decomp = lundGen(fj_jet);

        std::vector<float> thisJet_dR, thisJet_kT;

        for (const auto& emission : lund_decomp) {
                thisJet_dR.push_back(emission.Delta());
                thisJet_kT.push_back(emission.kt());
        }

	//cout << "in event " << eventnum << ", lund dR:" << thisJet_dR.back() << ", lund kT:" << thisJet_kT.back() <<endl;

        jet_lund_dR.push_back(thisJet_dR);
        jet_lund_kT.push_back(thisJet_kT);

}
   
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////////////////////////_Clustering_///////////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   //     This is the point where superjets will be formed !

   if(_verbose)std::cout << "before cluster" << std::endl;
   /////calculate COM frame boost////////////////////////////////////////////////////////////////////?????///////////////
   TVector3 totJetBeta = TVector3(tot_jet_px/tot_jet_E,tot_jet_py/tot_jet_E ,tot_jet_pz/tot_jet_E);


   //////////boost all jet particles into MPP frame//////////////////////////////////////////////////////////////////////
   std::vector<TLorentzVector> candsBoostedTLV_;
   double pxCOM = 0,pyCOM=0,pzCOM=0;
   for(auto iC = candsUnboosted.begin();iC != candsUnboosted.end(); iC++)
   {
      TLorentzVector iC_(iC->px(),iC->py(),iC->pz(),iC->energy());
      iC_.Boost(-totJetBeta.X(),-totJetBeta.Y(),-totJetBeta.Z());
      pxCOM+=iC_.Px();pyCOM+=iC_.Py();pzCOM+=iC_.Pz();
      candsBoostedTLV_.push_back( iC_ );

   }
   std::vector<fastjet::PseudoJet> candsBoostedPJ;
   for(auto iP=candsBoostedTLV_.begin();iP!=candsBoostedTLV_.end();iP++ )
   {
      candsBoostedPJ.push_back( fastjet::PseudoJet(iP->Px(),iP->Py(),iP->Pz(),iP->E()   ) );
   }

   ///////take all boosted particles in MPP frame and recluster, then sort these AK8 jets
   double R0 = 0.8;

    //fastjet::JetDefinition jet_def0(fastjet::antikt_algorithm, R0);  // alternate jet clustering algorithm
   fastjet::JetDefinition jet_def0(fastjet::cambridge_algorithm, R0);
   fastjet::ClusterSequence cs_jet0(candsBoostedPJ, jet_def0); 

   // shedding particles happens here -> can change this energy threshold, just changed it from 50. 
   std::vector<fastjet::PseudoJet> jetsFJ_jet0 = fastjet::sorted_by_E(cs_jet0.inclusive_jets(0.));//changed inclusive jet pt cut from 10 to 0
   double MPP_COM2_px = 0,MPP_COM2_py=0,MPP_COM2_pz =0,MPP_COM2_E = 0;
   int counter = 0;
   int nReclustered_CA8 = 0;
   std::vector<TLorentzVector> candsBoostedTLV;   
   for (auto iJet=jetsFJ_jet0.begin(); iJet<jetsFJ_jet0.end(); iJet++)                             
   {   
      if(iJet->E() > 250.) nReclustered_CA8++;
      if (iJet->constituents().size() > 0)
      {
         for(auto iDaughter = iJet->constituents().begin(); iDaughter != iJet->constituents().end(); iDaughter++)
         {
            counter++;
            candsBoosted.push_back(reco::LeafCandidate(+1, reco::Candidate::LorentzVector(iDaughter->px(), iDaughter->py(), iDaughter->pz(), iDaughter->E())));
            candsBoostedTLV.push_back(TLorentzVector( iDaughter->px(), iDaughter->py(), iDaughter->pz(), iDaughter->E()    ));
            if(counter > 300)return; // RETURN cut, event is bad because of fastjet/root bug
         }
      }
      MPP_COM2_px+= iJet->px(); MPP_COM2_py+= iJet->py(); MPP_COM2_pz+=iJet->pz() ; MPP_COM2_E+=iJet->E() ;
   }
   if(_verbose)std::cout << "In this event, there were " << nfatjets << " lab frame AK8 jets selected, and " << nReclustered_CA8 << " reclustered MPP frame CA jets w/ E> 250 GeV" << std::endl;
    TVector3 MPP_COM2 = TVector3(MPP_COM2_px/MPP_COM2_E,MPP_COM2_py/MPP_COM2_E ,MPP_COM2_pz/MPP_COM2_E);
    std::vector<reco::LeafCandidate> candsBoosted_MPPCOM2;   

    for(auto iPart = candsBoostedTLV.begin(); iPart != candsBoostedTLV.end(); iPart++)
    {
        iPart->Boost(-MPP_COM2.X(), -MPP_COM2.Y(), -MPP_COM2.Z());
        candsBoosted_MPPCOM2.push_back( reco::LeafCandidate(+1, reco::Candidate::LorentzVector( iPart->Px(),iPart->Py(),iPart->Pz(),iPart->E()   ) )       );
    }

    Thrust thrust_(candsBoosted.begin(), candsBoosted.end());                             //thrust axis in COM frame
    math::XYZVector thrustAxis = thrust_.axis();
    TVector3 thrust_vector(thrustAxis.X(),thrustAxis.Y(),thrustAxis.Z());
   


///////////////////////////get vector of sorted superjets//////////////////////////////////////////////////////   
    if(_verbose)std::cout << "sorted jets" << std::endl;

    std::vector<TLorentzVector> negSuperJet_preSort;
    std::vector<TLorentzVector> posSuperJet_preSort;
    std::vector<TLorentzVector> miscJets;

    for (auto iJet=jetsFJ_jet0.begin(); iJet<jetsFJ_jet0.end(); iJet++)                             
    {
        TLorentzVector candJet(iJet->px(),iJet->py(),iJet->pz(),iJet->E());
        TVector3 candJet_vec = candJet.Vect();
        double cosAngle = cos(candJet_vec.Angle(thrust_vector));
        if    (cosAngle < -0.85) negSuperJet_preSort.push_back(candJet);  
        else if(cosAngle > 0.85) posSuperJet_preSort.push_back(candJet);
        else{ miscJets.push_back(candJet); }
    }
   if(_verbose)std::cout << "enter sort jets" << std::endl;

   std::vector<TLorentzVector> negSuperJet;
   std::vector<TLorentzVector> posSuperJet;

   if(_verbose)std::cout << "In this event there are " << posSuperJet_preSort.size() << "/" << negSuperJet_preSort.size() << "/" << miscJets.size() << "pos/neg/misc jets" << std::endl;

   if(( miscJets.size() > 0) )
   {   
      sortJets testing(negSuperJet_preSort,posSuperJet_preSort,miscJets);
      negSuperJet = testing.finalSuperJet1;
      posSuperJet = testing.finalSuperJet2;
   }
   else
   {
      negSuperJet = negSuperJet_preSort;
      posSuperJet = posSuperJet_preSort;
   }
   if( ((negSuperJet.size() < 1 ) || (posSuperJet.size()<1) )  ) 
   {
      if(_verbose)std::cout << "Superjet with size < 1: superjet1 size is " << posSuperJet.size() << ", superjet 2 size is "<< negSuperJet.size()  <<"| EXITING ----------------" <<std::endl;
      return; // RETURN cut - event is bad because of one superjet has 0 jets
   }
   TLorentzVector SJ1(0,0,0,0);
   TLorentzVector SJ2(0,0,0,0);

   for(unsigned int iii = 0; iii < negSuperJet.size();iii++)
   {
      SJ2+=negSuperJet[iii];
   }

   for(unsigned int iii = 0; iii < posSuperJet.size();iii++)
   {
      SJ1+=posSuperJet[iii];
   }
///////////////////////////////////////////////////////////////////////////////////////////

   /////////// Associating lab AK8 jets with one of the COM reclustered AK8 jets which gives assignment of lab AK8 jets to the superjets
   int lab_AK8_num = 0;

   for(auto iLabAK8 = selectedAK8_TLV.begin(); iLabAK8!=selectedAK8_TLV.end(); iLabAK8++)
   { 
      //abs(candJet.Angle(iJet->Vect()))
      iLabAK8->Boost(-totJetBeta.X(),-totJetBeta.Y(),-totJetBeta.Z()); // boost jet to COM frame
      double minAngle = 99999;
      AK8_SJ_assignment[lab_AK8_num] = -999;
      for( auto iSJ_jet = negSuperJet.begin(); iSJ_jet!= negSuperJet.end();iSJ_jet++ )
      {
         if( abs(iSJ_jet->Angle(iLabAK8->Vect())) < minAngle)
         {
            AK8_SJ_assignment[lab_AK8_num] = 1; // means NEGATIVE superjet
            minAngle =  abs(iSJ_jet->Angle(iLabAK8->Vect()));
         } 
      }
      for( auto iSJ_jet = posSuperJet.begin(); iSJ_jet!= posSuperJet.end();iSJ_jet++ )
      {
         if( abs(iSJ_jet->Angle(iLabAK8->Vect())) < minAngle)
         {
            AK8_SJ_assignment[lab_AK8_num] = 0; // means POSITIVE superjet
            minAngle =  abs(iSJ_jet->Angle(iLabAK8->Vect()));
         } 
      } 
      lab_AK8_num++;
   } 

   for(int jjj = 0; jjj < nfatjets; jjj++) // loop over all AK8 jets
   {
      for(int iii = 0; iii < nAK4; iii++) // loop over all AK4 jets
      {
         if (lab_AK4_AK8_parent[iii] != -1 ) // if the AK4 jet actually has a matched parent
         {
            if( lab_AK4_AK8_parent[iii] == jjj ) // if the AK4 jet is a daughter of this AK8 jet
            {
               AK4_SJ_assignment[iii] = AK8_SJ_assignment[jjj]; // the AK4 jet gets the AK8 jet superjet assignment
            }
         }
      }
   }

            
   ///// selectedAK8_TLV are now boosted!!

    //get vector of pseudo jet particles for all particles in each superjet, one per superjet
    for (auto iJet=jetsFJ_jet0.begin(); iJet<jetsFJ_jet0.end(); iJet++)                             
    {   
        TLorentzVector candJet(iJet->px(),iJet->py(),iJet->pz(),iJet->E());

        int SJMatch = 0;
        if      (isMatchedtoSJ(posSuperJet,candJet)) SJMatch = 1;
        else if (isMatchedtoSJ(negSuperJet,candJet)) SJMatch = 2;
        else if(SJMatch == 0) std::cout << "Something didn't work... " << std::endl;


        //sort jet particles into either SuperJet 1 or SuperJet 2
        if (iJet->constituents().size() > 0)
        {
            for(auto iJ = iJet->constituents().begin(); iJ != iJet->constituents().end(); iJ++)
            {  
                if     (SJMatch==1)superJetOneLV.push_back(TLorentzVector(iJ->px(),iJ->py(),iJ->pz(),iJ->E()));
                else if(SJMatch==2)superJetTwoLV.push_back(TLorentzVector(iJ->px(),iJ->py(),iJ->pz(),iJ->E()));
            }
         }        
   }

   for(auto iPart = superJetOneLV.begin(); iPart != superJetOneLV.end(); iPart++)
   {
         superJetOne.push_back(fastjet::PseudoJet(iPart->Px(),iPart->Py(),iPart->Pz(),iPart->E()));
   }
   for(auto iPart = superJetTwoLV.begin(); iPart != superJetTwoLV.end(); iPart++)
   {
        superJetTwo.push_back(fastjet::PseudoJet(iPart->Px(),iPart->Py(),iPart->Pz(),iPart->E()));
   }



   ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
   ////////////////////////////////////////// superjet analysis /////////////////////////////////////////////////
   //////////////////////////////////////////////////////////////////////////////////////////////////////////////


   if(_verbose)std::cout << "Starting superjet loop." << std::endl;

   double superJetpx,superJetpy,superJetpz,superJetE;
   std::vector<std::vector<fastjet::PseudoJet>> superJets;
   superJets.push_back(superJetOne); //pos
   superJets.push_back(superJetTwo); //neg

   nSuperJets = 0;   
   tot_nAK4_70 = 0; 
   tot_nAK4_50 = 0;
   tot_mpp_AK4 = 0;
   double diSuperJet_E = 0, diSuperJet_px = 0,diSuperJet_py = 0,diSuperJet_pz =0;
   double diSuperJet_E_100 = 0, diSuperJet_px_100 = 0,diSuperJet_py_100 = 0,diSuperJet_pz_100 =0;
   if(_verbose)std::cout << "about to loop over superjets" << std::endl;

   for(auto iSJ = superJets.begin();iSJ!= superJets.end();iSJ++)
   {
     if(_verbose)std::cout << "starting superjet" << nSuperJets <<std::endl;

      ///////////////// /inside superjet loop //////////////////////
      ///////         fill BESTmap for superjet ////////////////////
      //////////////////////////////////////////////////////////////

	/*
      //////get BEST scores for superjet /////////
      BESTmap.clear();
      BESTmap["tot_pt"] = allAK8.Pt();
      BESTmap["tot_HT"] = totHT;

      BESTmap["eventNumber"] = eventNumber;   */


      /*
      std::cout << "-------------------------------- new SJ ----------------------------------------- " << std::endl;
      std::cout << "eventNumber" << " " << eventNumber << std::endl;
      std::cout << "superjet_num" << " " << nSuperJets + 1<< std::endl;
      std::cout << "tot_HT" << " " << totHT << std::endl;
      //std::cout << "tot_pt" << " " << allAK8.Pt() << std::endl;
      
      std::vector<float> BESTScores;
      if (!fillSJVars(BESTmap, *iSJ,nSuperJets))    //if this fails somehow, event is skipped
      {
         return;   // RETURN cut - tree was filled incorrectly
      }
      BESTScores = BEST_->getPrediction(BESTmap);
      ///store BEST scores in tree
      int decision = std::distance(BESTScores.begin(), std::max_element(BESTScores.begin(), BESTScores.end() ) );

      if (nSuperJets == 0)
      {
         SJ1_BEST_scores[0] = (double) BESTScores[0];
         SJ1_BEST_scores[1] = (double) BESTScores[1];
         SJ1_BEST_scores[2] = (double) BESTScores[2];
         //SJ1_BEST_scores[3] = (double) BESTScores[3];
         //SJ1_BEST_scores[4] = (double) BESTScores[4];
         SJ1_decision = decision;
      }
      else if (nSuperJets == 1)
      {
         SJ2_BEST_scores[0] = (double) BESTScores[0];
         SJ2_BEST_scores[1] = (double) BESTScores[1];
         SJ2_BEST_scores[2] = (double) BESTScores[2];
         //SJ2_BEST_scores[3] = (double) BESTScores[3];
         //SJ2_BEST_scores[4] = (double) BESTScores[4];
         SJ2_decision = decision;
      } */

      superJetpx=0;superJetpy=0;superJetpz=0;superJetE=0;
      for(auto iP = iSJ->begin();iP != iSJ->end();iP++)
      {
         superJetpx+=iP->px();superJetpy+=iP->py();superJetpz+=iP->pz();superJetE +=iP->E();
         diSuperJet_E +=iP->E();diSuperJet_px+=iP->px();diSuperJet_py+=iP->py();diSuperJet_pz+=iP->pz();
      }
      superJet_mass[nSuperJets] = sqrt(pow(superJetE,2)-pow(superJetpx,2)-pow(superJetpy,2)-pow(superJetpz,2));
      
      //saving pos and neg SJ mass
      if (nSuperJets == 0)
	      {posSJ_mass = sqrt(pow(superJetE,2)-pow(superJetpx,2)-pow(superJetpy,2)-pow(superJetpz,2));}
      else if (nSuperJets == 1)
	      {negSJ_mass = sqrt(pow(superJetE,2)-pow(superJetpx,2)-pow(superJetpy,2)-pow(superJetpz,2));}

      TLorentzVector superJetTLV(superJetpx,superJetpy,superJetpz,superJetE);    //Lorentz vector representing jet axis -> now minimize the parallel momentum

      std::vector<fastjet::PseudoJet> boostedSuperJetPart;

      //boost particles in SuperJet to its COM frame
      for(auto iP = iSJ->begin();iP != iSJ->end();iP++)
      {
         TLorentzVector iP_(iP->px(),iP->py(),iP->pz(),iP->E());
         iP_.Boost(-superJetpx/superJetE,-superJetpy/superJetE,-superJetpz/superJetE);
         boostedSuperJetPart.push_back(fastjet::PseudoJet(iP_.Px(),iP_.Py(),iP_.Pz(),iP_.E()));
      }

      ///reclustering SuperJet that is now boosted into the MPP frame
      double R = 0.4;
      //fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, R);
      fastjet::JetDefinition jet_def(fastjet::cambridge_algorithm, R);
      fastjet::ClusterSequence cs_jet(boostedSuperJetPart, jet_def); 
      std::vector<fastjet::PseudoJet> jetsFJ_jet = fastjet::sorted_by_E(cs_jet.inclusive_jets(0.0));

      double SJ_50_px = 0, SJ_50_py=0,SJ_50_pz=0,SJ_50_E=0;
      double SJ_70_px = 0, SJ_70_py=0,SJ_70_pz=0,SJ_70_E=0;
      double SJ_100_px = 0, SJ_100_py=0,SJ_100_pz=0,SJ_100_E=0;
      double SJ_125_px = 0, SJ_125_py=0,SJ_125_pz=0,SJ_125_E=0;
      double SJ_150_px = 0, SJ_150_py=0,SJ_150_pz=0,SJ_150_E=0;
      double SJ_200_px = 0, SJ_200_py=0,SJ_200_pz=0,SJ_200_E=0;
      double SJ_300_px = 0, SJ_300_py=0,SJ_300_pz=0,SJ_300_E=0;
      double SJ_400_px = 0, SJ_400_py=0,SJ_400_pz=0,SJ_400_E=0;
      double SJ_600_px = 0, SJ_600_py=0,SJ_600_pz=0,SJ_600_E=0;
      double SJ_800_px = 0, SJ_800_py=0,SJ_800_pz=0,SJ_800_E=0;
      double SJ_1000_px = 0, SJ_1000_py=0,SJ_1000_pz=0,SJ_1000_E=0;

      /* not using these vars now, but could in the future
         double SJ1_AK41_px, SJ1_AK41_py, SJ1_AK41_pz, SJ1_AK41_E;
         double SJ1_AK42_px, SJ1_AK42_py, SJ1_AK42_pz, SJ1_AK42_E;
         double SJ1_AK43_px, SJ1_AK43_py, SJ1_AK43_pz, SJ1_AK43_E;
         double SJ1_AK44_px, SJ1_AK44_py, SJ1_AK44_pz, SJ1_AK44_E;

         double SJ2_AK41_px, SJ2_AK41_py, SJ2_AK41_pz, SJ2_AK41_E;
         double SJ2_AK42_px, SJ2_AK42_py, SJ2_AK42_pz, SJ2_AK42_E;
         double SJ2_AK43_px, SJ2_AK43_py, SJ2_AK43_pz, SJ2_AK43_E;
         double SJ2_AK44_px, SJ2_AK44_py, SJ2_AK44_pz, SJ2_AK44_E;
      */

      SJ_nAK4_50[nSuperJets] = 0,  SJ_nAK4_70[nSuperJets] =0,SJ_nAK4_100[nSuperJets] = 0,SJ_nAK4_125[nSuperJets] = 0,SJ_nAK4_150[nSuperJets] = 0,SJ_nAK4_200[nSuperJets] = 0,SJ_nAK4_300[nSuperJets] = 0,SJ_nAK4_400[nSuperJets] = 0,SJ_nAK4_1000[nSuperJets] = 0;
      SJ_nAK4_600[nSuperJets] = 0, SJ_nAK4_800[nSuperJets] = 0;
      for (auto iPJ=jetsFJ_jet.begin(); iPJ<jetsFJ_jet.end(); iPJ++)                             
      {

         if(nSuperJets == 0)  //SJ1
         {
            SJ1_AK4_px[tot_mpp_AK4] = iPJ->px();
            SJ1_AK4_py[tot_mpp_AK4] = iPJ->py();
            SJ1_AK4_pz[tot_mpp_AK4] = iPJ->pz();
            SJ1_AK4_E[tot_mpp_AK4]  = iPJ->E();           
         }
         else if(nSuperJets == 1) //SJ2
         {
            SJ2_AK4_px[tot_mpp_AK4] = iPJ->px();
            SJ2_AK4_py[tot_mpp_AK4] = iPJ->py(); 
            SJ1_AK4_pz[tot_mpp_AK4] = iPJ->pz();
            SJ2_AK4_E[tot_mpp_AK4]  = iPJ->E();          
         }

         AK4_E[tot_mpp_AK4]= iPJ->E();
         if(iPJ->E()>50.)
         {
            SJ_AK4_50_mass[tot_nAK4_50] = iPJ->m();
            SJ_50_px+=iPJ->px();SJ_50_py+=iPJ->py();SJ_50_pz+=iPJ->pz();SJ_50_E+=iPJ->E();
            SJ_nAK4_50[nSuperJets]++;
            tot_nAK4_50++;
         }
         if(iPJ->E()> 70.)
         {
            SJ_AK4_70_mass[tot_nAK4_70] = iPJ->m();
            SJ_70_px+=iPJ->px();SJ_70_py+=iPJ->py();SJ_70_pz+=iPJ->pz();SJ_70_E+=iPJ->E();
            SJ_nAK4_70[nSuperJets]++;
            tot_nAK4_70++;
         }
         if(iPJ->E()>100)
         {
            SJ_100_px+=iPJ->px();SJ_100_py+=iPJ->py();SJ_100_pz+=iPJ->pz();SJ_100_E+=iPJ->E();
            diSuperJet_E_100 +=iPJ->E();diSuperJet_px_100+=iPJ->px();diSuperJet_py_100+=iPJ->py();diSuperJet_pz_100+=iPJ->pz();

            SJ_nAK4_100[nSuperJets]++; 
         }
         if(iPJ->E()>125)
         {
            SJ_125_px+=iPJ->px();SJ_125_py+=iPJ->py();SJ_125_pz+=iPJ->pz();SJ_125_E+=iPJ->E();

            SJ_nAK4_125[nSuperJets]++; 
         }
         if(iPJ->E()>150)
         {
            SJ_150_px+=iPJ->px();SJ_150_py+=iPJ->py();SJ_150_pz+=iPJ->pz();SJ_150_E+=iPJ->E();

            SJ_nAK4_150[nSuperJets]++; 
         }
         if(iPJ->E()>200)
         {
            SJ_200_px+=iPJ->px();SJ_200_py+=iPJ->py();SJ_200_pz+=iPJ->pz();SJ_200_E+=iPJ->E();

            SJ_nAK4_200[nSuperJets]++; 
         }
         if(iPJ->E()>300)
         {
            SJ_300_px+=iPJ->px();SJ_300_py+=iPJ->py();SJ_300_pz+=iPJ->pz();SJ_300_E+=iPJ->E();

            SJ_nAK4_300[nSuperJets]++; 

            // loop over selectedAK8_TLV
            // see if SJ designation matches current SJ
            // boost that jet to the SJ COM frame
            // check to see if this reclustered CA4 jet is within R = 0.8
            int selected_AK8_num = 0;
            for(auto iAK8_selected = selectedAK8_TLV.begin(); iAK8_selected!=selectedAK8_TLV.end();iAK8_selected++)
            {
               if(AK8_SJ_assignment[selected_AK8_num] == nSuperJets)
               {
                  TLorentzVector boosted_AK8(iAK8_selected->Px(),iAK8_selected->Py(),iAK8_selected->Pz(),iAK8_selected->E());
                  boosted_AK8.Boost(-superJetpx/superJetE,-superJetpy/superJetE,-superJetpz/superJetE); // boost the AK8 jet to its respective SJ COM frame
                  if( sqrt(pow(boosted_AK8.Phi() - iPJ->phi(),2)+ pow(boosted_AK8.Eta() - iPJ->eta(),2)) < 0.8  ) AK8_is_near_highE_CA4[selected_AK8_num] = true;  // the AK8 jet has been tracked into its sorted SJ and whether it is close to a high energy reclustered CA4 jet is tracked
               }
               selected_AK8_num++;
            }

            // loop over the selected AK4 jets and see if there is overlap between these and high energy substructure based on flavour
            int selected_AK4_num = 0;

            for(auto iAK4_selected = selectedAK4_TLV.begin(); iAK4_selected!=selectedAK4_TLV.end();iAK4_selected++)
            {
               if (lab_AK4_AK8_parent[selected_AK4_num] != -1) // -1 were AK4 jets that didn't overlap with AK8 jets 
               {

                  // if this AK4 jet overlaps with this AK8 jet (in the lab frame) AND that AK8 jet is part of this superjet
                  if(AK8_SJ_assignment[ lab_AK4_AK8_parent[selected_AK4_num] ] == nSuperJets)
                  {
                     TLorentzVector boosted_AK4(iAK4_selected->Px(),iAK4_selected->Py(),iAK4_selected->Pz(),iAK4_selected->E());
                     boosted_AK4.Boost(-superJetpx/superJetE,-superJetpy/superJetE,-superJetpz/superJetE); // boost the AK8 jet to its respective SJ COM frame
                     if( sqrt(pow(boosted_AK4.Phi() - iPJ->phi(),2)+ pow(boosted_AK4.Eta() - iPJ->eta(),2)) < 0.8  ) AK4_is_near_highE_CA4[selected_AK4_num] = true;  // the AK8 jet has been tracked into its sorted SJ and whether it is close to a high energy reclustered CA4 jet is tracked, generous: delta R of 0.8
                  }
               }
               selected_AK4_num++;
            }    
         }
         if(iPJ->E()>400)
         {
            SJ_400_px+=iPJ->px();SJ_400_py+=iPJ->py();SJ_400_pz+=iPJ->pz();SJ_400_E+=iPJ->E();

            SJ_nAK4_400[nSuperJets]++; 
         }
         if(iPJ->E()>600)
         {
            SJ_600_px+=iPJ->px();SJ_600_py+=iPJ->py();SJ_600_pz+=iPJ->pz();SJ_600_E+=iPJ->E();

            SJ_nAK4_600[nSuperJets]++; 
         }
         if(iPJ->E()>800)
         {
            SJ_800_px+=iPJ->px();SJ_800_py+=iPJ->py();SJ_800_pz+=iPJ->pz();SJ_800_E+=iPJ->E();

            SJ_nAK4_800[nSuperJets]++; 
         }
         if(iPJ->E()>1000)
         {
            SJ_1000_px+=iPJ->px();SJ_1000_py+=iPJ->py();SJ_1000_pz+=iPJ->pz();SJ_1000_E+=iPJ->E();

            SJ_nAK4_1000[nSuperJets]++; 
         }
         tot_mpp_AK4++;
      }

      SJ_mass_50[nSuperJets]= sqrt(pow(SJ_50_E,2)-pow(SJ_50_px,2)-pow(SJ_50_py,2)-pow(SJ_50_pz,2));
      SJ_mass_70[nSuperJets]= sqrt(pow(SJ_70_E,2)-pow(SJ_70_px,2)-pow(SJ_70_py,2)-pow(SJ_70_pz,2));
      SJ_mass_100[nSuperJets]= sqrt(pow(SJ_100_E,2)-pow(SJ_100_px,2)-pow(SJ_100_py,2)-pow(SJ_100_pz,2));
      SJ_mass_125[nSuperJets]= sqrt(pow(SJ_125_E,2)-pow(SJ_125_px,2)-pow(SJ_125_py,2)-pow(SJ_125_pz,2));
      SJ_mass_150[nSuperJets]= sqrt(pow(SJ_150_E,2)-pow(SJ_150_px,2)-pow(SJ_150_py,2)-pow(SJ_150_pz,2));
      SJ_mass_200[nSuperJets]= sqrt(pow(SJ_200_E,2)-pow(SJ_200_px,2)-pow(SJ_200_py,2)-pow(SJ_200_pz,2));
      SJ_mass_300[nSuperJets]= sqrt(pow(SJ_300_E,2)-pow(SJ_300_px,2)-pow(SJ_300_py,2)-pow(SJ_300_pz,2));
      SJ_mass_400[nSuperJets]= sqrt(pow(SJ_400_E,2)-pow(SJ_400_px,2)-pow(SJ_400_py,2)-pow(SJ_400_pz,2));
      SJ_mass_600[nSuperJets]= sqrt(pow(SJ_600_E,2)-pow(SJ_600_px,2)-pow(SJ_600_py,2)-pow(SJ_600_pz,2));
      SJ_mass_800[nSuperJets]= sqrt(pow(SJ_800_E,2)-pow(SJ_800_px,2)-pow(SJ_800_py,2)-pow(SJ_800_pz,2));
      SJ_mass_1000[nSuperJets]= sqrt(pow(SJ_1000_E,2)-pow(SJ_1000_px,2)-pow(SJ_1000_py,2)-pow(SJ_1000_pz,2));

      boostedSuperJetPart.clear();   //shouldn't be needed, just in case
      if(_verbose)std::cout << "ending superjet" << nSuperJets <<std::endl;
      nSuperJets++; 
   }

//posSJ mass calculation
//superJetpx=0;superJetpy=0;superJetpz=0;superJetE=0;
//for(auto iP = iSJ->begin();iP != iSJ->end();iP++)
//   {
//      superJetpx+=iP->px();superJetpy+=iP->py();superJetpz+=iP->pz();superJetE +=iP->E();
//      diSuperJet_E +=iP->E();diSuperJet_px+=iP->px();diSuperJet_py+=iP->py();diSuperJet_pz+=iP->pz();
//   }
//posSuperJet_mass = sqrt(pow(superJetE,2)-pow(superJetpx,2)-pow(superJetpy,2)-pow(superJetpz,2));







   if(_verbose)std::cout << "calculating disuperjet masses" <<std::endl;
   diSuperJet_mass = sqrt(pow(diSuperJet_E,2)-pow(diSuperJet_px,2)-pow(diSuperJet_py,2)-pow(diSuperJet_pz,2));
   diSuperJet_mass_100 = sqrt(pow(diSuperJet_E_100,2)-pow(diSuperJet_px_100,2)-pow(diSuperJet_py_100,2)-pow(diSuperJet_pz_100,2));
   if(_verbose)std::cout << "clear superjets"  <<std::endl;

   superJetOne.clear();
   superJetTwo.clear();
   if(_verbose)std::cout << "Filling the tree" <<std::endl;
   tree->Fill();

   if(_verbose)std::cout << " -----------------event end -----------------" << std::endl;
}   

DEFINE_FWK_MODULE(clusteringAnalyzerAll_process);
//_bottom_
