

void cleantreegrabber(){
	const Char_t* host = "mysql://seaquel.physics.illinois.edu:3283/";
	const Char_t* user = "user";
	const Char_t* pass = "user"; 
	TSQLServer* con = TSQLServer::Connect(host, user, pass); 
	const int tablenum=4; 
	TFile *f= new TFile("MonteCarloTrees.root", "RECREATE");
	TTree * collection[tablenum];
	TString table[tablenum]={"mc_drellyan_LD2_M026_S002_messy","mc_drellyan_dump_M026_S002_messy",
	"mc_jpsi_LD2_M026_S002_messy","mc_psiprime_LD2_M026_S002_messy"};
	float mass;
	float zp;
	float yp;
	float zm;
	float ym;
	float x;
	float dx, dy, dz;
	int event;
	TSQLResult* resm[tablenum];
	TSQLRow* row[tablenum];
	for(int j=0;  j< tablenum; j++){
		collection[j]= new TTree(table[j], table[j]);
		collection[j]->Branch("z01",&zp,"z01/F");
		collection[j]->Branch("y01",&yp,"y01/F");
		collection[j]->Branch("z02",&zm,"z02/F");
		collection[j]->Branch("dz",&dz,"dz/F");
		collection[j]->Branch("dy",&dy,"dy/F");
		collection[j]->Branch("dx",&dx,"dx/F");
		collection[j]->Branch("y02",&ym,"y02/F");
		collection[j]->Branch("mass",&mass,"mass/F");
		collection[j]->Branch("xT",&x,"xT/F");
		collection[j]->Branch("eventID",&event,"eventID/I");
		TString querys = "SELECT h.eventID, h.mass, g.z0, g.eventID, g.y0, h.xT, h.dz, h.dx, h.dy FROM "
		 +table[j]+".kDimuon AS h LEFT JOIN "+table[j]+ ".kTrack AS g ON h.eventID=g.eventID";
		 TString queryz;
		if(j<2) queryz = " WHERE abs(h.trackSeparation)<250 AND abs(h.costh)<.5 AND h.chisq_dimuon<18 AND abs(h.dx)<.25 and g.numHits>13 and h.xT*h.xT+(g.yT-1.6)*(g.yT-1.6)<320 and abs(h.dy-1.6)<.22 and h.dz>-350 and abs(h.dpx)<1.8 and abs(h.dpy)<2 and h.dpx*h.dpx+h.dpy*h.dpy<5 and h.dpz>38 and h.dpz<116";
		else queryz="";
		TString query=querys+queryz;
		resm[j] = con->Query(query);
		Int_t nRowsm = resm[j]->GetRowCount();
		int prevevent=0;
		for (int i = 0; i < nRowsm; ++i)
		{
			row[j] = resm[j]->Next(); //moves to the next line
			if(!row[j]) continue;
			event=atoi(row[j]->GetField(0));
			mass=atof(row[j]->GetField(1));
			x=atof(row[j]->GetField(5));
			dz=atof(row[j]->GetField(6));
			dy=atof(row[j]->GetField(7));
			dx=atof(row[j]->GetField(8));
			if(prevevent==event){ ym=atof(row[j]->GetField(4)); zm=atof(row[j]->GetField(2)); }
			else{ yp=atof(row[j]->GetField(4)); zp=atof(row[j]->GetField(2)); }
			prevevent=event;
			collection[j]->Fill();
		}
		collection[j]->Write();
	}
	f->Close();
}
