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
c = ROOT.TCanvas("c", "Tau21 Plots", 800, 700)

# Define output PDF
pdf_name = "sig_vs_BG_plots.pdf"

# Start multipage PDF
c.Print(pdf_name + "[")

# --------------------------------------------
# tau1 with all number of gen quark overlay
# --------------------------------------------
hist1 = ROOT.TH1F("hist1", ";#tau_{1};Normalized Events", 50, 0, 1)
tree1.Draw("tau1 >> hist1", "@tau1.size()>1")
hist1.SetLineColor(ROOT.kBlack)
hist1.Scale(1.0 / hist1.Integral())
hist1.SetTitle("Normalized #tau_{1} Distribution (signal vs Background)")
#hist1.Draw("HIST")  #signal

hist2 = ROOT.TH1F("hist2", ";#tau_{1};Normalized Events", 50, 0, 1)
tree2.Draw("tau1 >> hist2", "@tau1.size()>1")
hist2.SetLineColor(ROOT.kRed)
hist2.Scale(1.0 / hist2.Integral())
#hist2.Draw("HIST")  #QCD

hist3 = ROOT.TH1F("hist3", ";#tau_{1};Normalized Events", 50, 0, 1)
tree3.Draw("tau1 >> hist3", "@tau1.size()>1")
hist3.SetLineColor(ROOT.kBlue)
hist3.Scale(1.0 / hist3.Integral())
#hist3.Draw("HIST")  #TTBar

hist4 = ROOT.TH1F("hist4", ";#tau_{1};Normalized Events", 50, 0, 1)
tree4.Draw("tau1 >> hist4", "@tau1.size()>1")
hist4.SetLineColor(ROOT.kGreen)
hist4.Scale(1.0 / hist4.Integral())
#hist4.Draw("HIST")  #W Hadronic

hist5 = ROOT.TH1F("hist5", ";#tau_{1};Normalized Events", 50, 0, 1)
tree5.Draw("tau1 >> hist5", "@tau1.size()>1")
hist5.SetLineColor(ROOT.kOrange)
hist5.Scale(1.0 / hist5.Integral())
#hist5.Draw("HIST")  #W Hadronic

max_y = max(hist1.GetMaximum(), hist2.GetMaximum(), hist3.GetMaximum(), hist4.GetMaximum(), hist5.GetMaximum())
hist1.GetYaxis().SetRangeUser(0, max_y)

hist1.SetLineWidth(2)
hist2.SetLineWidth(2)
hist3.SetLineWidth(2)
hist4.SetLineWidth(2)
hist5.SetLineWidth(2)
hist1.Draw("HIST")
hist2.Draw("HIST SAME")
hist3.Draw("HIST SAME")
hist4.Draw("HIST SAME")
hist5.Draw("HIST SAME")


legend = ROOT.TLegend(0.65, 0.75, 0.88, 0.88)
legend.AddEntry(hist1, "T'T'1800", "l")
legend.AddEntry(hist2, "QCD1500to2000", "l")
legend.AddEntry(hist3, "TTBar1200to2500", "l")
legend.AddEntry(hist4, "W Hadronic800toInf", "l")
legend.AddEntry(hist5, "W Leptonic2500toInf", "l")

legend.Draw()
c.SetGrid()
c.Print(pdf_name)

# --------------------------------------------
# tau2 with all number of gen quark overlay
# --------------------------------------------
hist6 = ROOT.TH1F("hist6", ";#tau_{2};Normalized Events", 50, 0, 1)
tree1.Draw("tau2 >> hist6", "@tau2.size()>1")
hist6.SetLineColor(ROOT.kBlack)
hist6.Scale(1.0 / hist6.Integral())
hist6.SetTitle("Normalized #tau_{2} Distribution (Signal vs Background)")
#hist6.Draw("HIST")  #signal

