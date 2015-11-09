# $Id: Makefile.am 8182 2011-02-02 14:08:35Z sloot $
# $URL $

AM_CPPFLAGS = -I@top_srcdir@/include
AM_CXXFLAGS = -std=c++0x

bin_PROGRAMS = foliatest

foliatest_SOURCES = foliatest.cxx

TESTS = tst.sh
TESTS_ENVIRONMENT = topsrcdir=$(top_srcdir)
EXTRA_DIST = tst.sh
