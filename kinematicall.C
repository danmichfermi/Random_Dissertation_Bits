/*All Kinematic plots 

reesponse curves?

costh -.585,.585
phi -.318,.318
xF -.15,.9
xB - .3, .95 (6)



intensity_p- trigger: 0,90k; 0,64k (actual)

*/


//considerations- do we want Empty Data?
#include <ROOT/RDataFrame.hxx>
#include "styleset.C"
void plotcut(){

  styleset();
   gStyle->SetErrorX(0.);    

  ROOT::RDataFrame H2cut("LH2","filterCorrH.root");
  ROOT::RDataFrame D2cut("LD2","filterCorrD.root");

  auto H2cutxB= H2cut.Histo1D({"H2cutxB","",52,.3, .95},"xB");
  auto D2cutxB= D2cut.Histo1D({"D2cutxB","",52,.3, .95},"xB");
  auto H2cutxF= H2cut.Histo1D({"H2cutxF","",42,-.15,.9},"xF");
  auto D2cutxF= D2cut.Histo1D({"D2cutxF","",42,-.15,.9},"xF");

  auto H2costh= H2cut.Histo1D({"H2cutxB","",26,-.585,.585},"costh");
  auto D2costh= D2cut.Histo1D({"D2cutxB","",26,-.585,.585},"costh");
  auto H2phi= H2cut.Histo1D({"H2cutxF","",24,-.318,.318},"phi");
  auto D2phi= D2cut.Histo1D({"D2cutxF","",24,-.318,.318},"phi");

  auto H2intens= H2cut.Histo1D({"H2cutintens","",24,1,64000},"Intensity_p");
  auto D2intens= D2cut.Histo1D({"D2cutintens","",24,1,64000},"Intensity_p");

  H2cutxB->Scale(1/H2cutxB->Integral());
  D2cutxB->Scale(1/D2cutxB->Integral());
  H2cutxF->Scale(1/H2cutxF->Integral());
  D2cutxF->Scale(1/D2cutxF->Integral());

  H2costh->Scale(1/H2costh->Integral());
  D2costh->Scale(1/D2costh->Integral());
  H2phi->Scale(1/H2phi->Integral());
  D2phi->Scale(1/D2phi->Integral());

  H2intens->Scale(1/H2intens->Integral());
  D2intens->Scale(1/D2intens->Integral());


  TCanvas c1;

  TH1D* temps= (TH1D*) H2cutxB->Clone();
  H2cutxB->Draw("hist");
  H2cutxB->SetFillColor(4);
  H2cutxB->SetFillStyle(3003);  
  H2cutxB->GetXaxis()->SetTitle("x_{1}");
  H2cutxB->GetYaxis()->SetTitle("Normalized Dimuon Count");
  c1.SetGridx();
  c1.SetGridy();
  H2cutxB->SetMarkerColor(12);
	H2cutxB->SetMarkerStyle(2);
	temps->Draw("E1same");
  temps->SetMarkerColor(4);
  temps->SetMarkerStyle(21);
  temps->SetMarkerSize(.5);
  c1.Update();
  c1.SaveAs("H2cutxB.png");
  c1.SetLogy();
  H2cutxB->GetYaxis()->SetTitle("Log(Normalized Dimuon Count)"); 

  c1.Update();
  c1.SaveAs("LogH2cutxB.png");
  c1.SetLogy(0);
 
  TH1D* temp= (TH1D*) D2cutxB->Clone();
  D2cutxB->Draw("hist");
  D2cutxB->SetFillColor(2);
  D2cutxB->SetFillStyle(3003); 
  D2cutxB->GetXaxis()->SetTitle("x_{1}");
  D2cutxB->GetYaxis()->SetTitle("Normalized Dimuon Count");
  c1.SetGridx();
  c1.SetGridy();
  temp->Draw("E1same");
  temp->SetMarkerColor(4);
  temp->SetMarkerStyle(21);
  temp->SetMarkerSize(.5);
  c1.Update();
  c1.SaveAs("D2cutxB.png");
  c1.SetLogy();
  D2cutxB->GetYaxis()->SetTitle("Log(Normalized Dimuon Count)"); 
  D2cutxB->SetMaximum(350000);
  D2cutxB->SetMinimum(1);
  c1.Update();
  c1.SaveAs("LogD2cutxB.png");
  c1.SetLogy(0);

  TH1D* temp2= (TH1D*)H2cutxF->Clone();
  H2cutxF->Draw("hist");
  H2cutxF->SetFillColor(4);
  H2cutxF->SetFillStyle(3003);  
  H2cutxF->GetXaxis()->SetTitle("x_{F}");
  H2cutxF->GetYaxis()->SetTitle("Normalized Dimuon Count");
  c1.SetGridx();
  c1.SetGridy();
  temp2->Draw("E1same");
  temp2->SetMarkerColor(4);
  temp2->SetMarkerStyle(21);
  temp2->SetMarkerSize(.5);
  c1.Update();
  c1.SaveAs("H2cutxF.png");
  c1.SetLogy();
  H2cutxF->GetYaxis()->SetTitle("Log(Normalized Dimuon Count)"); 
  //H2cutxF->SetMaximum(350000);
  //H2cutxF->SetMinimum(1);
  c1.Update();
  c1.SaveAs("LogH2cutxF.png");
  c1.SetLogy(0);

 
  TH1D* temp3= (TH1D*)D2cutxF->Clone();
  D2cutxF->Draw("hist");
  D2cutxF->SetFillColor(2);
  D2cutxF->SetFillStyle(3003);  
  D2cutxF->GetXaxis()->SetTitle("x_{F}");
  D2cutxF->GetYaxis()->SetTitle("Normalized Dimuon Count");
  c1.SetGridx();
  c1.SetGridy();
  	D2cutxF->SetMarkerColor(4);
	D2cutxF->SetMarkerStyle(2);
	temp3->Draw("E1same");
  temp3->SetMarkerColor(4);
  temp3->SetMarkerStyle(21);
  temp3->SetMarkerSize(.5);
  c1.Update();
  c1.SaveAs("D2cutxF.png");
  c1.SetLogy();
  D2cutxF->GetYaxis()->SetTitle("Log(Normalized Dimuon Count)"); 
  //D2cutxF->SetMaximum(350000);
  //D2cutxF->SetMinimum(1);
  c1.Update();
  c1.SaveAs("LogD2cutxF.png");
    c1.SetLogy(0);



//Theta and Phi
  TH1D* temp4= (TH1D*)H2costh->Clone();
  H2costh->Draw("hist");
  H2costh->SetFillColor(4);
  H2costh->SetFillStyle(3003);  
  H2costh->GetXaxis()->SetTitle("cos(#Theta)");
  H2costh->GetYaxis()->SetTitle("Normalized Dimuon Count");
  c1.SetGridx();
  c1.SetGridy();
  temp4->Draw("E1same");
  temp4->SetMarkerColor(4);
  temp4->SetMarkerStyle(21);
  temp4->SetMarkerSize(.5);
    H2costh->SetMaximum(.08);
  H2costh->SetMinimum(.00001);
  c1.Update();
  c1.SaveAs("H2costh.png");
  c1.SetLogy();
  H2costh->GetYaxis()->SetTitle("Log(Normalized Dimuon Count)"); 

  c1.Update();
  c1.SaveAs("LogH2costh.png");
    c1.SetLogy(0);


  TH1D* temp5= (TH1D*) D2costh->Clone();
  D2costh->Draw("hist");
  D2costh->SetFillColor(2);
  D2costh->SetFillStyle(3003);  
  D2costh->GetXaxis()->SetTitle("cos(#Theta)");
  D2costh->GetYaxis()->SetTitle("Normalized Dimuon Count");
  c1.SetGridx();
  c1.SetGridy();
    	D2costh->SetMarkerColor(12);
	D2costh->SetMarkerStyle(2);
  temp5->Draw("E1same");
  temp5->SetMarkerColor(4);
  temp5->SetMarkerStyle(21);
  temp5->SetMarkerSize(.5);
  c1.Update();
  c1.SaveAs("D2costh.png");
  c1.SetLogy();
  D2costh->GetYaxis()->SetTitle("Log(Normalized Dimuon Count)"); 
  D2costh->SetMaximum(.055);
  D2costh->SetMinimum(0000.1);
  c1.Update();
  c1.SaveAs("LogD2costh.png");
    c1.SetLogy(0);


  TH1D* temp6= (TH1D*) H2phi->Clone();
  H2phi->Draw("hist");
  H2phi->SetFillColor(4);
  H2phi->SetFillStyle(3003);  
  H2phi->GetXaxis()->SetTitle("#Phi in Radians");
  H2phi->GetYaxis()->SetTitle("Normalized Dimuon Count");
  c1.SetGridx();
  c1.SetGridy();
    H2phi->SetMaximum(0.05);
  H2phi->SetMinimum(0);
  temp6->Draw("E1same");
  temp6->SetMarkerColor(4);
  temp6->SetMarkerStyle(21);
  temp6->SetMarkerSize(.5);
  c1.Update();
  c1.SaveAs("H2phi.png");
  c1.SetLogy();
  H2phi->GetYaxis()->SetTitle("Log(Normalized Dimuon Count)"); 

  c1.Update();
  c1.SaveAs("LogH2phi.png");
    c1.SetLogy(0);


  TH1D* temp7= (TH1D*) D2phi->Clone();
  D2phi->Draw("hist");
  D2phi->SetFillColor(2);
  D2phi->SetFillStyle(3003);  
  D2phi->GetXaxis()->SetTitle("#Phi in Radians");
  D2phi->GetYaxis()->SetTitle("Normalized Dimuon Count");
  c1.SetGridx();
  c1.SetGridy();
    D2phi->SetMaximum(.055);
  D2phi->SetMinimum(0);
    temp7->Draw("E1same");
  temp7->SetMarkerColor(4);
  temp7->SetMarkerStyle(21);
  temp7->SetMarkerSize(.5);
  c1.Update();
  c1.SaveAs("D2phi.png");
  c1.SetLogy();
  D2phi->GetYaxis()->SetTitle("Log(Normalized Dimuon Count)"); 
  c1.Update();
  c1.SaveAs("LogD2phi.png");
    c1.SetLogy(0);
    




//Intensity
  TH1D* temp8= (TH1D*) H2intens->Clone();
  H2intens->Draw("hist");
  H2intens->SetFillColor(4);
  H2intens->SetFillStyle(3003);  
  H2intens->GetXaxis()->SetTitle("Trigger Intensity");
  H2intens->GetYaxis()->SetTitle("Normalized Dimuon Count");
  c1.SetGridx();
  c1.SetGridy();
  temp8->Draw("E1same");
  temp8->SetMarkerColor(4);
  temp8->SetMarkerStyle(21);
  temp8->SetMarkerSize(.5);
  c1.Update();
  c1.SaveAs("H2intens.png");
  c1.SetLogy();
  H2intens->GetYaxis()->SetTitle("Log(Normalized Dimuon Count)"); 
  //H2intens->SetMaximum(350000);
  //H2intens->SetMinimum(1);
  c1.Update();
  c1.SaveAs("LogH2intens.png");
    c1.SetLogy(0);


  TH1D* temp9= (TH1D*) D2intens->Clone();
  D2intens->Draw("hist");
  D2intens->SetFillColor(2);
  D2intens->SetFillStyle(3003);  
  D2intens->GetXaxis()->SetTitle("Trigger Intensity");
  D2intens->GetYaxis()->SetTitle("Normalized Dimuon Count");
  c1.SetGridx();
  c1.SetGridy();
  temp9->Draw("E1same");
  temp9->SetMarkerColor(4);
  temp9->SetMarkerStyle(21);
  temp9->SetMarkerSize(.5);
  c1.Update();
  c1.SaveAs("D2intens.png");
  c1.SetLogy();
  D2intens->GetYaxis()->SetTitle("Log(Normalized Dimuon Count)"); 
  //D2intens->SetMaximum(350000);
  //D2intens->SetMinimum(1);
  c1.Update();
  c1.SaveAs("LogD2intens.png");
    c1.SetLogy(0);

}