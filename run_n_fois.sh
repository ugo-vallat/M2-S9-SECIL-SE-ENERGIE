#!/usr/bin/env bash

# Vérification des arguments
if [ "$#" -lt 2 ]; then
    echo "Usage: $0 <nb_iterations> <programme> [args...]"
    exit 1
fi

N=$1
shift
CMD=("$@")

total=0

echo "Exécution de : ${CMD[*]}"
echo "Nombre d'itérations : $N"
echo

for ((i=1; i<=N; i++)); do
    # On récupère uniquement le temps réel en secondes (%e)
    t=$(/usr/bin/time -f "%e" "${CMD[@]}" 2>&1 >/dev/null)
    echo "Run $i : $t s"

    # Addition (float)
    total=$(echo "$total + $t" | bc -l)
done

moyenne=$(echo "$total / $N" | bc -l)

echo
echo "Temps moyen : $moyenne s"
