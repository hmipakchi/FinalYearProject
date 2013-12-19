reset

#################################
###                           ###
###   Plot Adjacency Matrix   ###
###                           ###
#################################


# produce postscript (.ps) output plot
set term postscript eps enhanced color size 5,5
set output "adjacencyMatrix_sparse.eps"

set border linewidth 0
unset key
unset colorbox

set palette maxcolors 2
set palette defined (0 "white", 1 "red")

# note: n = 1000
set xrange [-0.5:999.5]
set yrange [-0.5:999.5]

unset xtics
unset ytics

set format x ""
set format y ""

plot 'adjacencyMatrix.dat' matrix with image


reset



##########################################
###                                    ###
###   Plot Labelled Adjacency Matrix   ###
###                                    ###
##########################################


# produce postscript (.ps) output plot
set term postscript eps enhanced color size 5,5
set output "labelledAdjacencyMatrix_sparse.eps"

set border linewidth 0
unset key
unset colorbox

set palette maxcolors 2
set palette defined (0 "white", 1 "red")

# note: n = 1000
set xrange [-0.5:999.5]
set yrange [-0.5:999.5]

unset xtics
unset ytics

set format x ""
set format y ""

plot 'labelledAdjacencyMatrix.dat' matrix with image