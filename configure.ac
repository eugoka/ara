AC_INIT([ara], [0.0.1])
AC_CONFIG_AUX_DIR([aux])
AM_INIT_AUTOMAKE([-Wall foreign])

PKG_PROG_PKG_CONFIG
PKG_CHECK_EXISTS([gtest
                  libftdi
                  magic-enum
                  ])

AC_PROG_CXX([clang++])
AC_CONFIG_FILES([Makefile
                 src/Makefile
                 test/Makefile
                 test/unit/Makefile
                 ])
CXXFLAGS+=" -std=c++23"
AC_OUTPUT
