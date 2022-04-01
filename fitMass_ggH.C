/*
ROOT macro that runs a simultaneous fit over 5 mass points to parametrize m4l.
All the pdf parameters are expressed with a linear dependence in mH.
A pdf for each bin of pT(H) considered in the analysis is defined.
suffix = redTree to use TTrees in [105,140] GeV
suffix = 105160 to use TTrees in [105, 160] GeV
The input TTrees are produced with rapidSkim.C
To run:
root -q -b fitMass_ggH.C
*/

void fitMass_ggH(int year=2016, TString pmode = "ggH", TString suffix = "redTree"){
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1111);

    TString chanName[3]={"4mu","4e","2e2mu"};
    std::cout<<"------------------------------"<<year<<std::endl;
    int ntot=0.0;
    if(year==2016){
        ntot = 4;
    } else {
        ntot = 5;
    }

    // Point at 120GeV not available for 2016
    // Set at the end of array so that it's skipped for 2016 (ntot=4)
    int mass[5]={125,124,126,130,120}; 

    RooRealVar *ZZMass =new RooRealVar("ZZMass","",105,140);
    if(suffix == "105160") {ZZMass->setRange(105, 160);}
    RooRealVar *chan=new RooRealVar("chan","",0,1000);
    RooRealVar *weight=new RooRealVar("weight","",1.e-9,1.e9);
    RooRealVar *ZZPt=new RooRealVar("ZZPt", "",1.e-9,1.e4);

    // pT(H) differential bins.
    // This macro produces a p.d.f. in each of these bins.
    int boundaries[10] = {0,10,20,30,45,60,80,120,200,13000};

    for (int _bin=0; _bin<9; _bin++){
        for (int j =0;j<3;j++){
            ofstream outmass;
            outmass.open(Form("sim_massParam_%s_%s_%s_%d_%d_%d.txt",pmode.Data(), suffix.Data(), chanName[j].Data(),year,boundaries[_bin], boundaries[_bin+1]));
            RooDataSet *data[ntot];
            RooDataSet *data_red[ntot];
            for (int i =0;i<ntot;i++){
                TChain *t = new TChain ("SelectedTree");
                t->Add(Form("redTrees/%s%d_%s_%d.root",pmode.Data(),mass[i],suffix.Data(),year));

                data[i]=new RooDataSet(Form("data%d_%d",i,mass[i]),"",t,RooArgSet(*ZZMass,*ZZPt,*weight,*chan),Form("chan==%d",j+1),"weight");
                data_red[i] = (RooDataSet *)data[i]->reduce(Form("ZZPt>%d && ZZPt<=%d", boundaries[_bin], boundaries[_bin+1]));
            }
    
            RooCategory massrc("massrc","");
            RooSimultaneous simPdf("simPdf","simultaneous pdf",massrc) ;
    
            RooDataSet* data_stage_cat[ntot]; 
            RooDataSet* data_all;
            RooDoubleCBFast* cpdf[ntot]; 
            RooAddPdf* addpdf[ntot]; 
    
            for (int i=0;i<ntot;i++){
                TString cat_type =Form("mh%d",mass[i]);
                massrc.defineType(cat_type,mass[i]);
            }
                    
            RooRealVar *a1_1=new RooRealVar("a1_1","",0.002,-0.3,0.3);
            RooRealVar *a1_0=new RooRealVar("a1_0","",1.5,1.,30.);
            RooRealVar *a2_1=new RooRealVar("a2_1","",0.003,-0.3,0.3);
            RooRealVar *a2_0=new RooRealVar("a2_0","",1.5,0.5,30.);
            RooRealVar *n1_1=new RooRealVar("n1_1","",0.002,-0.3,0.3);
            RooRealVar *n1_0=new RooRealVar("n1_0","",2.0,1.,40.);
            RooRealVar *n2_1=new RooRealVar("n2_1","",0.001,-0.3,0.3);
            RooRealVar *n2_0=new RooRealVar("n2_0","",3.0,0.,10.);
    
            RooRealVar *mean_1=new RooRealVar("mean_1","",1,0,1.5);
            RooRealVar *mean_0=new RooRealVar("mean_0","",124.9,120.,130);
    
            RooRealVar *sigma_1=new RooRealVar("sigma_1","",0.01,-0.3,0.3);
            RooRealVar *sigma_0=new RooRealVar("sigma_0","",1.3,0.1,4.);
            
            RooRealVar* mean_l_0 = new RooRealVar("landau_mean_0","",130,110,140);
            RooRealVar* mean_l_1 = new RooRealVar("landau_mean_1","",1,-1.5,1.5);
            RooRealVar* sigma_l_0 = new RooRealVar("landau_sigma_0","",9,2,20);
            RooRealVar* sigma_l_1 = new RooRealVar("landau_sigma_1","",0.,-1,1);
            RooRealVar* frac_0 = new RooRealVar("frac_0","",0.9,0,1);
            RooRealVar* frac_1 = new RooRealVar("frac_1","",0.1,0.,1);
            
     
            for (int i=0;i<ntot;i++){
                RooConstVar *MH=new RooConstVar("MH","",mass[i]);
                RooFormulaVar* a1=new RooFormulaVar(Form("a1_%d",mass[i]),"","@0+@1*(MH-125)",RooArgList(*a1_0,*a1_1,*MH));
                RooFormulaVar* a2=new RooFormulaVar(Form("a2_%d",mass[i]),"","@0+@1*(MH-125)",RooArgList(*a2_0,*a2_1,*MH));
                RooFormulaVar* n1=new RooFormulaVar(Form("n1_%d",mass[i]),"","@0+@1*(MH-125)",RooArgList(*n1_0,*n1_1,*MH));
                RooFormulaVar* n2=new RooFormulaVar(Form("n2_%d",mass[i]),"","@0+@1*(MH-125)",RooArgList(*n2_0,*n2_1,*MH));
                RooFormulaVar* mean=new RooFormulaVar(Form("mean_%d",mass[i]),"","@0+@1*(MH-125)",RooArgList(*mean_0,*mean_1,*MH));
                RooFormulaVar* sigma=new RooFormulaVar(Form("sigma_%d",mass[i]),"","@0+@1*(MH-125)",RooArgList(*sigma_0,*sigma_1,*MH));
                RooFormulaVar* sigma_l=new RooFormulaVar(Form("sigma_l_%d",mass[i]),"","@0+@1*(MH-125)",RooArgList(*sigma_l_0,*sigma_l_1,*MH));
                RooFormulaVar* mean_l=new RooFormulaVar(Form("mean_l_%d",mass[i]),"","@0+@1*(MH-125)",RooArgList(*mean_l_0,*mean_l_1,*MH));
                RooFormulaVar* frac0=new RooFormulaVar(Form("frac0_%d",mass[i]),"","@0",RooArgList(*frac_0));
                RooFormulaVar* frac1=new RooFormulaVar(Form("frac1_%d",mass[i]),"","@0",RooArgList(*frac_1));
    
                TString cat_type =Form("mh%d",mass[i]);
                data_stage_cat[i] = new RooDataSet(Form("data_%d",i),"",RooArgSet(*ZZMass,*weight),Index(massrc),Import(cat_type,*data_red[i]),WeightVar("weight")); 
                cpdf[i] = new RooDoubleCBFast(Form("DCBall_%d",i),"",*ZZMass,*mean,*sigma,*a1,*n1,*a2,*n2);
                RooLandau *pdf_landau = new RooLandau("landau","",*ZZMass, *mean_l, *sigma_l);
                addpdf[i] = new RooAddPdf(Form("apdf_%d",i),"",RooArgList(*cpdf[i],*pdf_landau),RooArgList(*frac0),true);
                cout<< data_stage_cat[i]->sumEntries()<<endl;
                
                // Additional Landau contribution for VH and ttH
                if (pmode.Contains("Wplus")|| pmode.Contains("WHMinus") || pmode.Contains("ZH") || pmode.Contains("ttH"))
                    simPdf.addPdf(*addpdf[i],cat_type);
                else
                    simPdf.addPdf(*cpdf[i],cat_type);
    
                if(i==0)
                    data_all = data_stage_cat[i];
                else
                    data_all->append(*data_stage_cat[i]);
    
                if(i==0){
                    RooDoubleCBFast *cpdftmp = new RooDoubleCBFast("DCBall","",*ZZMass,*mean_0,*sigma_0,*a1_0,*n1_0,*a2_0,*n2_0);
                    RooLandau *pdf_landautmp = new RooLandau("landautmp","",*ZZMass, *mean_l_0, *sigma_l_0);
                    RooAddPdf *addpdftmp = new RooAddPdf(Form("apdf_%d",i),"",RooArgList(*cpdftmp,*pdf_landautmp),RooArgList(*frac0),true);
                    if (pmode.Contains("Wplus")|| pmode.Contains("WHMinus") || pmode.Contains("ZH") || pmode.Contains("ttH"))
                        addpdftmp->fitTo(*data_stage_cat[i],SumW2Error(kFALSE),InitialHesse(true),Strategy(2));
                    else{
                        cpdftmp->fitTo(*data_stage_cat[i],SumW2Error(kFALSE),InitialHesse(true),Strategy(2));            
                    }
                    sigma_0->setConstant(1);
                    mean_0->setConstant(1);
                    mean_l_0->setConstant(1);
                }
            }
            simPdf.fitTo(*data_all,SumW2Error(kFALSE),InitialHesse(true),Strategy(2)) ;
    
            TString a1outs= Form ("a1_%s_%d \t %.4f+%.5f*(MH-125)",chanName[j].Data(),year,a1_0->getVal(),a1_1->getVal());
            TString a2outs= Form ("a2_%s_%d \t %.4f+%.5f*(MH-125)",chanName[j].Data(),year,a2_0->getVal(),a2_1->getVal());
            TString n1outs= Form ("n1_%s_%d \t %.4f+%.5f*(MH-125)",chanName[j].Data(),year,n1_0->getVal(),n1_1->getVal());
            TString n2outs= Form ("n2_%s_%d \t %.4f+%.5f*(MH-125)",chanName[j].Data(),year,n2_0->getVal(),n2_1->getVal());
            TString meanouts= Form ("mean_%s_%d \t %.4f+%.5f*(MH-125)",chanName[j].Data(),year,mean_0->getVal(),mean_1->getVal());
            TString sigmaouts= Form ("sigma_%s_%d \t %.4f+%.5f*(MH-125)",chanName[j].Data(),year,sigma_0->getVal(),sigma_1->getVal());
    
            outmass<< a1outs<<endl;
            outmass<< a2outs<<endl;
            outmass<< meanouts<<endl;
            outmass<< sigmaouts<<endl;
            outmass<< n1outs<<endl;
            outmass<< n2outs<<endl;
    
            // Additional Landau contribution for WH and ZH
            if (pmode.Contains("Wplus")|| pmode.Contains("WHMinus") || pmode.Contains("ZH") || pmode.Contains("ttH")){
                outmass<<Form ("mean_l_%s_%d \t %.4f+%.5f*(MH-125)",chanName[j].Data(),year,mean_l_0->getVal(),mean_l_1->getVal())<<endl;
                outmass<<Form ("sigma_l_%s_%d \t %.4f+%.5f*(MH-125)",chanName[j].Data(),year,sigma_l_0->getVal(),sigma_l_1->getVal())<<endl;
                outmass<<Form ("frac_%s_%d \t %.4f",chanName[j].Data(),year,frac_0->getVal())<<endl;
            }
    
            for (int i =0;i<ntot;i++){
                if(mass[i]!=125){continue;} // Save only plots at 125 GeV
                RooPlot *frame = ZZMass->frame();
                TString cat_type =Form("mh%d",mass[i]);
                data_all->plotOn(frame,Cut(Form("massrc==massrc::mh%d",mass[i]))) ;
                simPdf.plotOn(frame,Slice(massrc,cat_type),ProjWData(massrc,*data_all)) ;                    
                frame->Draw();
                            
                gPad->Print(Form("simFit_%s_%s_%d_%s_%d_%d_%d.png",pmode.Data(),suffix.Data(),mass[i],chanName[j].Data(),year, boundaries[_bin], boundaries[_bin+1]));
            }
            outmass.close();
        }
    }
}

