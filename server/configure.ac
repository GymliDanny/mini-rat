AC_INIT([mratd], [0.01])
AC_PREREQ([2.71])
AC_CONFIG_SRCDIR([src/mini-rat.c])
AC_CONFIG_HEADERS([include/config.h])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([../m4])

AM_INIT_AUTOMAKE([1.16 -Wall -Werror foreign subdir-objects])
AC_CONFIG_FILES([Makefile])

AC_CHECK_HEADERS([stdio.h stdlib.h string.h pthread.h arpa/inet.h sys/socket.h])

AX_CHECK_COMPILE_FLAG([-Wall], [CFLAGS+=" -Wall"])
AX_CHECK_COMPILE_FLAG([-Wextra], [CFLAGS+= "-Wextra"])

AC_PROG_CC
AC_OUTPUT
