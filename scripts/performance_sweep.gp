set key outside above font ',12'

set ytics font ',12' 0,0.25
set xtics font ',12' 0,0.25

set xlabel "initial density" font ',12'
set ylabel "proportion correctly classified" font ',12'

# set style line 1 lc rgb '#253494' lt 1 lw 2
# set style line 2 lc rgb '#2c7fb8' lt 1 lw 2
# set style line 3 lc rgb '#41b6c4' lt 1 lw 2
# set style line 4 lc rgb '#7fcdbb' lt 1 lw 2
# set style line 5 lc rgb '#c7e9b4' lt 1 lw 2
# set style line 6 lc rgb '#7a0177' lt 2 lw 2
# set style line 7 lc rgb '#c51b8a' lt 2 lw 2
# set style line 8 lc rgb '#f768a1' lt 2 lw 2
# set style line 9 lc rgb '#fa9fb5' lt 2 lw 2
# set style line 10 lc rgb '#fcc5c0' lt 2 lw 2

set style line 1 lc rgb '#c7e9b4'  lt 1 lw 2
set style line 2 lc rgb '#7fcdbb'  lt 3 lw 2
set style line 3 lc rgb '#41b6c4'  lt 1 lw 2
set style line 4 lc rgb '#1d91c0'  lt 1 lw 2
set style line 5 lc rgb '#225ea8'  lt 1 lw 2
set style line 6 lc rgb '#253494'  lt 1 lw 2
set style line 7 lc rgb '#081d58'  lt 1 lw 2

set multiplot layout 1,2
unset key
set grid

plot 'fixed1.txt'  u 2:3 w lines title "radius = 1"  ls 1, \
     'fixed5.txt'  u 2:3 w lines title "radius = 5"  ls 2, \
     'fixed7.txt'  u 2:3 w lines title "radius = 7"  ls 3, \
     'fixed14.txt' u 2:3 w lines title "radius = 14" ls 4, \
     'fixed25.txt' u 2:3 w lines title "radius = 25" ls 5, \
     'fixed32.txt' u 2:3 w lines title "radius = 32" ls 6, \
     'fixed63.txt' u 2:3 w lines title "radius = 63" ls 7

set key inside bottom
unset ylabel
plot 'cr-1_results.txt'  u 1:2 w lines title "radius = 1"  ls 1, \
     'cr-5_results.txt'  u 1:2 w lines title "radius = 5"  ls 2, \
     'cr-7_results.txt'  u 1:2 w lines title "radius = 7"  ls 3, \
     'cr-14_results.txt' u 1:2 w lines title "radius = 14" ls 4, \
     'cr-25_results.txt' u 1:2 w lines title "radius = 25" ls 5, \
     'cr-32_results.txt' u 1:2 w lines title "radius = 32" ls 6, \
     'cr-63_results.txt' u 1:2 w lines title "radius = 63" ls 7

# unset xtics
# unset ytics
# unset xlabel
# unset ylabel
# set yrange [0:1]
# set xrange [0:1]

# set key inside center
# plot 2 ls 1 title "radius = 1", \
#      2 ls 2 title "radius = 5", \
#      2 ls 3 title "radius = 7", \
#      2 ls 4 title "radius = 14", \
#      2 ls 5 title "radius = 25", \
#      2 ls 6 title "radius = 32", \
#      2 ls 7 title "radius = 63"
