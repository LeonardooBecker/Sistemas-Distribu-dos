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
set xrange [0:2200]
#set yrange [0:20]
set xlabel  "N (Quantidade de processos)"


set key outside right # posição da legenda: outside {left | right}
unset logscale y
unset logscale x
set datafile separator whitespace
set ylabel  "Latência (rodadas de testes)"
set title   "Latência para detecção"
set datafile separator ","
plot 'logLatencia.dat' using 1:2 title "<Rodada de testes>" lc rgb "blue" with linespoints
pause -1


# Gerando figura PNG
set terminal png
set output "latencia_trabalho0.png"
plot 'logLatencia.dat' using 1:2 title "<Rodada de testes>" lc rgb "blue" with linespoints
replot
unset output