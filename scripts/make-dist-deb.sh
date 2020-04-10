set -e
PACKAGE_NAME=lcui
PACKAGE_VERSION=2.0.0
PACKAGE_NAME_WITH_VERSION=$PACKAGE_NAME-$PACKAGE_VERSION
PACKAGE_FILE=$PACKAGE_NAME_WITH_VERSION.tar.gz
PACKAGE_ORIG_FILE=${PACKAGE_NAME}_${PACKAGE_VERSION}.orig.tar.gz
DEBEMAIL="lc-soft@live.cn"
DEBFULLNAME="Liu Chao"
make dist
[ -d build/debian ] || mkdir build/debian
cd build/debian
[ -f $PACKAGE_ORIG_FILE ] && rm $PACKAGE_ORIG_FILE
[ -d $PACKAGE_NAME_WITH_VERSION ] && rm -rf $PACKAGE_NAME_WITH_VERSION
mv ../../$PACKAGE_FILE ./$PACKAGE_ORIG_FILE
tar xzf $PACKAGE_ORIG_FILE
cd $PACKAGE_NAME_WITH_VERSION
dpkg-buildpackage --unsigned-changes --unsigned-source
