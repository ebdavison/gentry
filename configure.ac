dnl Process this file with autoconf to produce a configure script.

AC_INIT(gladesrc.c)
AM_INIT_AUTOMAKE(gentry, 0.1.11)
AM_CONFIG_HEADER(config.h)

AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC

PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES(GTK, gtk+-2.0 > 2.0.0)

dnl Only use -Wall if we have gcc
if test "x$GCC" = "xyes"; then
  if test -z "`echo "$CFLAGS" | grep "\-Wall" 2> /dev/null`" ; then
    CFLAGS="$CFLAGS -Wall"
  fi
fi

AC_LIB_Z

AC_LIB_MYSQL
AC_SUBST(MYSQL_INCLUDE)
AC_SUBST(MYSQL_LIB)

AC_OUTPUT(Makefile, echo timestamp > stamp-h)

