
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
void getNames(vector <TString> &inVec, TString mlNum, TString cutVer, TString tar)
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

void getHighNames(vector <TString> &inVec, TString mlNum, TString cutVer, TString tar)
{
  inVec.resize(4);
  TString mon = "";
  if (tar == "LD2")
  {
    if (mlNum == "200")
    {
      mon = "2111vsigma_xTabove5_single_eval_wfileSwitch_Inh2_00000230_200_00105230_200_00205231_200_00300230_200_00405231_200_00505231_200_.root";
    }
    if (mlNum == "300")
    {
      mon = "2111vsigma_xTabove5_single_eval_wfileSwitch_Inh2_00001210_300_00107210_300_00201210_300_00301210_300_00407210_300_00500210_300_.root";
    }
  }

  if (tar == "LH2")
  {
    if (mlNum == "200")
    {
      mon = "2111vsigma_xTabove5_single_eval_wfileSwitch_Inh2_00000230_200_00105230_200_00205231_200_00300230_200_00405231_200_00505231_200_.root";
    }
    if (mlNum == "300")
    {
      mon = "2111vsigma_xTabove5_single_eval_wfileSwitch_Inh2_00001210_300_00107210_300_00201210_300_00301210_300_00407210_300_00500210_300_.root";
    }
  }


  inVec[0] = "./high/mu_67_LD2_" + mon;
  inVec[1] = "./high/mu_67_EMP_" + mon;
  inVec[2] = "./high/mu_67_LH2_" + mon;
  inVec[3] = inVec[1];
}

void getNames5(vector <TString> &inVec, TString mlNum, TString cutVer, TString tar)
{
  inVec.resize(4);
  TString mon = "";
  if (tar == "LD2")
  {
    if (mlNum == "200")
    {
      mon = "2111v2sigma_nomassxTInh2_noTransMom_single_eval_wfileSwitch_Inh2_00000230_200_00105230_200_00205231_200_00300230_200_00405231_200_00505231_200_.root";
    }
    if (mlNum == "300")
    {
      mon = "2111v2sigma_nomassxTInh2_noTransMom_single_eval_wfileSwitch_Inh2_00001210_300_00107210_300_00201210_300_00301210_300_00407210_300_00500210_300_.root";
    }
  }

  if (tar == "LH2")
  {
    if (mlNum == "200")
    {
      mon = "2111v2sigma_nomassxTInh2_noTransMom_single_eval_wfileSwitch_Inh2_00000230_200_00105230_200_00205231_200_00300230_200_00405231_200_00505231_200_.root";
    }
    if (mlNum == "300")
    {
      mon = "2111v2sigma_nomassxTInh2_noTransMom_single_eval_wfileSwitch_Inh2_00001210_300_00107210_300_00201210_300_00301210_300_00407210_300_00500210_300_.root";
    }
  }


  inVec[0] = "mu_78_LD2_" + mon;
  inVec[1] = "mu_78_EMP_" + mon;
  inVec[2] = "mu_78_LH2_" + mon;
  inVec[3] = inVec[1];
}

//Sets the new MECs for the ML cuts these are based on refitted values 
//used to maximize DY over JMIX after 4.2 Gev
void setMEC(TString &cutStr, TString inStr, TString tar)
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


/*void cutmaking(){
  int mlCutInt = 0;
  TString cutVer, mlNum, tar;
  if (mlCutInt == 0){cutVer = "sigma"; mlNum = "300"; tar = "LH2";}
  if (mlCutInt == 1){cutVer = "sigma"; mlNum = "200"; tar = "LD2";}
  if (mlCutInt == 2){cutVer = "sigma"; mlNum = "300"; tar = "LH2";}
  
  TString fnames[8];
  char* tnames[8];
  TString loopCuts;

  //Rootfile and textfile output names
  loopCuts += "(mass>4.2)&&(mass<8.5)";
  //loopCuts += xfBin(xbin);
  if (cutVer == "42")
  {
    loopCuts += "&&(xF>-0.1)&&(xF<0.95)";
    loopCuts += "&&(fabs(costh)<0.5)";
    loopCuts += "&&(D1<400)&&(D2<400)&&(D3<400)";
    loopCuts += "&&(D1+D2+D3<1000)";
  }
  if (cutVer == "sigma")
  {
    setMEC(loopCuts, mlNum, tar);
    loopCuts += mlCut(mlCutInt);
  }
  cout<<loopCuts<<endl;
  vector <TString> nameVec;
  getNames(nameVec, mlNum, cutVer, tar);
  for (int i = 0; i < 4; ++i)
  {
    cout<<nameVec[i]<<endl;
  }
}*/

