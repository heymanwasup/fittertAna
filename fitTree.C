#define fitTree_cxx
#include "fitTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void fitTree::FillHists(const double & e0,const double & e1,const double & t0,const double & t1,const double & DT,const double & rE,const bool &single_island) {

   
   histSvc->BookFillHist("deltaT",1600,-800,800,DT);
   histSvc->BookFillHist("deltaT_vs_t0",1600,-800,800,200,0,600e3,DT,t0);
   histSvc->BookFillHist("deltaT_vs_E0",1600,-800,800,4000,0,4e3,DT,e0);
   histSvc->BookFillHist("deltaT_vs_E1",1600,-800,800,4000,0,4e3,DT,e1);
   histSvc->BookFillHist("deltaT_vs_rE",1600,-800,800,1000,0,1,DT,rE);

   if(single_island){
      histSvc->SetIslandNums(single_island);
      histSvc->BookFillHist("deltaT",1600,-800,800,DT);
      histSvc->BookFillHist("deltaT_vs_t0",1600,-800,800,200,0,600e3,DT,t0);
      histSvc->BookFillHist("deltaT_vs_E0",1600,-800,800,4000,0,4e3,DT,e0);
      histSvc->BookFillHist("deltaT_vs_E1",1600,-800,800,4000,0,4e3,DT,e1);
      histSvc->BookFillHist("deltaT_vs_rE",1600,-800,800,1000,0,1,DT,rE);
   }
   histSvc->ResetIslandTag();

}

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
      
      if(jentry%1000000==0) {
         std::cout << "Processed "<< jentry << std::endl;
      }

      

      if(m_task=="deltaT") {

         Ana_deltaT();
      }

   }
}

void fitTree::Ana_deltaT() {
   histSvc->InitNameTags();
   histSvc->SetProcessTag("fitter_ana");

   //only consider fit results with >1 hits
   if(energies->size()!=2) return;

   //remove islands with laser hits
   for(auto laserHit : *laserHits) {
      if(laserHit) return;
   }

   bool single_island = false;
   if(islandNums->size()==1 && chainedIslandNums->size()==1) single_island = true;

   double e0,e1,t0,t1;
   if(energies->at(0)>energies->at(1)) {
      e0 = energies->at(0);
      t0 = times->at(0);
      e1 = energies->at(1);
      t1 = times->at(1);
   }
   else {
      e0 = energies->at(1);
      t0 = times->at(1);
      e1 = energies->at(0);
      t1 = times->at(0);
   }

   double DT = t1 - t0;
   double rE = e1/e0;

   FillHists(e0,e1,t0,e1,DT,rE,single_island);
   if(caloNum==1) {
      histSvc->SetCaloTag(caloNum);
      FillHists(e0,e1,t0,e1,DT,rE,single_island);
      // histSvc->SetXtalTag(xtalNum);
      // FillHists(e0,e1,t0,e1,DT,rE,single_island);
   }

}