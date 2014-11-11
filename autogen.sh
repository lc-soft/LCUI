#!/bin/sh

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

DIE=0

(test -f $srcdir/configure.ac) || {
	echo "**Error**: Directory "\`$srcdir\'" does not look like the top-level package directory"
	exit 1
}

(autoconf --version) < /dev/null > /dev/null 2>&1 || {
	echo
	echo "**Error**: You must have \`autoconf' installed."
	DIE=1
}

(libtoolize --version) < /dev/null > /dev/null 2>&1 || {
     (glibtoolize --version) < /dev/null > /dev/null 2>&1 || {
         echo
         echo "**Error**: You must have \`libtool' installed."
         DIE=1
     }
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
	NO_AUTOMAKE=yes
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

libtoolize || glibtoolize
aclocal -I m4
automake --add-missing
autoconf
