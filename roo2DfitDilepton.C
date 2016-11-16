// Possible Scenarios
// "Vis_NoNuisance"
// "Full_NoNuisance"
// "Vis_Nuisance"
// "Full_Nuisance"

void roo2DfitDilepton(TString Scenario = "Vis_NoNuisance"){



  gSystem->Load("libRooFit") ;
  using namespace RooFit;

  TFile* fInput  = new TFile("ttbb_top-16-010.root");

  TH2D *hdata = (TH2D*) fInput->Get("h2_DATA_S6LL_CEN")->Clone();

  TH2D *httbb = (TH2D*) fInput->Get("h2_POWttbb_S6LL_csvweight")->Clone();

  TH2D *httb  = (TH2D*) fInput->Get("h2_POWttb_S6LL_csvweight")->Clone();
  TH2D *htt2b = (TH2D*) fInput->Get("h2_POWtt2b_S6LL_csvweight")->Clone();

  TH2D *httcc = (TH2D*) fInput->Get("h2_POWttcc_S6LL_csvweight")->Clone();
  TH2D *httLF = (TH2D*) fInput->Get("h2_POWttlf_S6LL_csvweight")->Clone();

  TH2D *hBkgtt     = (TH2D*) fInput->Get("h2_POWttot_S6LL_csvweight")->Clone();
  TH2D *hBkgOther  = (TH2D*) fInput->Get("bkghist")->Clone();
  TH2D *hBkgDD     = (TH2D*) fInput->Get("ddbkghist")->Clone();

  TH2D *httbj    = (TH2D*) httb->Clone();
  httbj->Add(htt2b);
  TH2D *httccLF  = (TH2D*) httcc->Clone();
  httccLF->Add(httLF);

  double R_ttbj_ttbb = httbj->Integral()/httbb->Integral() ;
  cout << "Ratio = " << R_ttbj_ttbb << endl;
  
  // Cross Sections from MC 
  RooRealVar *Vis_Xsecttjj   = new RooRealVar("Vis_Xsecttjj",  "ttjj cross section Vis Ph-Sp",    5.10, 1.0, 10.0);
  RooRealVar *Vis_Xsecttbb   = new RooRealVar("Vis_Xsecttbb",  "ttbb cross section Vis Ph-Sp",    0.07, 0.001, 0.5);
  RooRealVar *Full_Xsecttjj  = new RooRealVar("Full_Xsecttjj", "ttjj cross section Full Ph-Sp", 257.0,  150.0, 350.0);
  RooRealVar *Full_Xsecttbb  = new RooRealVar("Full_Xsecttbb", "ttbb cross section Full Ph-Sp",   3.2,  2.0, 5.0);

  // Ratio Xsecttbb/Xsecttjj in Visible    
  RooFormulaVar *Vis_Xsecttbb_Xsecttjj  = new RooFormulaVar("Vis_Xsecttbb_Xsecttjj",  "Xsecttbb/Xsecttjj Vis-PhSp",  "Vis_Xsecttbb/Vis_Xsecttjj",   RooArgList(*Vis_Xsecttbb,*Vis_Xsecttjj));
  RooFormulaVar *Full_Xsecttbb_Xsecttjj = new RooFormulaVar("Full_Xsecttbb_Xsecttjj", "Xsecttbb/Xsecttjj Full-PhSp", "Full_Xsecttbb/Full_Xsecttjj", RooArgList(*Full_Xsecttbb,*Full_Xsecttjj));

  // Efficiencies and Acceptances
  RooRealVar *Effttjj_nom  = new RooRealVar("Effttjj_nom",  "Nom. ttjj Efficiency",  0.1165);//, 0.05, 0.16);
  RooRealVar *Effttbb_nom  = new RooRealVar("Effttbb_nom",  "Nom. ttbb Efficiency",  0.2692);//, 0.16, 0.36);
  RooRealVar *Accttjj_nom  = new RooRealVar("Accttjj_nom",  "Nom. ttjj Acceptance",  0.020);
  RooRealVar *Accttbb_nom  = new RooRealVar("Accttbb_nom",  "Nom. ttbb Acceptance",  0.022);

  // Luminosity
  RooRealVar *Lumi_nom  = new RooRealVar("Lumi_nom",  "Luminosity",  2318.27);

  // Variables CSV2 and CSV3
  RooRealVar *CSV2 = new RooRealVar("CSV2", "CSV for Jet 3", httbb->GetXaxis()->GetXmin(), httbb->GetXaxis()->GetXmax()); 
  RooRealVar *CSV3 = new RooRealVar("CSV3", "CSV for Jet 4", httbb->GetYaxis()->GetXmin(), httbb->GetYaxis()->GetXmax());  
      
  // Background
  RooRealVar *NBkgtt    = new RooRealVar("NBkgtt",    "number of tt background events",          hBkgtt->Integral());
  RooRealVar *NBkgOther = new RooRealVar("NBkgOther", "number of Other background events",       hBkgOther->Integral());
  RooRealVar *NBkgDD    = new RooRealVar("NBkgDD",    "number of Data-Driven background events", hBkgDD->Integral());
  
  // 2D Arguments
  RooArgList *arg_CSV = new RooArgList(*CSV2, *CSV3);
  
  // ----------------------------------------------------------------------------------
  // ----------------------------------------------------------------------------------
  
  RooDataHist *DHttbb,*DHttbj,*DHttccLF,*DHBkgtt,*DHBkgOther,*DHBkgDD;
  ///////////////
  // CSV2-CSV3 //
  ///////////////
  // histograms: ttjj
  DHttbb   = new RooDataHist("DHttbb",   "ttbb Histogram", *arg_CSV, httbb);
  DHttbj   = new RooDataHist("DHttbj",   "ttbb Histogram", *arg_CSV, httbj);
  DHttccLF = new RooDataHist("DHttccLF", "ttbb Histogram", *arg_CSV, httccLF);
  // histograms: Bkg    
  DHBkgtt    = new RooDataHist("DHBkgtt",    "Bkgtt Histogram",    *arg_CSV, hBkgtt);
  DHBkgOther = new RooDataHist("DHBkgOther", "BkgOther Histogram", *arg_CSV, hBkgOther);
  DHBkgDD    = new RooDataHist("DHBkgDD",    "BkgDD Histogram",    *arg_CSV, hBkgDD);
  // histograms: Data    
  DHData = new RooDataHist("DHData", "Data Histogram", *arg_CSV, hdata);
  
  // ----------------------------------------------------------------------------------
  // ----------------------------------------------------------------------------------
  
  RooHistPdf *HPttbb,*HPttbj,*HPttccLF,*HPBkgtt,*HPBkgOther,*HPBkgDD;

  ///////////////
  // CSV2-CSV3 //
  ///////////////
  // pdf: ttjj 
  HPttbb   = new RooHistPdf("HPttbb",  "PDF for ttbb",   RooArgSet(*arg_CSV), *DHttbb);
  HPttbj   = new RooHistPdf("HPttbj",  "PDF for ttbj",   RooArgSet(*arg_CSV), *DHttbj);
  HPttccLF = new RooHistPdf("HPttccLF", "PDF for ttccLF", RooArgSet(*arg_CSV), *DHttccLF);
  // pdf: Bkg
  HPBkgtt    = new RooHistPdf("HPBkgtt",    "PDF for Bkgtt",    RooArgSet(*arg_CSV), *DHBkgtt);
  HPBkgOther = new RooHistPdf("HPBkgOther", "PDF for BkgOther", RooArgSet(*arg_CSV), *DHBkgOther);
  HPBkgDD    = new RooHistPdf("HPBkgDD",    "PDF for BkgDD",    RooArgSet(*arg_CSV), *DHBkgDD);

  // ----------------------------------------------------------------------------------
  // ----------------------------------------------------------------------------------
  
  /////////////////////////////////////
  // WorkSpace 1: Standard Fit Model //
  /////////////////////////////////////
  
  RooWorkspace *WS = new RooWorkspace("Fit ttbb/ttjj");
  
  // Parameters
  WS->import(*CSV2);
  WS->import(*CSV3);
  WS->import(*arg_CSV);
  
  WS->import(*Lumi_nom);
  WS->import(*Effttjj_nom);
  WS->import(*Effttbb_nom);
  WS->import(*Accttjj_nom);
  WS->import(*Accttbb_nom);
  WS->import(*Vis_Xsecttbb_Xsecttjj);
  WS->import(*Full_Xsecttbb_Xsecttjj);
  WS->factory("expr::Effttbbttjj('Effttbb_nom/Effttjj_nom',Effttbb_nom,Effttjj_nom)");
  WS->factory("expr::Accttbbttjj('Accttbb_nom/Accttjj_nom',Accttbb_nom,Accttjj_nom)");
  // Visible Ph-Sp
  WS->factory("prod::Vis_XsecRatiottbbttjj(Effttbbttjj,Vis_Xsecttbb_Xsecttjj)");
  WS->factory("prod::Vis_XsecRatiottbjttjj(2.43276,Vis_XsecRatiottbbttjj)"); // 2.43276 from ttbj/ttbb
  WS->factory("expr::Vis_Nttjj('Vis_Xsecttjj*Lumi_nom', Vis_Xsecttjj,Lumi_nom)");
  WS->factory("prod::Vis_k(Vis_Xsecttjj,0.196)"); // 0.196 from 1/5.1
  // Full Ph-Sp
  WS->factory("prod::Full_XsecRatiottbbttjj(Effttbbttjj,Accttbbttjj,Full_Xsecttbb_Xsecttjj)");
  WS->factory("prod::Full_XsecRatiottbjttjj(2.43276,Full_XsecRatiottbbttjj)"); // 2.43276 from ttbj/ttbb
  WS->factory("expr::Full_Nttjj('Full_Xsecttjj*Lumi_nom', Full_Xsecttjj,Lumi_nom)");
  WS->factory("prod::Full_k(Full_Xsecttjj,0.003891)"); // 0.196 from 1/257.

  WS->import(*NBkgtt);
  WS->import(*NBkgOther);
  WS->import(*NBkgDD);

  // P.D.F.
  WS->import(*HPttbb);
  WS->import(*HPttbj);
  WS->import(*HPttccLF);

  WS->import(*HPBkgtt);
  WS->import(*HPBkgOther);
  WS->import(*HPBkgDD);
    
  // Model Implementation For the Visible Ph-Sp
  WS->factory("SUM::Vis_ttjj(Vis_XsecRatiottbbttjj*HPttbb,Vis_XsecRatiottbjttjj*HPttbj,HPttccLF)");
  WS->factory("SUM::Vis_Model(prod(Vis_Nttjj,Effttjj_nom)*Vis_ttjj,prod(Vis_k,NBkgtt)*HPBkgtt,NBkgOther*HPBkgOther,NBkgDD*HPBkgDD)");

  // Model Implementation For the Full Ph-Sp
  WS->factory("SUM::Full_ttjj(Full_XsecRatiottbbttjj*HPttbb,Full_XsecRatiottbjttjj*HPttbj,HPttccLF)");
  WS->factory("SUM::Full_Model(prod(Full_Nttjj,Effttjj_nom,Accttjj_nom)*Full_ttjj,prod(Full_k,NBkgtt)*HPBkgtt,NBkgOther*HPBkgOther,NBkgDD*HPBkgDD)");

  WS->Print();

  //--------------------------------------------------------------------------------------------------------------
  // Including Error information (Acc, Eff and Lumi) into the Fit
  //--------------------------------------------------------------------------------------------------------------

  RooWorkspace *WS_Sys = new RooWorkspace("Fit ttbb/ttjj including Sys");
  
  // Parameters
  WS_Sys->import(*CSV2);
  WS_Sys->import(*CSV3);
  WS_Sys->import(*arg_CSV);
  // -- Luminosity  
  RooRealVar *Lumi_sys  = new RooRealVar("Lumi_sys",  "Luminosity",  2318.27, 2255.0, 2380.0);
  WS_Sys->import(*Lumi_sys);
  WS_Sys->factory("expr::betaLumi('pow(kappaLumi,alphaLumi)',kappaLumi[1.027],alphaLumi[-5.,5.])");
  // kappaLumi[1.027] is associated to the Lumi error: 2.7
  // alphaLumi is the nuisance parameter
  WS_Sys->factory("prod::Lumi(Lumi_sys,betaLumi)");
  // Efficiencies (Errors from Youngkwon Slide)
  RooRealVar *Effttjj_sys  = new RooRealVar("Effttjj_sys",  "ttjj Efficiency",  0.1165, 0.1047, 0.1283);   // +/- 10.1%
  RooRealVar *Effttbb_sys  = new RooRealVar("Effttbb_sys",  "ttbb Efficiency",  0.2692, 0.2342, 0.3042);   // +/- 13.0%
  WS_Sys->import(*Effttjj_sys);
  WS_Sys->import(*Effttbb_sys);
  // Efficiency for ttjj
  WS_Sys->factory("expr::betaEffttjj('pow(kappaEffttjj,alphaEffttjj)',kappaEffttjj[1.101],alphaEffttjj[-5.,5.])");
  WS_Sys->factory("prod::Effttjj(Effttjj_sys,betaEffttjj)");
  // Efficiency for ttbb
  WS_Sys->factory("expr::betaEffttbb('pow(kappaEffttbb,alphaEffttbb)',kappaEffttbb[1.130],alphaEffttbb[-5.,5.])");
  WS_Sys->factory("prod::Effttbb(Effttbb_sys,betaEffttbb)");
  // Acceptances (Errors from Youngkwon Slide)
  RooRealVar *Accttjj_sys  = new RooRealVar("Accttjj_sys",  "ttjj Acceptance",  0.020, 0.01956, 0.02044);   // +/- 2.2%
  RooRealVar *Accttbb_sys  = new RooRealVar("Accttbb_sys",  "ttbb Acceptance",  0.022, 0.02178, 0.02222);   // +/- 1.0%
  WS_Sys->import(*Accttjj_sys);
  WS_Sys->import(*Accttbb_sys);
  // Acceptance for ttjj
  WS_Sys->factory("expr::betaAccttjj('pow(kappaAccttjj,alphaAccttjj)',kappaAccttjj[1.022],alphaAccttjj[-5.,5.])");
  WS_Sys->factory("prod::Accttjj(Accttjj_sys,betaAccttjj)");
  // Acceptance for ttbb
  WS_Sys->factory("expr::betaAccttbb('pow(kappaAccttbb,alphaAccttbb)',kappaAccttbb[1.010],alphaAccttbb[-5.,5.])");
  WS_Sys->factory("prod::Accttbb(Accttbb_sys,betaAccttbb)");

  WS_Sys->import(*Vis_Xsecttbb_Xsecttjj);
  WS_Sys->import(*Full_Xsecttbb_Xsecttjj);
  WS_Sys->factory("expr::Effttbbttjj('Effttbb/Effttjj',Effttbb,Effttjj)");
  WS_Sys->factory("expr::Accttbbttjj('Accttbb/Accttjj',Accttbb,Accttjj)");
  // Visible Ph-Sp
  WS_Sys->factory("prod::Vis_XsecRatiottbbttjj(Effttbbttjj,Vis_Xsecttbb_Xsecttjj)");
  WS_Sys->factory("prod::Vis_XsecRatiottbjttjj(2.43276,Vis_XsecRatiottbbttjj)"); // 2.43276 from ttbj/ttbb
  WS_Sys->factory("expr::Vis_Nttjj('Vis_Xsecttjj*Lumi', Vis_Xsecttjj,Lumi)");
  WS_Sys->factory("prod::Vis_k(Vis_Xsecttjj,0.196)"); // 0.196 from 1/5.1
  // Full Ph-Sp
  WS_Sys->factory("prod::Full_XsecRatiottbbttjj(Effttbbttjj,Accttbbttjj,Full_Xsecttbb_Xsecttjj)");
  WS_Sys->factory("prod::Full_XsecRatiottbjttjj(2.43276,Full_XsecRatiottbbttjj)"); // 2.43276 from ttbj/ttbb
  WS_Sys->factory("expr::Full_Nttjj('Full_Xsecttjj*Lumi', Full_Xsecttjj,Lumi)");
  WS_Sys->factory("prod::Full_k(Full_Xsecttjj,0.003891)"); // 0.196 from 1/257.

  WS_Sys->import(*NBkgtt);
  WS_Sys->import(*NBkgOther);
  WS_Sys->import(*NBkgDD);

  // P.D.F.
  WS_Sys->import(*HPttbb);
  WS_Sys->import(*HPttbj);
  WS_Sys->import(*HPttccLF);

  WS_Sys->import(*HPBkgtt);
  WS_Sys->import(*HPBkgOther);
  WS_Sys->import(*HPBkgDD);
    
  // Model Implementation For the Visible Ph-Sp
  WS_Sys->factory("SUM::Vis_ttjj(Vis_XsecRatiottbbttjj*HPttbb,Vis_XsecRatiottbjttjj*HPttbj,HPttccLF)");
  WS_Sys->factory("SUM::Vis_ModelNoCon(prod(Vis_Nttjj,Effttjj)*Vis_ttjj,prod(Vis_k,NBkgtt,betaLumi)*HPBkgtt,prod(NBkgOther,betaLumi)*HPBkgOther,NBkgDD*HPBkgDD)");
  WS_Sys->factory("PROD::Vis_Model(Vis_ModelNoCon,Gaussian::ConLumi(alphaLumi,0.0,1.0),Gaussian::ConEffttjj(alphaEffttjj,0.0,1.0),Gaussian::ConEffttbb(alphaEffttbb,0.0,1.0))");

  // Model Implementation For the Full Ph-Sp
  WS_Sys->factory("SUM::Full_ttjj(Full_XsecRatiottbbttjj*HPttbb,Full_XsecRatiottbjttjj*HPttbj,HPttccLF)");
  WS_Sys->factory("SUM::Full_ModelNoCon(prod(Full_Nttjj,Effttjj,Accttjj)*Full_ttjj,prod(Full_k,NBkgtt,betaLumi)*HPBkgtt,prod(NBkgOther,betaLumi)*HPBkgOther,NBkgDD*HPBkgDD)");
  WS_Sys->factory("PROD::Full_Model(Full_ModelNoCon,Gaussian::ConLumi(alphaLumi,0.0,1.0),Gaussian::ConEffttjj(alphaEffttjj,0.0,1.0),Gaussian::ConEffttbb(alphaEffttbb,0.0,1.0),Gaussian::ConAccttjj(alphaAccttjj,0.0,1.0),Gaussian::ConAccttbb(alphaAccttbb,0.0,1.0))");


  WS_Sys->Print();




  // --------------------------------------------
  // Fits
  // --------------------------------------------

  if(Scenario == "Vis_NoNuisance")  WS->pdf("Vis_Model")->fitTo(*DHData);
  if(Scenario == "Full_NoNuisance") WS->pdf("Full_Model")->fitTo(*DHData);

  if(Scenario == "Vis_Nuisance")  WS_Sys->pdf("Vis_Model")->fitTo(*DHData);
  if(Scenario == "Full_Nuisance") WS_Sys->pdf("Full_Model")->fitTo(*DHData);
 
}


