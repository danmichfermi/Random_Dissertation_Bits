void roadsetlist(){
	const Char_t* host = "mysql://seaquestdb01.fnal.gov:3310/";
	const Char_t* user = "seaguest";
	const Char_t* pass = "qqbar2mu+mu-"; 
	TSQLServer* con = TSQLServer::Connect(host, user, pass); 
	const int tablenum=1; 
	TString table={"merged_roadset67_R006_V003"};
	int twotracks=0;
	int threetracks=0;
	int moretracks=0;
	TSQLResult* resm;
	TSQLRow* row;
	//LEFT JOIN "+table+".Spill AS g ON g.spillID=h.spillID AND g.dataQuality=0 
	TString query="SELECT h.eventID FROM "+table+".kTrack AS h JOIN "+table+".kDimuon AS j ON h.eventID=j.eventID WHERE h.kmstatus=1 AND h.numHits<18 AND h.runID>13800 AND j.isValid=1 LIMIT 100000";
	resm = con->Query(query);
	Int_t nRowsm = resm->GetRowCount();
	int current=0;
	int trackcount=1;
	int previous=0;
	for (int i = 0; i < nRowsm; ++i){
		row = resm->Next(); //moves to the next line
		if(!row) continue;
		current=atoi(row->GetField(0));
		if(previous==current) trackcount++;
		else{
			if(trackcount==2) twotracks++;
			if(trackcount==3) threetracks++;
			if(trackcount>3) moretracks++;
			trackcount=1;
		}
		previous=current;
	}
	cout<<twotracks<<"\t"<<threetracks<<"\t"<<moretracks<<endl;
	float percentage=(float)twotracks/((float)(twotracks+threetracks+moretracks));
	cout<<percentage<<endl;
}
