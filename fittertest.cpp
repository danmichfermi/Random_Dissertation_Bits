//from Marshall
#include <vector>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include "math.h"
#include "TChain.h"
#include "TFile.h"
#include "TEventList.h"
#include "TEntryList.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TF1.h"
#include "TVirtualFitter.h"
#include "TGraphErrors.h"
#include "TStyle.h"
using namespace std;

TString cutVer, mlNum, tar;
int mlCutInt = -1;

//Daniel Stuff
Double_t func(Double_t *val, Double_t *par);
double functionxy(double x, double y);
double trapintegral(TH1F* hist, double lower, double upper);
void newsigma(double LD2mean, double LD2sigma, double LH2mean,
        double LH2sigma, double *lowsigmafinal, double *highsigmafinal);

//Specifies the ML cut
TString mlCut(int inint)
{
  TString res;
  if (inint == 0)//lh2 300, same as 2111v42 +2% DY
  {
    res += "&&(pValRes_MESS >- 0.200002)";
    res += "&&(pValRes_JMIX > 0.009996)";
  }
  if (inint == 1)//ld2 200, 7% less dy, 20% less jmix
  {
    res += "&&(pValRes_MESS > -0.3)";
    res += "&&(pValRes_JMIX > -0.0300115)";
  }
  if (inint == 2)//circle, lh2 300, 12% more DY 160% more Jmix 
  {
    res += "&&(pValRes_JMIX<0.672891 + pow(fabs(-1.048425 - pow(pValRes_MESS + 0.824814, 2.0)), 0.5))";
    res += "&&(pValRes_JMIX>0.672891 - pow(fabs(-1.048425 - pow(pValRes_MESS + 0.824814, 2.0)), 0.5))";
  }
  return res;
} 

//Gets the names of the ML root files//
void getNames(vector <TString> &inVec, TString mlNum, TString cutVer)
{
  inVec.resize(4);
  TString mon = "";
  if (tar == "LD2")
  {
    if (mlNum == "100")
    {
      mon = "26v2_2111v2sigma_nomassxTInh2_Nim_Embed_single_eval_wfileSwitch_Inh2_00009230_100_00105231_100_00205231_100_00300230_100_00400230_100_00500230_100_.root";
    }
    if (mlNum == "200")
    {
      mon = "26v2_2111v2sigma_nomassxTInh2_Nim_Embed_single_eval_wfileSwitch_Inh2_00000230_200_00105230_200_00205231_200_00300230_200_00405231_200_00505231_200_.root";
    }
    if (mlNum == "300")
    {
      mon = "26v2_2111v2sigma_nomassxTInh2_Nim_Embed_single_eval_wfileSwitch_Inh2_00007234_300_00109231_300_00208230_300_00301230_300_00408230_300_00500230_300_.root";
    }
  }

  if (tar == "LH2")
  {
    if (mlNum == "100")
    {
      mon = "26v2_2111v2sigma_nomassxTInh2_Nim_Embed_single_eval_wfileSwitch_Inh2_00007210_100_00105210_100_00205211_100_00300210_100_00405211_100_00505211_100_.root";
    }
    if (mlNum == "200")
    {
      mon = "26v2_2111v2sigma_nomassxTInh2_Nim_Embed_single_eval_wfileSwitch_Inh2_00000210_200_00105211_200_00206211_200_00305211_200_00405211_200_00505211_200_.root";
    }
    if (mlNum == "300")
    {
      mon = "26v2_2111v2sigma_nomassxTInh2_Nim_Embed_single_eval_wfileSwitch_Inh2_00001210_300_00107210_300_00201210_300_00301210_300_00407210_300_00500210_300_.root";
    }
  }
  mon.ReplaceAll("Nim_Embed_single_", "");
  mon.ReplaceAll("26v2_", "");

  inVec[0] = "mu_57to70_LD2_" + mon;
  inVec[1] = "mu_57to70_EMP_" + mon;
  inVec[2] = "mu_57to70_LH2_" + mon;
  //inVec[1].ReplaceAll("wfileSwitch_", "");
  inVec[3] = inVec[1];
}

