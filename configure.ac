AC_INIT([gcolor3], 2.0, [hjdskes@gmail.com], [gcolor3],
	[https://github.com/Unia/gcolor3/])
AM_INIT_AUTOMAKE([1.11 -Wno-portability no-dist-gzip dist-xz foreign subdir-objects])
AM_SILENT_RULES([yes])

AC_PROG_CC
AC_PROG_CC_STDC
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_PREFIX_DEFAULT(/usr/local)
AC_PATH_PROG([GLIB_COMPILE_RESOURCES], [glib-compile-resources])

LT_PREREQ(2.2)
LT_INIT([disable-static])

# Internationalization
AM_GLIB_GNU_GETTEXT
IT_PROG_INTLTOOL([0.35.0])

GETTEXT_PACKAGE=AC_PACKAGE_NAME
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [Gettext package.])

# Package requirements
PKG_CHECK_MODULES([GTK3], [gtk+-3.0 >= 3.12.0])

AC_CONFIG_FILES(Makefile
		data/Makefile
		data/gcolor3.desktop.in
		data/icons/Makefile
		src/Makefile
		po/Makefile.in)

AC_OUTPUT
