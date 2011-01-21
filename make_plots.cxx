//__________________________________________________________________________________________
// First step in four dimentional fit for Phi
//
//_______________________________________________________________________________________

#if !defined(__CINT__)
#include "TROOT.h"
#include "Riostream.h"
#include "TFile.h"
#include "TH3D.h"
#include "TCanvas.h"
#include "TNtuple.h"
#include "TProfile.h"
#include "TH1F.h"
#include "TCut.h"
#include "TString.h"
#include "TMath.h"

#include "constants.h"
#include <cstdio>
#include <iostream>
#include <stdlib.h>
#endif



void make_plots()
{
#if defined (__CINT__)
gInterpreter->AddIncludePath("/home/jgpave/4DimentionalFit/includes");
#endif
	TFile *dataFile = new TFile("~/4DimentionalFit/DATA/acceptance_correction_new.root");
	TFile *phiPlots = new TFile("~/4DimentionalFit/DATA/phi_plots.root","RECREATE");

	if (dataFile->IsZombie()) throw std::exception();

	TNtuple *data = (TNtuple *)dataFile->Get("corrected_data");

	const Double_t deltaPt = (kPT_MAX-kPT_MIN)/kN_PT;
  	const Double_t deltaXb = (kXB_MAX-kXB_MIN)/kN_XB;
  	const Double_t deltaZh = (kZH_MAX-kZH_MIN)/kN_ZH;
  	Double_t ptMin,xbMin,zhMin;
  	// Looping for each bin
	for ( unsigned int i = 0; i < kN_PT; i++){
    	for ( unsigned int j = 0; j < kN_XB; j++){
      		for ( unsigned int k = 0; k < kN_ZH; k++){
      			ptMin = kPT_MIN + deltaPt*i;
      			xbMin = kXB_MIN + deltaXb*j;
      			zhMin = kZH_MIN + deltaZh*k;

      			// get Pt Histogram for each bin and write
      			TCut cut = Form("Pt > %f && Pt < %f && X > %f && X < %f && Zh > %f && Zh < %f",
      						ptMin,ptMin+deltaPt,xbMin,xbMin+deltaXb,zhMin,zhMin + deltaZh );
      			data->Draw(Form("n_data:Phi>>ptHist_%d_%d_%d",i,j,k),cut,"goffprofile");
      			data->Draw(Form("data_error:Phi>>errHist_%d_%d_%d",i,j,k),cut,"goffprofile");

     			TProfile *ptHistogram = (TProfile *)gDirectory->Get(Form("ptHist_%d_%d_%d",i,j,k));
      			TH1D *ptProyection = ptHistogram->ProjectionX(Form("proyx_%d_%d_%d",i,j,k));

      			TProfile *errHistogram = (TProfile *)gDirectory->Get(Form("errHist_%d_%d_%d",i,j,k));
      			TH1D *errProyection = errHistogram->ProjectionX(Form("proyerr_%d_%d_%d",i,j,k));

      			for ( unsigned int e = 1; e < kN_PHI; e++)
      				ptProyection->SetBinError(e,TMath::Sqrt(errProyection->GetBinContent(e)));

      			phiPlots->cd();
      			ptProyection->Write();


      			delete ptHistogram;
      			delete ptProyection;
      			delete errHistogram;
      			delete errProyection;
      		}
    	}
	}
	dataFile->Close();
	phiPlots->Close();
	delete phiPlots;
	delete dataFile;
}


