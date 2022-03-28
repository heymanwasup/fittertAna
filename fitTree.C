#define fitTree_cxx
#include "fitTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void fitTree::Loop(int events)
{

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      if (events>0 && jentry>=events) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      
      histSvc->InitNameTags();
      histSvc->SetProcessTag("fitter_ana");

      if(m_task=="deltaT") {
         Ana_deltaT();
      }

   }
}

void fitTree::Ana_deltaT() {
   //only consider fit results with >1 hits
   if(energies->size()!=2) return;

   //remove islands with laser hits
   for(auto laserHit : *laserHits) {
      if(laserHit) return;
   }
   bool single_island = false;
   if(islandNums->size()==1 && chainedIslandNums->size()==1) single_island = true;

   double e0 = energies->at(0);
   double e1 = energies->at(1);
   double t0 = times->at(0);
   double t1 = times->at(1);
   
   double t = e0>t1?t0 : t1;
   double DT_ordered = e0>e1? t1 - t0 : t0 - t1;   
   double DT = t1 - t0;



   histSvc->BookFillHist("deltaT",1600,-800,800,DT_ordered);
   if(single_island) histSvc->BookFillHist("single_deltaT",1600,-800,800,DT_ordered);

   histSvc->BookFillHist("deltaT_vs_t0",1600,-800,800,200,0,600e3,DT_ordered,t);
   if(single_island) histSvc->BookFillHist("single_deltaT_vs_t0",1600,-800,800,200,0,600e3,DT_ordered,t);
}