//Sets the new MECs for the ML cuts these are based on refitted values 
//used to maximize DY over JMIX after 4.2 Gev
void setMEC(TString &cutStr, TString inStr)
{
  vector <float> inFloatVec(4);
  if (tar == "LD2")
  {
    if (inStr == "100")
    {
      inFloatVec = {-0.145355,-0.576505,-0.054725, -0.054401};
    }
    if (inStr == "200")
    {
      //inFloatVec = {-0.3, -0.0399,-0.0300115,-0.00999601};//true
      //inFloatVec = {-1.0, -1.0,-0.0300115,-0.00999601};//mod
      inFloatVec = {-1.0, -0.0399,-1.0,-0.00999601};//mod
    }
    if (inStr == "300")
    {
      inFloatVec = {-0.155544, -0.109111,-0.119932,-0.093989};
    }
  }

  if (tar == "LH2")
  {
    if (inStr == "100")
    {
      inFloatVec = {-0.199993,-0.076412,-0.037624, 0.017763};
    }
    if (inStr == "200")
    {
      inFloatVec = {-0.199901, -0.0488515,-0.049955,-0.109643};
    }
    if (inStr == "300")
    {
      //inFloatVec = {-0.200002, -0.415518,0.009996,0.040175};//true
      //inFloatVec = {-0.3, -0.4,0.009996,0.040175};//mod
      //inFloatVec = {-1.0, -1.0,0.009996,0.040175};//mod for crazycut
      inFloatVec = {-1.0, -0.415518,-1.0,0.040175};//mod
    }
  }
  cutStr += " && (pValRes_MESS>pValMEC_MESS)";
  cutStr += " && (pValRes_PSIBACK>pValMEC_PSIBACK)";
  cutStr += " && (pValRes_JMIX>pValMEC_JMIX)";
  cutStr += " && (pValRes_DUMP>pValMEC_DUMP)";
  cutStr.ReplaceAll("pValMEC_MESS", Form("%f", inFloatVec[0]));
  cutStr.ReplaceAll("pValMEC_PSIBACK", Form("%f", inFloatVec[1]));
  cutStr.ReplaceAll("pValMEC_JMIX", Form("%f", inFloatVec[2]));
  cutStr.ReplaceAll("pValMEC_DUMP", Form("%f", inFloatVec[3]));
}


//Returns the trigger intensity using RF00, PotPerQie, and QieSum
//Potperqie is the same as g2sem/qiesum (pedestal corrected)
//Pedestal for mrs67 = 32.6
float trigIntensity(float inR00, float inPot, float inQie)
{
  //float res = (inR00-32.6)*(G2SEM)/(inQie-369000.*588.*32.6);
  float res = (inR00 - 32.6)*inPot;
  return res;
}

//Calculates the chisq and ndf for a holder given the it function
//This ignores bins with zero error
void calChi(TH1F * inHist, TF1 * infun, double &chi, double &ndf)
{
  ndf = 0.0, chi = 0.0;
  int tBinNum = inHist -> GetNbinsX();
  for (int i = 0; i < tBinNum; i++)
  {
    double bCon = inHist -> GetBinContent(i+1);
    double fnval = infun -> Eval(bCon);
    double chiLoop = pow(bCon - fnval, 2.0);
    double err = inHist -> GetBinError(i+1);

    if (err > 0){chiLoop = chiLoop/err;}
    if (fabs(bCon) > 0){ndf++;}
    chi += chiLoop;
    cout << bCon << " " << fnval << " " << err << endl;
  }
  ndf -= 2.0;
}

//A bool for setting the correct xTbins, returns true if the event is within the
//designated bin and false otherwise
bool xtBin(float inFloat, int binInt)
{
  double xtLow, xtHig;
  if (binInt == 0)     {xtLow = 0.100, xtHig = 0.130;}
  else if (binInt == 1){xtLow = 0.130, xtHig = 0.160;}
  else if (binInt == 2){xtLow = 0.160, xtHig = 0.195;}
  else if (binInt == 3){xtLow = 0.195, xtHig = 0.240;}
  else if (binInt == 4){xtLow = 0.240, xtHig = 0.290;}
  else if (binInt == 5){xtLow = 0.290, xtHig = 0.350;}
  else if (binInt == 6){xtLow = 0.350, xtHig = 0.450;}
  else if (binInt == 7){xtLow = 0.450, xtHig = 0.580;}
  
  bool res = 0;
  if ((xtLow < inFloat) && (inFloat <= xtHig)){res = 1;}
  return res;
}

//xT Bin cut
TString xtBin(int binInt)
{
  double xtLow, xtHig;
  if (binInt == 0)     {xtLow = 0.100, xtHig = 0.130;}
  else if (binInt == 1){xtLow = 0.130, xtHig = 0.160;}
  else if (binInt == 2){xtLow = 0.160, xtHig = 0.195;}
  else if (binInt == 3){xtLow = 0.195, xtHig = 0.240;}
  else if (binInt == 4){xtLow = 0.240, xtHig = 0.290;}
  else if (binInt == 5){xtLow = 0.290, xtHig = 0.350;}
  else if (binInt == 6){xtLow = 0.350, xtHig = 0.450;}
  else if (binInt == 7){xtLow = 0.450, xtHig = 0.580;}
  
  TString res = Form("&&(xT > %f)&&(xT < %f)", xtLow, xtHig);
  return res;
}

//same as above but for xF
bool xfBin(float inFloat, int binInt)
{
  double xfLow, xfHig;
  if (binInt == 0){xfLow = 0, xfHig = 0.350;}
  else if (binInt == 1){xfLow = 0.350, xfHig = 0.5;}
  else if (binInt == 2){xfLow = 0.5, xfHig = 0.65;}
  else if (binInt == 3){xfLow = 0.65, xfHig = 0.75;}
  else if (binInt == 4){xfLow = 0.75, xfHig = 10;}
  
  bool res = 0;
  if ((xfLow < inFloat) && (inFloat <= xfHig)){res = 1;}
  return res;
}

