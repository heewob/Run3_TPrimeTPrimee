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

//Nsubjettiness
#include "fastjet/contrib/Nsubjettiness.hh"

//#include "AnalysisDataFormats/TopObjects/interface/TtGenEvent.h"
//#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"



class clusteringAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
   explicit clusteringAnalyzer(const edm::ParameterSet&);
private:

   virtual void analyze(const edm::Event&, const edm::EventSetup&);
   void endJob() override;
   double calcMPP(TLorentzVector superJetTLV ); 
   const static bool isMatchedtoSJ(std::vector<TLorentzVector> superJetTLVs, TLorentzVector candJet); 
   bool fillSJVars(std::map<std::string, float> &treeVars, std::vector<fastjet::PseudoJet> iSJ, int nSuperJets);
   const static bool isgoodjet(const float eta, const float NHF,const float NEMF, const size_t NumConst,const float CHF,const int CHM, const float MUF, const float CEMF, bool jetPUid, const float iJet_pt);
   const static bool isgoodjet(const float eta, const float NHF,const float NEMF, const size_t NumConst,const float CHF,const int CHM, const float MUF, const float CEMF, int nfatjets);
   const bool isHEM(const float jet_eta, const float jet_phi);
   const static double top_pt_SF(double top_pt);
   const static double calcAlphas(double q2);
   double calcRenormWeight(double q2, int up_or_dn, int nQCD);
   const static double calcFactorizWeight(LHAPDF::PDF* pdf, double id1, double id2, double x1, double x2, double q2, int up_or_dn);
   const std::string returnJECFile(std::string year, std::string systematicType, std::string jet_type, std::string runType);
   double getJECUncertaintyFromSources(std::string jet_type, double pt, double eta);
   const reco::Candidate* parse_chain(const reco::Candidate* cand);
   const bool applyJERSource(std::string uncertainty_source, double eta);
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
   edm::EDGetTokenT<std::vector<reco::Vertex>> pvToken_;
   edm::EDGetTokenT<std::vector<reco::VertexCompositePtrCandidate>> svToken_;


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
   int eventnum = 0;  //total number of event processed
   int didntpasstotHT = 0; //number of events that didn't pass totHT cut
   int didntpassnfatjets = 0; //number of events that didn't pass nfatjet cut
   int didntpassnHeavyAK8_300_30 = 0; //number of events that didn't pass nHeavyAK8_300_30 cut
   int didntpassnSubSJ100 = 0; //number of events that didn't pass SJ_nAK4_100 cut

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

   //Tprime energy
   std::vector<double> Tprime_energy;
   std::vector<double> Tprime_momentum;
   std::vector<double> Tprime_pt;

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
   double posSJ_mass_after_boost,negSJ_mass_after_boost;
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
   LHAPDF::PDF* nomPDF;
   LHAPDF::PDF* varPDFs[102];
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
   std::unique_ptr<correction::CorrectionSet> cset;
   correction::Correction::Ref cset_corrector_bc;
   correction::Correction::Ref cset_corrector_light; 

   // Jet correction uncertainty classes
   JetCorrectionUncertainty *jecUnc_AK4;
   JetCorrectionUncertainty *jecUnc_AK8;
   std::unique_ptr<correction::CorrectionSet> PUjson;
   correction::Correction::Ref PUjson_year;

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

   std::vector<std::vector<float>> boosted_jet_lund_dR;
   std::vector<std::vector<float>> boosted_jet_lund_kT;

   std::vector<std::vector<float>> boostedSJ1_lund_dR, boostedSJ2_lund_dR;
   std::vector<std::vector<float>> boostedSJ1_lund_kT, boostedSJ2_lund_kT;

   //jet distribution
   double L_parallel;
   double L_perpendicular;
   double L_ratio;

   //Nsubjettiness
   std::vector<double> tau1_, tau2_, tau3_, tau21_, tau32_;
   std::vector<double> tau1COM_, tau2COM_, tau3COM_, tau21COM_, tau32COM_;
   std::vector<double> tau1_SJ1_, tau2_SJ1_, tau3_SJ1_, tau21_SJ1_, tau32_SJ1_, tau1_SJ2_, tau2_SJ2_, tau3_SJ2_, tau21_SJ2_, tau32_SJ2_;

   //Energy Fraction, Multiplicity..
   std::vector<double> totalE_SJ1_, totalMultiplicity_SJ1_, chargedHadronEnergy_SJ1_, neutralHadronEnergy_SJ1_, chargedEmEnergy_SJ1_, neutralEmEnergy_SJ1_, photonEnergy_SJ1_, electronEnergy_SJ1_, muonEnergy_SJ1_, chargedMuEnergy_SJ1_, chargedMultiplicity_SJ1_, neutralMultiplicity_SJ1_, HadronMultiplicity_SJ1_, chargedHadronMultiplicity_SJ1_, neutralHadronMultiplicity_SJ1_, EmMultiplicity_SJ1_, chargedEmMultiplicity_SJ1_, neutralEmMultiplicity_SJ1_, photonMultiplicity_SJ1_, electronMultiplicity_SJ1_, muonMultiplicity_SJ1_, EnergyFractionHadronic_SJ1_, EnergyFractionEm_SJ1_, EnergyFractionNeutralHadronic_SJ1_, EnergyFractionChargedHadronic_SJ1_, EnergyFractionNeutralEm_SJ1_, EnergyFractionChargedEm_SJ1_, EnergyFractionMuon_SJ1_;

   std::vector<double> totalE_SJ2_, totalMultiplicity_SJ2_, chargedHadronEnergy_SJ2_, neutralHadronEnergy_SJ2_, chargedEmEnergy_SJ2_, neutralEmEnergy_SJ2_, photonEnergy_SJ2_, electronEnergy_SJ2_, muonEnergy_SJ2_, chargedMuEnergy_SJ2_, chargedMultiplicity_SJ2_, neutralMultiplicity_SJ2_, HadronMultiplicity_SJ2_, chargedHadronMultiplicity_SJ2_, neutralHadronMultiplicity_SJ2_, EmMultiplicity_SJ2_, chargedEmMultiplicity_SJ2_, neutralEmMultiplicity_SJ2_, photonMultiplicity_SJ2_, electronMultiplicity_SJ2_, muonMultiplicity_SJ2_, EnergyFractionHadronic_SJ2_, EnergyFractionEm_SJ2_, EnergyFractionNeutralHadronic_SJ2_, EnergyFractionChargedHadronic_SJ2_, EnergyFractionNeutralEm_SJ2_, EnergyFractionChargedEm_SJ2_, EnergyFractionMuon_SJ2_;


   //subSJ CA8 jets mass constituents
   std::vector<double> CA8_mass_SJ1_, CA8_mass_SJ2_, CA8_constituents_SJ1_, CA8_constituents_SJ2_;

   //subSJ 4 vector and eta phi
   std::vector<double> CA8_px_SJ1_, CA8_px_SJ2_, CA8_py_SJ1_, CA8_py_SJ2_, CA8_pz_SJ1_, CA8_pz_SJ2_, CA8_pt_SJ1_, CA8_pt_SJ2_, CA8_E_SJ1_, CA8_E_SJ2_, CA8_eta_SJ1_, CA8_eta_SJ2_, CA8_phi_SJ1_, CA8_phi_SJ2_;


   //subSJ lab frame info
   std::vector<double> labeta_SJ1_, labphi_SJ1_, labeta_SJ2_, labphi_SJ2_;

   //subSJ secondary vertex info
   std::vector<double> SV_chi2_, SV_dlen_, SV_dlenSig_, SV_dxy_, SV_dxySig_, SV_eta_, SV_phi_, SV_pt_, SV_mass_, SV_pAngle_, SV_x_, SV_y_, SV_z_;
   std::vector<int> SV_ndof_, SV_ntracks_;
};