hist7 = ROOT.TH1F("hist7", ";#tau_{2};Normalized Events", 50, 0, 1)
tree2.Draw("tau2 >> hist7", "@tau2.size()>1")
hist7.SetLineColor(ROOT.kRed)
hist7.Scale(1.0 / hist7.Integral())
#hist7.Draw("HIST")  #QCD

hist8 = ROOT.TH1F("hist8", ";#tau_{2};Normalized Events", 50, 0, 1)
tree3.Draw("tau2 >> hist8", "@tau2.size()>1")
hist8.SetLineColor(ROOT.kBlue)
hist8.Scale(1.0 / hist8.Integral())
#hist8.Draw("HIST")  #TTBar

hist9 = ROOT.TH1F("hist9", ";#tau_{2};Normalized Events", 50, 0, 1)
tree4.Draw("tau2 >> hist9", "@tau2.size()>1")
hist9.SetLineColor(ROOT.kGreen)
hist9.Scale(1.0 / hist9.Integral())
#hist9.Draw("HIST")  #W Hadronic

hist10 = ROOT.TH1F("hist10", ";#tau_{2};Normalized Events", 50, 0, 1)
tree5.Draw("tau2 >> hist10", "@tau2.size()>1")
hist10.SetLineColor(ROOT.kOrange)
hist10.Scale(1.0 / hist10.Integral())
#hist10.Draw("HIST")  #W Hadronic

max_y = max(hist6.GetMaximum(), hist7.GetMaximum(), hist8.GetMaximum(), hist9.GetMaximum(), hist10.GetMaximum())
hist6.SetMaximum(1.2*max_y)

hist6.SetLineWidth(2)
hist7.SetLineWidth(2)
hist8.SetLineWidth(2)
hist9.SetLineWidth(2)
hist10.SetLineWidth(2)
hist6.Draw("HIST")
hist7.Draw("HIST SAME")
hist8.Draw("HIST SAME")
hist9.Draw("HIST SAME")
hist10.Draw("HIST SAME")

#c.Update()

legend = ROOT.TLegend(0.65, 0.75, 0.88, 0.88)
legend.AddEntry(hist6, "T'T'1800", "l")
legend.AddEntry(hist7, "QCD1500to2000", "l")
legend.AddEntry(hist8, "TTBar1200to2500", "l")
legend.AddEntry(hist9, "W Hadronic800toInf", "l")
legend.AddEntry(hist10, "W Leptonic2500toInf", "l")

legend.Draw()
c.SetGrid()
#c.Update()
c.Print(pdf_name)

# --------------------------------------------
# tau3 with all number of gen quark overlay
# --------------------------------------------
hist11 = ROOT.TH1F("hist11", ";#tau_{3};Normalized Events", 50, 0, 1)
tree1.Draw("tau3 >> hist11", "@tau3.size()>1")
hist11.SetLineColor(ROOT.kBlack)
hist11.Scale(1.0 / hist11.Integral())
hist11.SetTitle("Normalized #tau_{3} Distribution (Signal vs Background)")
#hist11.Draw("HIST")  #signal

hist12 = ROOT.TH1F("hist12", ";#tau_{3};Normalized Events", 50, 0, 1)
tree2.Draw("tau3 >> hist12", "@tau3.size()>1")
hist12.SetLineColor(ROOT.kRed)
hist12.Scale(1.0 / hist12.Integral())
#hist12.Draw("HIST")  #QCD

hist13 = ROOT.TH1F("hist13", ";#tau_{3};Normalized Events", 50, 0, 1)
tree3.Draw("tau3 >> hist13", "@tau3.size()>1")
hist13.SetLineColor(ROOT.kBlue)
hist13.Scale(1.0 / hist13.Integral())
#hist13.Draw("HIST")  #TTBar

hist14 = ROOT.TH1F("hist14", ";#tau_{3};Normalized Events", 50, 0, 1)
tree4.Draw("tau3 >> hist14", "@tau3.size()>1")
hist14.SetLineColor(ROOT.kGreen)
hist14.Scale(1.0 / hist14.Integral())
#hist14.Draw("HIST")  #W Hadronic

