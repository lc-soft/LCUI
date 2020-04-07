set -e
if [ "$TRAVIS_OS_NAME" = "linux" ]; then
    if [ "$WASM" = "1" ]; then
        . ./scripts/wasm/script.sh
    else
        . ./scripts/linux/script.sh
    fi
elif [ "$TRAVIS_OS_NAME" = "osx" ]; then
    . ./scripts/osx/script.sh
fi
