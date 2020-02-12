#include "styleset.C"
#include <ROOT/RDataFrame.hxx>

//LH=1
//Empty=2
//LD=3
//none=4

void buildgraph(){

	styleset();


	string lttree="LH2";
	string dtree="Roadset67";
	string fileName="FilteredHData.root";
	string EfileName="FilteredEMPData.root";

	string fileName2="Realfulldata.root";
	ROOT::RDataFrame targets(lttree, fileName, {"dz"});
	ROOT::RDataFrame dump(dtree, fileName2, {"dz"});
	ROOT::RDataFrame Empt("EMP", EfileName, {"dz"});

	//auto targets=target.Define("zavg", [](float z01, float z02){ return (z01+z02)/2;},{"z01","z02"});
	//auto dumps=dump.Define("zavg", [](float z01, float z02){ return (z01+z02)/2;},{"z01","z02"});

	auto dumps=dump.Filter("targetPos==1");

	auto xtarg=targets.Filter("xT>.15");
	auto xdump=dumps.Filter("xT>.15");
	auto hightarg=xtarg.Filter("mass>4.2");
			//.Filter("(z01-z02<100)&&(z02-z01<100)");
	auto highdump = xdump.Filter("mass>4.2");

	auto notarg = dump.Filter("(targetPos==2)&&(xT>.15)");
	auto notargx = notarg.Filter("mass>4.2");
	auto checks = Empt.Filter("(xT>.15)");

	auto hdump = dumps.Histo1D({"hdump","",50,-350,250},"dz");
	auto htarg = targets.Histo1D({"htarg","",50,-350,250},"dz");
	auto hno = notarg.Histo1D({"hno","",50,-350,250},"dz");
	auto hnox = notargx.Histo1D({"hnox","",50,-350,250},"dz");
	auto histhighdump = highdump.Histo1D({"hdumpx","",50,-350,250},"dz");
	auto histhighhtarg = hightarg.Histo1D({"htargx","",50,-350,250},"dz");
	auto empthist = checks.Histo1D({"hempt","",50,-350,250},"dz");

	TH1D *targetRegion = new TH1D("targetRegion","",1,-154.95,-104.14);
	TH1D *dumpRegion = new TH1D("dumpRegion","",2, 0, 500);

	for(int i=0; i<1000000 ; i++){ targetRegion->Fill(-125); }
	for(int i=0; i<1000000 ; i++){ dumpRegion->Fill(150);}




	//hjlow->Scale(1/jtotal);
	//hplow->Scale(1/ptotal);
	//hjhigh->Scale(1/jtotal);
	//hphigh->Scale(1/ptotal);

	TCanvas c1;
	hdump->Draw("CP");
	htarg->Draw("same");

	hdump->SetFillStyle(3001);
	htarg->SetFillStyle(3001);
	hdump->SetFillColor(4);
	htarg->SetFillColor(2);
	hno->SetFillColor(3);
	hno->SetFillStyle(4011);
	empthist->SetFillColor(6);
	empthist->SetFillStyle(3011);	
	hdump->GetXaxis()->SetTitle("Position on Beam (Z) Axis cm.");
	hdump->GetYaxis()->SetTitle("Dimuon Count");
	hno->Draw("same");
	empthist->Draw("Same");
	c1.Update();
	c1.SaveAs("Realsep.png");
	c1.SetLogy();
	c1.SetGridx();
	c1.SetGridy();
	hdump->GetYaxis()->SetTitle("Log(Dimuon Count)");
	c1.Update();
	c1.SaveAs("LogRealsep.png");
	c1.Update();
	c1.SaveAs("notarg.png");

	histhighdump->Draw();
	targetRegion->Draw("same");
	dumpRegion->Draw("same");
	histhighhtarg->Draw("same");
	//histhighdump->SetFillStyle(3001);
	histhighhtarg->SetFillStyle(3001);
	histhighdump->SetFillColor(0);
	histhighhtarg->SetFillColor(4);
	hnox->SetFillColor(3);
	hnox->SetFillStyle(3244);
	dumpRegion->SetFillStyle(3003);
	dumpRegion->SetFillColor(18);
	targetRegion->SetFillStyle(3002);
	targetRegion->SetFillColor(18);
	targetRegion->SetLineColor(0);
	dumpRegion->SetLineColor(0);	
	empthist->SetFillColor(2);
	empthist->SetFillStyle(1001);	
	histhighdump->GetXaxis()->SetTitle("Position on Beam (Z) Axis cm.");
	histhighdump->GetYaxis()->SetTitle("Dimuon Count");
	//Normalized
	hnox->Draw("same");	
	empthist->Draw("Same");
	TLegend *legend = new TLegend(.15,.75,.35,.85);
    legend->AddEntry("hdumpx","High Mass Uncut Data");
    legend->AddEntry("htargx","Cut Data");
    legend->AddEntry("hnox"," Empty Target Uncut Data");
    legend->AddEntry("hempt","Empty Target Cut Data");
    legend->Draw();
	c1.Update();
	c1.SaveAs("cutRealsep.png");
	c1.SetLogy();
	histhighdump->GetYaxis()->SetTitle("Log(Dimuon Count)");
	c1.Update();
	c1.SaveAs("logcutRealsep.png");
	c1.Update();
	c1.SaveAs("notargx.png");

}
