//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Mar 27 13:39:16 2022 by ROOT version 6.12/04
// from TTree fitTree/fitTree
// found on file: gm2offline_ana_31153210_24725.00199.root
//////////////////////////////////////////////////////////

#ifndef fitTree_h
#define fitTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <string>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "SimpleHistSVC.h"
#include <iostream>

using namespace std;

class fitTree {
public :   
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
   TDirectory     *output_file; //!
   SimpleHistSVC  *histSvc; //!
   string          m_task; //!

   

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           caloNum;
   Int_t           xtalNum;
   Int_t           fillNum;
   Int_t           subRunNum;
   Int_t           runNum;
   vector<int>     *islandNums;
   vector<int>     *chainedIslandNums;
   Double_t        chi2;
   Double_t        pedestal;
   vector<double>  *energies;
   vector<double>  *times;
   vector<int>     *statuses;
   vector<short>   *laserHits;

   // List of branches
   TBranch        *b_caloNum;   //!
   TBranch        *b_xtalNum;   //!
   TBranch        *b_fillNum;   //!
   TBranch        *b_subRunNum;   //!
   TBranch        *b_runNum;   //!
   TBranch        *b_islandNums;   //!
   TBranch        *b_chainedIslandNums;   //!
   TBranch        *b_chi2;   //!
   TBranch        *b_pedestal;   //!
   TBranch        *b_energies;   //!
   TBranch        *b_times;   //!
   TBranch        *b_statuses;   //!
   TBranch        *b_laserHits;   //!

   fitTree(string task);
   virtual ~fitTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(int events=-1);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);   
   virtual void     SetFile(TTree *tree, TDirectory *file);
   virtual void     WriteToFile();
   virtual void     Ana_deltaT();
};

#endif

#ifdef fitTree_cxx
fitTree::fitTree(string task) : 
   fChain(0),
   m_task(task) {}

fitTree::~fitTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
   delete histSvc;
}

Int_t fitTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t fitTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void fitTree::Init(TTree *tree)
{


   // Set object pointer
   islandNums = 0;
   chainedIslandNums = 0;
   energies = 0;
   times = 0;
   statuses = 0;
   laserHits = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("caloNum", &caloNum, &b_caloNum);
   fChain->SetBranchAddress("xtalNum", &xtalNum, &b_xtalNum);
   fChain->SetBranchAddress("fillNum", &fillNum, &b_fillNum);
   fChain->SetBranchAddress("subRunNum", &subRunNum, &b_subRunNum);
   fChain->SetBranchAddress("runNum", &runNum, &b_runNum);
   fChain->SetBranchAddress("islandNums", &islandNums, &b_islandNums);
   fChain->SetBranchAddress("chainedIslandNums", &chainedIslandNums, &b_chainedIslandNums);
   fChain->SetBranchAddress("chi2", &chi2, &b_chi2);
   fChain->SetBranchAddress("pedestal", &pedestal, &b_pedestal);
   fChain->SetBranchAddress("energies", &energies, &b_energies);
   fChain->SetBranchAddress("times", &times, &b_times);
   fChain->SetBranchAddress("statuses", &statuses, &b_statuses);
   fChain->SetBranchAddress("laserHits", &laserHits, &b_laserHits);

   histSvc = new SimpleHistSVC();
   Notify();
}

Bool_t fitTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void fitTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t fitTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

void fitTree::SetFile(TTree * tree, TDirectory * file){
   fChain = tree;
   output_file = file;   
   Init(tree);   
   histSvc->BookFile(file);
   
}

void fitTree::WriteToFile() {
   histSvc->Write();   
}

#endif // #ifdef fitTree_cxx
