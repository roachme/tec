#!/bin/env bash

CMD="$1"

if [ "$CMD" = "build" ]; then
    docker build -t tec -f tests/docker/Dockerfile .
elif [ "$CMD" = "run" ]; then
    docker run -it --rm -v $(pwd):/root/tec tec bash
else
    echo "Usage: $0 [run|build]"
    exit 1
fi
