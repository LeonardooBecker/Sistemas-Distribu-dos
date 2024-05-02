#!/bin/bash

# Lista dos valores para o loop
args=(8 16 32 64 128 256 512 1024 2048)


echo "" > logLatencia.dat
for i in "${args[@]}"; do
    ./tempo $i > aux.txt

    # Extrai a latência e o tempo de execução do arquivo auxiliar
    LATENCIA=$(grep -E "Latencia" aux.txt | cut -d ':' -f 2)
    TEMPO_EXECUCAO=$(grep -E "Tempo de execução" aux.txt | cut -d ':' -f 2)

    # Exibe os valores extraídos
    echo "$i,$LATENCIA,$TEMPO_EXECUCAO" >> logLatencia.dat
done

gnuplot latencia.gp
gnuplot tempo.gp