#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([foliatest],[0.12],[lamasoftware@science.ru.nl])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADER([config.h])

if test x"${CXXFLAGS+set}" = xset; then
  # the user set CXXFLAGS; don't override it.
  cxx_flags_were_set=true
else
  cxx_flags_were_set=false
fi

# Checks for programs.
AC_PROG_CXX( [g++] )

if $cxx_flags_were_set; then
  CXXFLAGS=$CXXFLAGS
fi

# check OpenMP support
AC_OPENMP

# use libtool
LT_INIT

# when running tests, use CXX
AC_LANG([C++])

# check for headers

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

PKG_PROG_PKG_CONFIG

if test "x$PKG_CONFIG_PATH" = x; then
    export PKG_CONFIG_PATH="$prefix/lib/pkgconfig"
else
    export PKG_CONFIG_PATH="$prefix/lib/pkgconfig:$PKG_CONFIG_PATH"
fi

AC_MSG_NOTICE( [pkg-config search path: $PKG_CONFIG_PATH] )

AC_OSX_PKG( [icu4c] )

PKG_CHECK_MODULES([ICU], [icu-uc >= 50 icu-io] )
CXXFLAGS="$CXXFLAGS $ICU_CFLAGS"
LIBS="$ICU_LIBS $LIBS"

PKG_CHECK_MODULES([ticcutils], [ticcutils >= 0.29] )
CXXFLAGS="$CXXFLAGS $ticcutils_CFLAGS"
LIBS="$LIBS $ticcutils_LIBS"

PKG_CHECK_MODULES([XML2], [libxml-2.0 >= 2.6.16] )
CXXFLAGS="$CXXFLAGS $XML2_CFLAGS"
LIBS="$XML2_LIBS $LIBS"

PKG_CHECK_MODULES([folia],[folia >= 2.4])
CXXFLAGS="$folia_CFLAGS $CXXFLAGS"
LIBS="$folia_LIBS $LIBS"

FOLVER=`pkg-config --modversion folia`
FOLINT="${FOLVER//.}"
AC_DEFINE_UNQUOTED( [FOLIA_INT_VERSION],
	   	    [${FOLINT}],
		    [The integer representation of the folialib version])
AC_CONFIG_FILES([
  Makefile
  src/Makefile
])
AC_OUTPUT
