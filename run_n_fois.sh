#!/usr/bin/env bash

if [ "$#" -lt 2 ]; then
    echo "Usage: $0 <nb_iterations> <programme> [args...]"
    exit 1
fi

N=$1
shift
CMD=("$@")

total=0

echo "Programme : ${CMD[*]}"
echo "Itérations : $N"
echo

for ((i=1; i<=N; i++)); do
    start=$(date +%s.%N)       # timestamp haute précision
    "${CMD[@]}" >/dev/null 2>&1
    end=$(date +%s.%N)

    # Calcul de la durée
    duration=$(echo "$end - $start" | bc -l)

    # Convertir en microsecondes si tu veux
    us=$(echo "$duration * 1000000" | bc -l)

    echo "Run $i : $us µs"

    total=$(echo "$total + $us" | bc -l)
done

moyenne=$(echo "$total / $N" | bc -l)

echo
echo " > Temps moyen : $moyenne µs"

