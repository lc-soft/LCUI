#!/bin/sh

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

DIE=0

# On MacOS X, the GNU libtool is named `glibtool'.
HOSTOS=`uname`
if test "$LIBTOOLIZE"x != x; then
  :
elif test "$HOSTOS"x = Darwinx; then
  LIBTOOLIZE=glibtoolize
else
  LIBTOOLIZE=libtoolize
fi

(test -f $srcdir/configure.ac) || {
	echo "**Error**: Directory "\`$srcdir\'" does not look like the top-level package directory"
	exit 1
}

(autoconf --version) < /dev/null > /dev/null 2>&1 || {
	echo
	echo "**Error**: You must have \`autoconf' installed."
	DIE=1
}

($LIBTOOLIZE --version) < /dev/null > /dev/null 2>&1 || {
	echo
	echo "**Error**: You must have \`libtool' installed."
	DIE=1
}

(pkg-config --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "You must have pkg-config installed to compile $package."
           result="no"
    DIE=1
}

(automake --version) < /dev/null > /dev/null 2>&1 || {
	echo
	echo "**Error**: You must have \`automake' (1.7 or later) installed."
	DIE=1
}

# if no automake, don't bother testing for aclocal
test -n "$NO_AUTOMAKE" || (aclocal --version) < /dev/null > /dev/null 2>&1 || {
	echo
	echo "**Error**: Missing \`aclocal'.  The version of \`automake'"
	echo "installed doesn't appear recent enough."
	DIE=1
}

if test "$DIE" -eq 1; then
	exit 1
fi

$LIBTOOLIZE --force --copy --install
aclocal -I m4 --force
automake --add-missing
autoconf --force
