//refill trees for use in making graphs and running evaluations

void cleantreegrabber(){
    const Char_t* host = "mysql://seaquestdb01.fnal.gov:3310";
    const Char_t* host2 = "mysql://seaquestdb03.fnal.gov:3310";
	const Char_t* user = "seaguest";
	const Char_t* pass = "qqbar2mu+mu-"; 
	TSQLServer* con = TSQLServer::Connect(host, user, pass); 
	TSQLServer* con2 = TSQLServer::Connect(host2, user, pass); 
	TSQLResult* resPtr;
    TSQLRow* rowPtr;
	const int tablenum=4; 
	TFile *f= new TFile("Realfulldata.root", "RECREATE");
	//TTree * tree= new TTree("Roadset67","Roadset67");
	TTree * tree= new TTree("Roadset6_somecuts","Roadset67_somecuts");
	ifstream infile("sampleroadset.txt");
	float mass;
	float xT;
	float xB;
	float dx, dy, dz;
	int event;
	int targetPos;
	int isTarget;
	int run;
	//tree->Branch("run",&run,"eventID/I");
	tree->Branch("targetPos",&targetPos,"targetPos/I");
	tree->Branch("isTarget",&isTarget,"isTarget/I");
	tree->Branch("mass",&mass,"mass/F");
	tree->Branch("xB",&xB,"xB/F");
	tree->Branch("xT",&xT,"xT/F");
	tree->Branch("dz",&dz,"dz/F");
	tree->Branch("dy",&dy,"dy/F");
	tree->Branch("dx",&dx,"dx/F");
	stringstream linestream;
	string line;
	int iter=0;
    while (std::getline(infile, line)) {
    	TString selectStr, fromStr, whereStr;
	    TString schema = "run_0"+line+"_R008";
	    selectStr = "SELECT h.eventID, h.mass, h.xT, h.xB, h.dz, h.dx, h.dy, h.targetPos, h.isTarget FROM "
		 +schema+".kDimuon AS h LEFT JOIN "+schema+ ".kTrack AS g ON h.eventID=g.eventID";	    
   		whereStr = " WHERE h.isValid=1 AND g.numHits>13";
   		//AND abs(h.trackSeparation)<250 AND abs(h.costh)<.5 AND h.chisq_dimuon<18";
  		//whereStr +=" AND abs(h.dx)<.25 and g.numHits>13 AND h.xT*h.xT+(g.yT-1.6)*(g.yT-1.6)<320 AND abs(h.dy-1.6)<.22 AND h.dz>-350";
   		//whereStr +=" AND abs(h.dpx)<1.8 AND abs(h.dpy)<2 AND h.dpx*h.dpx+h.dpy*h.dpy<5 and h.dpz>38 AND h.dpz<116";
	    TString query  = selectStr + whereStr;
	    //We connect and execute the query with the following:
	    if (iter<1215){ iter++; continue; } //resPtr = con2->Query(query);
	    resPtr = con->Query(query);
	   	//cout<<query<<endl;
	    //making sure the run exists
	    if (resPtr) {
	        //Get total row number so we can loop through the result table
	        Int_t rowNum = resPtr->GetRowCount();
		//looping through rows
			for (int i = 0; i < rowNum; i++) {
	       		rowPtr = resPtr->Next();
	       		mass = atof(rowPtr->GetField(1));
	       		targetPos = atoi(rowPtr->GetField(7));
		    	xT=atof(rowPtr->GetField(2));
				dz=atof(rowPtr->GetField(4));
				dy=atof(rowPtr->GetField(6));
				dx=atof(rowPtr->GetField(5));
	       		xB = atof(rowPtr->GetField(3));
	       		isTarget = atoi(rowPtr->GetField(8));
	       		tree->Fill();
			}
	    }
	    iter++;
	}
	tree->Write();
	f->Close();
}


