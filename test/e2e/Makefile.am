check_PROGRAMS = test
AM_LDFLAGS = $(top_builddir)/src/ftdi.pcm $(top_builddir)/src/bit_field.pcm -lftdi1 -lusb-1.0
AM_CXXFLAGS = -fprebuilt-module-path=$(top_builddir)/src
BUILT_SOURCES = $(top_builddir)/src/bit_field.pcm \
				$(top_builddir)/src/ftdi.pcm \
				$(top_builddir)/src/jtag.pcm

test_SOURCES = test.cc $(top_builddir)/src/bit_field.cc $(BUILT_SOURCES)
test_CPPFLAGS = -I$(top_builddir)/src

TESTS = $(check_PROGRAMS)
