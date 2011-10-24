#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
# $Id: configure.ac 9271 2011-04-04 12:08:35Z sloot $
# $URL: https://ilk.uvt.nl/svn/sources/ucto/trunk/configure.ac $

AC_PREREQ(2.59)
AC_INIT([foliatest], [0.1], [timbl@uvt.nl])
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

AC_PROG_RANLIB
# use libtool
AC_PROG_LIBTOOL
AC_SUBST([LIBTOOL_DEPS])

# when running tests, use CXX
AC_LANG([C++])

PKG_PROG_PKG_CONFIG
# ugly hack when PKG_CONFIG_PATH isn't defined.
# couldn't get it to work otherwise
if test "x$PKG_CONFIG_PATH" = x; then
    export PKG_CONFIG_PATH=""
fi

AC_ARG_WITH(folia,
       [  --with-folia=DIR       use libfolia installed in <DIR>; 
             note that you can install libfolia in a non-default directory with
             ./configure --prefix=<DIR> in the libfolia installation directory],
       [PKG_CONFIG_PATH="$PKG_CONFIG_PATH:$withval/lib/pkgconfig"],
       [PKG_CONFIG_PATH="$PKG_CONFIG_PATH:$prefix/lib/pkgconfig"])
AC_MSG_NOTICE( [pkg-config search path: $PKG_CONFIG_PATH] )

PKG_CHECK_MODULES([folia],[folia >= 0.0])
CXXFLAGS="$CXXFLAGS $folia_CFLAGS"
LIBS="$LIBS $folia_LIBS"

PKG_CHECK_MODULES([CppUnit], [cppunit >= 1.12.1] )
CXXFLAGS="$CXXFLAGS $CppUnit_CFLAGS"
LIBS="$LIBS $CppUnit_LIBS"

PKG_CHECK_MODULES([XML2], [libxml-2.0 >= 2.6.16] )
CXXFLAGS="$CXXFLAGS $XML2_CFLAGS"
LIBS="$LIBS $XML2_LIBS"

useICU=1;
# inspired by feh-1.3.4/configure.ac.  Tnx Tom Gilbert and feh hackers.
AC_ARG_WITH(icu,
       [  --with-icu=DIR       use ICU installed in <DIR>],
       [if test "$with_icu" = "no"; then
           useICU=0
	else
	   CXXFLAGS="$CXXFLAGS -I$withval/include"
           LIBS="-L$withval/lib $LIBS"
	fi] )

if test "$useICU" = "1"; then
  AX_ICU_CHECK( [3.6],
		[CXXFLAGS="$CXXFLAGS $ICU_CPPSEARCHPATH"
		LIBS="$ICU_LIBPATH $ICU_LIBS $ICU_IOLIBS $LIBS"],
		[AC_MSG_FAILURE( "No ICU development environment found. Please check if libicu-dev or the like is installed" )] )
  AC_DEFINE(HAVE_ICU, 1, we want to use ICU )
else
  AC_MSG_ERROR("ICU support is required")
fi

AC_OUTPUT([
  Makefile
  src/Makefile
])
