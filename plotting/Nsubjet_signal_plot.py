import ROOT

# Open file and get tree
f = ROOT.TFile.Open("clusteringAnalyzer_NsubJet_TprimeTprime1800_2017_output.root")
dir = f.Get("selcetionStudy_Et150")
tree = dir.Get("tree_nom_Et150.000000")

# Create canvas
c = ROOT.TCanvas("c", "Signal Plots", 800, 700)

# Define output PDF
pdf_name = "signal_plots.pdf"

# Start multipage PDF
c.Print(pdf_name + "[")

# ------------------------------------------
# Plot 1: 2D correlation tau21[1] vs tau21[0]
# ------------------------------------------
hist21_ = ROOT.TH2F("hist21_", ";Leading jet #tau_{21};Subleading jet #tau_{21}", 50, 0, 1, 50, 0, 1)
tree.Draw("tau21[1]:tau21[0] >> hist21_", "@tau21.size()>1", "COLZ")
hist21_.SetTitle("2D Correlation of #tau_{21}(jet1) vs #tau_{21}(jet2)")
ROOT.gStyle.SetOptStat(0)
c.Print(pdf_name)

# ------------------------------------------
# Plot 2: 2D correlation tau32[1] vs tau32[0]
# ------------------------------------------
hist32 = ROOT.TH2F("hist32", ";Leading jet #tau_{32};Subleading jet #tau_{32}", 50, 0, 1, 50, 0, 1)
tree.Draw("tau32[1]:tau32[0] >> hist32", "@tau32.size()>1", "COLZ")
hist32.SetTitle("2D Correlation of #tau_{32}(jet1) vs #tau_{32}(jet2)")
ROOT.gStyle.SetOptStat(0)
c.Print(pdf_name)


# ------------------------------------------
# Plot 2: tau21 distribution for leading jet
# ------------------------------------------
hist1 = ROOT.TH1F("hist1", ";#tau_{21}(jet1);Normalized Events", 50, 0, 1)
tree.Draw("tau21[0] >> hist1", "@tau21.size()>1")
hist1.SetLineColor(ROOT.kBlue)
hist1.Scale(1.0 / hist1.Integral())  # normalize
hist1.SetTitle("Normalized #tau_{21} Distribution (Leading Jet)")
hist1.Draw("HIST")
c.Print(pdf_name)

# ------------------------------------------
# Plot 3: tau21 distribution for subleading jet
# ------------------------------------------
hist2 = ROOT.TH1F("hist2", ";#tau_{21}(jet2);Normalized Events", 50, 0, 1)
tree.Draw("tau21[1] >> hist2", "@tau21.size()>1")
hist2.SetLineColor(ROOT.kRed)
hist2.Scale(1.0 / hist2.Integral())
hist2.SetTitle("Normalized #tau_{21} Distribution (Subleading Jet)")
hist2.Draw("HIST")
c.Print(pdf_name)

# ------------------------------------------
# Plot 4: Overlay of both histograms (normalized)
# ------------------------------------------
hist1.SetLineWidth(2)
hist2.SetLineWidth(2)
hist1.Draw("HIST")
hist2.Draw("HIST SAME")

legend = ROOT.TLegend(0.65, 0.75, 0.88, 0.88)
legend.AddEntry(hist1, "Leading Jet", "l")
legend.AddEntry(hist2, "Subleading Jet", "l")
legend.Draw()

c.SetGrid()
c.Print(pdf_name)

# --------------------------------------------
# tau 1 with all number of gen quark overlay
# --------------------------------------------
hist4 = ROOT.TH1F("hist4", ";#tau_{1};Normalized Events", 50, 0, 1)
tree.Draw("tau1 >> hist4", "@tau1.size()>1")
hist4.SetLineColor(ROOT.kBlack)
hist4.Scale(1.0 / hist4.Integral())
hist4.SetTitle("Normalized #tau_{1} Distribution (All Jets)")
hist4.Draw("HIST")
c.Print(pdf_name)

hist5 = ROOT.TH1F("hist5", ";#tau_{1};Normalized Events", 50, 0, 1)
tree.Draw("tau1 >> hist5", "@tau1.size()>1&&jetMultiplicity==6")
hist5.SetLineColor(ROOT.kRed)
hist5.Scale(1.0 / hist5.Integral())
hist5.Draw("HIST")

