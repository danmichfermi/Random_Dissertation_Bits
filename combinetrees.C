void combinetrees(){

	const auto filename = "FilteredHData.root";
    const auto filename2 = "FilteredHHData.root";
	TFile oldfile(filename);
   	TFile fileHigh(filename2);
 
    TTree *oldtree;
    TTree *hightree;

    oldfile.GetObject("LH2", oldtree);
    fileHigh.GetObject("LH2", hightree);

    
   	TFile newfile("filterCorrH.root", "recreate");

   	auto newtree = oldtree->CloneTree();

	auto newtreehigh = hightree->CloneTree();
	TList *list = new TList;
   	list->Add(newtree);
   	list->Add(newtreehigh);
 

   	TTree *totaltree = TTree::MergeTrees(list);
   	totaltree->SetName("LH2");
   totaltree->Write();
    oldfile.Close();
   fileHigh.Close();
}

void cloner(){
	TString fname[3]={
    	"filterCorrD.root",
    	"FilteredEMPData.root",
    	"filterCorrH.root"
	};
	TString tname[3]={"LD2","EMP","LH2"};
	for (int i = 0; i < 3; ++i)
	{
		TTree *oldtree;
		TFile curfile(fname[i], "UPDATE");
	    curfile.GetObject(tname[i], oldtree);
	    auto newtree = oldtree->CloneTree();
	    newtree->SetName("background");
	    newtree->Write();
 		curfile.Close();
	}
}