//// return back the JEC file for a given systematic, year, and jet type
const std::string clusteringAnalyzer::returnJECFile(std::string year, std::string systematicType, std::string jet_type, std::string runType)
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
   if (data_type.find("data") != std::string::npos ) return  ("Run3_TPrimeTprime/data/JEC_uncertainty_sources/" + file_map[year][data_type] + "/" + file_map[year][data_type] + "_UncertaintySources_" +jet_str  + ".txt" ).c_str();
   else { return  ("Run3_TPrimeTprime/data/JEC_uncertainty_sources/" + file_map[year][data_type] + "/" + file_map[year][data_type] + "_UncertaintySources_" +jet_str  + ".txt" ).c_str(); }

}


//recursively returns status-change copies of generated particles until you get to new decays
const reco::Candidate* clusteringAnalyzer::parse_chain(const reco::Candidate* cand)
{  
   for (unsigned int iii=0; iii<cand->numberOfDaughters(); iii++)
   {
      if(cand->daughter(iii)->pdgId() == cand->pdgId()) return parse_chain(cand->daughter(iii));
   }
   return cand;
}


// return the JER scale factor for a given uncertainty source and jet eta
const bool clusteringAnalyzer::applyJERSource(std::string uncertainty_source, double eta)
{
   if((uncertainty_source == "JER_up") || (uncertainty_source == "JER_down"))return true; // all jets are corrected for the "total" JER uncertainty
   else if(  (uncertainty_source.find("JER_eta193") != std::string::npos) && (abs(eta) < 1.93  ))return true;
   else if(  (uncertainty_source.find("JER_193eta25") != std::string::npos) && ( abs(eta) >=1.93  ) && (abs(eta) < 2.5  ))return true;
   else {return false;}
}


