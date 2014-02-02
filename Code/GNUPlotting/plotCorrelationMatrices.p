###############################################
###                                         ###
###   Plot Synthetic Correlation Matrix 3   ###
###                                         ###
###############################################

reset

# produce postscript (.ps) output plot
set term postscript eps enhanced color size 6,5
set output "syntheticCorrelationMatrices_3.eps"

set border linewidth 0
unset key

# note: n = 100
set xrange [-0.5:99.5]
set yrange [-0.5:99.5]

unset xtics
unset ytics

#set format x ""
#set format y ""

load 'default.plt'

set cbrange [ -1 : 1 ] noreverse nowriteback

plot 'syntheticCorrelationMatrices_3.dat' matrix with image

###############################################
###                                         ###
###   Plot Synthetic Correlation Matrix 9   ###
###                                         ###
###############################################

reset

# produce postscript (.ps) output plot
set term postscript eps enhanced color size 6,5
set output "syntheticCorrelationMatrices_9.eps"

set border linewidth 0
unset key

# note: n = 100
set xrange [-0.5:99.5]
set yrange [-0.5:99.5]

unset xtics
unset ytics

#set format x ""
#set format y ""

load 'default.plt'

set cbrange [ -1 : 1 ] noreverse nowriteback

plot 'syntheticCorrelationMatrices_9.dat' matrix with image


############################################
###                                      ###
###   Plot FTSE 100 Correlation Matrix   ###
###                                      ###
############################################

reset

# produce postscript (.ps) output plot
set term postscript eps enhanced color size 6,5
set output "correlationMatrix_FTSE100_n_87_T_2501.eps"

set border linewidth 0
unset key

# note: n = 87
set xrange [-0.5:86.5]
set yrange [-0.5:86.5]

unset xtics
unset ytics

#set format x ""
#set format y ""

set palette defined ( 0 "#000090",\
                      1 "#000fff",\
                      2 "#0090ff",\
                      3 "#0fffee",\
                      4 "#90ff70",\
                      5 "#ffee00",\
                      6 "#ff7000",\
                      7 "#ee0000",\
                      8 "#7f0000")

#set cbrange [ -1 : 1 ] noreverse nowriteback

plot 'crossCorrelation_n_87_T_2501.dat' matrix with image

reset