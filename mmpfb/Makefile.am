## Process this file with automake to produce Makefile.in
AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = mmpfb
man_MANS = mmpfb.1

mmpfb_SOURCES = myfont.cc myfont.hh \
	main.cc \
	t1minimize.cc t1minimize.hh \
	t1rewrit.cc t1rewrit.hh

mmpfb_LDADD = ../libefont/libefont.a ../liblcdf/liblcdf.a

AM_CPPFLAGS = -I$(srcdir)/../include
CLEANFILES = @TEMPLATE_OBJS@

EXTRA_DIST = mmpfb.1

distclean-local:
	-rm -rf .deps || rmdir .deps
