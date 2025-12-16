#ifndef ENERGY_FRACTIONS_H
#define ENERGY_FRACTIONS_H

#include <vector>
#include <cmath>
#include "fastjet/PseudoJet.hh"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

struct EnergyFractions {
    double totalE = 0.0;
    double totalMultiplicity = 0.0;

    double chargedHadronEnergy = 0.0;
    double neutralHadronEnergy = 0.0;
    double chargedEmEnergy = 0.0;
    double neutralEmEnergy = 0.0;

    double photonEnergy = 0.0;
    double electronEnergy = 0.0;
    double muonEnergy = 0.0;
    //double HFHadronEnergy = 0.0;
    //double HFEMEnergy = 0.0;
    double chargedMuEnergy = 0.0;
    
    double chargedMultiplicity = 0.0;
    double neutralMultiplicity = 0.0;

    double HadronMultiplicity = 0.0;
    double chargedHadronMultiplicity = 0.0;
    double neutralHadronMultiplicity = 0.0;

    double EmMultiplicity = 0.0;
    double chargedEmMultiplicity = 0.0;
    double neutralEmMultiplicity = 0.0;

    double photonMultiplicity = 0.0;
    double electronMultiplicity = 0.0;
    double muonMultiplicity = 0.0;

    double EnergyFractionHadronic = 0.0;
    double EnergyFractionEm = 0.0;
    double EnergyFractionNeutralHadronic = 0.0;
    double EnergyFractionChargedHadronic = 0.0;
    double EnergyFractionNeutralEm = 0.0;
    double EnergyFractionChargedEm = 0.0;
    double EnergyFractionMuon = 0.0;
};

inline void updateEnergyFractions(EnergyFractions& out, int pdgid, double energy) {
    double e = energy;
    int apid = std::abs(pdgid);

    out.totalE += e;

    switch (apid) {
        case 211:   //charged hadron
            out.chargedHadronEnergy += e;
            out.chargedHadronMultiplicity += 1.0;
	    out.chargedMultiplicity += 1.0;
	    out.totalMultiplicity += 1.0;
            break;

        case 130:   //neutral hadron
            out.neutralHadronEnergy += e;
	    out.neutralHadronMultiplicity += 1.0;
            out.neutralMultiplicity += 1.0;
	    out.totalMultiplicity += 1.0;
            break;

        case 22:   //photon
            out.photonEnergy += e;
	    out.photonMultiplicity += 1.0;
            out.neutralEmEnergy += e;
            out.neutralMultiplicity += 1.0;
	    out.totalMultiplicity += 1.0;
            break;

        case 11:   //electron
            out.electronEnergy += e;
	    out.electronMultiplicity += 1.0;
            out.chargedEmEnergy += e;
            out.chargedMultiplicity += 1.0;
	    out.totalMultiplicity += 1.0;
            break;

        case 13:   //muon
            out.muonEnergy += e;
	    out.muonMultiplicity += 1.0;
            out.chargedMuEnergy += e;
            out.chargedMultiplicity += 1.0;
	    out.totalMultiplicity += 1.0;
            break;

        //case 1:   //Hadron in HF. I apply cuts of abs(eta)<2.4, which is far away from HF(starts at about abs(eta)>3.0)
        //    out.HFHadronEnergy += e;
        //    out.neutralHadronEnergy += e;
        //    out.neutralMultiplicity += 1.0;
        //    break;

        //case 2:   //EM in HF. I apply cuts of abs(eta)<2.4, which is far away from HF(starts at about abs(eta)>3.0)
        //    out.HFEMEnergy += e;
        //    out.neutralEmEnergy += e;
        //    out.neutralMultiplicity += 1.0;
        //    break;

        default:
            break;
    }
}

inline void finalizeEnergyFractions(EnergyFractions& out) {
    double eHad = out.chargedHadronEnergy +
                  out.neutralHadronEnergy /*+
                  out.HFHadronEnergy*/;

    double eEm = out.chargedEmEnergy +
                 out.neutralEmEnergy /*+
                 out.HFEMEnergy*/;

    if (out.totalE > 0) {
        out.EnergyFractionHadronic = eHad / out.totalE;
        out.EnergyFractionEm = eEm / out.totalE;
	out.EnergyFractionChargedHadronic = out.chargedHadronEnergy / out.totalE;
        out.EnergyFractionChargedEm = out.chargedEmEnergy / out.totalE;
	out.EnergyFractionNeutralHadronic = out.neutralHadronEnergy / out.totalE;
        out.EnergyFractionNeutralEm = out.neutralEmEnergy / out.totalE;
	out.EnergyFractionMuon = out.muonEnergy / out.totalE;
	out.HadronMultiplicity = out.chargedHadronMultiplicity + out.neutralHadronMultiplicity;
	out.chargedEmMultiplicity = out.electronMultiplicity + out.muonMultiplicity;
	out.neutralEmMultiplicity = out.photonMultiplicity;
	out.EmMultiplicity = out.chargedEmMultiplicity + out.neutralEmMultiplicity;

    }
}

inline EnergyFractions computeEnergyFractions(
    const std::vector<fastjet::PseudoJet>& constituents,
    const std::vector<reco::LeafCandidate>& candsUnboosted,
    const std::vector<float>& puppi_list,
    bool usePuppi = true
) {
    EnergyFractions ef;

    for (const auto& p : constituents) {
        int idx = p.user_index();
        int pdgid = candsUnboosted.at(idx).pdgId();
        double energy = usePuppi ? (p.E() / puppi_list[idx]) : p.E();

        updateEnergyFractions(ef, pdgid, energy);
    }

    finalizeEnergyFractions(ef);
    return ef;
}

#endif

