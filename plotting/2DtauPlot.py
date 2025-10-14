import ROOT

# Open file and get tree
f1 = ROOT.TFile.Open("signalCfgs/clusteringAnalyzer_NsubJet_TprimeTprime1800_2017_output.root")
dir1 = f1.Get("selcetionStudy_Et150")
tree1 = dir1.Get("tree_nom_Et150.000000")  #signal
f2 = ROOT.TFile.Open("clusteringAnalyzer_NsubJet_QCDMC1500to2000_2017_output.root")
dir2 = f2.Get("selcetionStudy_Et150")
tree2 = dir2.Get("tree_nom_Et150.000000")  #QCD
f3 = ROOT.TFile.Open("clusteringAnalyzer_NsubJet_TTJetsMCHT1200to2500_2017_output2.root")
dir3 = f3.Get("selcetionStudy_Et150")
tree3 = dir3.Get("tree_nom_Et150.000000")  #TTBar
f4 = ROOT.TFile.Open("clusteringAnalyzer_NsubJet_WJetsMC_QQ-HT800toInf_2017_output.root")
dir4 = f4.Get("selcetionStudy_Et150")
tree4 = dir4.Get("tree_nom_Et150.000000")  #W Hadronic
f5 = ROOT.TFile.Open("clusteringAnalyzer_NsubJet_WJetsMC_LNu-HT2500toInf_2017_output.root")
dir5 = f5.Get("selcetionStudy_Et150")
tree5 = dir5.Get("tree_nom_Et150.000000")  #W leptonic

# Create canvas
c = ROOT.TCanvas("c", "Signal Plots", 800, 700)

# Define output PDF
pdf_name = "2D_plots.pdf"

# Start multipage PDF
c.Print(pdf_name + "[")

# ------------------------------------------
# Plot 1: 2D correlation tau21[1] vs tau21[0]
# ------------------------------------------
hist1 = ROOT.TH2F("hist1", ";Leading jet #tau_{21};Subleading jet #tau_{21}", 50, 0, 1, 50, 0, 1)
tree1.Draw("tau21[1]:tau21[0] >> hist1", "@tau21.size()>1", "COLZ")
hist1.SetTitle("T'T' 1800 2D Correlation of #tau_{21}(jet1) vs #tau_{21}(jet2)")
ROOT.gStyle.SetOptStat(0)
c.Print(pdf_name)

hist2 = ROOT.TH2F("hist2", ";Leading jet #tau_{21};Subleading jet #tau_{21}", 50, 0, 1, 50, 0, 1)
tree2.Draw("tau21[1]:tau21[0] >> hist2", "@tau21.size()>1", "COLZ")
hist2.SetTitle("QCD1500to2000 2D Correlation of #tau_{21}(jet1) vs #tau_{21}(jet2)")
ROOT.gStyle.SetOptStat(0)
c.Print(pdf_name)

hist3 = ROOT.TH2F("hist3", ";Leading jet #tau_{21};Subleading jet #tau_{21}", 50, 0, 1, 50, 0, 1)
tree3.Draw("tau21[1]:tau21[0] >> hist3", "@tau21.size()>1", "COLZ")
hist3.SetTitle("TTBar1200to2500 2D Correlation of #tau_{21}(jet1) vs #tau_{21}(jet2)")
ROOT.gStyle.SetOptStat(0)
c.Print(pdf_name)

hist4 = ROOT.TH2F("hist4", ";Leading jet #tau_{21};Subleading jet #tau_{21}", 50, 0, 1, 50, 0, 1)
tree4.Draw("tau21[1]:tau21[0] >> hist4", "@tau21.size()>1", "COLZ")
hist4.SetTitle("W Hadronic800toInf 2D Correlation of #tau_{21}(jet1) vs #tau_{21}(jet2)")
ROOT.gStyle.SetOptStat(0)
c.Print(pdf_name)

hist5 = ROOT.TH2F("hist5", ";Leading jet #tau_{21};Subleading jet #tau_{21}", 50, 0, 1, 50, 0, 1)
tree5.Draw("tau21[1]:tau21[0] >> hist5", "@tau21.size()>1", "COLZ")
hist5.SetTitle("W Leptonic2500toInf 2D Correlation of #tau_{21}(jet1) vs #tau_{21}(jet2)")
ROOT.gStyle.SetOptStat(0)
c.Print(pdf_name)

# ------------------------------------------
# Plot 2: 2D correlation tau32[1] vs tau32[0]
# ------------------------------------------
hist_1 = ROOT.TH2F("hist_1", ";Leading jet #tau_{32};Subleading jet #tau_{32}", 50, 0, 1, 50, 0, 1)
tree1.Draw("tau32[1]:tau32[0] >> hist_1", "@tau32.size()>1", "COLZ")
hist_1.SetTitle("T'T' 1800 2D Correlation of #tau_{32}(jet1) vs #tau_{32}(jet2)")
ROOT.gStyle.SetOptStat(0)
c.Print(pdf_name)

hist_2 = ROOT.TH2F("hist_2", ";Leading jet #tau_{32};Subleading jet #tau_{32}", 50, 0, 1, 50, 0, 1)
tree2.Draw("tau32[1]:tau32[0] >> hist_2", "@tau32.size()>1", "COLZ")
hist_2.SetTitle("QCD1500to2000 2D Correlation of #tau_{32}(jet1) vs #tau_{32}(jet2)")
ROOT.gStyle.SetOptStat(0)
c.Print(pdf_name)

hist_3 = ROOT.TH2F("hist_3", ";Leading jet #tau_{32};Subleading jet #tau_{32}", 50, 0, 1, 50, 0, 1)
tree3.Draw("tau32[1]:tau32[0] >> hist_3", "@tau32.size()>1", "COLZ")
hist_3.SetTitle("TTBar1200to2500 2D Correlation of #tau_{32}(jet1) vs #tau_{32}(jet2)")
ROOT.gStyle.SetOptStat(0)
c.Print(pdf_name)

hist_4 = ROOT.TH2F("hist_4", ";Leading jet #tau_{32};Subleading jet #tau_{32}", 50, 0, 1, 50, 0, 1)
tree4.Draw("tau32[1]:tau32[0] >> hist_4", "@tau32.size()>1", "COLZ")
hist_4.SetTitle("W Hadronic800toInf 2D Correlation of #tau_{32}(jet1) vs #tau_{32}(jet2)")
ROOT.gStyle.SetOptStat(0)
c.Print(pdf_name)

hist_5 = ROOT.TH2F("hist_5", ";Leading jet #tau_{32};Subleading jet #tau_{32}", 50, 0, 1, 50, 0, 1)
tree5.Draw("tau32[1]:tau32[0] >> hist_5", "@tau32.size()>1", "COLZ")
hist_5.SetTitle("W Leptonic2500toInf 2D Correlation of #tau_{32}(jet1) vs #tau_{32}(jet2)")
ROOT.gStyle.SetOptStat(0)
c.Print(pdf_name)


# ------------------------------------------
# Close the multipage PDF
# ------------------------------------------
c.Print(pdf_name + "]")

f1.Close()
f2.Close()
f3.Close()
f4.Close()
f5.Close()
print(f"Saved all plots to {pdf_name}")