hist6 = ROOT.TH1F("hist6", ";#tau_{1};Normalized Events", 50, 0, 1)
tree.Draw("tau1 >> hist6", "@tau1.size()>1&&jetMultiplicity==8")
hist6.SetLineColor(ROOT.kBlue)
hist6.Scale(1.0 / hist6.Integral())
hist6.Draw("HIST")

hist7 = ROOT.TH1F("hist7", ";#tau_{1};Normalized Events", 50, 0, 1)
tree.Draw("tau1 >> hist7", "@tau1.size()>1&&jetMultiplicity==10")
hist7.SetLineColor(ROOT.kGreen)
hist7.Scale(1.0 / hist7.Integral())
hist7.Draw("HIST")

hist4.SetLineWidth(2)
hist5.SetLineWidth(2)
hist6.SetLineWidth(2)
hist6.SetLineWidth(2)
hist4.Draw("HIST")
hist5.Draw("HIST SAME")
hist6.Draw("HIST SAME")
hist7.Draw("HIST SAME")

legend = ROOT.TLegend(0.65, 0.75, 0.88, 0.88)
legend.AddEntry(hist4, "All type", "l")
legend.AddEntry(hist5, "6 Gen Quarks", "l")
legend.AddEntry(hist6, "8 Gen Quarks", "l")
legend.AddEntry(hist7, "10 Gen Quarks", "l")

legend.Draw()

c.SetGrid()
c.Print(pdf_name)

# -------------------------------------------
# tau 2 with all number of gen quark overlay
# --------------------------------------------
hist8 = ROOT.TH1F("hist8", ";#tau_{2};Normalized Events", 50, 0, 1)
tree.Draw("tau2 >> hist8", "@tau2.size()>1")
hist8.SetLineColor(ROOT.kBlack)
hist8.Scale(1.0 / hist8.Integral())
hist8.SetTitle("Normalized #tau_{2} Distribution (All Jets)")
hist8.Draw("HIST")
c.Print(pdf_name)

hist9 = ROOT.TH1F("hist9", ";#tau_{2};Normalized Events", 50, 0, 1)
tree.Draw("tau2 >> hist9", "@tau2.size()>1&&jetMultiplicity==6")
hist9.SetLineColor(ROOT.kRed)
hist9.Scale(1.0 / hist9.Integral())
hist9.Draw("HIST")

hist10 = ROOT.TH1F("hist10", ";#tau_{2};Normalized Events", 50, 0, 1)
tree.Draw("tau2 >> hist10", "@tau2.size()>1&&jetMultiplicity==8")
hist10.SetLineColor(ROOT.kBlue)
hist10.Scale(1.0 / hist10.Integral())
hist10.Draw("HIST")

hist11 = ROOT.TH1F("hist11", ";#tau_{2};Normalized Events", 50, 0, 1)
tree.Draw("tau2 >> hist11", "@tau2.size()>1&&jetMultiplicity==10")
hist11.SetLineColor(ROOT.kGreen)
hist11.Scale(1.0 / hist11.Integral())
hist11.Draw("HIST")

hist8.SetLineWidth(2)
hist9.SetLineWidth(2)
hist10.SetLineWidth(2)
hist11.SetLineWidth(2)
hist8.Draw("HIST")
hist9.Draw("HIST SAME")
hist10.Draw("HIST SAME")
hist11.Draw("HIST SAME")

legend = ROOT.TLegend(0.65, 0.75, 0.88, 0.88)
legend.AddEntry(hist8, "All type", "l")
legend.AddEntry(hist9, "6 Gen Quarks", "l")
legend.AddEntry(hist10, "8 Gen Quarks", "l")
legend.AddEntry(hist11, "10 Gen Quarks", "l")

legend.Draw()
c.SetGrid()
c.Print(pdf_name)


# -------------------------------------------
# tau 3 with all number of gen quark overlay
# --------------------------------------------
hist12 = ROOT.TH1F("hist12", ";#tau_{3};Normalized Events", 50, 0, 1)
tree.Draw("tau3 >> hist12", "@tau3.size()>1")
hist12.SetLineColor(ROOT.kBlack)
hist12.Scale(1.0 / hist12.Integral())
hist12.SetTitle("Normalized #tau_{3} Distribution (All Jets)")
hist12.Draw("HIST")
c.Print(pdf_name)

