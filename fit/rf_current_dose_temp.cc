// -*- C++ -*-
//
// RooFit for ATLAS ITk current dose temp
//
// Original Author:  SHI Xin <shixin@ihep.ac.cn>
//         Created:  [2016-09-08 Thu 16:32] 
//

#include <RooWorkspace.h>
#include <RooDataSet.h>
#include <RooRealVar.h>




void rf_current_dose_temp()
{
  using namespace RooFit ;
  
  RooWorkspace w("w",kTRUE) ;

  w.factory("Gaussian::gauss(mes[5.20,5.30],mean[5.28,5.2,5.3],width[0.0027,0.001,1])");
  w.factory("ArgusBG::argus(mes,5.291,argpar[-20,-100,-1])") ;
  w.factory("SUM::sum(nsig[200,0,10000]*gauss,nbkg[800,0,10000]*argus)") ;

  // --- Generate a toyMC sample from composite PDF ---
  RooDataSet *data = w::sum.generate(w::mes,2000) ;

  // --- Perform extended ML fit of composite PDF to toy data ---
  w::sum.fitTo(*data) ;

  // --- Plot toy data and composite PDF overlaid ---
  RooPlot* mesframe = w::mes.frame() ;
  data->plotOn(mesframe) ;
  w::sum.plotOn(mesframe) ;
  w::sum.plotOn(mesframe,Components(w::argus),LineStyle(kDashed)) ;

  mesframe->Draw()  ;
  // Construct F(x|y) -- a Gaussian in x with a mean that depends on y
  // w.factory("PolyVar::meanx(y[-5,5],{a0[-0.5,-5,5],a1[-0.5,-1,1]})") ;
  // w.factory("Gaussian::gaussx(x[-5,5],meanx,sigmax[0.5])") ;
  
  // Construct G(y) -- an Exponential in y
  // w.factory("Exponential::gaussy(y,-0.2)") ;
 
  // Construct M(x,y) = F(x|y)*G(y)
  // w.factory("PROD::model(F|y,G)") ;

  // RooDataSet *data = w::model.generate(RooArgSet(w::x,w::y),10000) ;

  // w::model.fitTo(*data) ;
  
  // RooPlot* xframe = w::x.frame() ;
  // data->plotOn(frame) ;
  // w::model.plotOn(frame) ;
 
}
