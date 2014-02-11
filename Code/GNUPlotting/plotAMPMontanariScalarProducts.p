########################################################################################
###                                                                                  ###
###   Plot scalar product of NLPI Algorithm with ground-truth (using Montanari SBM)  ###
###                                                                                  ###
########################################################################################

reset

# produce postscript (.eps) output plot
set term postscript eps enhanced color size 7,5
set output "NLPIMontanariSyntheticDataScalarProducts.eps"

set border 3 linewidth 1

unset key

set xlabel "{/Symbol e}" font "sans-serif, 24"
set ylabel "{/Symbol l}" font "sans-serif, 24"

set xtics nomirror font "sans-serif, 18"
set ytics nomirror font "sans-serif, 18"

# note: NOIT = 100
XMIN = 0.02 # = XMAX/NOIT
XMAX = 0.992
YMIN = 0.012 # = YMAX/NOIT
YMAX = 1.2

set xrange [XMIN:XMAX]
set yrange [YMIN:YMAX]

set palette rgb 21,22,23;
#set palette model XYZ rgbformulae 7,5,15

set cbrange [ 0 : 1 ] noreverse nowriteback

plot 'AMPMontanariSyntheticDataScalarProducts.dat' using 1:2:3 with image


#######################################################################################
###                                                                                 ###
###   Plot scalar product of AMP Algorithm with ground-truth (using Montanari SBM)  ###
###                                                                                 ###
#######################################################################################

reset

# produce postscript (.eps) output plot
set term postscript eps enhanced color size 7,5
set output "AMPMontanariSyntheticDataScalarProducts.eps"

set border 3 linewidth 1

unset key

set xlabel "{/Symbol e}" font "sans-serif, 24"
set ylabel "{/Symbol l}" font "sans-serif, 24"

set xtics nomirror font "sans-serif, 18"
set ytics nomirror font "sans-serif, 18"

# note: NOIT = 100
XMIN = 0.02 # = XMAX/NOIT
XMAX = 0.992
YMIN = 0.012 # = YMAX/NOIT
YMAX = 1.2

set xrange [XMIN:XMAX]
set yrange [YMIN:YMAX]

set palette rgb 21,22,23;
#set palette model XYZ rgbformulae 7,5,15

set cbrange [ 0 : 1 ] noreverse nowriteback

plot 'AMPMontanariSyntheticDataScalarProducts.dat' using 1:2:4 with image

