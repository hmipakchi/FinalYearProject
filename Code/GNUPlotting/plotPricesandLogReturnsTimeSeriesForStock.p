#################################################
###                                           ###
###   Plot Price and Log-Return Time Series   ###
###                                           ###
#################################################

reset

# produce postscript (.ps) output plot
set term postscript eps enhanced color size 7,5
set output "priceAndLogReturnTimeSeries.eps"

set grid nopolar
set grid xtics nomxtics ytics nomytics noztics nomztics \
 nox2tics nomx2tics noy2tics nomy2tics nocbtics nomcbtics
set grid layerdefault   linetype 0 linewidth 1.000,  linetype 0 linewidth 1.000

set xtics  norangelimit
set ytics  norangelimit

# x-axis data is time with format specified
set xdata time
set timefmt "%Y-%m-%d"
set format x "%Y"

# style for prices algorithm
set style line 1 lc rgb '#1E90FF' lt 1 lw 1.5 ps 1.5

set multiplot layout 2,1

set xlabel "Time (year)"
set ylabel "Price (GBX)"
plot 'price_and_logreturn_timeseries_AAL.dat' using 1:2 notitle with lines ls 1

set xlabel "Time (year)"
set ylabel "Log-return"
plot 'price_and_logreturn_timeseries_AAL.dat' using 1:3 notitle with lines ls 1

unset multiplot
