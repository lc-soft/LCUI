set -e
export LD_LIBRARY_PATH=/usr/local/clang/lib:$LD_LIBRARY_PATH
if [ "$CODECOV" = "1" ]; then
    ./configure --enable-debug --enable-code-coverage
else
    ./configure
fi
make
make distcheck
make test
if [ -z "$NO_VALGRIND" ]; then make test-with-valgrind; fi
