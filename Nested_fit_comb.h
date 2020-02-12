//library for special nested fit regression, returning combined values using information theory/relative likelihoods
//Add fit info individually to get get results and print out
//Daniel Morton

struct FitPoint{
	double value;
	double error;
	double Loglikelihood=0;
	double ChiSq;
	int ndf;
};

class CombinedFit{
	private:
		vector <FitPoint> CombinedGraphsVal;
		vector <double> AIC;
		vector <double> weights;
		double AIC_min, total_likelihood;
	public:
		CombinedFit(){};
		CombinedFit(vector <FitPoint> Point){
			CombinedGraphsVal=Point;
//calculate Akaike Information Criterion 
			for(std::size_t i = 0; i != (sizeof CombinedGraphsVal / sizeof CombinedGraphsVal[0]); i++) {
				if(CombinedGraphsVal[i].Loglikelihood==0){
					AIC.push_back(2*(Point[i].ndf)-2*Point[i].ChiSq);
				}
				else AIC.push_back(2*(Point[i].ndf)-2* Point[i].Loglikelihood);

				if(i<1) AIC_min= AIC[0];
				if(AIC[i]<AIC_min){
					AIC_min=AIC[i];
				}
			}
			//All of this could be done in one loop by updating values, but this is operationally similar and N should not be above 100, 1000 being unrealistic
			for(std::size_t i = 0; i != (sizeof CombinedGraphsVal / sizeof CombinedGraphsVal[0]); i++) {
				weights.push_back(exp(-(AIC[i]-AIC_min)/2));
				total_likelihood+=weights[i];
			}
		};
		void add(FitPoint addition);
		double CombMean();
		double CombDev();
		void PrintMeta();
		void PrintValues();
		void PrintAll();

		~CombinedFit(){};
};

//allows for additional point to be added to current object
void CombinedFit::add(FitPoint addition){
	CombinedGraphsVal.push_back(addition);
	if(addition.Loglikelihood==0)	AIC.push_back(2*(addition.ndf)-2*addition.ChiSq);
	else	AIC.push_back(2*addition.ndf-2*addition.Loglikelihood);
	if(AIC.back()<AIC_min){
		AIC_min=AIC.back();
		total_likelihood=0;
		for(std::size_t i = 0; i != (sizeof CombinedGraphsVal / sizeof CombinedGraphsVal[0])-1; i++){
			weights[i]=exp(-(AIC[i]-AIC_min)/2);
			total_likelihood+=weights[i];
		}
	}
	weights.push_back(exp(-(AIC.back()-AIC_min)/2.0));
	total_likelihood+=weights.back();
}
//Calculate Combined Means
//Values Calculated at time of use in case of constant updating- shouldn't require many calls
double CombinedFit::CombMean(){
	double combined_mean=0;
	for(std::size_t i = 0; i != (sizeof CombinedGraphsVal / sizeof CombinedGraphsVal[0]); i++)
	{
		combined_mean+=CombinedGraphsVal[i].value*weights[i];
	}
	combined_mean=combined_mean/total_likelihood;
	return combined_mean;
}

//Returns the estimated deviation values
double CombinedFit::CombDev(){
	double sqr_sum=0;
	double comb_var=0;
	double combined_mean=0;
	for(std::size_t i = 0; i != (sizeof CombinedGraphsVal / sizeof CombinedGraphsVal[0]); i++)
	{
		combined_mean+=CombinedGraphsVal[i].value*weights[i];
		sqr_sum+=combined_mean*combined_mean;
		comb_var+=CombinedGraphsVal[i].error*weights[i];
	}
	combined_mean=combined_mean/total_likelihood;
	double variance=sqr_sum/total_likelihood - combined_mean*combined_mean;
	comb_var=comb_var*comb_var/total_likelihood/total_likelihood;
	double Dev = sqrt(variance+comb_var);
	return Dev;
}

//prints info to the screen used in the calculations
void CombinedFit::PrintMeta(){
	cout<<"Minimum AIC: "<<AIC_min<<endl;
	cout<<"Total Ratio Likelihoods: "<<total_likelihood<<endl;
	double sqr_sum=0;
	double comb_var=0;
	double combined_mean=0;
	for(std::size_t i = 0; i != (sizeof CombinedGraphsVal / sizeof CombinedGraphsVal[0]); i++)
	{
		combined_mean+=CombinedGraphsVal[i].value*weights[i];
		sqr_sum+=combined_mean*combined_mean;
		comb_var+=CombinedGraphsVal[i].error*weights[i];
	}
	combined_mean=combined_mean/total_likelihood;
	double variance=sqr_sum/total_likelihood - combined_mean*combined_mean;
	comb_var=comb_var*comb_var/total_likelihood/total_likelihood;
	cout<<"Variance between parameter error: "<<variance<<endl;
	cout<<"Combined Variance Terms: "<<comb_var;
}
//Prints goal or output values
void CombinedFit::PrintValues(){
	double sqr_sum=0;
	double comb_var=0;
	double combined_mean=0;
	for(std::size_t i = 0; i != (sizeof CombinedGraphsVal / sizeof CombinedGraphsVal[0]); i++)
	{
		combined_mean+=CombinedGraphsVal[i].value*weights[i];
		sqr_sum+=combined_mean*combined_mean;
		comb_var+=CombinedGraphsVal[i].error*weights[i];
	}
	combined_mean=combined_mean/total_likelihood;
	double variance=sqr_sum/total_likelihood - combined_mean*combined_mean;
	comb_var=comb_var*comb_var/total_likelihood/total_likelihood;
	double Dev = sqrt(variance+comb_var);
	cout<<"Combined Mean:"<<combined_mean<<endl;
	cout<<"Total Combined Error: "<<Dev<<endl;
}
//Prints all values
void CombinedFit::PrintAll(){
	cout<<"Printing all component fit information:"<<endl;
	cout<<"Parameter Value, Parameter Error, Log(Likelihood), ChiSq, NDF, AIC, Weights"<<endl;
	for(std::size_t i = 0; i != (sizeof CombinedGraphsVal / sizeof CombinedGraphsVal[0]); i++)
	{
		cout<<CombinedGraphsVal[i].value<<","<<CombinedGraphsVal[i].error<<","<<CombinedGraphsVal[i].Loglikelihood<<","<<CombinedGraphsVal[i].ChiSq<<","<<CombinedGraphsVal[i].ndf<<","<<AIC[i]<<","<<weights[i]<<endl;
	}
}
