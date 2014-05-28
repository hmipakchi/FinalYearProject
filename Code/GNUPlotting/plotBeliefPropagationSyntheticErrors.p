##############################################################################
###                                                                        ###
###   Plot Belief Propagation Synthetic Data Errors with pin = 0.1, q = 2  ###
###                                                                        ###
##############################################################################

reset

# produce postscript (.eps) output plot
set term postscript eps enhanced color size 7,5
set output "beliefPropagationSyntheticDataErrors_pin_0_1_q_2.eps"

set border 3 linewidth 1

set key inside center font "sans-serif, 18"

set xlabel "c_{out}/c_{in}" font "sans-serif, 18"
set ylabel "Overlap" font "sans-serif, 18"

set xtics nomirror font "sans-serif, 18"
set ytics nomirror font "sans-serif, 18"

# style for plot lines
set style line 1 lc rgb '#0000ff' lt 1 lw 2 pt 2 ps 1.5

plot 'bpOverlapResults_N_200_pin_0.1_q_2.dat' using 1:2 title 'BP Algorithm, p_{in} = 0.1' with points ls 1


##############################################################################
###                                                                        ###
###   Plot Belief Propagation Synthetic Data Errors with pin = 0.5, q = 2  ###
###                                                                        ###
##############################################################################

reset

# produce postscript (.eps) output plot
set term postscript eps enhanced color size 7,5
set output "beliefPropagationSyntheticDataErrors_pin_0_5_q_2.eps"

set border 3 linewidth 1

set key inside center font "sans-serif, 18"

set xlabel "c_{out}/c_{in}" font "sans-serif, 18"
set ylabel "Overlap" font "sans-serif, 18"

set xtics nomirror font "sans-serif, 18"
set ytics nomirror font "sans-serif, 18"

# style for plot lines
set style line 1 lc rgb '#0000ff' lt 1 lw 2 pt 2 ps 1.5

plot 'bpOverlapResults_N_200_pin_0.5_q_2.dat' using 1:2 title 'BP Algorithm, p_{in} = 0.5' with points ls 1


##############################################################################
###                                                                        ###
###   Plot Belief Propagation Synthetic Data Errors with pin = 0.9, q = 2  ###
###                                                                        ###
##############################################################################

reset

# produce postscript (.eps) output plot
set term postscript eps enhanced color size 7,5
set output "beliefPropagationSyntheticDataErrors_pin_0_9_q_2.eps"

set border 3 linewidth 1

set key inside center font "sans-serif, 18"

set xlabel "c_{out}/c_{in}" font "sans-serif, 18"
set ylabel "Overlap" font "sans-serif, 18"

set xtics nomirror font "sans-serif, 18"
set ytics nomirror font "sans-serif, 18"

# style for plot lines
set style line 1 lc rgb '#0000ff' lt 1 lw 2 pt 2 ps 1.5

plot 'bpOverlapResults_N_200_pin_0.9_q_2.dat' using 1:2 title 'BP Algorithm, p_{in} = 0.9' with points ls 1


##############################################################################
###                                                                        ###
###   Plot Belief Propagation Synthetic Data Errors with pin = 0.1, q = 4  ###
###                                                                        ###
##############################################################################

reset

# produce postscript (.eps) output plot
set term postscript eps enhanced color size 7,5
set output "beliefPropagationSyntheticDataErrors_pin_0_1_q_4.eps"

set border 3 linewidth 1

set key inside center font "sans-serif, 18"

set xlabel "c_{out}/c_{in}" font "sans-serif, 18"
set ylabel "Overlap" font "sans-serif, 18"

set xtics nomirror font "sans-serif, 18"
set ytics nomirror font "sans-serif, 18"

# style for plot lines
set style line 1 lc rgb '#0000ff' lt 1 lw 2 pt 2 ps 1.5

plot 'bpOverlapResults_N_200_pin_0.1_q_4.dat' using 1:2 title 'BP Algorithm, p_{in} = 0.1' with points ls 1


