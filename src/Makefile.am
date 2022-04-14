AM_CPPFLAGS = -I@top_srcdir@/include
AM_CXXFLAGS = -std=c++11 -g -W -Wall -pedantic

bin_PROGRAMS = foliatest

foliatest_SOURCES = sanity_tests.cxx edit_tests.cxx text_tests.cxx \
	foliapy_tests.cxx foliatest.cxx

TESTS = tst.sh
TESTS_ENVIRONMENT = topsrcdir=$(top_srcdir)
EXTRA_DIST = tst.sh
