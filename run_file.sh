#!/bin/bash

# Vérifier le nombre d'arguments
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <input_file> <output_file>"
    exit 1
fi

freq=10
input="$1"
output="mojitos_$2"

./mojitos -u -f $freq -o "tmp/local_output/$output" -- python3 $input
