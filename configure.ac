dnl#                                               -*- Autoconf -*-
dnl# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([lcdf-typetools],[2.99],[ekohler@gmail.com])
AC_CONFIG_SRCDIR([NEWS])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([autoconf.h])
AC_CANONICAL_TARGET
AC_OBJEXT
AC_EXEEXT
AC_USE_SYSTEM_EXTENSIONS

AM_INIT_AUTOMAKE([foreign check-news -Wall])
AM_MAINTAINER_MODE
AM_PROG_INSTALL_STRIP

if test "x${CC}" = "x"; then
  test -z "${CC}" || unset CC
  AC_PROG_CC
else
  test -n "${CC}" && export CC
  AC_SUBST([CC])dnl
  ## (just in case this branch gets left empty...)
fi
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_INSTALL
AC_PROG_LN_S

AC_ARG_ENABLE([warnings],
  [AS_HELP_STRING([--enable-warnings],[compile with -Wall -Wextra])],
  [if test "x${enableval}" = "xyes"; then
    CFLAGS="${CFLAGS} -W -Wall"; CXXFLAGS="${CXXFLAGS} -Wall -Wextra"
  fi])

AC_ARG_ENABLE([precondition-checking],
  [AS_HELP_STRING([--enable-precondition-checking],
                  [include precondition checking assertions])],
  [if test "x${enableval}" = "xyes"; then
    AC_DEFINE([HAVE_PRECONDITION_CHECKING],[1],
              [Define to include precondition checking assertions.])
  fi])

AN_MAKEVAR([AR],[AM_PROG_AR])dnl
AN_PROGRAM([ar],[AM_PROG_AR])dnl
AM_PROG_AR

AC_PROG_RANLIB

dnl# Available from the GNU Autoconf Macro Archive at:
dnl# http://www.gnu.org/software/ac-archive/htmldoc/ac_cxx_template_objs.html
dnl#
AC_DEFUN([AC_CXX_TEMPLATE_OBJS],
[AC_REQUIRE([AC_PROG_CXX])
 AC_CACHE_CHECK([where template objects are stored],[ac_cv_cxx_templobjs],
 [ AC_LANG_PUSH([C++])
   ac_cv_cxx_templobjs='unknown'
   if test "$GXX" = yes; then
     ac_cv_cxx_templobjs='nowhere'
   else
     case $CXX in
       CC|*/CC)
        cat > conftest.cc <<EOF
template<class T> class A { public : A () {} };
template<class T> void f (const A<T>&) {}
main()
{ A<double> d;
  A<int> i;
  f (d);
  f (i);
  return 0;
}
EOF
        if test "$ac_cv_cxx_templobjs" = 'unknown' ; then
          if test -d Templates.DB ; then
            rm -fr Templates.DB
          fi
          if $CXX $CXXFLAGS -ptr. -c conftest.cc 1> /dev/null 2>&1; then
            if test -d Templates.DB ; then
            # this should be Sun CC <= 4.2
              CXXFLAGS="$CXXFLAGS -ptr."
              if test x"$LIBTOOL" = x ; then
                ac_cv_cxx_templobjs='Templates.DB/*.o'
              else
                ac_cv_cxx_templobjs='Templates.DB/*.lo'
              fi
              rm -fr Templates.DB
            fi
          fi
        fi
        if test "$ac_cv_cxx_templobjs" = 'unknown' ; then
          if test -d SunWS_cache ; then
            rm -fr SunWS_cache
          fi
          if $CXX $CXXFLAGS -c conftest.cc 1> /dev/null 2>&1; then
            if test -d SunWS_cache ; then
            # this should be Sun WorkShop C++ compiler 5.x
            # or Sun Forte C++ compiler >= 6.x
              if test x"$LIBTOOL" = x ; then
                ac_cv_cxx_templobjs='SunWS_cache/*/*.o'
              else
                ac_cv_cxx_templobjs='SunWS_cache/*/*.lo'
              fi
              rm -fr SunWS_cache
            fi
          fi
        fi
        rm -f conftest* ;;
     esac
   fi
   case "x$ac_cv_cxx_templobjs" in
     xunknown|xnowhere)
     TEMPLATE_OBJS="" ;;
     *)
     TEMPLATE_OBJS="$ac_cv_cxx_templobjs" ;;
   esac])
   AC_SUBST([TEMPLATE_OBJS])
   AC_LANG_POP
])