//xF bin cut
TString xfBin(int binInt)
{
  double xfLow, xfHig;
  if (binInt == 0)     {xfLow = 0.000, xfHig = 0.350;}
  else if (binInt == 1){xfLow = 0.350, xfHig = 0.500;}
  else if (binInt == 2){xfLow = 0.500, xfHig = 0.650;}
  else if (binInt == 3){xfLow = 0.650, xfHig = 0.750;}
  else if (binInt == 4){xfLow = 0.750, xfHig = 1.000;}
  
  TString res = Form("&&(xF > %f)&&(xF < %f)", xfLow, xfHig);
  return res;
}

//Master function that runs the above sub functions
void takeRatios(int xbin, TFile &f, ofstream &outs1)
{
  //gStyle -> SetOptStat(0);
  gStyle -> SetOptFit();
  double Gh, Gd, Gratio;//Ratio   
  double LD2mean, LD2sigma, LH2mean, LH2sigma, lowSigmaFinal, highSigmaFinal;
  /* THIS IS THE IMPORTANT STUFF
  G = (1-exp(-L*rho/lambda))*lambda/A
  L is flask length (50.8cm)
  rho = density, rho_ld2 = 0.1634 g/moL, rho_lh2 = 0.0708 g/moL
  A = atomic mass, A_ld2 = 2.00140 g/moL, A_lh2 = 1.00794 g/moL
  lambda = nuclear intraction length, lambda_ld2 = 71.8g/cm^2, lambda_lh2 = 52.0 g/cm^2
  */
  Gh = (1.0 - TMath::Exp(-50.8*0.0708/52.0))*52.0/1.00794;
  Gd = (1.0 - TMath::Exp(-50.8*0.1634/71.8))*71.8/2.00140;
  Gratio = (Gh/Gd);
  Gratio = 0.8827;//Corrected for contamination assuming that the data is uniform
                  //along the spills. So based upon liveproton count 53.25% of LD2 is
                  //contaminated with LH2 with a the true percentage of LD2 being 91.2%
                  //Used effective stats and Gratio is the sum of the precentage corrected terms 
                  //The difference is slight, like 0.2 %
  //Gratio is updated to the value devised by Grassman docdb# 4993 
  Gratio = (3.597*2.014)/(7.90*1.008);
  
  int binNum = 8;
  if (xbin == 7){binNum = 5;}
  
  //TString var = "mass", min = "4.2", max = "8.5", cut = "";//3.9
  //TString var = "xT", min = ".15", max = ".6", cut = "";
  //TString var = "D1", min = "0", max = "400", cut = "";
  //TString var = "Intensity_p", min = "0", max = "60000", cut = "";

  TString var = "(RFp00-32.6)*(G2SEM)/(QIEsum-369000*588*32.6)", 
    min = "50.", max = "60000", cut = "";
  min = "0.0";
  max = "64000";

  vector <TString> fname;
bool cutdata=false;
if(cutdata){
  if (cutVer == "42")
  {
    fname.push_back("mu_67_LD2_2111v32.root");
    fname.push_back("mu_67_EMP_2111v32.root");
    fname.push_back("mu_67_LH2_2111v32.root");
    fname.push_back("mu_67_EMP_2111v32.root");
  }
  if (cutVer == "42")
  {
    fname.push_back("mu_67_LD2_2111v42_nomassInh_both.root");
    fname.push_back("mu_67_EMP_2111v42_nomassInh_both.root");
    fname.push_back("mu_67_LH2_2111v42_nomassInh_both.root");
    fname.push_back("mu_67_EMP_2111v42_nomassInh_both.root");
  }
  if (cutVer == "sigma")
  {
    fname.push_back("mu_67_LD2_2111v32.root");
    fname.push_back("mu_67_EMP_2111v32.root");
    fname.push_back("mu_67_LH2_2111v32.root");
    fname.push_back("mu_67_EMP_2111v32.root");  }
}

else{
    fname.push_back("filterCorrD.root");
    fname.push_back("FilteredEMPData.root");
    fname.push_back("filterCorrH.root");
    fname.push_back("FilteredEMPData.root");
}
  //Initialize Histograms
  TH1F *histOne[4];
  TH1F *histTwo[4];
  TH1F *histThr[4];
  
  
  /*
  //variable bin size
  Double_t edges[9] = {50,7178.3,12117,17241,22355,28308,35773,48003,60000};
  for (int i = 0; i < 4; i++)
  {
    TString hname = Form("h1_%d", i);
    histOne[i] = new TH1F(hname,"", binNum, edges);
    hname.ReplaceAll("h1", "h2");
    histTwo[i] = new TH1F(hname,"", binNum, edges);
    hname.ReplaceAll("h2", "h3");
    histThr[i] = new TH1F(hname,"", binNum, edges);
    histOne[i] -> Sumw2();
    histTwo[i] -> Sumw2();
    histThr[i] -> Sumw2();
  }
  //*/
  
  //old way
  
  for (int i = 0; i < 4; i++)
  {
    TString hname = Form("h1_%d", i);
    histOne[i] = new TH1F(hname,"", binNum, min.Atof(), max.Atof());
    hname.ReplaceAll("h1", "h2");
    histTwo[i] = new TH1F(hname,"", binNum, min.Atof(), max.Atof());
    hname.ReplaceAll("h2", "h3");
    histThr[i] = new TH1F(hname,"", binNum, min.Atof(), max.Atof());
    //Note the TH1::EBinErrorOpt(#) works for ROOT6 for ROOT5 the # will suffice
    histOne[i]->SetBinErrorOption(TH1::EBinErrorOpt(1));
    histTwo[i]->SetBinErrorOption(TH1::EBinErrorOpt(1));
    histThr[i]->SetBinErrorOption(TH1::EBinErrorOpt(1));
  
    histOne[i] -> Sumw2();
    histTwo[i] -> Sumw2();
    histThr[i] -> Sumw2();
  //*/
  }
  //*/

  //Here a series of cuts are written based upon the ML or 2111v42
  //By default the ML does not have any occupancy cuts

  TString loopCuts;
  loopCuts += "(mass>4.2)&&(mass<8.5)";
  loopCuts += xtBin(xbin);
  //loopCuts += xfBin(xbin);
  if (cutVer == "42")
  {
    loopCuts += "&&(xF>-0.1)&&(xF<0.95)";
    loopCuts += "&&(fabs(costh)<0.5)";
    loopCuts += "&&(D1<400)&&(D2<400)&&(D3<400)";
    loopCuts += "&&(D1+D2+D3<1000)";
  }
  if ( (cutVer == "sigma") && ( cutdata ) )
  {
    setMEC(loopCuts, mlNum);
    loopCuts += mlCut(mlCutInt);
  }

  //Calculation Loop
  //Tfiles are opened, branch addresses are set, and data entered
  //A TEntrylist of the events that pass the cuts is generated and used in the
  //loop. This streamlines the work and greatly simplifies the code. 
  for (int i = 0; i < fname.size(); i++)
  {
    auto tf = new TFile(fname[i]);
    auto tt = (TTree *)tf -> Get("background");

    float loopVar, rf001, potp1, qiesum1, chamberIntensity;

    if (!var.Contains("RFp00"))
    {
      tt -> SetBranchAddress(var, &loopVar); 
    }

    if (var.Contains("RFp00"))
    {
      tt -> SetBranchAddress("RFp00", &rf001);
      tt -> SetBranchAddress("PotPerQie", &potp1);
      tt -> SetBranchAddress("QIEsum", &qiesum1);
    }

    //Entries for all the files
    tt -> Draw(">>lList", loopCuts, "entrylist");
    TEntryList *eventList = (TEntryList *)gDirectory -> Get("lList");
    int entryNum = eventList -> GetN();    
    //int entryNum = tt -> GetEntriesFast();
    
    //Loops going over all the entries for all of the files
    //Events passing relevant cuts are filled in the histograms
    for (int j = 0; j < entryNum; j++) {
      TEntryList *tl = (TEntryList *)gDirectory -> Get("lList");
      int entry = tl -> GetEntry(j);
      tt -> GetEntry(entry);
      if (!var.Contains("RFp00"))
      {
        histOne[i] -> Fill(loopVar, loopVar);
        histTwo[i] -> Fill(loopVar);
      }

      if (var.Contains("RFp00"))
      {
        //this is the statement that (probably) needs to be changed
        float tIval = trigIntensity(rf001, potp1, qiesum1);
        //float tIval = chamberIntensity;
        //float tIval = oc[0]+oc[1]+oc[2];
        histOne[i] -> Fill(tIval, tIval);
        histTwo[i] -> Fill(tIval);
      }
      gDirectory -> Delete("loopEventList");
      tl = NULL;
    }//for enum
    eventList = NULL;
    tf -> Close("R");
    tt = NULL;
    tf -> Delete();
  }//for fname.size()

  //Histogram Average, Arun style
  //histThr has the averages, while histTwo still has the correct data
  histThr[0] -> Add(histOne[0], histOne[2], 1., 1.);
  histThr[1] -> Add(histTwo[0], histTwo[2], 1., 1.);
  histThr[2] -> Add(histThr[0], histOne[1], 1., 2.);
  histThr[3] -> Add(histThr[1], histTwo[1], 1., 2.);

  histThr[0] -> Divide(histThr[1]);
  histThr[2] -> Divide(histThr[3]);

  //Scales the histograms by 1/liveproton_target
  //Then histogram subtraction
  //Then histogram division
  //Then rescaling by GRatio
  //Then rescaling by 0.5
  double ld2_sc = 7.70196e+16, lh2_sc = 1.61288e+17, emp_sc = 3.66565e+16;
  
  histTwo[0] -> Scale(1./ld2_sc);
  histTwo[1] -> Scale(1./emp_sc);
  histTwo[2] -> Scale(1./lh2_sc);
  histTwo[3] -> Scale(1./emp_sc);
  
  histTwo[0] -> Add(histTwo[1], -1.);
  histTwo[2] -> Add(histTwo[3], -1.);
  
  histTwo[0] -> Divide(histTwo[2]);
  histTwo[0] -> Scale(Gratio);
  histTwo[0] -> Scale(0.5);
  
  TString qname = var;
  if (var.Contains("RFp00")){qname = "TriggerIntensity";}
  TString tname;
  
  //xT
  if (xbin == 0) {
    tname = Form("LD2/LH2 : %s xT 0.100-0.130", qname.Data());
  }
  else if (xbin == 1) {
    tname = Form("LD2/LH2 : %s xT 0.130-0.160", qname.Data());
  }
  else if (xbin == 2) {
    tname = Form("LD2/LH2 : %s xT 0.160-0.195", qname.Data());
  }
  else if (xbin == 3) {
    tname = Form("LD2/LH2 : %s xT 0.195-0.240", qname.Data());
  }
  else if (xbin == 4) {
    tname = Form("LD2/LH2 : %s xT 0.240-0.290", qname.Data());
  }
  else if (xbin == 5) {
    tname = Form("LD2/LH2 : %s xT 0.290-0.350", qname.Data());
  }
  else if (xbin == 6) {
    tname = Form("LD2/LH2 : %s xT 0.350-0.450", qname.Data());
  }
  else if (xbin == 7) {
    tname = Form("LD2/LH2 : %s xT 0.450-0.580", qname.Data());
  }
  
  //xF
  /*
  if (xbin == 0) {
    tname = Form("LD2/LH2 : %s xT < 0.35", qname.Data());
  }
  else if (xbin == 1) {
    tname = Form("LD2/LH2 : %s xT 0.35-0.5", qname.Data());
  }
  else if (xbin == 2) {
    tname = Form("LD2/LH2 : %s xT 0.5-0.65", qname.Data());
  }
  else if (xbin == 3) {
    tname = Form("LD2/LH2 : %s xT 0.65-0.75", qname.Data());
  }
  else if (xbin == 4) {
    tname = Form("LD2/LH2 : %s xT > 0.75", qname.Data());
  }
  */
  
  
  TString sname = Form("csr_images/res_plot_liquid_arun_evan_42_%s_%s_%d_%s_xT%d.png",
     tar.Data(), mlNum.Data(), mlCutInt, qname.Data(), xbin);
  if (cutVer == "42"){sname.ReplaceAll("42", "sigma");}
  
  bool hierarchy = false;
  
  TGraphAsymmErrors *tge = new TGraphAsymmErrors(histTwo[0]);
  tge -> SetTitle(tname);
  double xVal, yVal, xErr, yErr, yErr2;
  for (int i = 0; i < binNum; i++) {
    xVal = histThr[0]->GetBinContent(i+1);
    xErr = sqrt(histThr[1]->GetBinContent(i+1));
    yVal = histTwo[0]->GetBinContent(i+1);
    
    //TH1F asym = histTwo[0]->GetAsymmetry(histThr[1]);
    
    LD2mean = yVal;
    LD2sigma = histTwo[0]->GetBinError(i+1);
    LH2mean = histTwo[2]->GetBinContent(i+1);
    LH2sigma = histTwo[0]->GetBinError(i+1);
    
    
    newsigma(LD2mean, LD2sigma, LH2mean, LH2sigma, &lowSigmaFinal, &highSigmaFinal);
    
    
    //need to get asymmetric errors
    
    yErr = histTwo[0]->GetBinError(i+1);//old implementation for tgerrors
    yErr = 2*histTwo[0]->GetBinErrorUp(i+1);
    yErr2 = histTwo[0]->GetBinErrorLow(i+1);
    //cout << yErr << ' ' << yErr2 << '\n';
    //*/
    
    //last bin correction
    if (yVal <= 0) {
      yVal = 0;
      yErr2 = 0;
    }
    
    tge->SetPoint(i, xVal, yVal);
    tge->SetPointError(i, xErr, xErr, lowSigmaFinal*yErr, highSigmaFinal*yErr2);
  }
  
  
  bool logLikelihood = false;
  TCanvas *c = new TCanvas("c","c");
  if (!logLikelihood) {
    tge -> SetLineColor(kBlue);
    tge -> SetMarkerStyle(kFullCircle);
    tge -> GetYaxis() -> SetRange(0.,5.);
    tge->GetXaxis()->SetTitle("Trigger Intensity");
    tge->GetYaxis()->SetTitle("Cross Section Ratio");
    tge -> Draw("ap");
  }
  else {
    //histTwo[0]-> SetLineColor(kBlue);
    histTwo[0]-> SetMarkerStyle(kFullCircle);
    //histTwo[0]-> GetYaxis() -> SetRange(0.,5.);
    histTwo[0]->GetXaxis()->SetTitle("Total Occupancy");
    histTwo[0]->GetYaxis()->SetTitle("Cross Section Ratio");
    histTwo[0] -> Draw();
  }
  
  TString polX = "pol2";
  
  TF1 *tfun1 = new TF1("tfun", polX, min.Atof(), max.Atof());
  
  //tfun1->SetParameter(1,xbin);
  //tfun1->SetParLimits(1,double(xbin)+0.25,double(xbin)+0.5);
  
  
  if (!logLikelihood) {
    tge->Fit(tfun1);
  }
  else {
    histTwo[0]->Fit(tfun1,"L");
  }
  
  //TLegend *tLeng = new TLegend(.80, 1., 1., .85, "Cuts");
  //tLeng -> SetTextSize(0.03);
  tfun1->SetLineColor(kRed);
  
  //tLeng -> AddEntry(histOne[0], "2111v32", "l");
  //tLeng -> AddEntry(tge, "2111v32", "l");
  tfun1 -> Draw("same");
  //tLeng -> AddEntry(tfun1, "Fit", "l");
  //tLeng -> Draw("same");
  
  if (!hierarchy) {
    c -> SaveAs(sname);
    delete c;
  }
  //tLeng -> Delete();
  double h[3];
  h[0]  = tfun1 -> GetParameter(0);
  h[1]  = tfun1 -> GetParameter(1);
  //for common slope:
  //h[1] = -4.04e-6;
  if (polX == "pol1") {
    h[2]  = tfun1 -> GetChisquare()/tfun1 -> GetNDF();
  }
  else {
    h[2] = tfun1->GetParameter(2);
  }
  
  double binWidth = (max.Atof() - min.Atof()) / binNum;
  double currentX = (binWidth / 2) + min.Atof();
  
  //Stats readouts
  //need to change k whenever changing between linear and quadratic fits
  if (!hierarchy) {
    int k = 0;
    if (polX == "pol1") {
      int k = 2;
    }
    else {
      int k = 3;
    }
    double L = 0;
    double v_i;
    double d_i;
    
    //calculating L
    for (int i = 1; i < binNum + 1; i++) {
      d_i = histTwo[0]->GetBinContent(i);
      if (polX == "pol1") {
        v_i = h[0] + currentX*h[1];
      }
      else {
        v_i = h[0] + currentX*h[1] + currentX*currentX*h[2];
      }
      L += v_i - d_i + (d_i*log(d_i/v_i));
      currentX += binWidth;
    }
    L *= 2;//this is now technically -2ln(L)
    cout << "-2ln(L): " << L << '\n';
    
    cout << "AIC: " << L + 2*k << '\n';
    cout << "BIC: " << log(binNum)*k + L << '\n';
    
    //resetting this value
    currentX = (binWidth / 2) + min.Atof(); 
  }
  
  
  
  //hierarchical fitting
  if (hierarchy) {
    for (int i = 1; i < binNum + 1; i++) {
      histTwo[0]->SetBinContent(i, histTwo[0]->GetBinContent(i) - h[0]
             - currentX * h[1]);
      currentX += binWidth;
    }
    
    //second fitting
    TGraphErrors *tge2 = new TGraphErrors(histTwo[0]);
    tge2 -> SetTitle(tname);
    for (int i = 0; i < binNum; i++) {
    double xVal, yVal, xErr, yErr;
    xVal = histThr[0] -> GetBinContent(i+1);
    xErr = sqrt(histThr[1] -> GetBinContent(i+1));
    yVal = histTwo[0] -> GetBinContent(i+1);
    yErr = histTwo[0] -> GetBinError(i+1);
    tge2->SetPoint(i, xVal, yVal);
    tge2->SetPointError(i, xErr, yErr);
    }
    tge2->SetLineColor(kBlue);
    tge2->SetMarkerStyle(kFullCircle);
    tge2->GetYaxis()->SetRange(0.,5.);
    tge2->GetXaxis()->SetTitle("Total Occupancy");
    tge2->GetYaxis()->SetTitle("Cross Section Ratio");
    tge2 -> Draw("ap");
    
    TF1 *tfun2 = new TF1("tfun2", "pol2", min.Atof(), max.Atof());
    //log likelihood(?)
    tge2->Fit(tfun2);
    
    //TLegend *tLeng = new TLegend(.80, 1., 1., .85, "Cuts");
    //tLeng -> SetTextSize(0.03);
    tfun2->SetLineColor(kRed);
    
    //tLeng -> AddEntry(histOne[0], "2111v32", "l");
    //tLeng -> AddEntry(tge, "2111v32", "l");
    tfun2 -> Draw("same");
    //tLeng -> AddEntry(tfun1, "Fit", "l");
    //tLeng -> Draw("same");
    
    TString hname = Form("csr_images/res_plot_liquid_arun_evan_42_%s_%s_%d_%s_xT%d_hierarchy.png",
     tar.Data(), mlNum.Data(), mlCutInt, qname.Data(), xbin);
    if (cutVer == "42"){hname.ReplaceAll("42", "sigma");}
    
    c -> SaveAs(hname);
    delete c;
    cout << "\n\n\n\n";
  }//if hierarchy
  
  
  
  TString ostr = Form("f1 %f %f %f \n", h[0], h[1], h[2]);
  printf("%s",ostr.Data());
  
  
  //saving histograms
  //histTwo[0]->SetDirectory(f);
  //histTwo[0]->Write();
  histTwo[0]->SetTitle("Test");
  f.Write();
  for (int i = 0; i < 8; i++) {
    outs1 << histTwo[0]->GetBinContent(i+1) << " \n";
  }
  
  double a1, a2;
  //calChi(histTwo[0], tfun1, a1, a2);
  //printf("Chisq(%f) NDF(%f) RedChi(%f)\n", a1, a2, a1/a2);
  
  for (int i = 0; i < 4; i++) {
    histOne[i] -> Clear();
    histTwo[i] -> Clear();
    histThr[i] -> Clear();
    histOne[i] = NULL;
    histTwo[i] = NULL;
    histThr[i] = NULL;
  }
  
  gDirectory ->Delete("h1_*");
  gDirectory ->Delete("h2_*");
  gDirectory ->Delete("h3_*");
  
  tfun1 -> Delete();
  tge   -> Delete();
}//takeRatios

