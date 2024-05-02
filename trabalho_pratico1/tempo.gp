#!/usr/bin/gnuplot -c

## set encoding iso_8859_15
set encoding utf

# set terminal qt persist

set grid

set style data point
set style function line
set style line 1 lc 3 pt 7 ps 0.3
set boxwidth 1
set xtics
# set xrange [0:]
set xlabel  "N (Quantidade de processos)"


set key outside right # posição da legenda: outside {left | right}
unset logscale y
set logscale x 2
set datafile separator whitespace
set ylabel  "Tempo (s)"
set title   "Tempo de execução"
set datafile separator ","
plot 'logLatencia.dat' using 1:3 title "<Tempo>" lc rgb "blue" with linespoints
pause -1


# Gerando figura PNG
set terminal png
set output "tempoExecucao_trabalho0.png"
plot 'logLatencia.dat' using 1:3 title "<Tempo>" lc rgb "blue" with linespoints

replot
unset output