check_PROGRAMS = arm_semihosting
AM_LDFLAGS = -lgtest_main -lgtest -lgmock
AM_CXXFLAGS = -fprebuilt-module-path=$(top_builddir)/src
arm_semihosting_SOURCES = arm_semihosting.cc $(top_builddir)/src/arm_semihosting.pcm
BUILT_SOURCES = $(top_builddir)/src/arm_semihosting.pcm

TESTS = $(check_PROGRAMS)
