#include "styleset.C"
#include <ROOT/RDataFrame.hxx>

void buildgraph(){
	styleset();
	string lttree="mc_drellyan_LD2_M026_S002_messy";
	string dtree="mc_drellyan_dump_M026_S002_messy";
	string fileName="MonteCarloTrees.root";
	ROOT::RDataFrame target(lttree, fileName, {"dz"});
	ROOT::RDataFrame dump(dtree, fileName, {"dz"});




	auto xtarg=targets.Filter("xT>.15");
	auto xdump=dumps.Filter("xT>.15");
	auto hightarg=xtarg.Filter("mass>4.2");
			//.Filter("(z01-z02<100)&&(z02-z01<100)");
	auto highdump=xdump.Filter("mass>4.2");


	auto hdump=dumps.Histo1D({"hdump","",50,-350,250},"dz");
	auto htarg=targets.Histo1D({"htarg","",50,-350,250},"dz");
	auto histhighdump=highdump.Histo1D({"hdump","",50,-350,250},"dz");
	auto histhighhtarg=hightarg.Histo1D({"htarg","",50,-350,250},"dz");


	//hjlow->Scale(1/jtotal);
	//hplow->Scale(1/ptotal);
	//hjhigh->Scale(1/jtotal);
	//hphigh->Scale(1/ptotal);
	TCanvas c1;
	hdump->Draw();
	htarg->Draw("same");

	hdump->SetFillStyle(3001);
	htarg->SetFillStyle(3001);
	hdump->SetFillColor(4);
	htarg->SetFillColor(2);
	hdump->GetXaxis()->SetTitle("Position on Beam (Z) Axis cm.");
	hdump->GetYaxis()->SetTitle("Dimuon Count");
	c1.Update();
	c1.SaveAs("MCsep.png");
	c1.SetLogy();
	hdump->GetYaxis()->SetTitle("Log(Dimuon Count)");
	c1.Update();
	c1.SaveAs("LogMCsep.png");

	histhighdump->Draw();
	histhighhtarg->Draw("same");
	histhighdump->SetFillStyle(3001);
	histhighhtarg->SetFillStyle(3001);
	histhighdump->SetFillColor(4);
	histhighhtarg->SetFillColor(2);
	histhighdump->GetXaxis()->SetTitle("Position on Beam (Z) Axis cm.");
	histhighdump->GetYaxis()->SetTitle("Dimuon Count");
	//Normalized
	c1.Update();
	c1.SaveAs("cutMCsep.png");
	c1.SetLogy();
	histhighdump->GetYaxis()->SetTitle("Log(Dimuon Count)");
	c1.Update();
	c1.SaveAs("logcutMCsep.png");

}
