#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <server>"
    exit 1
fi

server="$1"

mkdir -p tmp/scp_output
scp g5k:$server/se-energie/tmp/local_output/* tmp/scp_output