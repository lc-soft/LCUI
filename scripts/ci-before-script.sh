if [ "$TRAVIS_OS_NAME" = "linux" ]; then
    if [ "$WASM" = "1" ]; then
        . ./scripts/wasm/before-script.sh
    else
        git config user.email "support@travis.org"
        git config user.name "Travis CI"
        . ./scripts/linux/before-script.sh
    fi
elif [ "$TRAVIS_OS_NAME" = "osx" ]; then
    . ./scripts/osx/before-script.sh
fi
