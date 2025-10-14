import ROOT

# Input files and tree paths
files = [
    ("signalCfgs/clusteringAnalyzer_NsubJet_TprimeTprime1800_2017_output.root", "selcetionStudy_Et150", "T'T'1800"),
    ("clusteringAnalyzer_NsubJet_QCDMC1500to2000_2017_output.root", "selcetionStudy_Et150", "QCD1500to2000"),
    ("clusteringAnalyzer_NsubJet_TTJetsMCHT1200to2500_2017_output2.root", "selcetionStudy_Et150", "TTBar1200to2500"),
    ("clusteringAnalyzer_NsubJet_WJetsMC_QQ-HT800toInf_2017_output.root", "selcetionStudy_Et150", "W Hadronic800toInf"),
    ("clusteringAnalyzer_NsubJet_WJetsMC_LNu-HT2500toInf_2017_output.root", "selcetionStudy_Et150", "W Leptonic2500toInf"),
]

# Create canvas and output PDF
c = ROOT.TCanvas("c", "Tau21 Correlation", 800, 700)
pdf_name = "2Dtau_extreme.pdf"
c.Print(pdf_name + "[")

# Style
ROOT.gStyle.SetOptStat(0)

#---------------------largest tau21-----------------------------------------------
# Loop over files and make 2D histograms
for fname, dname, label in files:
    f = ROOT.TFile.Open(fname)
    dir_ = f.Get(dname)
    if not dir_:
        print(f" Warning: directory {dname} not found in {fname}")
        continue
    tree = dir_.Get("tree_nom_Et150.000000")
    if not tree:
        print(f" Warning: tree not found in {fname}")
        continue

    hist2D = ROOT.TH2F(f"hist2D_{label}", f"{label};Largest #tau_{{21}};2nd Largest #tau_{{21}}", 50, 0, 1, 50, 0, 1)
    tau21 = ROOT.std.vector('double')()
    tree.SetBranchAddress("tau21", tau21)

    for i in range(tree.GetEntries()):
        tree.GetEntry(i)
        if len(tau21) < 2:
            continue
        sorted_tau = sorted(list(tau21), reverse=True)
        lead, sublead = sorted_tau[0], sorted_tau[1]
        hist2D.Fill(lead, sublead)

    c.Clear()
    hist2D.SetTitle(f"2D Correlation of Largest vs 2nd Largest #tau_{{21}} ({label})")
    hist2D.Draw("COLZ")

    # Add diagonal line for visual reference
    line = ROOT.TLine(0, 0, 1, 1)
    line.SetLineColor(ROOT.kBlack)
    line.SetLineStyle(2)
    line.Draw("same")

    # Add label text
    latex = ROOT.TLatex()
    latex.SetNDC(True)
    latex.SetTextSize(0.04)
    latex.DrawLatex(0.15, 0.93, f"{label}")

    # Print page
    c.Print(pdf_name)
    f.Close()

#---------------------smallest tau21-----------------------------------------------
# Loop over files and make 2D histograms
for fname, dname, label in files:
    f = ROOT.TFile.Open(fname)
    dir_ = f.Get(dname)
    if not dir_:
        print(f" Warning: directory {dname} not found in {fname}")
        continue
    tree = dir_.Get("tree_nom_Et150.000000")
    if not tree:
        print(f" Warning: tree not found in {fname}")
        continue

    hist2D = ROOT.TH2F(f"hist2D_{label}", f"{label};Smallest #tau_{{21}};2nd Smallest #tau_{{21}}", 50, 0, 1, 50, 0, 1)
    tau21 = ROOT.std.vector('double')()
    tree.SetBranchAddress("tau21", tau21)

    for i in range(tree.GetEntries()):
        tree.GetEntry(i)
        if len(tau21) < 2:
            continue
        sorted_tau = sorted(list(tau21), reverse=True)
        lead, sublead = sorted_tau[-1], sorted_tau[-2]
        hist2D.Fill(lead, sublead)

    c.Clear()
    hist2D.SetTitle(f"2D Correlation of Smallest vs 2nd Smallest #tau_{{21}} ({label})")
    hist2D.Draw("COLZ")

    # Add diagonal line for visual reference
    line = ROOT.TLine(0, 0, 1, 1)
    line.SetLineColor(ROOT.kBlack)
    line.SetLineStyle(2)
    line.Draw("same")

    # Add label text
    latex = ROOT.TLatex()
    latex.SetNDC(True)
    latex.SetTextSize(0.04)
    latex.DrawLatex(0.15, 0.93, f"{label}")

    # Print page
    c.Print(pdf_name)
    f.Close()

