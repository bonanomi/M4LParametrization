import ROOT
import numpy as np
from collections import OrderedDict as od
import matplotlib.pyplot as plt

INIT_PDF_PARAMS = ['mean', 'sigma', 'a1', 'n1', 'a2', 'n2']
ASSOCIATED_PRODUCTIONS = ["ZH", "WminusH", "WplusH", "ttH"]

def readParams(chan, year, fname = 'file.txt'):
  params = od()
  with open('%s'%fname, 'r') as f: 
    for line in f:
      _line = line.split(' ')
      _name = _line[0].split('_')[0]
      _value = _line[2].split('+')[0]
      _value = float(_value)
      if(_name in params): _name = _name+'_l'
      params[_name] = _value
  params = dict(params)
  print(chan, year, params)
  pdf_pars = []
  for name in PDF_PARAMS:
    pdf_pars.append(params[name])
  return pdf_pars

def loadDataset(path = "redTrees", year = 2017, fs = 1, pmode = "ggH", mass = "125", suffix = "redTree"):
  ZZMass = ROOT.RooRealVar("ZZMass","",105,140)
  if(suffix == "105160"): ZZMass.setRange(105, 160)
  chan= ROOT.RooRealVar("chan","",0,1000)
  weight= ROOT.RooRealVar("weight","",1.e-9,1.e9)

  t = ROOT.TChain("SelectedTree")
  fname = '%s/%s%s_%s_%d.root' %(path,pmode,mass,suffix,year)
  t.Add('%s' %fname)
  data = ROOT.RooDataSet("data", "", t, ROOT.RooArgSet(ZZMass, weight, chan), "chan==%d" %fs, "weight")
  print("--- Dataset Loaded!")
  return ZZMass, data

def npToRrv(params):
  rrvars = []
  for i, p in enumerate(params):
    np = ROOT.RooRealVar("p%d"%i,"",p,0,1e5)
    np.setConstant(1)
    rrvars.append(np)
  return rrvars

def buildPdf(pmode, params):
  # Convert parameters to RooRealVars to build pdfs
  roofitPars = npToRrv(params)
  if pmode not in ASSOCIATED_PRODUCTIONS:
    mean,sigma,a1,n1,a2,n2 = roofitPars
    pdf = ROOT.RooDoubleCBFast("pdf", "pdf", ZZMass, mean,sigma,a1,n1,a2,n2);
  else:
    mean,sigma,a1,n1,a2,n2,mean_l,sigma_l,frac = roofitPars
    dcb = ROOT.RooDoubleCBFast("dcb", "dcb", ZZMass, mean,sigma,a1,n1,a2,n2);
    landau = ROOT.RooLandau("landau", "landau", ZZMass, mean_l, sigma_l)
    frac = ROOT.RooFormulaVar("frac", "frac", "@0", ROOT.RooArgList(frac))
    pdf = ROOT.RooAddPdf("pdf", "pdf", ROOT.RooArgList(dcb,landau), ROOT.RooArgList(frac))

  return pdf

pmodes = ["ggH", "VBFH", "WminusH", "WplusH", "ttH"]
pmodes = ["ttH", "WminusH", "WplusH", "ZH"]
pmodes = ["ggH", "VBFH"]
CHANNELS = ["4mu", "4e", "2e2mu"]
YEAR = 2017

for pmode in pmodes:
  for i, ch in enumerate(CHANNELS):
    PDF_PARAMS = ['mean', 'sigma', 'a1', 'n1', 'a2', 'n2'] 
    if pmode in ASSOCIATED_PRODUCTIONS:
      PDF_PARAMS.extend(['mean_l', 'sigma_l', 'frac'])

    print(pmode, ch, YEAR)
    ZZMass, dataset = loadDataset(path = "redTrees", year = YEAR, fs = i+1, pmode = pmode, mass = "125", suffix = "redTree")

    #fname='/eos/user/m/mbonanom/HIG-21-009/sim_massParam_%s_%s_%d.txt' %(pmode, ch, YEAR)
    fname='/eos/user/m/mbonanom/www/Postdoc/HIG-21-009/param/m4l_105_140/fitParams/sim_massParam_HIG21009PDF_redTree_%s_%d.txt' %(ch,YEAR)
    params = readParams(ch, YEAR, fname = fname)

    #pdf = buildPdf(params)
     
    roofitPars = npToRrv(params)
    if pmode not in ASSOCIATED_PRODUCTIONS:
      mean,sigma,a1,n1,a2,n2 = roofitPars
      pdf = ROOT.RooDoubleCBFast("pdf", "pdf", ZZMass, mean,sigma,a1,n1,a2,n2);
    else:
      mean,sigma,a1,n1,a2,n2,mean_l,sigma_l,frac = roofitPars
      dcb = ROOT.RooDoubleCBFast("dcb", "dcb", ZZMass, mean,sigma,a1,n1,a2,n2);
      landau = ROOT.RooLandau("landau", "landau", ZZMass, mean_l, sigma_l)
      frac = ROOT.RooFormulaVar("frac", "frac", "@0", ROOT.RooArgList(frac))
      pdf = ROOT.RooAddPdf("pdf", "pdf", ROOT.RooArgList(dcb,landau), ROOT.RooArgList(frac))
    pdf.Print("V")
    print(pdf.createIntegral(ROOT.RooArgSet(ZZMass)).getVal())
    c = ROOT.TCanvas()
    frame = ZZMass.frame()
    dataset.plotOn(frame)
    pdf.plotOn(frame)#, ROOT.RooFit.LineColor(2))
    #landau.plotOn(frame, ROOT.RooFit.LineColor(2))
    #dcb.plotOn(frame, ROOT.RooFit.LineColor(3))
    frame.Draw()
    c.Modified()
    c.Update()

    figName = "m4lFit_%s_%s_%d.png" %(pmode, ch, YEAR)
    c.SaveAs(figName)    