hist13 = ROOT.TH1F("hist13", ";#tau_{3};Normalized Events", 50, 0, 1)
tree.Draw("tau3 >> hist13", "@tau3.size()>1&&jetMultiplicity==6")
hist13.SetLineColor(ROOT.kRed)
hist13.Scale(1.0 / hist13.Integral())
hist13.Draw("HIST")

hist14 = ROOT.TH1F("hist14", ";#tau_{3};Normalized Events", 50, 0, 1)
tree.Draw("tau3 >> hist14", "@tau3.size()>1&&jetMultiplicity==8")
hist14.SetLineColor(ROOT.kBlue)
hist14.Scale(1.0 / hist14.Integral())
hist14.Draw("HIST")

hist15 = ROOT.TH1F("hist15", ";#tau_{3};Normalized Events", 50, 0, 1)
tree.Draw("tau3 >> hist15", "@tau3.size()>1&&jetMultiplicity==10")
hist15.SetLineColor(ROOT.kGreen)
hist15.Scale(1.0 / hist15.Integral())
hist15.Draw("HIST")

hist12.SetLineWidth(2)
hist13.SetLineWidth(2)
hist14.SetLineWidth(2)
hist15.SetLineWidth(2)
hist12.Draw("HIST")
hist13.Draw("HIST SAME")
hist14.Draw("HIST SAME")
hist15.Draw("HIST SAME")

legend = ROOT.TLegend(0.65, 0.75, 0.88, 0.88)
legend.AddEntry(hist12, "All type", "l")
legend.AddEntry(hist13, "6 Gen Quarks", "l")
legend.AddEntry(hist14, "8 Gen Quarks", "l")
legend.AddEntry(hist15, "10 Gen Quarks", "l")

legend.Draw()
c.SetGrid()
c.Print(pdf_name)


# -------------------------------------------
# tau 21 with all number of gen quark overlay
# --------------------------------------------
hist16 = ROOT.TH1F("hist16", ";#tau_{21};Normalized Events", 50, 0, 1)
tree.Draw("tau21 >> hist16", "@tau21.size()>1")
hist16.SetLineColor(ROOT.kBlack)
hist16.Scale(1.0 / hist16.Integral())
hist16.SetTitle("Normalized #tau_{21} Distribution (All Jets)")
hist16.Draw("HIST")
c.Print(pdf_name)

hist17 = ROOT.TH1F("hist17", ";#tau_{21};Normalized Events", 50, 0, 1)
tree.Draw("tau21 >> hist17", "@tau21.size()>1&&jetMultiplicity==6")
hist17.SetLineColor(ROOT.kRed)
hist17.Scale(1.0 / hist17.Integral())
hist17.Draw("HIST")

hist18 = ROOT.TH1F("hist18", ";#tau_{21};Normalized Events", 50, 0, 1)
tree.Draw("tau21 >> hist18", "@tau21.size()>1&&jetMultiplicity==8")
hist18.SetLineColor(ROOT.kBlue)
hist18.Scale(1.0 / hist18.Integral())
hist18.Draw("HIST")

hist19 = ROOT.TH1F("hist19", ";#tau_{21};Normalized Events", 50, 0, 1)
tree.Draw("tau21 >> hist19", "@tau21.size()>1&&jetMultiplicity==10")
hist19.SetLineColor(ROOT.kGreen)
hist19.Scale(1.0 / hist19.Integral())
hist19.Draw("HIST")

hist16.SetLineWidth(2)
hist17.SetLineWidth(2)
hist18.SetLineWidth(2)
hist19.SetLineWidth(2)
hist16.Draw("HIST")
hist17.Draw("HIST SAME")
hist18.Draw("HIST SAME")
hist19.Draw("HIST SAME")

legend = ROOT.TLegend(0.65, 0.75, 0.88, 0.88)
legend.AddEntry(hist16, "All type", "l")
legend.AddEntry(hist17, "6 Gen Quarks", "l")
legend.AddEntry(hist18, "8 Gen Quarks", "l")
legend.AddEntry(hist19, "10 Gen Quarks", "l")

legend.Draw()
c.SetGrid()
c.Print(pdf_name)


