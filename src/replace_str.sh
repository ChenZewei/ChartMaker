grep "LP-FMLP" * -R | awk -F: '{print $1}' | sort | uniq | xargs sed -i 's/LP-FMLP/LP-GFP-FMLP/g'
grep "LP-DPCP" * -R | awk -F: '{print $1}' | sort | uniq | xargs sed -i 's/LP-DPCP/LP-PFP-DPCP/g'
grep "LP-MPCP" * -R | awk -F: '{print $1}' | sort | uniq | xargs sed -i 's/LP-MPCP/LP-PFP-MPCP/g'
grep "LL-EE-vpr" * -R | awk -F: '{print $1}' | sort | uniq | xargs sed -i 's/LL-EE-vpr/LP-EE-vpr/g'
grep "R-PCP-rm" * -R | awk -F: '{print $1}' | sort | uniq | xargs sed -i 's/R-PCP-rm/R-PCP/g'
grep "R-PCP-rm-opa" * -R | awk -F: '{print $1}' | sort | uniq | xargs sed -i 's/R-PCP-rm-opa/R-PCP-opa/g'
grep "R-NP-rm" * -R | awk -F: '{print $1}' | sort | uniq | xargs sed -i 's/R-NP-rm/R-NP/g'
