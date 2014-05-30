reset

########################################################
###                                                  ###
###   Plot testLouvain Method Temporal Communities   ###
###                                                  ###
########################################################


# produce postscript (.ps) output plot
set term postscript eps enhanced color size 9,7
set output "testLouvainMethodTemporalCommunities.eps"

set border linewidth 0
unset key
unset colorbox

# set this to max(testLouvainMethodTemporalCommunities)
set palette maxcolors 5
set palette defined (1 "red", 2 "green", 3 "blue", 4 "yellow", 5 "purple")

# note: n = 80, T = 240
set xrange [-0.5:239.5]
set yrange [-0.5:79.5]

set xlabel "Time (year)"
set ylabel "Ticker"

set xlabel font "Arial, 18"
set ylabel font "Arial, 18" 

set xtics font "Arial, 18"
set ytics font "Arial, 12" 

XTICS="`awk 'BEGIN{getline}{printf "%s ",$1}' testableDatesYearOnlyForPlotOnly.txt`"
YTICS="`awk 'BEGIN{getline}{printf "%s ",$1}' tickersForPlotOnly.txt`"

set for [i=1:words(XTICS):25] xtics ( word(XTICS,i) i-1)
set for [i=1:words(YTICS)] ytics ( word(YTICS,words(YTICS)-i+1) i-1)

plot 'testLouvainMethodTemporalCommunities.dat' matrix with image