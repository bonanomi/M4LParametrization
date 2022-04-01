# HIG-21-009: Signal parametrization

This repository contains scripts to produce the signal parametrization used in
the HIG-21-009 analysis (more generally, in the ZZ4l analyses).

The workflow is defined as follows:

 * Run `reduceTrees.sh`: this script will run the `rapidSkim.C` macro to produce
 lightweight TTrees used in the fitting procedure. These TTrees contain only the
 branches necessary to this scope, i.e.: `ZZPt`, `ZZMass`, `weight`, `chan` (i.e. final state).

 * Run `fitMass.sh`: this script will run the `fitMass.C` macro to produce the
 lineshape parametrization for all the production modes, final states, and data
 taking periods considered in the analysis. More in detail, the `fitMass.C` macro
 performs a simultaneous fit over 5 mass points (120-130 GeV) to parametrize the
 four-leptons resonance by means of a double sided Crystall-Ball function.

These two steps produce `TTree`s and signal line shapes for m4l resonance in
[105,140] GeV (range currently used in the analyses), as well as [105, 160] GeV 
(range under investigation, as it may allow measurements of background normalizations etc).

The notebook `HIG-21-009_Parametrization_Plotter` can be used to perform the final
studies on the parametrization (e.g. comparing `ggH` to the inclusive `H(125)` resonance)
and to produce nice plots. More details can be found in the notebook.

Two additional macros are present:
 
 * `fitMass_sp.C` can be used to perform a quick fit of the four-leptons resonance
 for a given `mH` point. Note that running this macro at `mH=125` GeV does not
 yield in the signal lineshape parametrization used in the analysis. For that,
 follow the steps above;

 * `fitMass_ggH.C` can be used to perform the simultaneous fit for `ggH` in differential
 bins of `pT(H)`. Currently, this parametrization is not used in the analysis,
 but it may be used in future versions, to enhance sensitivity to BSM/EFT effects.
 The `HIG-21-009_Parametrization_pTHBins` notebook can be used as a plotter for this scenario.