# -------------------------------------------
# tau 32 with all number of gen quark overlay
# --------------------------------------------
hist20 = ROOT.TH1F("hist20", ";#tau_{32};Normalized Events", 50, 0, 1)
tree.Draw("tau32 >> hist20", "@tau32.size()>1")
hist20.SetLineColor(ROOT.kBlack)
hist20.Scale(1.0 / hist20.Integral())
hist20.SetTitle("Normalized #tau_{32} Distribution (All Jets)")
hist20.Draw("HIST")
c.Print(pdf_name)

hist21 = ROOT.TH1F("hist21", ";#tau_{32};Normalized Events", 50, 0, 1)
tree.Draw("tau32 >> hist21", "@tau32.size()>1&&jetMultiplicity==6")
hist21.SetLineColor(ROOT.kRed)
hist21.Scale(1.0 / hist21.Integral())
hist21.Draw("HIST")

hist22 = ROOT.TH1F("hist22", ";#tau_{32};Normalized Events", 50, 0, 1)
tree.Draw("tau32 >> hist22", "@tau32.size()>1&&jetMultiplicity==8")
hist22.SetLineColor(ROOT.kBlue)
hist22.Scale(1.0 / hist22.Integral())
hist22.Draw("HIST")

hist23 = ROOT.TH1F("hist23", ";#tau_{32};Normalized Events", 50, 0, 1)
tree.Draw("tau32 >> hist23", "@tau32.size()>1&&jetMultiplicity==10")
hist23.SetLineColor(ROOT.kGreen)
hist23.Scale(1.0 / hist23.Integral())
hist23.Draw("HIST")

hist20.SetLineWidth(2)
hist21.SetLineWidth(2)
hist22.SetLineWidth(2)
hist23.SetLineWidth(2)
hist20.Draw("HIST")
hist21.Draw("HIST SAME")
hist22.Draw("HIST SAME")
hist23.Draw("HIST SAME")

legend = ROOT.TLegend(0.65, 0.75, 0.88, 0.88)
legend.AddEntry(hist20, "All type", "l")
legend.AddEntry(hist21, "6 Gen Quarks", "l")
legend.AddEntry(hist22, "8 Gen Quarks", "l")
legend.AddEntry(hist23, "10 Gen Quarks", "l")

legend.Draw()
c.SetGrid()
c.Print(pdf_name)

# -------------------------------------------
# tau 1 with all processes overlay
# --------------------------------------------
hist_1 = ROOT.TH1F("hist_1", ";#tau_{1};Normalized Events", 50, 0, 1)
tree.Draw("tau1 >> hist_1", "@tau1.size()>1")
hist_1.SetLineColor(ROOT.kBlack)
hist_1.Scale(1.0 / hist_1.Integral())
hist_1.SetTitle("Normalized #tau_{1} Distribution (All Jets)")
hist_1.Draw("HIST")
c.Print(pdf_name)

hist_2 = ROOT.TH1F("hist_2", ";#tau_{1};Normalized Events", 50, 0, 1)
tree.Draw("tau1 >> hist_2", "@tau1.size()>1&&process==0")
hist_2.SetLineColor(ROOT.kRed)
hist_2.Scale(1.0 / hist_2.Integral())
hist_2.Draw("HIST")

hist_3 = ROOT.TH1F("hist_3", ";#tau_{1};Normalized Events", 50, 0, 1)
tree.Draw("tau1 >> hist_3", "@tau1.size()>1&&process==1")
hist_3.SetLineColor(ROOT.kBlue)
hist_3.Scale(1.0 / hist_3.Integral())
hist_3.Draw("HIST")

hist_4 = ROOT.TH1F("hist_4", ";#tau_{1};Normalized Events", 50, 0, 1)
tree.Draw("tau1 >> hist_4", "@tau1.size()>1&&process==2")
hist_4.SetLineColor(ROOT.kGreen)
hist_4.Scale(1.0 / hist_4.Integral())
hist_4.Draw("HIST")

hist_5 = ROOT.TH1F("hist_5", ";#tau_{1};Normalized Events", 50, 0, 1)
tree.Draw("tau1 >> hist_5", "@tau1.size()>1&&process==3")
hist_5.SetLineColor(ROOT.kOrange)
hist_5.Scale(1.0 / hist_5.Integral())
hist_5.Draw("HIST")