hist15 = ROOT.TH1F("hist15", ";#tau_{3};Normalized Events", 50, 0, 1)
tree5.Draw("tau3 >> hist15", "@tau3.size()>1")
hist15.SetLineColor(ROOT.kOrange)
hist15.Scale(1.0 / hist15.Integral())
#hist15.Draw("HIST")  #W Hadronic

max_y = max(hist11.GetMaximum(), hist12.GetMaximum(), hist13.GetMaximum(), hist14.GetMaximum(), hist15.GetMaximum())
hist11.SetMaximum(1.2*max_y)

hist11.SetLineWidth(2)
hist12.SetLineWidth(2)
hist13.SetLineWidth(2)
hist14.SetLineWidth(2)
hist15.SetLineWidth(2)
hist11.Draw("HIST")
hist12.Draw("HIST SAME")
hist13.Draw("HIST SAME")
hist14.Draw("HIST SAME")
hist15.Draw("HIST SAME")

#c.Update()

legend = ROOT.TLegend(0.65, 0.75, 0.88, 0.88)
legend.AddEntry(hist11, "T'T'1800", "l")
legend.AddEntry(hist12, "QCD1500to2000", "l")
legend.AddEntry(hist13, "TTBar1200to2500", "l")
legend.AddEntry(hist14, "W Hadronic800toInf", "l")
legend.AddEntry(hist15, "W Leptonic2500toInf", "l")

legend.Draw()
c.SetGrid()
#c.Update()
c.Print(pdf_name)

# --------------------------------------------
# tau21 with all number of gen quark overlay
# --------------------------------------------
hist16 = ROOT.TH1F("hist16", ";#tau_{21};Normalized Events", 50, 0, 1)
tree1.Draw("tau21 >> hist16", "@tau21.size()>1")
hist16.SetLineColor(ROOT.kBlack)
hist16.Scale(1.0 / hist16.Integral())
hist16.SetTitle("Normalized #tau_{21} Distribution (Signal vs Background)")
#hist16.Draw("HIST")  #signal

hist17 = ROOT.TH1F("hist17", ";#tau_{21};Normalized Events", 50, 0, 1)
tree2.Draw("tau21 >> hist17", "@tau21.size()>1")
hist17.SetLineColor(ROOT.kRed)
hist17.Scale(1.0 / hist17.Integral())
#hist17.Draw("HIST")  #QCD

hist18 = ROOT.TH1F("hist18", ";#tau_{21};Normalized Events", 50, 0, 1)
tree3.Draw("tau21 >> hist18", "@tau21.size()>1")
hist18.SetLineColor(ROOT.kBlue)
hist18.Scale(1.0 / hist18.Integral())
#hist18.Draw("HIST")  #TTBar

hist19 = ROOT.TH1F("hist19", ";#tau_{21};Normalized Events", 50, 0, 1)
tree4.Draw("tau21 >> hist19", "@tau21.size()>1")
hist19.SetLineColor(ROOT.kGreen)
hist19.Scale(1.0 / hist19.Integral())
#hist19.Draw("HIST")  #W Hadronic

hist20 = ROOT.TH1F("hist20", ";#tau_{21};Normalized Events", 50, 0, 1)
tree5.Draw("tau21 >> hist20", "@tau21.size()>1")
hist20.SetLineColor(ROOT.kOrange)
hist20.Scale(1.0 / hist20.Integral())
#hist20.Draw("HIST")  #W Hadronic

max_y = max(hist16.GetMaximum(), hist17.GetMaximum(), hist18.GetMaximum(), hist19.GetMaximum(), hist20.GetMaximum())
hist16.SetMaximum(1.2*max_y)

