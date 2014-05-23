###########################################################
###                                                     ###
###   Plot Eigenvalue Constributions of different PCs   ###
###                                                     ###
###########################################################

reset

# produce postscript (.ps) output plot
set term postscript eps enhanced color size 7,5
set output "eigenvalueContributionsPC.eps"

# x-axis data is time with format specified
set xdata time
set timefmt "%Y-%m-%d"
set format x "%Y"

# style for PC 1
set style line 1 lc rgb '#0b24fb' lt 1 lw 1.5 ps 1.5

# style for PC 2
set style line 2 lc rgb '#FF0000' lt 1 lw 1.5 ps 1.5

# style for PC 3
set style line 3 lc rgb '#000000' lt 1 lw 1.5 ps 1.5

# style for PC 4
set style line 4 lc rgb '#800080' lt 1 lw 1.5 ps 1.5

# style for PC 5
set style line 5 lc rgb '#808080' lt 1 lw 1.5 ps 1.5

set key outside center bottom horizontal font "sans-serif, 18"

set xlabel "Time (year)"
set ylabel "{/Symbol b}_{k}/N"

plot 'eigenvalueContributionsPC.dat' using 1:2 title 'k=1' with lines ls 1, 'eigenvalueContributionsPC.dat' using 1:3 title 'k=2' with lines ls 2, 'eigenvalueContributionsPC.dat' using 1:4 title 'k=3' with lines ls 3, 'eigenvalueContributionsPC.dat' using 1:5 title 'k=4' with lines ls 4, 'eigenvalueContributionsPC.dat' using 1:6 title 'k=5' with lines ls 5
