for k in 125 120 124 126 130 
do
    for m in ggH VBFH WplusH WminusH ZH ttH
    do
        i=$m$k
        root -q -b rapidSkim.C"(\"$i\",\"105160\",2016)"
        root -q -b rapidSkim.C"(\"$i\",\"105160\",2017)"
        root -q -b rapidSkim.C"(\"$i\",\"105160\",2018)"
        root -q -b rapidSkim.C"(\"$i\",\"redTree\",2016)"
        root -q -b rapidSkim.C"(\"$i\",\"redTree\",2017)"
        root -q -b rapidSkim.C"(\"$i\",\"redTree\",2018)"
    done
done

