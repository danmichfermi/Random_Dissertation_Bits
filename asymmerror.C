//Code adapted to use MC technique to correct ratio distribution

double functionxy(double x, double y){
  return x/(y);}

double trapintegral(TH1F* hist, int lower, int upper){
  double normintegral=hist->Integral(lower, upper);
  double corr=normintegral-(hist->GetBinContent(lower)*.5);
  corr=corr-(hist->GetBinContent(upper)*.5);
  corr=corr/(hist->Integral()); 
  return corr;
}

double trapintegralAdd(TH1F* hist, int currentbin, int diff, double norm){
  double corr= (hist->GetBinContent(currentbin-diff)*.5);
  corr=corr+(hist->GetBinContent(currentbin)*.5);
  corr=corr/norm; 
  return corr;
}

int newsigma(double LD2mean, double LD2sigma, double LH2mean,
        double LH2sigma, double *lowsigmafinal, double *highsigmafinal, double * truemean, bool meanMode=true){
int faultCount=0;
//Variable initiation
double center=0;
const int points=50000;
int trials=100;
//int binCount=points/trials;
int binCount=1000;
double lowsigma=0; double highsigma=0;
//loop for trials
for(int a=0; a<trials ; a++){
//Variable initiation
  double mean, mode, stdev, rms, skewness;
  double lowpercent, highpercent;
  double lowpercentcor, highpercentcor;
  double x[points];
  double y[points];
  double norm=0;
  int percentileFraction=points/200; //99.5%
  if(!(percentileFraction>0)){ percentileFraction=1;}
  const int percentileNum=percentileFraction;
  TRandom *eventGenerator1 = new TRandom3(0);
  TRandom *eventGenerator2 = new TRandom3(0);
//max and min to use as histogram bounds 
  float maxxy=0;  int maxxylocation=0; 
  float minxy=5;  int minxylocation=0; 
  double overallskewness=0;
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
 // if(maxxy>10) maxxy=10;
  if(minxy<0) minxy=0;
  TH1F* hist= new TH1F("hist","Ratio PDF",binCount,minxy,maxxy);//.5,2.5);
  for(int i=0; i<points; i++){ 
    //non-general statement
    //physical cut
    if((x[i]>=0)&&(y[i]>0)){
      norm+=1.0;
      hist->Fill(functionxy(x[i],y[i]));//function(y[i]));
    }
  }
  //building histogram to derive properties
  hist->Draw();
  skewness=hist->GetSkewness();
  rms=hist->GetRMS();
  stdev=hist->GetStdDev();
  mean=hist->GetMean();
  //Finding important or key bin locations
  int maxbin=hist->GetMaximumBin();
  mode=hist->GetBinCenter(maxbin);
  int meanbin=hist->FindBin(mean);

  double expected=functionxy(LD2mean,LH2mean);
  double expecerror=expected*sqrt(LD2sigma*LD2sigma/(LD2mean*LD2mean)+(LH2sigma*LH2sigma)/(LH2mean*LH2mean));
  double corfirstint=.34134;
  int medianbin=hist->FindBin(expected);
  int lowmeanbin=hist->FindBin(expected-expecerror);
  int highmeanbin=hist->FindBin(expected+expecerror);
  int centerbin;
  double centerVal;
  if(meanMode){ centerbin=meanbin; centerVal=mean; }
  else{ centerbin=medianbin; centerVal=expected; }
  center+=centerVal;
  lowpercentcor=trapintegral(hist, lowmeanbin,centerbin);
  highpercentcor=trapintegral(hist,centerbin,highmeanbin);
  //computation of error multiplier
  int truehighbin=0;
  int truelowbin=0;
  //these bools implement a check against the other range against OoB values
  //they are mostly for completion sake, but I think can be removed?
  bool highgo=false;
  bool lowgo=false;
  bool bothlow=false;
  bool bothhigh=false;
  while(highpercentcor<corfirstint){
    truehighbin++;
    highgo=false;
    if((highmeanbin+truehighbin)>binCount){ 
      highgo=true; bothhigh=true; 
      truehighbin=0; highpercentcor=corfirstint-.001;
      break;  
    }
    highpercentcor+=trapintegralAdd(hist,highmeanbin+truehighbin,1, norm);
  }
  while(lowpercentcor>corfirstint){
    truelowbin++;
    lowgo=false;
    if((lowmeanbin+truelowbin)>centerbin){ 
      lowgo= true;  bothlow=true;  
      truelowbin=0; lowpercentcor=corfirstint-.001; //this is where it breaks down for me?
      break;
    }
    lowpercentcor+=trapintegralAdd(hist,lowmeanbin+truelowbin,1, norm);
  }
  while(highpercentcor>corfirstint){
    truehighbin--;
    bothhigh=false;
    highgo=false;
    if((highmeanbin+truehighbin)<centerbin){ break; bothhigh=true; }
    highpercentcor=trapintegral(hist,centerbin,highmeanbin+truehighbin);
  }
  while(lowpercentcor<corfirstint){
    truelowbin--;
    bothlow=false;
    lowgo=false;
    if((lowmeanbin+truelowbin)<0){ break; bothlow=true;  }
    lowpercentcor=trapintegral(hist, lowmeanbin+truelowbin, centerbin);
  }
  //value wasn't found in range caveat. this shouldn't generally be possible. 
  if(bothlow){
    truelowbin=0;
    faultCount++;
    //cout<<"Warning: out of range or NaN detected. Returning propagated value."<<endl;
  }
  if(bothhigh){
    faultCount++;
    //cout<<"Warning: out of range or NaN detected. Returning propagated value."<<endl;
  }
  lowsigma+=(double) (centerbin-lowmeanbin-truelowbin)*(maxxy-minxy)/binCount;
  highsigma+=(double) (truehighbin+highmeanbin-centerbin)*(maxxy-minxy)/binCount;
  hist->Delete();
 }
 lowsigma=lowsigma/((float) trials);
 highsigma=highsigma/((float) trials);
 center=center/((float) trials);
 *truemean=center;
 *lowsigmafinal=lowsigma;
 *highsigmafinal=highsigma;
 return faultCount;
}