hist_6 = ROOT.TH1F("hist_6", ";#tau_{1};Normalized Events", 50, 0, 1)
tree.Draw("tau1 >> hist_6", "@tau1.size()>1&&process==4")
hist_6.SetLineColor(ROOT.kMagenta)
hist_6.Scale(1.0 / hist_6.Integral())
hist_6.Draw("HIST")

hist_7 = ROOT.TH1F("hist_7", ";#tau_{1};Normalized Events", 50, 0, 1)
tree.Draw("tau1 >> hist_7", "@tau1.size()>1&&process==5")
hist_7.SetLineColor(ROOT.kCyan)
hist_7.Scale(1.0 / hist_7.Integral())
hist_7.Draw("HIST")

hist_1.SetLineWidth(2)
hist_2.SetLineWidth(2)
hist_3.SetLineWidth(2)
hist_4.SetLineWidth(2)
hist_5.SetLineWidth(2)
hist_6.SetLineWidth(2)
hist_7.SetLineWidth(2)

hist_1.Draw("HIST")
hist_2.Draw("HIST SAME")
hist_3.Draw("HIST SAME")
hist_4.Draw("HIST SAME")
hist_5.Draw("HIST SAME")
hist_6.Draw("HIST SAME")
hist_7.Draw("HIST SAME")

legend = ROOT.TLegend(0.65, 0.75, 0.88, 0.88)
legend.AddEntry(hist_1, "All type", "l")
legend.AddEntry(hist_2, "htht", "l")
legend.AddEntry(hist_3, "ZtZt", "l")
legend.AddEntry(hist_4, "htZt", "l")
legend.AddEntry(hist_5, "ZtWb", "l")
legend.AddEntry(hist_6, "htWb", "l")
legend.AddEntry(hist_7, "WbWb", "l")

legend.Draw()
c.SetGrid()
c.Print(pdf_name)

# -------------------------------------------
# tau2 with all processes overlay
# --------------------------------------------
hist_8 = ROOT.TH1F("hist_8", ";#tau_{2};Normalized Events", 50, 0, 1)
tree.Draw("tau2 >> hist_8", "@tau2.size()>1")
hist_8.SetLineColor(ROOT.kBlack)
hist_8.Scale(1.0 / hist_8.Integral())
hist_8.SetTitle("Normalized #tau_{2} Distribution (All Jets)")
hist_8.Draw("HIST")
c.Print(pdf_name)

hist_9 = ROOT.TH1F("hist_9", ";#tau_{2};Normalized Events", 50, 0, 1)
tree.Draw("tau2 >> hist_9", "@tau2.size()>1&&process==0")
hist_9.SetLineColor(ROOT.kRed)
hist_9.Scale(1.0 / hist_9.Integral())
hist_9.Draw("HIST")

hist_10 = ROOT.TH1F("hist_10", ";#tau_{2};Normalized Events", 50, 0, 1)
tree.Draw("tau2 >> hist_10", "@tau2.size()>1&&process==1")
hist_10.SetLineColor(ROOT.kBlue)
hist_10.Scale(1.0 / hist_10.Integral())
hist_10.Draw("HIST")

hist_11 = ROOT.TH1F("hist_11", ";#tau_{2};Normalized Events", 50, 0, 1)
tree.Draw("tau2 >> hist_11", "@tau2.size()>1&&process==2")
hist_11.SetLineColor(ROOT.kGreen)
hist_11.Scale(1.0 / hist_11.Integral())
hist_11.Draw("HIST")

hist_12 = ROOT.TH1F("hist_12", ";#tau_{2};Normalized Events", 50, 0, 1)
tree.Draw("tau2 >> hist_12", "@tau2.size()>1&&process==3")
hist_12.SetLineColor(ROOT.kOrange)
hist_12.Scale(1.0 / hist_12.Integral())
hist_12.Draw("HIST")

hist_13 = ROOT.TH1F("hist_13", ";#tau_{2};Normalized Events", 50, 0, 1)
tree.Draw("tau2 >> hist_13", "@tau2.size()>1&&process==4")
hist_13.SetLineColor(ROOT.kMagenta)
hist_13.Scale(1.0 / hist_13.Integral())
hist_13.Draw("HIST")

hist_14 = ROOT.TH1F("hist_14", ";#tau_{2};Normalized Events", 50, 0, 1)
tree.Draw("tau2 >> hist_14", "@tau2.size()>1&&process==5")
hist_14.SetLineColor(ROOT.kCyan)
hist_14.Scale(1.0 / hist_14.Integral())
hist_14.Draw("HIST")

