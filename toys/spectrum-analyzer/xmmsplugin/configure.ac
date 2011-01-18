AC_INIT(tutorial2,1.0.0)
AC_CONFIG_SRCDIR(src/plugin_main.c)
AC_CONFIG_AUX_DIR(config)

AM_CONFIG_HEADER(config.h)
AM_INIT_AUTOMAKE

# GCC , install and libtool must be present
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL

# Audacious 1.3.0 or greater must be installed
AM_PATH_GLIB(1.2.2,,AC_MSG_ERROR([*** GLIB >= 1.2.2 not installed - please install first ***]))
AM_PATH_XMMS(1.2.0, , AC_MSG_ERROR([*** xmms >= 1.2.0 not installed - please install first ***]))

# Audacious visualization plugin directory test
AVPD=`xmms-config --visualization-plugin-dir`
AC_SUBST(AVPDIR)

# Audacious visualization plugin data directory
AVPDD=`xmms-config --data-dir`-plugins
AC_SUBST(AVPDD)

AC_OUTPUT(Makefile src/Makefile)
