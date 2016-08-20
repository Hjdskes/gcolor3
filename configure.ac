AC_PREREQ(2.63.2)

AC_INIT([Gcolor3], 2.2, [hjdskes@gmail.com], [gcolor3], [https://unia.github.io/projects/gcolor3/])

# Dependencies
GTK_REQUIRED=3.12.0

AC_CONFIG_HEADERS(config.h)
AC_DEFINE_UNQUOTED(COPYRIGHT, ["2013 - 2016"], [Copyright years])
AC_DEFINE_UNQUOTED(UNUSED, [__attribute__ ((unused))], [Wrapper over __attribute__ ((unused))])

AM_INIT_AUTOMAKE([1.11 foreign dist-xz no-dist-gzip subdir-objects -Wno-portability])
AM_SILENT_RULES(yes)

# Check for programs
AC_PROG_CC
AC_PROG_CC_STDC

# Initialize libtool
LT_PREREQ(2.2.6)
LT_INIT(disable-static)

# Resources
AC_PATH_PROG(GLIB_COMPILE_RESOURCES, glib-compile-resources)

# Marshals
AC_PATH_PROG(GLIB_GENMARSHAL, glib-genmarshal)

# Internationalization
IT_PROG_INTLTOOL(0.50.1)
AM_GLIB_GNU_GETTEXT

AC_SUBST(GETTEXT_PACKAGE, AC_PACKAGE_NAME)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE", [Gettext package.])

# Package requirements
PKG_CHECK_MODULES([GCOLOR3], [gtk+-3.0 >= GTK_REQUIRED])

AC_CONFIG_FILES(Makefile
		data/Makefile
		data/gcolor3.desktop.in
		data/icons/Makefile
		src/Makefile
		po/Makefile.in)

AC_OUTPUT

AC_MSG_RESULT([
        $PACKAGE $VERSION
        ========

        prefix:                 ${prefix}
        libdir:                 ${libdir}
        sysconfdir:             ${sysconfdir}
        exec_prefix:            ${exec_prefix}
        bindir:                 ${bindir}
        datarootdir:            ${datarootdir}

        compiler:               ${CC}
        cflags:                 ${CFLAGS}
        ldflags:                ${LDFLAGS}
])