AN_MAKEVAR([TEMPLATE_OBJS],[AC_CXX_TEMPLATE_OBJS])
AC_CXX_TEMPLATE_OBJS

dnl#
dnl# standard headers
dnl#

AC_LANG_PUSH([C])
AC_HEADER_STDBOOL dnl# Calls AC_CHECK_HEADER_STDBOOL
AC_HEADER_DIRENT
AC_CHECK_HEADERS([ctype.h fcntl.h limits.h stddef.h sys/time.h sys/wait.h])
if test "x${ac_cv_header_inttypes_h}" = "x"; then
  test -z "${ac_cv_header_inttypes_h}" || unset ac_cv_header_inttypes_h
  AC_CHECK_HEADERS([inttypes.h])
fi
if test "x${ac_cv_header_unistd_h}" = "x"; then
  test -z "${ac_cv_header_unistd_h}" || unset ac_cv_header_unistd_h
  AC_CHECK_HEADERS([unistd.h])
fi
AC_LANG_POP

dnl#
dnl# <new> and/or <new.h>
dnl#

AC_LANG_PUSH([C++])
AC_CACHE_CHECK([whether <new> works],[ac_cv_good_new_hdr],
[AC_LINK_IFELSE([AC_LANG_SOURCE([[#include <new>]],[[
  int a;
  int *b = new(&a) int;
  return 0;
]])],[ac_cv_good_new_hdr=yes],[ac_cv_good_new_hdr=no])])
if test "$ac_cv_good_new_hdr" = yes; then
   AC_DEFINE([HAVE_NEW_HDR],[1],[Define if <new> exists and works.])
fi
AC_CHECK_HEADERS([new.h])

dnl#
dnl# endianness
dnl#

AC_C_BIGENDIAN([
  AC_DEFINE([WORDS_BIGENDIAN],[1],[Define to 1 if your processor stores words with the most significant byte first (like PowerPC, unlike Intel).])
  ],[
  AC_DEFINE([WORDS_LITTLEENDIAN],[1],[Define to 1 if your processor stores words with the least significant byte first (like Intel and VAX, unlike Motorola and SPARC).])
])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_INT8_T
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

dnl#
dnl# functions such as strerror, working strtod, working strnlen, strtoul,
dnl# time headers such as unistd.h
dnl#

AC_LANG_PUSH([C])
need_fixlibc=0
AC_CHECK_FUNCS([strerror],[],[need_fixlibc=1])
AC_SEARCH_LIBS([strerror],[cposix])dnl

AC_CHECK_HEADERS_ONCE([stdio.h])
AC_CACHE_CHECK([for broken strtod],[ac_cv_broken_strtod],
[AC_RUN_IFELSE([AC_LANG_SOURCE([[#include <stdio.h>
#include <stdlib.h>
int main(int c, char **v) {
  char s[] = "12 ";
  char *endp;
  double d = strtod(s, &endp);
  return (s + 2) == endp;
}]])],[ac_cv_broken_strtod=yes],[ac_cv_broken_strtod=no],
[ac_cv_broken_strtod=no])])
if test "x$ac_cv_broken_strtod" = xyes; then
    need_fixlibc=1
    AC_DEFINE([HAVE_BROKEN_STRTOD],[1],[Define if strtod is broken.])
fi
AC_LANG_POP

