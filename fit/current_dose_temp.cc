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

Double_t func(float x,float y,Double_t *par)
{
  Double_t value=( 1 + TMath::Power(par[0],x) * exp(-par[1]*(y+10)) );
  return value;
}

void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{
   const Int_t nbins = 3;
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

   // the x values
   x[0]=2.3;
   x[1]=2.3;
   x[2]=0.6;

   // the y values
   y[0] = -25;
   y[1] = -10;
   y[2] = -10;

   //initialize TMinuit with a maximum of 2 params
   TMinuit *gMinuit = new TMinuit(2);  
   gMinuit->SetFCN(fcn);

   Double_t arglist[10];
   Int_t ierflg = 0;

   arglist[0] = 1;
   gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);

   // Set starting values and step sizes for parameters
   static Double_t vstart[2] = {0.1, 0.1};
   static Double_t step[2] = {0.01 , 0.01};
   gMinuit->mnparm(0, "beta", vstart[0], step[0], 0, 0, ierflg);
   gMinuit->mnparm(1, "gamma", vstart[1], step[1], 0, 0, ierflg);

   // Now ready for minimization step
   arglist[0] = 500;
   arglist[1] = 1.;
   gMinuit->mnexcm("MIGRAD", arglist, 2, ierflg);
}