hist16.SetLineWidth(2)
hist17.SetLineWidth(2)
hist18.SetLineWidth(2)
hist19.SetLineWidth(2)
hist20.SetLineWidth(2)
hist16.Draw("HIST")
hist17.Draw("HIST SAME")
hist18.Draw("HIST SAME")
hist19.Draw("HIST SAME")
hist20.Draw("HIST SAME")

#c.Update()

legend = ROOT.TLegend(0.65, 0.75, 0.88, 0.88)
legend.AddEntry(hist16, "T'T'1800", "l")
legend.AddEntry(hist17, "QCD1500to2000", "l")
legend.AddEntry(hist18, "TTBar1200to2500", "l")
legend.AddEntry(hist19, "W Hadronic800toInf", "l")
legend.AddEntry(hist20, "W Leptonic2500toInf", "l")

legend.Draw()
c.SetGrid()
#c.Update()
c.Print(pdf_name)

# --------------------------------------------
# tau32 with all number of gen quark overlay
# --------------------------------------------
hist21 = ROOT.TH1F("hist21", ";#tau_{32};Normalized Events", 50, 0, 1)
tree1.Draw("tau32 >> hist21", "@tau32.size()>1")
hist21.SetLineColor(ROOT.kBlack)
hist21.Scale(1.0 / hist21.Integral())
hist21.SetTitle("Normalized #tau_{32} Distribution (Signal vs Background)")
#hist21.Draw("HIST")  #signal

hist22 = ROOT.TH1F("hist22", ";#tau_{32};Normalized Events", 50, 0, 1)
tree2.Draw("tau32 >> hist22", "@tau32.size()>1")
hist22.SetLineColor(ROOT.kRed)
hist22.Scale(1.0 / hist22.Integral())
#hist22.Draw("HIST")  #QCD

hist23 = ROOT.TH1F("hist23", ";#tau_{32};Normalized Events", 50, 0, 1)
tree3.Draw("tau32 >> hist23", "@tau32.size()>1")
hist23.SetLineColor(ROOT.kBlue)
hist23.Scale(1.0 / hist23.Integral())
#hist23.Draw("HIST")  #TTBar

hist24 = ROOT.TH1F("hist24", ";#tau_{32};Normalized Events", 50, 0, 1)
tree4.Draw("tau32 >> hist24", "@tau32.size()>1")
hist24.SetLineColor(ROOT.kGreen)
hist24.Scale(1.0 / hist24.Integral())
#hist24.Draw("HIST")  #W Hadronic

hist25 = ROOT.TH1F("hist25", ";#tau_{32};Normalized Events", 50, 0, 1)
tree5.Draw("tau32 >> hist25", "@tau32.size()>1")
hist25.SetLineColor(ROOT.kOrange)
hist25.Scale(1.0 / hist25.Integral())
#hist25.Draw("HIST")  #W Hadronic

max_y = max(hist21.GetMaximum(), hist22.GetMaximum(), hist23.GetMaximum(), hist24.GetMaximum(), hist25.GetMaximum())
hist21.SetMaximum(1.2*max_y)

hist21.SetLineWidth(2)
hist22.SetLineWidth(2)
hist23.SetLineWidth(2)
hist24.SetLineWidth(2)
hist25.SetLineWidth(2)
hist21.Draw("HIST")
hist22.Draw("HIST SAME")
hist23.Draw("HIST SAME")
hist24.Draw("HIST SAME")
hist25.Draw("HIST SAME")

#c.Update()

legend = ROOT.TLegend(0.65, 0.75, 0.88, 0.88)
legend.AddEntry(hist21, "T'T'1800", "l")
legend.AddEntry(hist22, "QCD1500to2000", "l")
legend.AddEntry(hist23, "TTBar1200to2500", "l")
legend.AddEntry(hist24, "W Hadronic800toInf", "l")
legend.AddEntry(hist25, "W Leptonic2500toInf", "l")

legend.Draw()
c.SetGrid()
#c.Update()
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


