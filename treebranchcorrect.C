
void treebranchcorrect(){
	//Get TTree
   const auto filename = "FilteredDData.root";
   const auto filename2 = "FilteredDHData.root";
   TFile oldfile(filename);
   TFile fileHigh(filename2);
 
   TTree *oldtree;
   TTree *hightree;
   float xT;
   oldfile.GetObject("LD2", oldtree);
   fileHigh.GetObject("LD2", hightree);

   // Deactivate all branches
   oldtree->SetBranchStatus("xT", 0);
   // Activate fraction
   //for (auto activeBranchName : {"xB", "xF", "mass", "Intensity_p", "dpy", "dpx", "dpz", "dz"})
     // oldtree->SetBranchStatus(activeBranchName, 1);
   // Create a new file + a clone of old tree in new file
   TFile newfile("filterCorrD.root", "recreate");
   auto newtree = oldtree->CloneTree();
   oldtree->SetBranchStatus("xT", 1);
   oldtree->SetBranchAddress("xT", &xT );
   TBranch *brx=newtree->Branch("xT", &xT,"xT/F");
   for (int i = 0; i < oldtree->GetEntries(); ++i)
   {
   	oldtree->GetEntry(i);
   	xT=TMath::Abs(xT);
   	brx->Fill();
   }
   oldfile.Close();

   //hightree->SetBranchStatus("", 0);
   // Activate fraction
   //for (auto activeBranchName : {"xB", "xF", "mass", "Intensity_p", "dpy", "dpx", "dpz", "dz", "xT"})
   //   hightree->SetBranchStatus(activeBranchName, 1);
   // Create a new file + a clone of old tree in new file
   auto newtreehigh = hightree->CloneTree();
   TList *list = new TList;
   list->Add(newtree);
   list->Add(newtreehigh);
   TTree *totaltree = TTree::MergeTrees(list);
   totaltree->SetName("LD2");
   totaltree->Write();
   fileHigh.Close();
}