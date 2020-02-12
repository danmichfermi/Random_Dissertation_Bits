#include "styleset.C"
#include <ROOT/RDataFrame.hxx>

void buildgraph(){
	styleset();
	string jtree="mc_jpsi_LD2_M026_S002_messy";
	string ptree="mc_psiprime_LD2_M026_S002_messy";
	string fileName="MonteCarloTrees.root";
	ROOT::RDataFrame jpsi(jtree, fileName, {"mass"});
	ROOT::RDataFrame psip(ptree, fileName, {"mass"});

	auto jlow=jpsi.Filter("mass<4.2");
	auto plow=psip.Filter("mass<4.2");
	auto jhigh=jpsi.Filter("mass>4.2");
	auto phigh=psip.Filter("mass>4.2");

	auto jpass=jhigh.Count();
	auto jfail=jlow.Count();
	auto ppass=phigh.Count();
	auto pfail=plow.Count();
	double jtotal=(*jpass+*jfail);
	double ptotal=(*ppass+*pfail);
	double jpassfraction= (double) *jpass/((*jpass+*jfail));
	double ppassfraction= (double) *ppass/(*ppass+*pfail);
	cout<<jpassfraction<<"\t"<<*jpass<<"\t"<<jtotal<<endl;
	cout<<ppassfraction<<"\t"<<*ppass<<"\t"<<ptotal<<endl;

	
	auto hjlow=jlow.Histo1D({"hjlow","",80,2,6},"mass");
	auto hplow=plow.Histo1D({"hplow","",80,2,6});
	auto hjhigh=jhigh.Histo1D({"hjhigh","",80,2,6});
	auto hphigh=phigh.Histo1D({"hphigh","",80,2,6});


	hjlow->Scale(1/jtotal);
	hplow->Scale(1/ptotal);
	hjhigh->Scale(1/jtotal);
	hphigh->Scale(1/ptotal);

	hjlow->SetMarkerColor(4);
	hplow->SetMarkerColor(2);
	hjhigh->SetMarkerColor(4);
	hphigh->SetMarkerColor(2);
	hjlow->SetMarkerStyle(4);
	hplow->SetMarkerStyle(2);
	hjhigh->SetMarkerStyle(4);
	hphigh->SetMarkerStyle(2);

	hjlow->SetLineColor(4);
	hplow->SetLineColor(2);
	hjhigh->SetLineColor(4);
	hphigh->SetLineColor(2);


	hjlow->GetXaxis()->SetTitle("Tracked Mass in GeV");
	hjlow->GetYaxis()->SetTitle("Normalized Count");

	TCanvas c1;
	hjlow->Draw("hist");

	/*hplow->Draw("CP");
	hjhigh->Draw("CP");
	hphigh->Draw("CP");*/
	hplow->Draw("same hist");
	hjhigh->Draw("same hist");
	hphigh->Draw("same hist");

	hjlow->SetFillColor(4);
	hplow->SetFillColor(2);
	hjhigh->SetFillColor(37);
	hphigh->SetFillColor(46);

	hjlow->SetFillStyle(3003);
	hplow->SetFillStyle(3003);
	hjhigh->SetFillStyle(3004);
	hphigh->SetFillStyle(3005);


	//TLatex* ltx1 = new TLatex(); 
	//TLatex* ltx2 = new TLatex(); 

    TLegend *legend = new TLegend(.72,.75,.87,.85);
    legend->AddEntry("hjlow","J/#Psi");
    legend->AddEntry("hplow","#Psi'");
    legend->Draw();

	c1.Update();
	c1.SaveAs("jpmasscutnoml.png");
	c1.SetLogy();
	hjlow->GetYaxis()->SetTitle("Normalized Count-Log Scale");
	c1.Update();
	c1.SaveAs("jpmasscutnomlLog.png");
}
