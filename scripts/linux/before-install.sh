set -e
sudo apt update -qq
sudo apt install valgrind libpng-dev libjpeg-dev libxml2-dev libfreetype6-dev libx11-dev -qq
npm install
npm install --save-dev @commitlint/travis-cli
