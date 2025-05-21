%.pcm: %.ccm
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) --precompile -o $@ $<

check_DATA = arm_semihosting.pcm \
			 bit_field.pcm \
			 jtag.pcm

all: $(check_DATA)

CLEANFILES = $(check_DATA)
