############################################################################
###                                                                      ###
###   Plot Fast Newman Method Renormalised Filtered Correlation Matrix   ###
###                                                                      ###
############################################################################

reset

# produce postscript (.ps) output plot
set term postscript eps enhanced color size 6,5
set output "renormalisedFilteredCorrelationMatrixFastNewmanMethod.eps"

set border linewidth 0
unset key

# note: n = 2
set xrange [-0.5:1.5]
set yrange [-0.5:1.5]

unset xtics
unset ytics

set format x ""
set format y ""

load 'default.plt'

set cbrange [ -1 : 1 ] noreverse nowriteback

plot 'renormalisedFilteredCorrelationMatrixFastNewmanMethod.dat' matrix with image


##########################################################################
###                                                                    ###
###   Plot Montanari Method Renormalised Filtered Correlation Matrix   ###
###                                                                    ###
##########################################################################

reset

# produce postscript (.ps) output plot
set term postscript eps enhanced color size 6,5
set output "renormalisedFilteredCorrelationMatrixMontanariMethod.eps"

set border linewidth 0
unset key

# note: n = 2
set xrange [-0.5:1.5]
set yrange [-0.5:1.5]

unset xtics
unset ytics

set format x ""
set format y ""

load 'default.plt'

set cbrange [ -1 : 1 ] noreverse nowriteback

plot 'renormalisedFilteredCorrelationMatrixMontanariMethod.dat' matrix with image


##########################################################################
###                                                                    ###
###   Plot Montanari Method Renormalised Filtered Correlation Matrix   ###
###                                                                    ###
##########################################################################

reset

# produce postscript (.ps) output plot
set term postscript eps enhanced color size 6,5
set output "renormalisedFilteredCorrelationMatrixFinancialSpectralClusteringMethod.eps"

set border linewidth 0
unset key

# note: n = 4
set xrange [-0.5:3.5]
set yrange [-0.5:3.5]

unset xtics
unset ytics

set format x ""
set format y ""

load 'default.plt'

set cbrange [ -1 : 1 ] noreverse nowriteback

plot 'renormalisedFilteredCorrelationMatrixFinancialSpectralClusteringMethod.dat' matrix with image
