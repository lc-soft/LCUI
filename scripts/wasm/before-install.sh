#!/bin/bash
set -e
docker run -dit --name emscripten -v $(pwd):/src trzeci/emscripten:sdk-incoming-64bit bash
docker exec -it emscripten apt update -qq
docker exec -it emscripten apt install automake autoconf libtool pkg-config -qq -y
