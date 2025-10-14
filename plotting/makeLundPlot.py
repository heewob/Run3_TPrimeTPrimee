import ROOT
import numpy as np

# Open your ROOT file
f = ROOT.TFile.Open("clusteringAnalyzer_jetDistribution_QCDMC1000to1500_2017_output.root")
dir = f.Get("selcetionStudy_Et150")
tree = dir.Get("tree_nom_Et150.000000")

# Create the 2D histogram
lund_hist = ROOT.TH2F("lund_plane", "Lund Jet Plane;log(1/#DeltaR);log(k_{T})", 60, 0, 7, 60, -2, 4)
jet_distribution = ROOT.TH1F("jet distribution", "jet distribution; entry; L_ratio", 20, 0, 1)

# Loop over entries
nEntries = tree.GetEntries()
for i in range(nEntries):
    tree.GetEntry(i)

    # Access branches
    dRvecs = getattr(tree, "boosted_jet_lund_dR")
    kTvecs = getattr(tree, "boosted_jet_lund_kT")
    L_ratio = getattr(tree, "L_ratio")

    # Loop over jets in the event
    for jet_dRs, jet_kTs in zip(dRvecs, kTvecs):
        for dR, kT in zip(jet_dRs, jet_kTs):
            if dR > 1e-4 and kT > 1e-4:  # Avoid log(0)
                x = np.log(1.0 / dR)
                y = np.log(kT)
                lund_hist.Fill(x, y)
    
    jet_distribution.Fill(L_ratio)

# Draw the plot
c = ROOT.TCanvas("c", "", 800, 700)
lund_hist.Draw("COLZ")
c.SaveAs("boosted_lund_jet_plane_Run2.png")

c.Clear()
jet_distribution.Draw()
c.SaveAs("jet_distrubution.png")
