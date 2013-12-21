########################################################################
###                                                                  ###
###   Plot Spectral Clustering Synthetic Data Errors with pin = 0.6  ###
###                                                                  ###
########################################################################

reset

# produce postscript (.eps) output plot
set term postscript eps enhanced color size 7,5
set output "spectralClusteringSyntheticDataErrors_pin_0.6.eps"

set border 3 linewidth 1

set key inside center font "sans-serif, 18"

set xlabel "c_{out}/c_{in}" font "sans-serif, 18"
set ylabel "Overlap" font "sans-serif, 18"

set xtics nomirror font "sans-serif, 18"
set ytics nomirror font "sans-serif, 18"

# style for Laplacian method
set style line 1 lc rgb '#00ff00' lt 1 lw 1.5 pt 1 ps 1.5

# style for Modularity Method
set style line 2 lc rgb '#ff0000' lt 1 lw 1.5 pt 2 ps 1.5

# style for AMP method
#set style line 3 lc rgb '#ff00ff' lt 1 lw 1.5 pt 6 ps 1.5

# style for AMP with Onsanger Method
set style line 4 lc rgb '#0060ad' lt 1 lw 1.5 pt 4 ps 1.5

plot 'syntheticDataErrors_pin_0.6.dat' using 1:2 title 'Laplacian Method' with points ls 1, 'syntheticDataErrors_pin_0.6.dat' using 1:3 title 'Modularity Method' with points ls 2, 'syntheticDataErrors_pin_0.6.dat' using 1:5 title 'AMP' with points ls 4



########################################################################
###                                                                  ###
###   Plot Spectral Clustering Synthetic Data Errors with pin = 0.9  ###
###                                                                  ###
########################################################################

reset

# produce postscript (.eps) output plot
set term postscript eps enhanced color size 7,5
set output "spectralClusteringSyntheticDataErrors_pin_0.9.eps"

set border 3 linewidth 1

set key inside center center font "sans-serif, 18"

set xlabel "c_{out}/c_{in}" font "sans-serif, 18"
set ylabel "Overlap" font "sans-serif, 18"

set xtics nomirror font "sans-serif, 18"
set ytics nomirror font "sans-serif, 18"

# style for Laplacian method
set style line 1 lc rgb '#00ff00' lt 1 lw 1.5 pt 1 ps 1.5

# style for Modularity Method
set style line 2 lc rgb '#ff0000' lt 1 lw 1.5 pt 2 ps 1.5

# style for AMP method
#set style line 3 lc rgb '#ff00ff' lt 1 lw 1.5 pt 6 ps 1.5

# style for AMP with Onsanger Method
set style line 4 lc rgb '#0060ad' lt 1 lw 1.5 pt 4 ps 1.5

plot 'syntheticDataErrors_pin_0.9.dat' using 1:2 title 'Laplacian Method' with points ls 1, 'syntheticDataErrors_pin_0.9.dat' using 1:3 title 'Modularity Method' with points ls 2, 'syntheticDataErrors_pin_0.9.dat' using 1:5 title 'AMP' with points ls 4

