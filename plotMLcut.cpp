#include <ROOT/RDataFrame.hxx>
#include "Cutmaking.cpp"
#include "styleset.C"


//considerations- do we want Empty Data?

void plotcut(){

  styleset();
  string jfname="./mc2/mu_LD2_jmixR008sample_2111v2sigma_nomass_0to300_eval_wfileSwitch_Inh2_00007234_300_00109231_300_00208230_300_00301230_300_00408230_300_00500230_300_.root";
  string jtname="background";
  string jfilter="(mass>4.2)&&(mass<8.5) && (pValRes_MESS>-1.000000) && (pValRes_PSIBACK>-0.039900) && (pValRes_JMIX>-1.000000) && (pValRes_DUMP>-0.009996) && (pValRes_MESS > -0.3) && (pValRes_JMIX > -0.0300115)";
//setfiles
  string ftree="Roadset67";
  string fileName="Realfulldata.root";
  ROOT::RDataFrame full(ftree, fileName);
  ROOT::RDataFrame H2cut("LH2","FilteredHData.root");
  ROOT::RDataFrame D2cut("LD2","FilteredDData.root");
  ROOT::RDataFrame EMPcut("LD2","FilteredDData.root");
  ROOT::RDataFrame jmix(jtname,jfname);

//split full data
  auto Dfull=full.Filter("targetPos==3"); 
  auto Hfull=full.Filter("targetPos==1");
  auto Empfull=full.Filter("targetPos==2");

  auto jcut=jmix.Filter(jfilter);

//Build Histograms
  auto Hfullmass= Hfull.Histo1D({"Hfullmass","",43,4.2,8.5},"mass");
  auto Dfullmass= Dfull.Histo1D({"Dfullmass","",43,4.2,8.5},"mass");
  auto HfullxT= Hfull.Histo1D({"HfullxT","",15,.1,.55},"xT");
  auto DfullxT= Dfull.Histo1D({"DfullxT","",15,.1,.55},"xT");

  auto Empmass= Empfull.Histo1D({"Empmass","",43,4.2,8.5},"mass");
  auto jmixmass= jmix.Histo1D({"jfullmass","",38,4.2,8.0},"mass");
  auto EmpxT= Empfull.Histo1D({"EmpxT","",15,.1,.55},"xT");
  auto jmixxT= jmix.Histo1D({"jfullxT","",15,.1,.55},"xT");

  auto HxT= H2cut.Histo1D({"HxT","",15,.1,.55},"xT");
  auto DxT= D2cut.Histo1D({"DxT","",15,.1,.55},"xT");
  auto Hmass=H2cut.Histo1D({"Hmass","",43,4.2,8.5},"mass");
  auto Dmass=D2cut.Histo1D({"Dmass","",43,4.2,8.5},"mass");

  auto exT= EMPcut.Histo1D({"exT","",15,.1,.55},"xT");
  auto jxT= jcut.Histo1D({"jxT","",15,.1,.55},"xT");
  auto emass=EMPcut.Histo1D({"emass","",43,4.2,8.5},"mass");
  auto jmass=jcut.Histo1D({"jmass","",38,4.2,8.0},"mass");


//this is repeated for fine tuning purposes
  TCanvas c1;
  Hfullmass->Draw("c");
  Hmass->Draw("Csame");
  TLegend *legend = new TLegend(.72,.75,.87,.85);
  legend->AddEntry("Hfullmass","Uncut Data"); 
  legend->AddEntry("Hmass","Cut Data");
  legend->Draw();
  Hmass->SetFillStyle(1001);
  Hmass->SetFillColor(4);
  Hfullmass->SetFillColor(2);
  Hfullmass->SetFillStyle(3003);  
  Hfullmass->GetXaxis()->SetTitle("Mass GeV");
  Hfullmass->GetYaxis()->SetTitle("Dimuon Count");
  c1.Update();
  c1.SaveAs("LH2massRealcut.png");
  c1.SetLogy();
  c1.SetGridx();
  c1.SetGridy();
  Hfullmass->GetYaxis()->SetTitle("Log(Dimuon Count)"); 
  Hfullmass->SetMaximum(350000);
  Hfullmass->SetMinimum(1);
  c1.Update();
  c1.SaveAs("LogLH2massRealcut.png");

  Dfullmass->Draw("C");
  Dmass->Draw("Csame");
  TLegend *legendd = new TLegend(.72,.75,.87,.85);
  legendd->AddEntry("Dfullmass","Uncut Data"); 
  legendd->AddEntry("Dmass","Cut Data");
  legendd->Draw();
  Dmass->SetFillStyle(1001);
  Dmass->SetFillColor(4);
  Dfullmass->SetFillColor(2);
  Dfullmass->SetFillStyle(3003);  
  Dfullmass->GetXaxis()->SetTitle("Mass GeV");
  Dfullmass->GetYaxis()->SetTitle("Dimuon Count");
  c1.Update();
  c1.SaveAs("LD2massRealcut.png");

  c1.SetLogy();
  Dfullmass->GetYaxis()->SetTitle("Log(Dimuon Count)");
  Dfullmass->SetMaximum(150000);
  Dfullmass->SetMinimum(1);
  c1.Update();
  c1.SaveAs("LogLD2massRealcut.png");

  HfullxT->Draw("C");
  HxT->Draw("Csame");
  TLegend *legendhx = new TLegend(.72,.75,.87,.85);
  legendhx->AddEntry("HfullxT","Uncut Data"); 
  legendhx->AddEntry("HxT","Cut Data");
  legendhx->Draw();
  HxT->SetFillStyle(1001);
  HxT->SetFillColor(4);
  HfullxT->SetFillColor(2);
  HfullxT->SetFillStyle(3003);  
  HfullxT->GetXaxis()->SetTitle("Target Bjorken-x");
  HfullxT->GetYaxis()->SetTitle("Dimuon Count");
  c1.Update();
  c1.SaveAs("LH2xTRealcut.png");
  c1.SetLogy();
  HfullxT->GetYaxis()->SetTitle("Log(Dimuon Count)");
  HfullxT->SetMaximum(4000000);
  HfullxT->SetMinimum(1);
  c1.Update();
  c1.SaveAs("LogLH2xTRealcut.png");

  DfullxT->Draw("C");
  DxT->Draw("Csame");
  TLegend *legenddx = new TLegend(.72,.75,.87,.85);
  legenddx->AddEntry("DfullxT","Uncut Data"); 
  legenddx->AddEntry("DxT","Cut Data");
  legenddx->Draw();
  DxT->SetFillStyle(1001);
  DxT->SetFillColor(4);
  DfullxT->SetFillColor(2);
  DfullxT->SetFillStyle(3003);  
  DfullxT->GetXaxis()->SetTitle("Target Bjorken-x");
  DfullxT->GetYaxis()->SetTitle("Dimuon Count");
  c1.Update();
  c1.SaveAs("LD2xTRealcut.png");
  c1.SetLogy();
  DfullxT->GetYaxis()->SetTitle("Log(Dimuon Count)");
  DfullxT->SetMaximum(1200000);
  DfullxT->SetMinimum(1);    
  c1.Update();
  c1.SaveAs("LogLD2xTRealcut.png");


  Empmass->Draw("hist");
  emass->Draw("Csame");
  TLegend *legendem = new TLegend(.72,.75,.87,.85);
  legendem->AddEntry("Empmass","Uncut Data"); 
  legendem->AddEntry("emass","Cut Data");
  legendem->Draw();
  emass->SetFillStyle(1001);
  emass->SetFillColor(4);
  Empmass->SetFillColor(2);
  Empmass->SetFillStyle(3003);  
  Empmass->GetXaxis()->SetTitle("Mass GeV");
  Empmass->GetYaxis()->SetTitle("Dimuon Count");
  c1.Update();
  c1.SaveAs("empRealcut.png");
  c1.SetLogy();
  Empmass->GetYaxis()->SetTitle("Log(Dimuon Count)");
  Empmass->SetMaximum(150000);
  Empmass->SetMinimum(1);
  c1.Update();
  c1.SaveAs("LogempmassRealcut.png");

  EmpxT->Draw("hist");
  exT->Draw("Csame");
  TLegend *legendex = new TLegend(.72,.75,.87,.85);
  legendex->AddEntry("EmpxT","Uncut Data"); 
  legendex->AddEntry("exT","Cut Data");
  legendex->Draw();
  exT->SetFillStyle(1001);
  exT->SetFillColor(4);
  EmpxT->SetFillColor(2);
  EmpxT->SetFillStyle(3003);  
  EmpxT->GetXaxis()->SetTitle("Target Bjorken-x");
  EmpxT->GetYaxis()->SetTitle("Dimuon Count");
  c1.Update();
  c1.SaveAs("empxTRealcut.png");
  c1.SetLogy();
  EmpxT->GetYaxis()->SetTitle("Log(Dimuon Count)");
  EmpxT->SetMaximum(4000000);
  EmpxT->SetMinimum(1);
  c1.Update();
  c1.SaveAs("LogempxTRealcut.png");


  jmixmass->Draw("hist");
  jmass->Draw("histsame");
  TLegend *legendjm = new TLegend(.72,.75,.87,.85);
  legendjm->AddEntry("jfullmass","Uncut Data"); 
  legendjm->AddEntry("jmass","Cut Data");
  legendjm->Draw();
  jmass->SetFillStyle(1001);
  jmass->SetFillColor(4);
  jmixmass->SetFillColor(2);
  jmixmass->SetFillStyle(3003);  
  jmixmass->GetXaxis()->SetTitle("Mass GeV");
  jmixmass->GetYaxis()->SetTitle("Dimuon Count");
  TH1D* tempmixmass= (TH1D*) jmixmass->Clone();
  TH1D* tempmass= (TH1D*) jmass->Clone();
  tempmass->Draw("E1same");
  tempmass->SetMarkerColor(4);
  tempmass->SetMarkerStyle(21);
  tempmass->SetMarkerSize(.5);
  tempmixmass->Draw("E1same");
  tempmixmass->SetMarkerColor(2);
  tempmixmass->SetMarkerStyle(21);
  tempmixmass->SetMarkerSize(.5);
  c1.Update();
  c1.SaveAs("jRealcut.png");
  c1.SetLogy();
  jmixmass->GetYaxis()->SetTitle("Log(Dimuon Count)");
  jmixmass->SetMaximum(1000);
  jmixmass->SetMinimum(1);
  c1.Update();
  c1.SaveAs("LogjmassRealcut.png");

  jmixxT->Draw("hist");
  jxT->Draw("histsame");
  TLegend *legendjx = new TLegend(.72,.75,.87,.85);
  legendjx->AddEntry("jfullxT","Uncut Data"); 
  legendjx->AddEntry("jxT","Cut Data");
  legendjx->Draw();
  jxT->SetFillStyle(1001);
  jxT->SetFillColor(4);
  jmixxT->SetFillColor(2);
  jmixxT->SetFillStyle(3003);  
  jmixxT->GetXaxis()->SetTitle("Target Bjorken-x");
  jmixxT->GetYaxis()->SetTitle("Dimuon Count");
  TH1D* tempmixxT= (TH1D*) jmixxT->Clone();
  TH1D* tempxT= (TH1D*) jxT->Clone();
  tempxT->Draw("E1same");
  tempxT->SetMarkerColor(4);
  tempxT->SetMarkerStyle(21);
  tempxT->SetMarkerSize(.5);
  tempmixxT->Draw("E1same");
  tempmixxT->SetMarkerColor(2);
  tempmixxT->SetMarkerStyle(21);
  tempmixxT->SetMarkerSize(.5);
  c1.Update();
  c1.SaveAs("jxTRealcut.png");
  c1.SetLogy();
  jmixxT->GetYaxis()->SetTitle("Log(Dimuon Count)");
  jmixxT->SetMaximum(3000);
  jmixxT->SetMinimum(1);
  c1.Update();
  c1.SaveAs("LogjxTRealcut.png");

 }