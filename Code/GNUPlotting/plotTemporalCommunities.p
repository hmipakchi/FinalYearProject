##################################################################################################################
###                                                                                                            ###
###   Plot Averaged Normalised Variation of Information for all Multislice Algorithms vs. testLouvain Method   ###
###                                                                                                            ###
##################################################################################################################

reset

# produce postscript (.ps) output plot
set term postscript eps enhanced color size 7,5
set output "averagedNormalisedVariationOfInformationTemporalSyntheticData.eps"

# style for omega = 0.25
set style line 1 lc rgb '#0b24fb' lt 1 lw 1.5 ps 1.5

# style for omega = 0.5
set style line 2 lc rgb '#FF0000' lt 1 lw 1.5 ps 1.5

# style for omega = 0.75
set style line 3 lc rgb '#000000' lt 1 lw 1.5 ps 1.5

# style for omega = 1
set style line 4 lc rgb '#808080' lt 1 lw 1.5 ps 1.5

# style for traditional Louvain method
set style line 5 lc rgb '#00FFFF' lt 1 lw 1.5 ps 1.5

set key outside center bottom horizontal font "Arial, 16"

set xrange [1:]

set xlabel font "Arial, 18"
set ylabel font "Arial, 18" 

set xlabel "Time Window Index"
set ylabel "Averaged Normalised Variation of Information over different SNRs"

plot 'averagedNormalisedVariationOfInformationTemporalSyntheticData.dat' using 1:2 title 'GLM w/ {/Symbol w}=0.25' with lines ls 1, 'averagedNormalisedVariationOfInformationTemporalSyntheticData.dat' using 1:3 title 'GLM w/ {/Symbol w}=0.5' with lines ls 2, 'averagedNormalisedVariationOfInformationTemporalSyntheticData.dat' using 1:4 title 'GLM w/ {/Symbol w}=0.75' with lines ls 3, 'averagedNormalisedVariationOfInformationTemporalSyntheticData.dat' using 1:5 title 'GLM w/ {/Symbol w}=1' with lines ls 4, 'averagedNormalisedVariationOfInformationTemporalSyntheticData.dat' using 1:6 title 'Louvain Method' with lines ls 5


########################################################
###                                                  ###
###   Plot testLouvain Method Temporal Communities   ###
###                                                  ###
########################################################

reset

# produce postscript (.ps) output plot
set term postscript eps enhanced color size 10,7
set output "testLouvainMethodTemporalCommunities.eps"

set border linewidth 0
unset key
set colorbox default vertical
set cblabel "Communities"
set cblabel font "Arial, 24"

# set this to max(testLouvainMethodTemporalCommunities)
set palette maxcolors 5
set palette defined (1 "red", 2 "green", 3 "blue", 4 "yellow", 5 "purple")

set cbtics ('1' 1,'5' 5)


# note: n = 80, T = 240
set xrange [-0.5:239.5]
set yrange [-0.5:79.5]

set xlabel "Time (year)"
set ylabel "Ticker"

set xlabel font "Arial, 24"
set ylabel font "Arial, 24" 

set xtics font "Arial, 18"
set ytics font "Arial, 12" 

XTICS="`awk 'BEGIN{getline}{printf "%s ",$1}' testableDatesYearOnlyForPlotOnly.txt`"
YTICS="`awk 'BEGIN{getline}{printf "%s ",$1}' tickersForPlotOnly.txt`"

set for [i=1:words(XTICS):25] xtics ( word(XTICS,i) i-1)
set for [i=1:words(YTICS)] ytics ( word(YTICS,words(YTICS)-i+1) i-1)

plot 'testLouvainMethodTemporalCommunities.dat' matrix with image


#############################################################################
###                                                                       ###
###   Plot multiSliceLouvain Method with omega = 1 Temporal Communities   ###
###                                                                       ###
#############################################################################

reset

# produce postscript (.ps) output plot
set term postscript eps enhanced color size 10,7
set output "testMultiSliceLouvainMethodCommunities_omega_1.eps"

set border linewidth 0
unset key
set colorbox default vertical
set cblabel "Communities"
set cblabel font "Arial, 24"

# set this to max(max(testMultiSliceLouvainMethodCommunities))
set palette maxcolors 10
set palette defined (1 "red", 2 "green", 3 "blue", 4 "yellow", 5 "purple", 6 "cyan", 7 "black", 8 "#d3d3d3", 9 "#ff00ff", 10 "#228b22")

set cbtics ('1' 1,'10' 10)


# note: n = 80, T = 240
set xrange [-0.5:239.5]
set yrange [-0.5:79.5]

set xlabel "Time (year)"
set ylabel "Ticker"

set xlabel font "Arial, 24"
set ylabel font "Arial, 24" 

set xtics font "Arial, 18"
set ytics font "Arial, 12" 

XTICS="`awk 'BEGIN{getline}{printf "%s ",$1}' testableDatesYearOnlyForPlotOnly.txt`"
YTICS="`awk 'BEGIN{getline}{printf "%s ",$1}' tickersForPlotOnly.txt`"

set for [i=1:words(XTICS):25] xtics ( word(XTICS,i) i-1)
set for [i=1:words(YTICS)] ytics ( word(YTICS,words(YTICS)-i+1) i-1)

plot 'testMultiSliceLouvainMethodCommunities_omega_1.dat' matrix with image


############################################################################################################################
###                                                                                                                      ###
###   Plot Normalised Varaition of Information between testLouvainMethod and generalised Louvain method with omega = 1   ###
###                                                                                                                      ###
############################################################################################################################

reset

# produce postscript (.ps) output plot
set term postscript eps enhanced color size 7,5
set output "normalisedVariationOfInformationTemporalCommunities.eps"


# x-axis data is time with format specified
set xdata time
set timefmt "%Y-%m-%d"
set format x "%Y"

# style for normalised variation of infromation line plot
set style line 1 lc rgb '#0000FF' lt 1 lw 2 ps 1.5

set xlabel "Time (year)"
set ylabel "Normalised Variation of Information"
plot 'normalisedVariationOfInformationTemporalCommunities.dat' using 1:2 notitle with lines ls 1


###################################################################################################################
###                                                                                                             ###
###   Plot modified modulartities generated by generalised Louvain method with omega = 1 and testLouvainMethod  ###
###                                                                                                             ###
###################################################################################################################

reset

# produce postscript (.ps) output plot
set term postscript eps enhanced color size 7,5
set output "modifiedModularitiesEachPartition.eps"


# x-axis data is time with format specified
set xdata time
set timefmt "%Y-%m-%d"
set format x "%Y"

# style for generalised Louvain method with omega = 1
set style line 1 lc rgb '#0000FF' lt 1 lw 2 ps 1.5

# style for testLouvainMethod
set style line 2 lc rgb '#FF0000' lt 1 lw 2 ps 1.5

set key outside center bottom horizontal font "Arial, 16"

set xlabel "Time (year)"
set ylabel "Modified Modularity"
plot 'modifiedModularitiesEachPartition.dat' using 1:2 title 'GLM with {/Symbol g}=1 and {/Symbol w}=1' with lines ls 1, 'modifiedModularitiesEachPartition.dat' using 1:3 title 'Louvain method' with lines ls 2