hist_9.SetLineWidth(2)
hist_10.SetLineWidth(2)
hist_11.SetLineWidth(2)
hist_12.SetLineWidth(2)
hist_13.SetLineWidth(2)
hist_14.SetLineWidth(2)
hist_8.SetLineWidth(2)

hist_8.Draw("HIST")
hist_9.Draw("HIST SAME")
hist_10.Draw("HIST SAME")
hist_11.Draw("HIST SAME")
hist_12.Draw("HIST SAME")
hist_13.Draw("HIST SAME")
hist_14.Draw("HIST SAME")

legend = ROOT.TLegend(0.65, 0.75, 0.88, 0.88)
legend.AddEntry(hist_8, "All type", "l")
legend.AddEntry(hist_9, "htht", "l")
legend.AddEntry(hist_10, "ZtZt", "l")
legend.AddEntry(hist_11, "htZt", "l")
legend.AddEntry(hist_12, "ZtWb", "l")
legend.AddEntry(hist_13, "htWb", "l")
legend.AddEntry(hist_14, "WbWb", "l")

legend.Draw()
c.SetGrid()
c.Print(pdf_name)

# -------------------------------------------
# tau3 with all processes overlay
# --------------------------------------------
hist_15 = ROOT.TH1F("hist_15", ";#tau_{3};Normalized Events", 50, 0, 1)
tree.Draw("tau3 >> hist_15", "@tau3.size()>1")
hist_15.SetLineColor(ROOT.kBlack)
hist_15.Scale(1.0 / hist_15.Integral())
hist_15.SetTitle("Normalized #tau_{3} Distribution (All Jets)")
hist_15.Draw("HIST")
c.Print(pdf_name)

hist_16 = ROOT.TH1F("hist_16", ";#tau_{3};Normalized Events", 50, 0, 1)
tree.Draw("tau3 >> hist_16", "@tau3.size()>1&&process==0")
hist_16.SetLineColor(ROOT.kRed)
hist_16.Scale(1.0 / hist_16.Integral())
hist_16.Draw("HIST")

hist_17 = ROOT.TH1F("hist_17", ";#tau_{3};Normalized Events", 50, 0, 1)
tree.Draw("tau3 >> hist_17", "@tau3.size()>1&&process==1")
hist_17.SetLineColor(ROOT.kBlue)
hist_17.Scale(1.0 / hist_17.Integral())
hist_17.Draw("HIST")

hist_18 = ROOT.TH1F("hist_18", ";#tau_{3};Normalized Events", 50, 0, 1)
tree.Draw("tau3 >> hist_18", "@tau3.size()>1&&process==2")
hist_18.SetLineColor(ROOT.kGreen)
hist_18.Scale(1.0 / hist_18.Integral())
hist_18.Draw("HIST")

hist_19 = ROOT.TH1F("hist_19", ";#tau_{3};Normalized Events", 50, 0, 1)
tree.Draw("tau3 >> hist_19", "@tau3.size()>1&&process==3")
hist_19.SetLineColor(ROOT.kOrange)
hist_19.Scale(1.0 / hist_19.Integral())
hist_19.Draw("HIST")

hist_20 = ROOT.TH1F("hist_20", ";#tau_{3};Normalized Events", 50, 0, 1)
tree.Draw("tau3 >> hist_20", "@tau3.size()>1&&process==4")
hist_20.SetLineColor(ROOT.kMagenta)
hist_20.Scale(1.0 / hist_20.Integral())
hist_20.Draw("HIST")

hist_21 = ROOT.TH1F("hist_21", ";#tau_{3};Normalized Events", 50, 0, 1)
tree.Draw("tau3 >> hist_21", "@tau3.size()>1&&process==5")
hist_21.SetLineColor(ROOT.kCyan)
hist_21.Scale(1.0 / hist_21.Integral())
hist_21.Draw("HIST")

hist_15.SetLineWidth(2)
hist_16.SetLineWidth(2)
hist_17.SetLineWidth(2)
hist_18.SetLineWidth(2)
hist_19.SetLineWidth(2)
hist_20.SetLineWidth(2)
hist_21.SetLineWidth(2)

