## Process this file with automake to produce Makefile.in
AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = mmafm
man_MANS = mmafm.1

mmafm_SOURCES = main.cc

mmafm_LDADD = ../libefont/libefont.a ../liblcdf/liblcdf.a

AM_CPPFLAGS = -I$(srcdir)/../include
CLEANFILES = @TEMPLATE_OBJS@

EXTRA_DIST = mmafm.1

distclean-local:
	-rm -rf .deps || rmdir .deps