int plot_ratio_liq_arun_evan() {

  //Machine learning cut version declaration 
  mlCutInt = 1;
  if (mlCutInt == 0){cutVer = "sigma"; mlNum = "300"; tar = "LH2";}
  if (mlCutInt == 1){cutVer = "sigma"; mlNum = "200"; tar = "LD2";}
  if (mlCutInt == 2){cutVer = "sigma"; mlNum = "300"; tar = "LH2";}
  
  TString fnames[8];
  char* tnames[8];

  //Rootfile and textfile output names
  for (int i = 0; i < 8; i++)
  {
    fnames[i] = Form("hist%d_mlcut%d_0.root", i, mlCutInt);
    tnames[i] = Form("int%d_mlcut%d.txt", i, mlCutInt);
  }
  //*/
  /*
  tnames[0] = "occ0.txt";
  tnames[1] = "occ1.txt";
  tnames[2] = "occ2.txt";
  tnames[3] = "occ3.txt";
  tnames[4] = "occ4.txt";
  tnames[5] = "occ5.txt";
  tnames[6] = "occ6.txt";
  tnames[7] = "occ7.txt";
  //*/
  /*
  tnames[0] = "d0.txt";
  tnames[1] = "d1.txt";
  tnames[2] = "d2.txt";
  tnames[3] = "d3.txt";
  tnames[4] = "d4.txt";
  tnames[5] = "d5.txt";
  tnames[6] = "d6.txt";
  tnames[7] = "d7.txt";
  //*/
  
  TStopwatch *st = new TStopwatch();
  //number of bins, < 8 for xT, < 5 for xF
  for (int i = 0; i < 8; i++)
  {
    TFile f(fnames[i], "RECREATE");
    ofstream outs1;
    outs1.open(tnames[i]);
    takeRatios(i, f, outs1);
    outs1.close();
  }
  st -> Print();
  st -> Delete();
  return 0;
}