hist_15.Draw("HIST")
hist_16.Draw("HIST SAME")
hist_17.Draw("HIST SAME")
hist_18.Draw("HIST SAME")
hist_19.Draw("HIST SAME")
hist_20.Draw("HIST SAME")
hist_21.Draw("HIST SAME")

legend = ROOT.TLegend(0.65, 0.75, 0.88, 0.88)
legend.AddEntry(hist_15, "All type", "l")
legend.AddEntry(hist_16, "htht", "l")
legend.AddEntry(hist_17, "ZtZt", "l")
legend.AddEntry(hist_18, "htZt", "l")
legend.AddEntry(hist_19, "ZtWb", "l")
legend.AddEntry(hist_20, "htWb", "l")
legend.AddEntry(hist_21, "WbWb", "l")

legend.Draw()
c.SetGrid()
c.Print(pdf_name)

# -------------------------------------------
# tau21 with all processes overlay
# --------------------------------------------
hist_22 = ROOT.TH1F("hist_22", ";#tau_{21};Normalized Events", 50, 0, 1)
tree.Draw("tau21 >> hist_22", "@tau21.size()>1")
hist_22.SetLineColor(ROOT.kBlack)
hist_22.Scale(1.0 / hist_22.Integral())
hist_22.SetTitle("Normalized #tau_{21} Distribution (All Jets)")
hist_22.Draw("HIST")
c.Print(pdf_name)

hist_23 = ROOT.TH1F("hist_23", ";#tau_{21};Normalized Events", 50, 0, 1)
tree.Draw("tau21 >> hist_23", "@tau21.size()>1&&process==0")
hist_23.SetLineColor(ROOT.kRed)
hist_23.Scale(1.0 / hist_23.Integral())
hist_23.Draw("HIST")

hist_24 = ROOT.TH1F("hist_24", ";#tau_{21};Normalized Events", 50, 0, 1)
tree.Draw("tau21 >> hist_24", "@tau21.size()>1&&process==1")
hist_24.SetLineColor(ROOT.kBlue)
hist_24.Scale(1.0 / hist_24.Integral())
hist_24.Draw("HIST")

hist_25 = ROOT.TH1F("hist_25", ";#tau_{21};Normalized Events", 50, 0, 1)
tree.Draw("tau21 >> hist_25", "@tau21.size()>1&&process==2")
hist_25.SetLineColor(ROOT.kGreen)
hist_25.Scale(1.0 / hist_25.Integral())
hist_25.Draw("HIST")

hist_26 = ROOT.TH1F("hist_26", ";#tau_{21};Normalized Events", 50, 0, 1)
tree.Draw("tau21 >> hist_26", "@tau21.size()>1&&process==3")
hist_26.SetLineColor(ROOT.kOrange)
hist_26.Scale(1.0 / hist_26.Integral())
hist_26.Draw("HIST")

hist_27 = ROOT.TH1F("hist_27", ";#tau_{21};Normalized Events", 50, 0, 1)
tree.Draw("tau21 >> hist_27", "@tau21.size()>1&&process==4")
hist_27.SetLineColor(ROOT.kMagenta)
hist_27.Scale(1.0 / hist_27.Integral())
hist_27.Draw("HIST")

hist_28 = ROOT.TH1F("hist_28", ";#tau_{21};Normalized Events", 50, 0, 1)
tree.Draw("tau21 >> hist_28", "@tau21.size()>1&&process==5")
hist_28.SetLineColor(ROOT.kCyan)
hist_28.Scale(1.0 / hist_28.Integral())
hist_28.Draw("HIST")

hist_22.SetLineWidth(2)
hist_23.SetLineWidth(2)
hist_24.SetLineWidth(2)
hist_25.SetLineWidth(2)
hist_26.SetLineWidth(2)
hist_27.SetLineWidth(2)
hist_28.SetLineWidth(2)

hist_22.Draw("HIST")
hist_23.Draw("HIST SAME")
hist_24.Draw("HIST SAME")
hist_25.Draw("HIST SAME")
hist_26.Draw("HIST SAME")
hist_27.Draw("HIST SAME")
hist_28.Draw("HIST SAME")

legend = ROOT.TLegend(0.65, 0.75, 0.88, 0.88)
legend.AddEntry(hist_22, "All type", "l")
legend.AddEntry(hist_23, "htht", "l")
legend.AddEntry(hist_24, "ZtZt", "l")
legend.AddEntry(hist_25, "htZt", "l")
legend.AddEntry(hist_26, "ZtWb", "l")
legend.AddEntry(hist_27, "htWb", "l")
legend.AddEntry(hist_28, "WbWb", "l")

