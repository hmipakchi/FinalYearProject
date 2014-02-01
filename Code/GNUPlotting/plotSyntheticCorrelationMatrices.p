###############################################
###                                         ###
###   Plot Synthetic Correlation Matrix 1   ###
###                                         ###
###############################################

reset

# produce postscript (.ps) output plot
set term postscript eps enhanced color size 6,5
set output "syntheticCorrelationMatrices_1.eps"

set border linewidth 0
unset key

# note: n = 100
set xrange [-0.5:99.5]
set yrange [-0.5:99.5]

unset xtics
unset ytics

set format x ""
set format y ""

load 'default.plt'

set cbrange [ -1 : 1 ] noreverse nowriteback

plot 'syntheticCorrelationMatrices_1.dat' matrix with image

###############################################
###                                         ###
###   Plot Synthetic Correlation Matrix 2   ###
###                                         ###
###############################################

reset

# produce postscript (.ps) output plot
set term postscript eps enhanced color size 6,5
set output "syntheticCorrelationMatrices_2.eps"

set border linewidth 0
unset key

# note: n = 100
set xrange [-0.5:99.5]
set yrange [-0.5:99.5]

unset xtics
unset ytics

set format x ""
set format y ""

load 'default.plt'

set cbrange [ -1 : 1 ] noreverse nowriteback

plot 'syntheticCorrelationMatrices_2.dat' matrix with image

reset