/*
Double_t func(Double_t *val, Double_t *par) {
  Float_t x = val[0];
  double xt[8] = {0.119,0.146,0.179,0.218,0.262,0.315,0.385,0.483};
  Double_t f = par[0] + (-3.24e-6 - 5.529e-6*xt[par[1]])*x;
  return f;
}
//*/

Double_t func(Double_t *val, Double_t *par) {
  Float_t x = val[0];
  double xt[8] = {0.119,0.146,0.179,0.218,0.262,0.315,0.385,0.483};
  Double_t f = par[0] + (2.5837e-6)*x - ((1.238e-10)*x*x);
  return f;
}

//Daniel's code
double functionxy(double x, double y){
  return x/y;}

double trapintegral(TH1F* hist, double lower, double upper){
  double normintegral=hist->Integral(lower, upper);
  double corr=normintegral-(hist->GetBinContent(lower)*.5);
  corr=corr-(hist->GetBinContent(upper)*.5);
  corr=corr/(hist->Integral()); 
  return corr;
}

void newsigma(double LD2mean, double LD2sigma, double LH2mean,
        double LH2sigma, double *lowsigmafinal, double *highsigmafinal){
//Variable initiation
const int points=500000;
int trials=10;
double lowsigma=0; double highsigma=0;
//loop for trials
for(int a=0; a<trials ; a++){
//Variable initiation
  double mean;
  double mode;
  double stdev;
  double rms;
  double skewness;
  double lowpercent;
  double highpercent;
  double lowpercentcor;
  double highpercentcor;
  double x[points];
  double y[points];
  int percentileFraction=points/200; //99.5%
  if(!(percentileFraction>0)){ percentileFraction=1;}
  const int percentileNum=percentileFraction;
  TRandom *eventGenerator1 = new TRandom3(0);
  TRandom *eventGenerator2 = new TRandom3(0);
//max and min to use as histogram bounds 
  float maxxy=0;  int maxxylocation=0;
  float minxy=5;  int minxylocation=0;

//Gaussian MC
  for(int i=0; i<points ;i++){
    //fill array values
    x[i]=eventGenerator1->Gaus(LD2mean,LD2sigma);
    y[i]=eventGenerator2->Gaus(LH2mean, LH2sigma);    
    //find  max and min values
    if(functionxy(x[i],y[i])>maxxy){
      maxxy=functionxy(x[i],y[i]); maxxylocation=i;
    }
    if(functionxy(x[i],y[i])<minxy){
      minxy=functionxy(x[i],y[i]); minxylocation=i;
    }
  }
//hard coded cap for gas cases
  if(maxxy>6) maxxy=6;
  TH1F* hist= new TH1F("hist","Ratio PDF",1000,minxy,maxxy);//.5,2.5);
  for(int i=0; i<points; i++){ 
    if((x[i]>0)&&(y[i]>0)){
      hist->Fill(functionxy(x[i],y[i]));//function(y[i]));
    }
  }
  hist->Draw();
  skewness=hist->GetSkewness();
  rms=hist->GetRMS();
  stdev=hist->GetStdDev();
  mean=hist->GetMean();
  int maxbin=hist->GetMaximumBin();
  mode=hist->GetBinCenter(maxbin);
  int norm=hist->Integral();
  int meanbin=hist->FindBin(mean);
  double expected=LD2mean/LH2mean;
  double expecerror=expected*sqrt(LD2sigma*LD2sigma/(LD2mean*LD2mean)+LH2sigma*LH2sigma/(LH2mean*LH2mean));
  double corfirstint=.34134;
  int centerbin=hist->FindBin(expected);
  int lowmeanbin=hist->FindBin(expected-expecerror);
  int highmeanbin=hist->FindBin(expected+expecerror);

  lowpercent=(hist->Integral(lowmeanbin,centerbin))/norm;
  highpercent=(hist->Integral(centerbin,highmeanbin))/norm;
  lowpercentcor=trapintegral(hist, lowmeanbin,centerbin);
  highpercentcor=trapintegral(hist,centerbin,highmeanbin);
  //lowpercent=(lowpercent-)/corfirstint;
  //highpercent=(highpercent-)/corfirstint;
  double margin=1;
  double upperstnddif=1;
  double upminpoint=1;
  double lowstnddif=1;
  double lowminpoint=1;


  //computation of error multiplier
  if(highpercent<.34034){
    for(int i=0; i<10; i++){
      for(int j=0; j<10; j++){
        double testbound= expected+
    (double)expecerror*(1+(double)i*0.1+j*0.01);
        
        //if(testbound>upperbound) break;
        int testboundbin=hist->FindBin(testbound);
        double loopint=trapintegral(hist, centerbin, testboundbin)-
    corfirstint;
        //cout<<loopint<<"\t"<<upminpoint;
        if(loopint*loopint<upperstnddif){
    upperstnddif=loopint*loopint;
    upminpoint=1+(double)i*0.1+j*0.01;
        }
      }
    }
  }
  if(lowpercent>.34234){
    for(int i=0; i<10; i++){
      for(int j=0; j<10; j++){
        double testbound= expected-expecerror*(1.0-(0.1*i+j*0.01));
        //if(testbound<lowerbound) break;
        int testboundbin=hist->FindBin(testbound);
        double loopint=trapintegral(hist,testboundbin, centerbin)-
    corfirstint;
        if(loopint*loopint<lowstnddif){
    lowstnddif=loopint*loopint; lowminpoint=1-0.1*i-j*0.01;
        }
      }
    }
  }
  if(highpercent>.34234){
    for(int i=0; i<10; i++){
      for(int j=0; j<10; j++){
        double testbound=expected+(double)expecerror*(1.0-(0.1*i+j*0.01));
        //cout<<testbound<<"\t"<<upperbound<<endl;
        //if(testbound>upperbound) break;
        int testboundbin=hist->FindBin(testbound);
        double loopint=trapintegral(hist, centerbin, testboundbin)-
    corfirstint;
        if(loopint*loopint<upperstnddif){
    upperstnddif=loopint*loopint; upminpoint=1.0-0.1*i-j*0.01;
        }
      }
    }
  }
  if(lowpercent<.34034){
    for(int i=0; i<10; i++){
      for(int j=0; j<10; j++){
        double testbound= expected-expecerror*(1.0+(0.1*i+j*0.01));
        //if(testbound<lowerbound) break;
        int testboundbin=hist->FindBin(testbound);
        double loopint=trapintegral(hist,testboundbin,centerbin)-
    corfirstint;
        if(loopint*loopint<lowstnddif){
    lowstnddif=loopint*loopint; lowminpoint=1+0.1*i+j*0.01;
        }
      }
    }
  }
  lowsigma+=lowminpoint; highsigma+=upminpoint;
  hist->Delete();
 }
 lowsigma=lowsigma/((float) trials);
 highsigma=highsigma/((float) trials);
 //*skewd=skewness;
 *lowsigmafinal= lowsigma;
 *highsigmafinal=highsigma;
 
}
