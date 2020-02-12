//gathers some roadset information for quality check

void roadsetlist(){
	const Char_t* host = "mysql://seaquestdb01.fnal.gov:3310/";
	const Char_t* user = "seaguest";
	const Char_t* pass = "qqbar2mu+mu-"; 
	TSQLServer* con = TSQLServer::Connect(host, user, pass); 
	const int tablenum=1; 
	TString table={"merged_roadset67_R006_V003.production"};
	int run;
	TSQLResult* resm;
	TSQLRow* row;
	TString query="SELECT run FROM "+table;
	resm = con->Query(query);
	Int_t nRowsm = resm->GetRowCount();
	int prevevent=0;
	ofstream outfile("sampleroadset.txt");
	for (int i = 0; i < nRowsm; ++i){
		row = resm->Next(); //moves to the next line
		if(!row) continue;
		run=atoi(row->GetField(0));
		outfile<<run<<"\n";		
	}
}
