/*
** ROOT macro to create lightweight version of analysis TTrees to be used
** the signal parametrization. Similarly to the fitting macros:
** suffix = redTree to use TTrees in [105,140] GeV
** suffix = 105160 to use TTrees in [105, 160] GeV
** The output TTrees contain the following branches:
** ZZPt, ZZMass, weight, chan (i.e. final state)
** To run:
** root -q -b rapidSkim.C
*/
void rapidSkim(TString inputname="ggH125", TString suffix="redTree", int year = 2018, bool isPreVFP=false){
    float _lumi = 1.0;
    if(year == 2016) {
        _lumi = 35.9;
    }
    else if (year == 2017) {
        _lumi = 41.5;
    }
    else {
        _lumi = 59.7;
    }

    TFile *input_file;
    TString fullpath = Form("");

    TString input_dir = "/eos/cms/store/group/phys_higgs/cmshzz4l/cjlst/RunIIUL";
    TString output_dir = "./redTrees";

    if (inputname.Contains("AllData")) {
        fullpath = Form("%s/data%i/%s/ZZ4lAnalysis.root", input_dir.Data(), year, inputname.Data());
    }
    if (!inputname.Contains("AllData"))  {
        if((year==2016) && (isPreVFP)){
            fullpath = Form("%s/MC%ipre/%s/ZZ4lAnalysis.root", input_dir.Data(), year, inputname.Data());
        } else if ((year==2016) && !(isPreVFP)){
            fullpath = Form("%s/MC%ipost/%s/ZZ4lAnalysis.root", input_dir.Data(), year, inputname.Data());
        } else {
            fullpath = Form("%s/MC%i/%s/ZZ4lAnalysis.root", input_dir.Data(), year, inputname.Data());
        }
    }

    std::cout << "Reading file : " << fullpath << std::endl;
    input_file = TFile::Open(fullpath.Data());

    TH1F *hCounters;
    float gen_sum_weights ;
    if (!inputname.Contains("AllData")){
        hCounters= (TH1F*)input_file->Get("ZZTree/Counters");
        gen_sum_weights = hCounters->GetBinContent(40);
    }
    input_file->Close();
    std::cout << "Sum of weights = " << gen_sum_weights << std::endl;

    TChain *tsig;
    tsig= new TChain("ZZTree/candTree");
    tsig->Add(fullpath.Data());

    float ZZPt, ZZsel, ZZMass;
    float xsec,overallEventWeight;
    float ggH_NNLOPS_weight;

    short Z1Flav,Z2Flav;

    Float_t L1prefiringWeight = 0;
    Float_t dataMCWeight = 0;

    tsig->SetBranchStatus("*",0);
    tsig->SetBranchAddress("L1prefiringWeight", &L1prefiringWeight);

    if (!inputname.Contains("AllData")){
        tsig->SetBranchAddress("dataMCWeight", &dataMCWeight);
    }

    tsig->SetBranchAddress("ZZPt",&ZZPt);
    tsig->SetBranchAddress("ZZMass",&ZZMass);
    tsig->SetBranchAddress("Z1Flav",&Z1Flav);
    tsig->SetBranchAddress("Z2Flav",&Z2Flav);
    tsig->SetBranchAddress("xsec",&xsec);
    tsig->SetBranchAddress("ZZsel",&ZZsel);
    tsig->SetBranchAddress("overallEventWeight",&overallEventWeight);
    if (inputname.Contains("ggH") )
        tsig->SetBranchAddress("ggH_NNLOPS_weight",&ggH_NNLOPS_weight);

    float weight;
    int chan;

    TFile *fnew = new TFile(Form("%s/%s_%s_%d.root",output_dir.Data(),inputname.Data(),suffix.Data(),year),"recreate");
    TTree *tnew = new TTree("SelectedTree","SelectedTree");
    tnew->Branch("ZZPt",&ZZPt);
    tnew->Branch("ZZMass",&ZZMass);
    tnew->Branch("weight",&weight,"weight/F");
    tnew->Branch("chan",&chan,"chan/I");

    for(int i=0;i<tsig->GetEntries();i++){
        tsig->GetEntry(i);

        if (i%1000==0)
            cout<<i<<endl;
        if (ZZsel<0)
            continue;
        if((suffix=="105160") && ((ZZMass<105) || (ZZMass>160)))
            continue;
        if((suffix!="105160") && ((ZZMass<105) || (ZZMass>140)))
            continue;

        // Define decay channel
        // 1: 4mu, 2: 4e, 3: 2e2mu
        if(abs(Z1Flav)==abs(Z2Flav) && abs(Z1Flav)==121){
            chan=2;
        }
        else if (abs(Z1Flav)==abs(Z2Flav) && abs(Z1Flav)!=121){
            chan=1;
        }
        else{
            chan=3;
        }

        if(xsec<=0) 
            xsec=1;

        if (!inputname.Contains("AllData")){
            weight= L1prefiringWeight * xsec* overallEventWeight/gen_sum_weights*_lumi*1000;
        }
        if (inputname.Contains("ggH")) {
            weight *= ggH_NNLOPS_weight;
        }

        tnew->Fill();
    }
    fnew->cd();
    fnew->Write();
    fnew->Close();
}