#---------------------largest tau32-----------------------------------------------
# Loop over files and make 2D histograms
for fname, dname, label in files:
    f = ROOT.TFile.Open(fname)
    dir_ = f.Get(dname)
    if not dir_:
        print(f" Warning: directory {dname} not found in {fname}")
        continue
    tree = dir_.Get("tree_nom_Et150.000000")
    if not tree:
        print(f" Warning: tree not found in {fname}")
        continue

    hist2D = ROOT.TH2F(f"hist2D_{label}", f"{label};Largest #tau_{{32}};2nd Largest #tau_{{32}}", 50, 0, 1, 50, 0, 1)
    tau32 = ROOT.std.vector('double')()
    tree.SetBranchAddress("tau32", tau32)

    for i in range(tree.GetEntries()):
        tree.GetEntry(i)
        if len(tau32) < 2:
            continue
        sorted_tau = sorted(list(tau32), reverse=True)
        lead, sublead = sorted_tau[0], sorted_tau[1]
        hist2D.Fill(lead, sublead)

    c.Clear()
    hist2D.SetTitle(f"2D Correlation of Largest vs 2nd Largest #tau_{{32}} ({label})")
    hist2D.Draw("COLZ")

    # Add diagonal line for visual reference
    line = ROOT.TLine(0, 0, 1, 1)
    line.SetLineColor(ROOT.kBlack)
    line.SetLineStyle(2)
    line.Draw("same")

    # Add label text
    latex = ROOT.TLatex()
    latex.SetNDC(True)
    latex.SetTextSize(0.04)
    latex.DrawLatex(0.15, 0.93, f"{label}")

    # Print page
    c.Print(pdf_name)
    f.Close()

#---------------------smallest tau32-----------------------------------------------
# Loop over files and make 2D histograms
for fname, dname, label in files:
    f = ROOT.TFile.Open(fname)
    dir_ = f.Get(dname)
    if not dir_:
        print(f" Warning: directory {dname} not found in {fname}")
        continue
    tree = dir_.Get("tree_nom_Et150.000000")
    if not tree:
        print(f" Warning: tree not found in {fname}")
        continue

    hist2D = ROOT.TH2F(f"hist2D_{label}", f"{label};Smallest #tau_{{32}};2nd Smallest #tau_{{32}}", 50, 0, 1, 50, 0, 1)
    tau32 = ROOT.std.vector('double')()
    tree.SetBranchAddress("tau32", tau32)

    for i in range(tree.GetEntries()):
        tree.GetEntry(i)
        if len(tau32) < 2:
            continue
        sorted_tau = sorted(list(tau32), reverse=True)
        lead, sublead = sorted_tau[-1], sorted_tau[-2]
        hist2D.Fill(lead, sublead)

    c.Clear()
    hist2D.SetTitle(f"2D Correlation of Smallest vs 2nd Smallest #tau_{{32}} ({label})")
    hist2D.Draw("COLZ")

    # Add diagonal line for visual reference
    line = ROOT.TLine(0, 0, 1, 1)
    line.SetLineColor(ROOT.kBlack)
    line.SetLineStyle(2)
    line.Draw("same")

    # Add label text
    latex = ROOT.TLatex()
    latex.SetNDC(True)
    latex.SetTextSize(0.04)
    latex.DrawLatex(0.15, 0.93, f"{label}")

    # Print page
    c.Print(pdf_name)
    f.Close()


# Close the multipage PDF
c.Print(pdf_name + "]")

print(f" Saved multipage PDF: {pdf_name}")
