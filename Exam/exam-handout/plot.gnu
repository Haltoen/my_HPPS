set terminal png
set output outputfile
unset border
unset xtics
unset ytics
unset ztics
set xrange [-2:2]
set yrange [-2:2]
set zrange [-2:2]
splot inputfile using "%*lf %lf %lf %lf %*lf %*lf %*lf" with dots notitle
