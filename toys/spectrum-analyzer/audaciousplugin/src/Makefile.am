lib_LTLIBRARIES = libexample2.la

libdir = @AVPDIR@

AM_CFLAGS = -Wall @AUDACIOUS_CFLAGS@ @CFLAGS@ -DDATADIR=\"@AVPDD@\"
LIBS = @AUDACIOUS_LIBS@

libexample2_la_LDFLAGS = -module -avoid-version
libexample2_la_SOURCES = plugin_main.c 


