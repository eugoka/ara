check_PROGRAMS = arm_semihosting bit_field jtag
AM_LDFLAGS = -lgtest_main -lgtest -lgmock
AM_CXXFLAGS = -fprebuilt-module-path=$(top_builddir)/src
BUILT_SOURCES = $(top_builddir)/src/arm_semihosting.pcm \
				$(top_builddir)/src/bit_field.pcm \
				$(top_builddir)/src/jtag.pcm

arm_semihosting_SOURCES = arm_semihosting.cc $(top_builddir)/src/arm_semihosting.pcm
bit_field_SOURCES = bit_field.cc $(top_builddir)/src/bit_field.pcm
bit_field_LDFLAGS = $(AM_LDFLAGS) $(top_builddir)/src/bit_field.pcm
jtag_SOURCES = jtag.cc $(top_builddir)/src/jtag.pcm

TESTS = $(check_PROGRAMS)
