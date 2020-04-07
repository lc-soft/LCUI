set -e
docker exec -it emscripten emconfigure ./configure --enable-video-output=no --with-openmp=no --disable-shared
docker exec -it emscripten make
