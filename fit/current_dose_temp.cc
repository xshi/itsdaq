// -*- C++ -*-
//
// RooFit for ATLAS ITk current dose temp
//
// Original Author:  SHI Xin <shixin@ihep.ac.cn>
//         Created:  [2016-09-06 Tue 16:16] 
//

#include <TMath.h>
#include <TMinuit.h>

Float_t z[3],x[3],y[3];
// Float_t z[7],x[7],y[7];

Double_t func(float x,float y,Double_t *par)
{
  Double_t value=( 1 + par[0]*TMath::Power(par[1],x) * exp(-par[2]*(y+10)) );
  return value;
}

void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{
  const Int_t nbins = 3;
  //const Int_t nbins = 7;
  Int_t i;
  
  //calculate chisquare
  Double_t chisq = 0;
  Double_t delta;
  for (i=0;i<nbins; i++) {
    delta  = (z[i]-func(x[i],y[i],par));
    chisq += delta*delta;
  }
  f = chisq;
}

void current_dose_temp()
{

  // The z values
   z[0] = 2.5;
   z[1] = 1.9;
   z[2] = 1.3;
   // z[3] = 9.7;
   // z[4] = 3.9;
   // z[5] = 13.6;
   // z[6] = 5.2;
   
   // the x values
   x[0] = 2.3;
   x[1] = 2.3;
   x[2] = 0.6;
   // x[3] = 1250;
   // x[4] = 62;
   // x[5] = 2250;
   // x[6] = 2250;
   
   // the y values
   y[0] = -25;
   y[1] = -10;
   y[2] = -10;
   // y[3] = -25;
   // y[4] = -15;
   // y[5] = -15;
   // y[6] = 20; 

   //initialize TMinuit with a maximum of 3 params
   TMinuit *gMinuit = new TMinuit(3);  
   gMinuit->SetFCN(fcn);

   Double_t arglist[10];
   Int_t ierflg = 0;

   arglist[0] = 1;
   gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);

   // Set starting values and step sizes for parameters
   static Double_t vstart[3] = {0.1, 0.1, 0.1};
   static Double_t step[3] = {0.01, 0.01, 0.01};
   gMinuit->mnparm(0, "alpha", vstart[0], step[0], 0, 0, ierflg);
   gMinuit->mnparm(1, "beta", vstart[1], step[1], 0, 0, ierflg);
   gMinuit->mnparm(2, "gamma", vstart[2], step[2], 0, 0, ierflg);

   // Now ready for minimization step
   arglist[0] = 500;
   arglist[1] = 1.;
   gMinuit->mnexcm("MIGRAD", arglist, 2, ierflg);
}
