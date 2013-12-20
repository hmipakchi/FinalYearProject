######################################
###                                ###
###   Plot Crowdsourcing Data (l)  ###
###                                ###
######################################

reset

# produce postscript (.eps) output plot
set term postscript eps enhanced color size 7,5
set output "crowdsourcingErrors_l.eps"

set border 3 linewidth 1

set key font "sans-serif, 18"

set logscale y

set xlabel "Number of queries per task" font "sans-serif, 18"
set ylabel "Probability of error" font "sans-serif, 18"

set xtics nomirror font "sans-serif, 18"
set ytics nomirror font "sans-serif, 18"

# style for MV algorithm
set style line 1 lc rgb '#0060ad' lt 1 lw 1.5 pt 5 ps 1.5

# style for SV algorithm
set style line 2 lc rgb '#ff0000' lt 1 lw 1.5 pt 7 ps 1.5

plot 'crowdSourcingErrors_l.dat' using 1:2 title 'Majority Voting' with linespoints ls 1, 'crowdSourcingErrors_l.dat' using 1:3 title 'Singular Vector' with linespoints ls 2


######################################
###                                ###
###   Plot Crowdsourcing Data (q)  ###
###                                ###
######################################

reset

set term postscript eps enhanced color size 7,5
set output "crowdsourcingErrors_q.eps"

set border 3 linewidth 1

set key font "sans-serif, 18"

set logscale y

set xlabel "Collective quality of the crowd, q" font "sans-serif, 18"
set ylabel "Probability of error" font "sans-serif, 18"

set xtics nomirror font "sans-serif, 18"
set ytics nomirror font "sans-serif, 18"

# style for MV algorithm
set style line 1 lc rgb '#0060ad' lt 1 lw 1.5 pt 5 ps 1.5

# style for SV algorithm
set style line 2 lc rgb '#ff0000' lt 1 lw 1.5 pt 7 ps 1.5

plot 'crowdSourcingErrors_q.dat' using 1:2 title 'Majority Voting' with linespoints ls 1, 'crowdSourcingErrors_q.dat' using 1:3 title 'Singular Vector' with linespoints ls 2