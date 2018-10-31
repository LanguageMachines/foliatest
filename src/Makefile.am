AM_CPPFLAGS = -I@top_srcdir@/include
AM_CXXFLAGS = -std=c++11 -W -Wall -pedantic

bin_PROGRAMS = foliatest

foliatest_SOURCES = foliatest.cxx

TESTS = tst.sh
TESTS_ENVIRONMENT = topsrcdir=$(top_srcdir)
EXTRA_DIST = tst.sh
