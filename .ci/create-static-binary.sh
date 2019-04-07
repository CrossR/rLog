#!/bin/bash

echo "Building binary in docker"

docker build . -t crossr/reason-logger-static

echo "Copying to ./reasonLogger"

docker run --rm -v $(pwd):$(pwd) --workdir $(pwd) crossr/reason-logger-static cp /app/_build/default/executable/ReasonLogger.exe ./reasonLogger
strip ./reasonLogger