AC_LANG_PUSH([C++])
AC_CHECK_DECLS([strnlen],[],[],[#include <string.h>])
AC_CHECK_FUNCS([strnlen],[have_strnlen=1],[need_fixlibc=1])
if test "x${have_strnlen}" = "x1"; then
    AC_CACHE_CHECK([for broken strnlen],[ac_cv_broken_strnlen],
    [AC_RUN_IFELSE([AC_LANG_SOURCE([[#include <string.h>
#include <stdio.h>
int main(int c, char **v) {
  char s[] = "01234567891";
  return strnlen(s, 10) == 10;
}]])],[ac_cv_broken_strnlen=yes],[ac_cv_broken_strnlen=no],[ac_cv_broken_strnlen=no])])
    if test "x${ac_cv_broken_strnlen}" = "xyes"; then
	need_fixlibc=1
	AC_DEFINE([HAVE_BROKEN_STRNLEN],[1],[Define if strnlen is broken.])
    fi
fi
AC_LANG_POP

# Checks for library functions.
AC_LANG_PUSH([C])
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRNLEN
AC_FUNC_STRTOD
AC_CHECK_FUNCS([ctime ftruncate memchr memcpy memmove memset mkdir \
                mkstemp printf putenv sigaction sqrt strchr strdup \
                strrchr strstr strtol strtoul vsnprintf waitpid])
AC_CHECK_FUNC([floor],[],[AC_CHECK_LIB([m],[floor])])
AC_CHECK_FUNC([fabs],[],[AC_CHECK_LIB([m],[fabs])])
AM_CONDITIONAL([FIXLIBC],[test "x${need_fixlibc}" = "x1"])
AC_LANG_POP

AC_LANG_PUSH([C++])
AC_CHECK_FUNCS([floor])
AC_LANG_POP

dnl#
dnl# integer types
dnl#

AC_LANG_PUSH([C])
AC_CHECK_HEADER([inttypes.h],[have_inttypes_h=yes],[have_inttypes_h=no])
AC_CHECK_HEADER([sys/types.h],[have_sys_types_h=yes],[have_sys_types_h=no])

if test $have_inttypes_h = no -a $have_sys_types_h = yes; then
    AC_CACHE_CHECK([for uintXX_t typedefs], ac_cv_uint_t,
    [AC_EGREP_HEADER(dnl
changequote(<<,>>)<<(^|[^a-zA-Z_0-9])uint32_t[^a-zA-Z_0-9]>>changequote([,]),
    [sys/types.h],[ac_cv_uint_t=yes],[ac_cv_uint_t=no])])
fi
if test $have_inttypes_h = no -a $have_sys_types_h = yes -a "$ac_cv_uint_t" = no; then
    AC_CACHE_CHECK([for u_intXX_t typedefs], ac_cv_u_int_t,
    [AC_EGREP_HEADER(dnl
changequote(<<,>>)<<(^|[^a-zA-Z_0-9])u_int32_t[^a-zA-Z_0-9]>>changequote([,]),
    [sys/types.h],[ac_cv_u_int_t=yes],[ac_cv_u_int_t=no])])
fi
if test $have_inttypes_h = yes -o "$ac_cv_uint_t" = yes; then
    :
elif test "$ac_cv_u_int_t" = yes; then
    AC_DEFINE([HAVE_U_INT_TYPES],[1],[Define if you have u_intXX_t types but not uintXX_t types.])
else
    AC_MSG_WARN([
=========================================

Neither uint32_t nor u_int32_t defined by <inttypes.h> or <sys/types.h>!
Assuming "short" has 16 bits and "int" has 32 bits.

=========================================])
    AC_DEFINE([HAVE_FAKE_INT_TYPES],[1],[Define if intXX_t types are not available.])
fi

AC_CHECK_TYPES([uintptr_t],[],[],
[#if defined(HAVE_INTTYPES_H) && HAVE_INTTYPES_H
# include <inttypes.h>
#endif /* HAVE_INTTYPES_H */
#if defined(HAVE_SYS_TYPES_H) && HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif /* HAVE_SYS_TYPES_H */
])dnl

AC_CHECK_SIZEOF([void *])
AC_CHECK_ALIGNOF([void *])
AC_CHECK_SIZEOF([unsigned long])
AC_CHECK_ALIGNOF([unsigned long])
AC_CHECK_SIZEOF([unsigned int])
AC_CHECK_ALIGNOF([unsigned int])
AC_LANG_POP

dnl#
dnl# ntohs, ntohl (need them in C++ code)
dnl#

AC_LANG_PUSH([C++])
AC_CHECK_HEADERS([byteorder.h netinet/in.h sys/param.h])
AC_CHECK_HEADERS_ONCE([winsock2.h])
AC_MSG_CHECKING([whether ntohs and ntohl are defined])
ac_ntoh_defined=no
AC_COMPILE_IFELSE(
	[AC_LANG_PROGRAM([[#if HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif
#if HAVE_BYTEORDER_H
# include <byteorder.h>
#elif HAVE_NETINET_IN_H
# include <netinet/in.h>
#elif HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif
#ifdef WIN32
# ifdef __MSC_VER
#  pragma warning (disable: 4290)
# endif
# include <winsock2.h>
#endif
]],[[(void) ntohs(0x0020), (void) ntohl(0x03040020);]])],
	[AC_MSG_RESULT([yes])
ac_ntoh_defined=yes],
	[AC_MSG_RESULT([no])])
if test $ac_ntoh_defined = no; then
    AC_CHECK_HEADERS([arpa/inet.h],[have_arpa_inet_h=yes],[have_arpa_inet_h=no])
    if test $have_arpa_inet_h = yes; then
	AC_MSG_CHECKING([whether ntohs and ntohl are defined in <arpa/inet.h>])
	AC_COMPILE_IFELSE(
		[AC_LANG_PROGRAM([[#if HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <arpa/inet.h>
]], [[(void) ntohs(0x0020), (void) ntohl(0x03040020);]])],
		[AC_MSG_RESULT([yes])
AC_DEFINE([NEED_ARPA_INET_H],[1],[Define to 1 if you must include <arpa/inet.h> to get `ntohl'.])
ac_ntoh_defined=yes],
		[AC_MSG_RESULT([no])])
    fi
fi
if test $ac_ntoh_defined = no; then
    AC_MSG_ERROR([
=========================================

Cannot find a definition for ntohs and/or ntohl!

=========================================])
fi
AC_LANG_POP

AC_LANG_PUSH([C])
AC_SEARCH_LIBS([ntohs],[-lnet -lwinsock32])
AC_LANG_POP

dnl#
dnl# is va_list addressable?
dnl#

AC_LANG_PUSH([C++])
AC_CACHE_CHECK([for addressable va_list type],
    [ac_cv_va_list_addr],
    [AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <stdarg.h>
void f(va_list *) {
}
void g(va_list val) {
    f(&val);
}
void h(int a, ...) {
    va_list val;
    va_start(val, a);
    g(val);
    va_end(val);
}]],[[h(2, 3, 4);]])],[ac_cv_va_list_addr=yes],[ac_cv_va_list_addr=no])])
if test "x${ac_cv_va_list_addr}" = "xyes"; then
    AC_DEFINE([HAVE_ADDRESSABLE_VA_LIST],[1],[Define if the va_list type is addressable.])
fi
AC_LANG_POP

dnl#
dnl# select programs to be compiled and automatically run
dnl#

m4_include([m4/lcdf-typetools.m4])dnl

SELECTED_SUBDIRS=""
m4_foreach_w([Kpse_Prog],[kpse_lcdf_typetools_progs],[test "x$enable_[]Kpse_Prog" = "xno" || SELECTED_SUBDIRS="${SELECTED_SUBDIRS} Kpse_Prog"
])
AC_SUBST([SELECTED_SUBDIRS])dnl

m4_foreach_w([Kpse_Opt],[kpse_otftotfm_auto_opts],[AS_IF([test "x$enable_auto_]Kpse_Opt[" != xno],
                  [AC_DEFINE([HAVE_AUTO_]AS_TR_CPP(Kpse_Opt), 1,
                             [Define to run ]Kpse_Opt[ from otftotfm.])])
])

dnl#
dnl# kpathsea
dnl#

AC_ARG_WITH([kpathsea],
  [AS_HELP_STRING([--with-kpathsea=PREFIX],
                  [Kpathsea is installed (under PREFIX)])],
  [kpathsea=${withval}],[kpathsea=""])

KPATHSEA_INCLUDES=""
KPATHSEA_LIBS=""
KPATHSEA_DEPEND=""

if test "x${kpathsea}" != "xno"; then
    SAVE_CPPFLAGS="${CPPFLAGS}"; SAVE_LDFLAGS="${LDFLAGS}"
    if test "x${kpathsea}" != "x" -a "x${kpathsea}" != "xyes"; then
	KPATHSEA_INCLUDES="-I${kpathsea}/include"
	KPATHSEA_LIBS="-L${kpathsea}/lib"
    else
	kpathsea=yes
    fi

    dnl# check for kpathsea/kpathsea.h
    if true; then
	CPPFLAGS="${SAVE_CPPFLAGS} ${KPATHSEA_INCLUDES}"
	AC_CHECK_HEADER([kpathsea/kpathsea.h],[kpse_header=yes], [kpse_header=no])
    fi

    dnl# if not found and kpathsea prefix not set, check /usr/local,
    dnl# and then some other places
    if test "x${kpse_header}" != "xyes" -a "x${kpathsea}" = "xyes"; then
	if test -r /usr/local/include/kpathsea/tex-file.h; then
	    KPATHSEA_INCLUDES="-I/usr/local/include"
	    KPATHSEA_LIBS="-L/usr/local/lib"
	elif test -d "${prefix}" -a -r ${prefix}/include/kpathsea/tex-file.h; then
	    KPATHSEA_INCLUDES="-I${prefix}/include"
	    KPATHSEA_LIBS="-L${prefix}/lib"
	elif test -d "${exec_prefix}" -a -r ${exec_prefix}/include/kpathsea/tex-file.h; then
	    KPATHSEA_INCLUDES="-I${exec_prefix}/include"
	    KPATHSEA_LIBS="-L${exec_prefix}/lib"
	elif test -d "${includedir}" -a -r ${includedir}/kpathsea/tex-file.h -a -d "${libdir}"; then
	    KPATHSEA_INCLUDES="-I${includedir}"
	    KPATHSEA_LIBS="-L${libdir}"
	elif test -d "${oldincludedir}" -a -r ${oldincludedir}/kpathsea/tex-file.h -a -d "${oldincludedir}/../lib"; then
	    KPATHSEA_INCLUDES="-I${oldincludedir}"
	    KPATHSEA_LIBS="-L${oldincludedir}/../lib"
	else
	    AC_MSG_WARN([unsure where to look for kpathsea])
	fi
	CPPFLAGS="${SAVE_CPPFLAGS} ${KPATHSEA_INCLUDES}"
	AC_CHECK_HEADER([kpathsea/tex-file.h],[kpse_header=yes],[kpse_header=no])
    fi

    dnl# check for -lkpathsea
    if true; then
	LDFLAGS="${SAVE_LDFLAGS} ${KPATHSEA_LIBS}"
	AC_CHECK_LIB([kpathsea],[kpse_set_program_name],[kpse_library=yes],[kpse_library=no])
    fi

    dnl# bail out if not found
    if test "x${kpse_header}" != "xyes" -o "x${kpse_library}" != "xyes"; then
	AC_MSG_ERROR([
=========================================

I cannot find the kpathsea library and/or header files. Tell me where to
look using the --with-kpathsea=PREFIX option (header files should be under
PREFIX/include/kpathsea, and library under PREFIX/lib), or disable support
using --without-kpathsea.

=========================================])
    fi

    dnl# OK, found
    AC_DEFINE([HAVE_KPATHSEA],[1],[Define if you want to use kpathsea.])
    if true; then
	KPATHSEA_LIBS="${KPATHSEA_LIBS} -lkpathsea"
    fi

    dnl# does it have opentype support?
    if false; then
	AC_DEFINE([HAVE_DECL_KPSE_OPENTYPE_FORMAT],[1],
	          [Define to 1 if you have a declaration for the kpse opentype format.])
    else
	AC_CHECK_DECLS([kpse_opentype_format],[],[],[#include <kpathsea/kpathsea.h>])
    fi

    dnl# does it have a separate format for encoding files?
    AC_CHECK_DECLS([kpse_enc_format],[],[],[#include <kpathsea/kpathsea.h>])

    CPPFLAGS="${SAVE_CPPFLAGS}"; LDFLAGS="${SAVE_LDFLAGS}"
fi

AM_CONDITIONAL([have_kpathsea],[test "x${with_kpathsea}" != "xno"])
AC_SUBST([KPATHSEA_INCLUDES])dnl
AC_SUBST([KPATHSEA_LIBS])dnl
AC_SUBST([KPATHSEA_DEPEND])dnl

AC_ARG_ENABLE([selfauto-set],
  [AS_HELP_STRING([--disable-selfauto-set],
                  [disable setting SELFAUTO variables from kpsewhich])],
  [],[enable_selfauto_set=yes])

if test "x$enable_selfauto_set" = xyes -a "x$kpathsea" != xno; then
    kpsewhich='kpsewhich'
    test "x$kpathsea" != xyes -a -x "$kpathsea/bin/kpsewhich" && kpsewhich="$kpathsea/bin/kpsewhich"
    SELFAUTOLOC=`$kpsewhich --expand-var='$SELFAUTOLOC' 2>/dev/null`
    SELFAUTODIR=`$kpsewhich --expand-var='$SELFAUTODIR' 2>/dev/null`
    SELFAUTOPARENT=`$kpsewhich --expand-var='$SELFAUTOPARENT' 2>/dev/null`
    if test -z "$SELFAUTODIR"; then
	AC_MSG_WARN([
=========================================

Could not extract SELFAUTO variables from $kpsewhich.
Either supply the correct PREFIX to --with-kpathsea, or supply
--disable-selfauto-loc.

=========================================])
    fi
    AC_DEFINE_UNQUOTED([SELFAUTOLOC],["$SELFAUTOLOC"],[kpsewhich's $SELFAUTOLOC variable])
    AC_DEFINE_UNQUOTED([SELFAUTODIR],["$SELFAUTODIR"],[kpsewhich's $SELFAUTODIR variable])
    AC_DEFINE_UNQUOTED([SELFAUTOPARENT],["$SELFAUTOPARENT"],[kpsewhich's $SELFAUTOPARENT variable])
fi


dnl#
dnl# glyphlist.txt and *.enc installation locations
dnl#

AC_ARG_ENABLE([glyphlistdir],
  [AS_HELP_STRING([--enable-glyphlistdir=DIR],[store glyphlist.txt files in DIR [pkgdatadir]])],[glyphlistdir="$enableval"],[glyphlistdir=NONE])
AC_ARG_ENABLE([encdir],
  [AS_HELP_STRING([--enable-encdir=DIR],[store encoding files in DIR [pkgdatadir]])],[encdir="$enableval"],[encdir=NONE])

test "x$glyphlistdir" = xNONE -o "x$glyphlistdir" = xno \
     -o "x$glyphlistdir" = x && glyphlistdir='${pkgdatadir}'
test "x$encdir" = xNONE -o "x$encdir" = xno \
     -o "x$encdir" = x && encdir='${pkgdatadir}'

AC_SUBST([glyphlistdir])dnl
AC_SUBST([encdir])dnl

dnl#
dnl# TeX Live build stub
dnl#

KPATHSEA_RULE=""
AC_SUBST([KPATHSEA_RULE])dnl

dnl#
dnl# verbatim portions of the header
dnl#

AC_DEFINE([HAVE_PERMSTRING],[1],[Define if PermStrings are available.])
AC_DEFINE([MMAFM_RUN_MMPFB],[1],[Define to 0 if you do not want mmafm to run mmpfb when it needs to get an intermediate master conversion program.])
dnl# PATHNAME_SEPARATOR?


dnl#
dnl# set path variables
dnl#

shell_expand () {
    val=`eval echo '$'"$1"`
    while echo "x$val" | grep '\$' >/dev/null 2>&1; do val=`eval echo "$val"`; done
    val=`echo "$val" | sed 's,//*,/,g'`
    eval "$1='$val'"
}

dnl# Preset $prefix and $exec_prefix.
test "x$prefix" = xNONE && prefix=$ac_default_prefix
test "x$exec_prefix" = xNONE && exec_prefix='${prefix}'

pkgdatadir="${datadir}/$PACKAGE"
gdir="$glyphlistdir"
for i in data dataroot pkgdata g; do shell_expand ${i}dir; done
AC_DEFINE_UNQUOTED([GLYPHLISTDIR], ["$gdir"], [Directory for glyphlist.txt files.])

dnl#
dnl# Output
dnl#

AC_CONFIG_FILES([Makefile liblcdf/Makefile libefont/Makefile cfftot1/Makefile mmafm/Makefile mmpfb/Makefile otfinfo/Makefile otftotfm/Makefile t1dotlessj/Makefile t1lint/Makefile t1rawafm/Makefile t1reencode/Makefile t1testpage/Makefile ttftotype42/Makefile])
AC_OUTPUT
