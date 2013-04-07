#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(Gcolor3, 1.0, https://github.com/Unia/gcolor3/issues)
AM_INIT_AUTOMAKE()
AC_CONFIG_SRCDIR(source/gcolor3.h)
AC_CONFIG_HEADERS(config.h)

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.
GTK3_VERSION=3.0.0
AC_SUBST(GTK3_VERSION)
PKG_CHECK_MODULES(GTK3,[gtk+-3.0 >= $GTK3_VERSION])

CFLAGS+=" -std=c99 -Wall -Wextra -Wno-deprecated-declarations `pkg-config --cflags --libs gtk+-3.0`"

AC_ARG_ENABLE(debug, [  --enable-debug enable debugging flags ])

if test "$enable_debug" = "yes"; then
 CFLAGS+=" -g"
fi

AC_CONFIG_FILES([Makefile
                 data/Makefile
                 source/Makefile])
AC_OUTPUT

echo -e "\nYou are now ready to compile gcolor3"
echo "Type \"make\" to compile gcolor3"
