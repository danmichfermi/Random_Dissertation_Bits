//special test run

#include "asymmerror.C"

void convert(){

	double x,y, staterr, syserr;
	ofstream outfilemean;
    outfilemean.open("asymm_mean.csv");
	ofstream outfilemed;
    outfilemed.open("asymm_med.csv");

double   LD2mean[]={
3627383.237,
60029505.59,
44001055.56,
17436732.37,
5809868.881, //5
1593417.274,
370905.7916,
73386.10308,
8761.813386,
401.8178794,
30.65017285 //11
    };

  double  LH2mean[]={
1764758.736,
27724103.55,
20188590.78,
7956502.625,
2547429.443, //5
732053.697, 
152796.2357,
37137.88925,
3538.75,
253,
15 //11
    };

double LD2sigma[]={
113117.6355,
442413.2628,
329360.0097,
159867.6654,
65797.98931,
26303.26899,
7759.559794,
2914.813791,
438.5168196,
65.51791115,
7.392080705
};


double LH2sigma[]={
79182.46166,
655505.3761,
481165.4437,
232574.3044,
98285.11187,
37888.51698,
11766.88125,
4076.096345,
671.7467901,
98.41493789,
10.95445115
};

for (int i = 0; i < 11; ++i)
{
	double lowsigmafinal; double highsigmafinal; double truemean; int faults;
	faults=newsigma(LD2mean[i], LD2sigma[i], LH2mean[i], LH2sigma[i],  &lowsigmafinal,  &highsigmafinal,   &truemean, true);
	outfilemean<<truemean<<","<<lowsigmafinal<<","<<highsigmafinal<<",0,0"<<endl;
    if(faults)cout<<"Warning: out of range or NaN detected. Returning propagated value. Bin:"<<i<<"Count:"<<faults<<endl;
	faults=newsigma(LD2mean[i], LD2sigma[i], LH2mean[i], LH2sigma[i],  &lowsigmafinal,  &highsigmafinal,   &truemean, false);
    if(faults)cout<<"Warning: out of range or NaN detected. Returning propagated value. Bin:"<<i<<"Count:"<<faults<<endl;
	outfilemed<<truemean<<","<<lowsigmafinal<<","<<highsigmafinal<<",0,0"<<endl;
}




}
