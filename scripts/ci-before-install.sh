if [ "$TRAVIS_OS_NAME" = "linux" ]; then
    if [ "$WASM" = "1" ]; then
        . ./scripts/wasm/before-install.sh
    else
        . ./scripts/linux/before-install.sh
    fi
fi
