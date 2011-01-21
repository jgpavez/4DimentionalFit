#ifndef __CINT__
#include <map>
#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TCut.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TProfile.h"

#include "RooFit.h"
#include "RooRealVar.h"
#include "RooGenericPdf.h"
#include "RooDataHist.h"
#include "RooPlot.h"

#include <cstdio>
#include <iostream>

#endif


void fit_plots()
{
	using namespace RooFit;

	TFile *phiHist = new TFile("~/4DimentionalFit/DATA/phi_plots.root");
	TFile *phiHistFit = new TFile("~/4DimentionalFit/DATA/phi_plots_fit.root","RECREATE");

	const char *phiFormula = "A+B*cos(x*TMath::Pi()/180.)+C*cos(2*x*TMath::Pi()/180.)";

	Double_t sum_chi;

	for ( unsigned int i = 0 ; i < kN_PT ; i++){
		for ( unsigned int j = 0; j < kN_XB; j++){
			for ( unsigned int k = 0; k < kN_ZH; k++){

				phiHist->cd();

				TH1D *phiPlot = (TH1D *)phiHist->Get(Form("proyx_%d_%d_%d",i,j,k));
				if (phiPlot == NULL) continue;

				RooRealVar x("x","x",-180,180);
				RooRealVar A("A","A",2,200);
				RooRealVar B("B","B",-20,20);
				RooRealVar C("C","C",20,20);

				RooPlot *phiFrame = x.frame();
				phiFrame->SetName(Form("phihist_%d_%d_%d",i,j,k));

	            RooDataHist dh("dh", "dh", x, Import(*phiPlot));
				dh.plotOn(phiFrame,DataError(RooAbsData::SumW2));

				RooGenericPdf formula("formula","phi formula",phiFormula,
			            RooArgSet(x,A,B,C));
				formula.fitTo(dh,SumW2Error(kTRUE),Save());

				formula.plotOn(phiFrame);

				//std::cout<<phiframe->chiSquare()<<std::endl;
				//sum_chi += phiframe->chiSquare();

				phiHistFit->cd();
				phiFrame->Write();
				delete phiPlot;
				delete phiFrame;

			}
		}
	}
	phiHistFit->Close();
	phiHist->Close();
	delete phiHist;
	delete phiHistFit;
}
