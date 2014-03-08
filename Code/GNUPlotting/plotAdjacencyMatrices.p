reset

#####################################
###                               ###
###   Plot PPM Adjacency Matrix   ###
###                               ###
#####################################


# produce postscript (.ps) output plot
set term postscript eps enhanced color size 5,5
set output "ppmAdjacencyMatrix.eps"

set border linewidth 0
unset key
unset colorbox

set palette maxcolors 2
set palette defined (0 "white", 1 "red")

# note: n = 300
set xrange [-0.5:299.5]
set yrange [-0.5:299.5]

unset xtics
unset ytics

set format x ""
set format y ""

plot 'ppmAdjacencyMatrix.dat' matrix with image


reset


##############################################
###                                        ###
###   Plot PPM Labelled Adjacency Matrix   ###
###                                        ###
##############################################


# produce postscript (.ps) output plot
set term postscript eps enhanced color size 5,5
set output "ppmLabelledAdjacencyMatrix.eps"

set border linewidth 0
unset key
unset colorbox

set palette maxcolors 2
set palette defined (0 "white", 1 "red")

# note: n = 300
set xrange [-0.5:299.5]
set yrange [-0.5:299.5]

unset xtics
unset ytics

set format x ""
set format y ""

plot 'ppmLabelledAdjacencyMatrix.dat' matrix with image


reset

#####################################
###                               ###
###   Plot HCM Adjacency Matrix   ###
###                               ###
#####################################


# produce postscript (.ps) output plot
set term postscript eps enhanced color size 5,5
set output "hcmAdjacencyMatrix.eps"

set border linewidth 0
unset key
unset colorbox

set palette maxcolors 2
set palette defined (0 "white", 1 "red")

# note: n = 300
set xrange [-0.5:299.5]
set yrange [-0.5:299.5]

unset xtics
unset ytics

set format x ""
set format y ""

plot 'hcmAdjacencyMatrix.dat' matrix with image


reset


##############################################
###                                        ###
###   Plot HCM Labelled Adjacency Matrix   ###
###                                        ###
##############################################


# produce postscript (.ps) output plot
set term postscript eps enhanced color size 5,5
set output "hcmLabelledAdjacencyMatrix.eps"

set border linewidth 0
unset key
unset colorbox

set palette maxcolors 2
set palette defined (0 "white", 1 "red")

# note: n = 300
set xrange [-0.5:299.5]
set yrange [-0.5:299.5]

unset xtics
unset ytics

set format x ""
set format y ""

plot 'hcmLabelledAdjacencyMatrix.dat' matrix with image