##############################################################################
###                                                                        ###
###   Plot Belief Propagation Synthetic Data Errors with pin = 0.5, q = 4  ###
###                                                                        ###
##############################################################################

reset

# produce postscript (.eps) output plot
set term postscript eps enhanced color size 7,5
set output "beliefPropagationSyntheticDataErrors_pin_0_5_q_4.eps"

set border 3 linewidth 1

set key inside center font "sans-serif, 18"

set xlabel "c_{out}/c_{in}" font "sans-serif, 18"
set ylabel "Overlap" font "sans-serif, 18"

set xtics nomirror font "sans-serif, 18"
set ytics nomirror font "sans-serif, 18"

# style for plot lines
set style line 1 lc rgb '#0000ff' lt 1 lw 2 pt 2 ps 1.5

plot 'bpOverlapResults_N_200_pin_0.5_q_4.dat' using 1:2 title 'BP Algorithm, p_{in} = 0.5' with points ls 1


##############################################################################
###                                                                        ###
###   Plot Belief Propagation Synthetic Data Errors with pin = 0.9, q = 4  ###
###                                                                        ###
##############################################################################

reset

# produce postscript (.eps) output plot
set term postscript eps enhanced color size 7,5
set output "beliefPropagationSyntheticDataErrors_pin_0_9_q_4.eps"

set border 3 linewidth 1

set key inside center font "sans-serif, 18"

set xlabel "c_{out}/c_{in}" font "sans-serif, 18"
set ylabel "Overlap" font "sans-serif, 18"

set xtics nomirror font "sans-serif, 18"
set ytics nomirror font "sans-serif, 18"

# style for plot lines
set style line 1 lc rgb '#0000ff' lt 1 lw 2 pt 2 ps 1.5

plot 'bpOverlapResults_N_200_pin_0.9_q_4.dat' using 1:2 title 'BP Algorithm, p_{in} = 0.9' with points ls 1


#############################################################################################
###                                                                                       ###
###   Plot Belief Propagation Synthetic Data Errors with pin = 0.01, q = 2 and N = 10000  ###
###                                                                                       ###
#############################################################################################

reset

# produce postscript (.eps) output plot
set term postscript eps enhanced color size 7,5
set output "beliefPropagationSyntheticDataErrors_pin_0_01_q_2_n_10000.eps"

set border 3 linewidth 1

set key inside center font "sans-serif, 18"

set xlabel "c_{out}/c_{in}" font "sans-serif, 18"
set ylabel "Overlap" font "sans-serif, 18"

set xtics nomirror font "sans-serif, 18"
set ytics nomirror font "sans-serif, 18"

# style for plot lines
set style line 1 lc rgb '#0000ff' lt 1 lw 2 pt 2 ps 1.5

plot 'bpOverlapResults_N_10000_pin_0.01_q_2.dat' using 1:2 title 'BP Algorithm, p_{in} = 0.01 and k = 2' with points ls 1

##############################################################################################
###                                                                                        ###
###   Plot Belief Propagation Synthetic Data Errors with pin = 0.01, q = 4 and N = 10000   ###
###                                                                                        ###
##############################################################################################

reset

# produce postscript (.eps) output plot
set term postscript eps enhanced color size 7,5
set output "beliefPropagationSyntheticDataErrors_pin_0_01_q_4_n_10000.eps"

set border 3 linewidth 1

set key inside center font "sans-serif, 18"

set xlabel "c_{out}/c_{in}" font "sans-serif, 18"
set ylabel "Overlap" font "sans-serif, 18"

set xtics nomirror font "sans-serif, 18"
set ytics nomirror font "sans-serif, 18"

# style for plot lines
set style line 1 lc rgb '#ff0000' lt 1 lw 2 pt 2 ps 1.5

plot 'bpOverlapResults_N_10000_pin_0.01_q_4.dat' using 1:2 title 'BP Algorithm, p_{in} = 0.01 and k = 4' with points ls 1