// bool corresponding to if AK4 jet passes tight ID
const bool clusteringAnalyzer::isgoodjet(const float eta, const float NHF,const float NEMF, const size_t NumConst,const float CHF,const int CHM, const float MUF, const float CEMF,bool jetPUid, const float iJet_pt)
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
const bool clusteringAnalyzer::isgoodjet(const float eta, const float NHF,const float NEMF, const size_t NumConst,const float CHF,const int CHM, const float MUF, const float CEMF, int nfatjets)
{
  //if ( (nfatjets < 2) && (abs(eta) > 2.4) ) return false;
  //else if ( (nfatjets >= 2) && (abs(eta) > 1.4) ) return false;

  //if ((NHF>0.9) || (NEMF>0.9) || (NumConst<1) || (CHF<0.) || (CHM<0) || (MUF > 0.8) || (CEMF > 0.8)) 
  //{
  //  return false;
  //}

  if ( (abs(eta) > 2.4) || (NHF>0.9) || (NEMF>0.9) || (NumConst<1) || (CHF<0.) || (CHM<0) || (MUF > 0.8) || (CEMF > 0.8)) 
  {
    return false;  //new isgoodjet to match john's passJetID
  }
  else{ return true;}

}

// returns bool if jet (or more generally, object) is within the HEM region
const bool clusteringAnalyzer::isHEM(const float jet_eta, const float jet_phi)
{
  if(year != "2018") return false; // HEM is only relevant for 2018

  if( (jet_phi >  -1.57)&&( jet_phi < -0.87) )
  {
    if( (jet_eta > -3.0)&&(jet_eta < -1.3))return true;

  }
  return false;
}

// returns the top pt scale factor as detailed here - https://twiki.cern.ch/twiki/bin/view/CMS/TopPtReweighting#Run_1_strategy_Obsolete
const double clusteringAnalyzer::top_pt_SF(double top_pt)
{

  if (top_pt > 500.) top_pt = 500.;
  //$SF(p_T)=e^{0.0615-0.0005\cdot p_T}$ for data/POWHEG+Pythia8
  //return 0.103*exp(-0.0118*top_pt) -0.000134*top_pt+ 0.973;
  return exp(0.0615-0.0005*top_pt);  // this is the scale factor based on data aka data-NLO and data-NNLO weights
}

// tells you if a TLorentzVector is associated with a candidate jet (via delta R matching)
const bool clusteringAnalyzer::isMatchedtoSJ(std::vector<TLorentzVector> superJetTLVs, TLorentzVector candJet)
{
  for(auto iJet = superJetTLVs.begin(); iJet!=superJetTLVs.end(); iJet++)
  {
    if( abs(candJet.Angle(iJet->Vect())) < 0.001) return true;  
  }
  return false;
}


// for QCD scale uncertainty
const double clusteringAnalyzer::calcAlphas(double q2) 
{ 
  double mZ = 91.2; //Z boson mass in the NNPDF31_nnlo_as_0118 docs (http://lhapdfsets.web.cern.ch/lhapdfsets/current/NNPDF31_nnlo_as_0118/NNPDF31_nnlo_as_0118.info )
  double alphas_mZ = 0.118; //alpha_s evaluated at Z boson mass, based on the NNPDF31_nnlo_as_0118 docs (http://lhapdfsets.web.cern.ch/lhapdfsets/current/NNPDF31_nnlo_as_0118/NNPDF31_nnlo_as_0118.info )
  int nFlavors = 5; //effective number of flavors
  double b0 = (33 - 2.0 * nFlavors) / (12 * M_PI); 
  return alphas_mZ / (1 + alphas_mZ * b0 * std::log(q2 / std::pow(mZ,2))); // alphas evolution
}


// for QCD factorization uncertainty
const double clusteringAnalyzer::calcFactorizWeight(LHAPDF::PDF* pdf, double id1, double id2, double x1, double x2, double q2, int up_or_dn) 
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

