##################################################################
###                                                            ###
###   Plot Expected Returns and Volatility of FTSE100 stocks   ###
###                                                            ###
##################################################################

reset

# produce postscript (.eps) output plot
set term postscript eps enhanced color size 7,5
set output "expectedReturnsAndVolatility.eps"

set border 3 linewidth 1

set key font "sans-serif, 18"

set xlabel "Volatility" font "sans-serif, 18"
set ylabel "Expected Return" font "sans-serif, 18"

set xtics nomirror font "sans-serif, 18"
set ytics nomirror font "sans-serif, 18"

# style for data points
set style line 1 lc rgb '#000066' lt 1 lw 1.5 pt 13 ps 1.5

plot 'expectedReturnsAndVolatility_n_81_T_511.dat' using 1:2 notitle with points ls 1