legend.Draw()
c.SetGrid()
c.Print(pdf_name)

# -------------------------------------------
# tau32 with all processes overlay
# --------------------------------------------
hist_29 = ROOT.TH1F("hist_29", ";#tau_{32};Normalized Events", 50, 0, 1)
tree.Draw("tau32 >> hist_29", "@tau32.size()>1")
hist_29.SetLineColor(ROOT.kBlack)
hist_29.Scale(1.0 / hist_29.Integral())
hist_29.SetTitle("Normalized #tau_{32} Distribution (All Jets)")
hist_29.Draw("HIST")
c.Print(pdf_name)

hist_30 = ROOT.TH1F("hist_30", ";#tau_{32};Normalized Events", 50, 0, 1)
tree.Draw("tau32 >> hist_30", "@tau32.size()>1&&process==0")
hist_30.SetLineColor(ROOT.kRed)
hist_30.Scale(1.0 / hist_30.Integral())
hist_30.Draw("HIST")

hist_31 = ROOT.TH1F("hist_31", ";#tau_{32};Normalized Events", 50, 0, 1)
tree.Draw("tau32 >> hist_31", "@tau32.size()>1&&process==1")
hist_31.SetLineColor(ROOT.kBlue)
hist_31.Scale(1.0 / hist_31.Integral())
hist_31.Draw("HIST")

hist_32 = ROOT.TH1F("hist_32", ";#tau_{32};Normalized Events", 50, 0, 1)
tree.Draw("tau32 >> hist_32", "@tau32.size()>1&&process==2")
hist_32.SetLineColor(ROOT.kGreen)
hist_32.Scale(1.0 / hist_32.Integral())
hist_32.Draw("HIST")

hist_33 = ROOT.TH1F("hist_33", ";#tau_{32};Normalized Events", 50, 0, 1)
tree.Draw("tau32 >> hist_33", "@tau32.size()>1&&process==3")
hist_33.SetLineColor(ROOT.kOrange)
hist_33.Scale(1.0 / hist_33.Integral())
hist_33.Draw("HIST")

hist_34 = ROOT.TH1F("hist_34", ";#tau_{32};Normalized Events", 50, 0, 1)
tree.Draw("tau32 >> hist_34", "@tau32.size()>1&&process==4")
hist_34.SetLineColor(ROOT.kMagenta)
hist_34.Scale(1.0 / hist_34.Integral())
hist_34.Draw("HIST")

hist_35 = ROOT.TH1F("hist_35", ";#tau_{32};Normalized Events", 50, 0, 1)
tree.Draw("tau32 >> hist_35", "@tau32.size()>1&&process==5")
hist_35.SetLineColor(ROOT.kCyan)
hist_35.Scale(1.0 / hist_35.Integral())
hist_35.Draw("HIST")

hist_29.SetLineWidth(2)
hist_30.SetLineWidth(2)
hist_31.SetLineWidth(2)
hist_32.SetLineWidth(2)
hist_33.SetLineWidth(2)
hist_34.SetLineWidth(2)
hist_35.SetLineWidth(2)

hist_29.Draw("HIST")
hist_30.Draw("HIST SAME")
hist_31.Draw("HIST SAME")
hist_32.Draw("HIST SAME")
hist_33.Draw("HIST SAME")
hist_34.Draw("HIST SAME")
hist_35.Draw("HIST SAME")

legend = ROOT.TLegend(0.65, 0.75, 0.88, 0.88)
legend.AddEntry(hist_29, "All type", "l")
legend.AddEntry(hist_30, "htht", "l")
legend.AddEntry(hist_31, "ZtZt", "l")
legend.AddEntry(hist_32, "htZt", "l")
legend.AddEntry(hist_33, "ZtWb", "l")
legend.AddEntry(hist_34, "htWb", "l")
legend.AddEntry(hist_35, "WbWb", "l")

legend.Draw()
c.SetGrid()
c.Print(pdf_name)

# ------------------------------------------
# Close the multipage PDF
# ------------------------------------------
c.Print(pdf_name + "]")

f.Close()
print(f"Saved all plots to {pdf_name}")


