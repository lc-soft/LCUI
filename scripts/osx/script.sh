set -e
./configure --with-openmp=no
make
make test
