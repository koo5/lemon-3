lib_LTLIBRARIES = libexample2.la

libdir = @XMMS_VISUALIZATION_PLUGIN_DIR@

AM_CFLAGS = -Wall @XMMS_CFLAGS@ @CFLAGS@ -DDATADIR=\"@AVPDD@\"
LIBS = @XMMS_LIBS@

libexample2_la_LDFLAGS = -module -avoid-version
libexample2_la_SOURCES = plugin_main.c

