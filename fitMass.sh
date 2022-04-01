for k in 2016 2017 2018
do
  for pmode in VBFH ggH WminusH WplusH ttH ZH
  do
     # Run fits for both [105,160] and [105,140] GeV masss ranges
     root -q -b fitMass.C"($k,\"$pmode\",\"105160\")"
     root -q -b fitMass.C"($k,\"$pmode\",\"redTree\")"
  done
done
