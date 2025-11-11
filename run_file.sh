#!/bin/bash

# Vérifier le nombre d'arguments
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <output_file> <cmd>"
    exit 1
fi

freq=10
output="mojitos_$1"
cmd=$2

mojitos -u -f $freq -o "tmp/local_output/$output" -- $cmd
