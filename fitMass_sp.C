/*
ROOT macro that runs a double sided CB fit at a given mH value.
This script can be used for qualitative checks, but does not reflect the
way in which the parametrization at mH=125 GeV used in the analysis is obtained.
All the pdf parameters are expressed with a linear dependence in mH.
The input TTrees are produced with rapidSkim.C
To run:
root -q -b fitMass_sp.C
*/

void fitMass_sp(int year=2017, TString pmode = "ggH", TString mass = "130"){

    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);

    TString chanName[3]={"4mu","4e","2e2mu"};

    RooRealVar *ZZMass =new RooRealVar("ZZMass","",105,140);
    RooRealVar *chan=new RooRealVar("chan","",0,1000);
    RooRealVar *weight=new RooRealVar("weight","",1.e-9,1.e9);

    for (int j =0;j<3;j++){
        ofstream outmass ;
        outmass.open(Form("sim_massParam_%s_%d_%d_%s.txt",pmode.Data(),j,year,mass.Data()));
        TChain *t = new TChain ("SelectedTree");
        t->Add(Form("%s%s_redTree_%d.root",pmode.Data(),mass.Data(),year));
        RooDataSet *data = new RooDataSet("data","",t,RooArgSet(*ZZMass,*weight,*chan),Form("chan==%d",j+1),"weight");
        
        RooRealVar *a1_1=new RooRealVar("a1_1","",0.00111,-0.2,0.2);
        RooRealVar *a1_2=new RooRealVar("a1_2","",0.00146,-0.1,0.1);
        RooRealVar *a1_0=new RooRealVar("a1_0","",1.2917,1.,5.);
        RooRealVar *a2_1=new RooRealVar("a2_1","",0.00130,-0.2,0.2);
        RooRealVar *a2_2=new RooRealVar("a2_2","",0.00127,-0.1,0.1);
        RooRealVar *a2_0=new RooRealVar("a2_0","",1.8125,1.,5.);
        RooRealVar *n1_1=new RooRealVar("n1_1","",0.00138,-0.2,0.2);
        RooRealVar *n1_2=new RooRealVar("n1_2","",0.00185,-0.1,0.1);
        RooRealVar *n1_0=new RooRealVar("n1_0","",2.0312,1.,7.);
        RooRealVar *n2_1=new RooRealVar("n2_1","",0.001,-0.2,0.2);
        RooRealVar *n2_2=new RooRealVar("n2_2","",-0.001,-0.1,0.1);
        RooRealVar *n2_0=new RooRealVar("n2_0","",3.0369,1.,5.);
        /*
        RooRealVar *a1_1=new RooRealVar("a1_1","",0.002,-0.3,0.3);
        RooRealVar *a1_0=new RooRealVar("a1_0","",1.5,1.,30.);
        RooRealVar *a2_1=new RooRealVar("a2_1","",0.003,-0.3,0.3);
        RooRealVar *a2_0=new RooRealVar("a2_0","",1.5,0.5,30.);
        RooRealVar *n1_1=new RooRealVar("n1_1","",0.002,-0.3,0.3);
        RooRealVar *n1_0=new RooRealVar("n1_0","",2.0,1.,40.);
        RooRealVar *n2_1=new RooRealVar("n2_1","",0.001,-0.3,0.3);
        RooRealVar *n2_0=new RooRealVar("n2_0","",3.0,0.,10.);
        */
        RooRealVar *mean_1=new RooRealVar("mean_1","",1,0,1.5);
        RooRealVar *mean_0=new RooRealVar("mean_0","",124.9,120.,130);

        RooRealVar *sigma_1=new RooRealVar("sigma_1","",0.01,-0.3,0.3);
        RooRealVar *sigma_0=new RooRealVar("sigma_0","",1.3,0.1,4.);

        //RooRealVar *mean_1=new RooRealVar("mean_1","",1,0,1.5);
        //RooRealVar *mean_0=new RooRealVar("mean_0","",124.8424,120.,130);

        //RooRealVar *sigma_1=new RooRealVar("sigma_1","",0.00860,-0.5,0.5);
        //RooRealVar *sigma_0=new RooRealVar("sigma_0","",1.5,0.5,3.);

        RooRealVar* mean_l_0 = new RooRealVar("landau_mean_0","",130,110,140);
        RooRealVar* mean_l_1 = new RooRealVar("landau_mean_1","",0,-1.5,1.5);
        RooRealVar* sigma_l_0 = new RooRealVar("landau_sigma_0","",15,2,20);
        RooRealVar* sigma_l_1 = new RooRealVar("landau_sigma_1","",0.,-1,1);

        RooRealVar* frac_0 = new RooRealVar("frac_0","",0.65,0,1);
        
        RooConstVar *MH=new RooConstVar("MH","",125);

        RooFormulaVar* a1=new RooFormulaVar(Form("a1_125"),"","@0+@1*(MH-125)",RooArgList(*a1_0,*a1_1,*MH));
        RooFormulaVar* a2=new RooFormulaVar(Form("a2_125"),"","@0+@1*(MH-125)",RooArgList(*a2_0,*a2_1,*MH));
        RooFormulaVar* n1=new RooFormulaVar(Form("n1_125"),"","@0+@1*(MH-125)",RooArgList(*n1_0,*n1_1,*MH));
        RooFormulaVar* n2=new RooFormulaVar(Form("n2_125"),"","@0+@1*(MH-125)",RooArgList(*n2_0,*n2_1,*MH));
        RooFormulaVar* mean=new RooFormulaVar(Form("mean_125"),"","@0+@1*(MH-125)",RooArgList(*mean_0,*mean_1,*MH));
        RooFormulaVar* sigma=new RooFormulaVar(Form("sigma_125"),"","@0+@1*(MH-125)",RooArgList(*sigma_0,*sigma_1,*MH));
        RooFormulaVar* sigma_l=new RooFormulaVar(Form("sigma_l_125"),"","@0+@1*(MH-125)",RooArgList(*sigma_l_0,*sigma_l_1,*MH));
        RooFormulaVar* mean_l=new RooFormulaVar(Form("mean_l_125"),"","@0+@1*(MH-125)",RooArgList(*mean_l_0,*mean_l_1,*MH));

        RooFormulaVar* frac=new RooFormulaVar("frac","","@0",RooArgList(*frac_0));

        RooDoubleCBFast *cpdftmp = new RooDoubleCBFast(Form("DCBall_125"),"",*ZZMass,*mean_0,*sigma_0,*a1_0,*n1_0,*a2_0,*n2_0);
        RooLandau *pdf_landautmp = new RooLandau("landautmp","",*ZZMass, *mean_l_0, *sigma_l_0);
        RooAddPdf *addpdftmp = new RooAddPdf("apdftmp","",RooArgList(*cpdftmp,*pdf_landautmp),*frac);
        if (pmode.Contains("Wplus")|| pmode.Contains("Wminus") || pmode.Contains("ZH"))
            addpdftmp->fitTo(*data,SumW2Error(kFALSE),InitialHesse(true),Strategy(2));
        else{
            cpdftmp->fitTo(*data,SumW2Error(kFALSE),InitialHesse(true),Strategy(2));
        }

        TString a1outs= Form ("a1_%s_%d \t %.4f",chanName[j].Data(),year,a1_0->getVal());
        TString a2outs= Form ("a2_%s_%d \t %.4f",chanName[j].Data(),year,a2_0->getVal());
        TString n1outs= Form ("n1_%s_%d \t %.4f",chanName[j].Data(),year,n1_0->getVal());
        TString n2outs= Form ("n2_%s_%d \t %.4f",chanName[j].Data(),year,n2_0->getVal());
        TString meanouts= Form ("mean_%s_%d \t %.4f",chanName[j].Data(),year,mean_0->getVal());
        TString sigmaouts= Form ("sigma_%s_%d \t %.4f",chanName[j].Data(),year,sigma_0->getVal());

        outmass<< a1outs<<endl;
        outmass<< a2outs<<endl;
        outmass<< meanouts<<endl;
        outmass<< sigmaouts<<endl;
        outmass<< n1outs<<endl;
        outmass<< n2outs<<endl;
        outmass.close();
        RooPlot *frame = ZZMass->frame();
        TString cat_type =Form("mh125");
        data->plotOn(frame);
        cpdftmp->plotOn(frame); //,Slice(massrc,cat_type),ProjWData(massrc,*data_all)) ;
        frame->Draw();
        gPad->Print(Form("simFit_%s.pdf",chanName[j].Data()));
    }
}

