#!/bin/bash

echo "Building binary in docker"

docker build . -t crossr/rlog-static-binary

echo "Copying to ./rLog"

docker run --rm -v $(pwd):$(pwd) --workdir $(pwd) crossr/rlog-static-binary cp /app/_build/default/executable/RLog.exe ./rLog
strip ./rLog
