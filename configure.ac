#############################################################################
# Configuration file for flexIRF.                                           #
# The whole file is copy-pasted from the configuration file from GammaLib,  #
# by Juergen Knoedlseder, so all credits go to him, and GammaLib developers #
#############################################################################
AC_PREREQ([2.69])
AC_INIT(flexIRF, 0.6, thassan@ifae.es)
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([gnu no-dependencies -Wno-portability subdir-objects])

AC_OUTPUT
AC_CONFIG_SRCDIR([src/IRFgenerator_rootio.cc])
AC_CONFIG_HEADER([config.h])
LT_INIT
ACLOCAL_AMFLAGS="-I m4"


#############################################################################
# Set prefix                                                                #
# ------------------------------------------------------------------------- #
# By default, flexIRF gets installed into "/usr/local/flexIRF".  The default #
# can be overwritten using the --prefix option.                             #
#############################################################################
AC_PREFIX_DEFAULT([/usr/local/flexIRF])
if test "x$prefix" = xNONE; then
  flexIRF_prefix="/usr/local/flexIRF"
else
  flexIRF_prefix=${prefix}
fi
AC_DEFINE_UNQUOTED([PACKAGE_PREFIX], "${flexIRF_prefix}", [Location where library is installed])


#############################################################################
# Determine source directory for unit testing                               #
#############################################################################
PACKAGE_SOURCE=`pwd`
AC_DEFINE_UNQUOTED([PACKAGE_SOURCE], "${PACKAGE_SOURCE}", [Location where source is installed])
AC_SUBST(PACKAGE_SOURCE)


#############################################################################
# Check for programs that are required by automake                          #
#############################################################################
AC_PROG_MAKE_SET


#############################################################################
# Determine the build and host system type                                  #
#############################################################################
AC_CANONICAL_HOST


#############################################################################
# Check for valid C++ compiler.  This has to come before the check for      #
# header files, otherwise we won't detect if the C++ compiler is missing.   #
#                                                                           #
# A special kluge was added for Mac OS X >=10.7. As Apple switched to clang #
# we search from this version on for the clang compiler.                    #
#                                                                           #
# We also signal whether Mac OS X >=10.9 (Mavericks) and Mac OS X >=10.11   #
# (El Capitan).                                                             #
#############################################################################
# Set default Mac OS X version flags
osx_lion=no
osx_mavericks=no
osx_el_capitan=no

# Set default compiler lists
cxx_list="g++ c++ gpp aCC CC cxx cc++ cl.exe FCC KCC RCC xlC_r xlC clang++"
cc_list="gcc cc cl.exe clang"

# Check if we have Mac OS X >= 10.7, >= 10.9 and >= 10.11
if test `uname -s` = "Darwin"; then
  version=`sw_vers -productVersion | sed 's/\(10\.[[0-9]]*\).*/\1/' | sed -e 's/\.//g'`
  if [ test $version -ge 107 ]; then
    cxx_list="clang++ g++ c++ gpp aCC CC cxx cc++ cl.exe FCC KCC RCC xlC_r xlC"
    cc_list="clang gcc cc cl.exe"
    osx_lion=yes
  fi
  if [ test $version -ge 109 ]; then
    osx_mavericks=yes
  fi
  if [ test $version -ge 1011 ]; then
    osx_el_capitan=yes
  fi
fi
AC_MSG_CHECKING([for Mac OS X >= 10.7])
if [test "x$osx_lion" = "xyes"]; then
        AC_MSG_RESULT(yes)
else
        AC_MSG_RESULT(no)
fi
AC_MSG_CHECKING([for Mac OS X >= 10.9])
if [test "x$osx_mavericks" = "xyes"]; then
        AC_MSG_RESULT(yes)
else
        AC_MSG_RESULT(no)
fi
AC_MSG_CHECKING([for Mac OS X >= 10.11])
if [test "x$osx_el_capitan" = "xyes"]; then
        AC_MSG_RESULT(yes)
else
        AC_MSG_RESULT(no)
fi
AM_CONDITIONAL(IS_ELCAPITAN, test "x$osx_el_capitan" = "xyes")

# Our language is C++
AC_LANG(C++)
AC_PROG_CXX($cxx_list)
AC_PROG_CC($cc_list)


#############################################################################
# Darwin option: Determine architecture of universal library to be built    #
# ------------------------------------------------------------------------- #
# --with-univeral-archs=VALUE, where VALUE is one of:                       #
# 32-bit - ppc, i386                                                        #
# 64-bit - ppc64, x86_64                                                    #
# 3-way  - ppc, i386, x86_64 (default)                                      #
# intel  - i386, x86_64                                                     #
# all    - ppc, ppc64, i386, x86_64                                         #
#############################################################################
UNIVERSAL_ARCHS="3-way"
AC_SUBST(LIPO_32BIT_FLAGS)
AC_MSG_CHECKING(for --with-universal-archs)
AC_ARG_WITH(universal-archs,
    AS_HELP_STRING([--with-universal-archs=ARCH],
                   [select architectures for universal build ("32-bit", "64-bit", "3-way", "intel", or "all")]),
[
	AC_MSG_RESULT($withval)
	UNIVERSAL_ARCHS="$withval"
],
[
 	AC_MSG_RESULT(3-way)
])


#############################################################################
# Darwin option: Determine if a universal library should be built           #
# ------------------------------------------------------------------------- #
# --enable-universalsdk=PATH takes an optional argument that specifies      #
# which OSX SDK should be used to perform the build.                        #
# This defaults to:                                                         #
# "/Developer/SDKs/MacOSX.10.4u.sdk" for a '32-bit' build                   #
# "/Developer/SDKs/MacOSX.10.5.sdk"  for any other build                    #
#############################################################################
AC_MSG_CHECKING([for --enable-universalsdk])
AC_ARG_ENABLE(universalsdk,
	AS_HELP_STRING([--enable-universalsdk@<:@=SDKDIR@:>@], [Build against Mac OS X 10.4 SDK (ppc/i386)]),
[
	case $enableval in
	yes)
        if [test "${UNIVERSAL_ARCHS}" = "32-bit";] then
            enableval=/Developer/SDKs/MacOSX10.4u.sdk
        else
            enableval=/Developer/SDKs/MacOSX10.5.sdk
        fi
		if [test ! -d "${enableval}";] then
			enableval=/
        fi
		;;
	esac
	case $enableval in
	no)
		UNIVERSALSDK=
		enable_universalsdk=
		;;
	*)
		UNIVERSALSDK=$enableval
		if [test ! -d "${UNIVERSALSDK}"]; then
			AC_MSG_ERROR([--enable-universalsdk specifies non-existing SDK: ${UNIVERSALSDK}])
		fi
		;;
	esac
	
],[
   	UNIVERSALSDK=
	enable_universalsdk=
])
if [test -n "${UNIVERSALSDK}"]; then
	AC_MSG_RESULT(${UNIVERSALSDK})
else
	AC_MSG_RESULT(no)
fi
AC_SUBST(UNIVERSALSDK)


#############################################################################
# Darwin option: Check whether --with-universal-archs was specified without #
#                --enable-universalsdk                                      #
#############################################################################
if [test "${withval}"]; then
    if [test "${enable_universalsdk}"]; then
		:
	else
		AC_MSG_ERROR([--with-universal-archs without --enable-universalsdk.])
	fi
fi


#############################################################################
# Set name for machine-dependent library files                              #
#############################################################################
AC_SUBST(MACHDEP)
AC_MSG_CHECKING(MACHDEP)
if test -z "$MACHDEP"; then
    ac_sys_system=`uname -s`
	if test "$ac_sys_system" = "AIX" \
	-o "$ac_sys_system" = "UnixWare" -o "$ac_sys_system" = "OpenUNIX"; then
		ac_sys_release=`uname -v`
	else
		ac_sys_release=`uname -r`
	fi
	ac_md_system=`echo $ac_sys_system |
			   tr -d '[/ ]' | tr '[[A-Z]]' '[[a-z]]'`
	ac_md_release=`echo $ac_sys_release |
			   tr -d '[/ ]' | sed 's/^[[A-Z]]\.//' | sed 's/\..*//'`

    # Set MACHDEP
    MACHDEP="$ac_md_system$ac_md_release"
    case $MACHDEP in
    cygwin*) MACHDEP="cygwin";;
    darwin*) MACHDEP="darwin";;
    atheos*) MACHDEP="atheos";;
    irix646) MACHDEP="irix6";;
    '')	MACHDEP="unknown";;
    esac
fi
AC_MSG_CHECKING(machine type as reported by uname -m)
ac_sys_machine=`uname -m`
AC_MSG_RESULT($ac_sys_machine)


#############################################################################
# Set system-dependent library definitions                                  #
#############################################################################
AC_MSG_CHECKING([for the shared library paths environment variable])
case $ac_sys_system in
    BeOS*)
        SHAREDSUFFIX='.so'
        ;;
    CYGWIN*)
        SHAREDSUFFIX='.dll.a'
        ;;
    SunOS*)
        SHAREDSUFFIX='.so'
        RUNSHARED='LD_LIBRARY_PATH'
        ;;
    Linux*|GNU*|NetBSD*|FreeBSD*|DragonFly*)
        SHAREDSUFFIX='.so'
        RUNSHARED='LD_LIBRARY_PATH'
        ;;
    hp*|HP*)
        case `uname -m` in
            ia64)
                SHAREDSUFFIX='.so'
                ;;
            *)
                SHAREDSUFFIX='.sl'
                ;;
        esac
        RUNSHARED='SHLIB_PATH'
        ;;
    OSF*)
        SHAREDSUFFIX='.so'
        RUNSHARED='LD_LIBRARY_PATH'
        ;;
    atheos*)
        SHAREDSUFFIX='.so'
        RUNSHARED='DLL_PATH'
	    ;;
    Darwin*)
        SHAREDSUFFIX='.dylib'
        RUNSHARED='DYLD_LIBRARY_PATH'
        ;;
    AIX*)
        SHAREDSUFFIX='.so'
        RUNSHARED='LIBPATH'
        ;;
esac
AC_MSG_RESULT($RUNSHARED)
AC_SUBST(SHAREDSUFFIX)
AC_SUBST(RUNSHARED)


#############################################################################
# Set -arch flags for universal builds on OSX and tweak BASECFLAGS based on #
# compiler and platform (note that BASECFLAGS are not yet used).            #
#############################################################################
# Initialise the -arch flags for universal builds on OSX
UNIVERSAL_ARCH_FLAGS=
AC_SUBST(UNIVERSAL_ARCH_FLAGS)

# Tweak BASECFLAGS based on compiler and platform
case $GCC in
yes)

    # if using gcc on alpha, use -mieee to get (near) full IEEE 754
    # support.  Without this, treatment of subnormals doesn't follow
    # the standard.
    case $ac_sys_machine in
        alpha*)
            BASECFLAGS="$BASECFLAGS -mieee"
            ;;
    esac

    case $ac_sys_system in
	SCO_SV*)
	    BASECFLAGS="$BASECFLAGS -m486 -DSCO5"
	    ;;
	# is there any other compiler on Darwin besides gcc?
	Darwin*)
	    # -Wno-long-double, -no-cpp-precomp, and -mno-fused-madd
	    # used to be here, but non-Apple gcc doesn't accept them.
	    if test "${CC}" = gcc; then
            AC_MSG_CHECKING(which compiler should be used)
		    case "${UNIVERSALSDK}" in
		    */MacOSX10.4u.sdk)
			    # Build using 10.4 SDK, force usage of gcc when the 
			    # compiler is gcc, otherwise the user will get very
			    # confusing error messages when building on OSX 10.6
			    CC=gcc-4.0
			    CXX=g++-4.0
			    CPP=cpp-4.0
                ac_ct_CC="$CC"
                ac_ct_CXX="$CXX"
                ac_cv_prog_ac_ct_CC="$ac_ct_CC"
                ac_cv_prog_ac_ct_CXX="$ac_ct_CXX"
			    ;;
		    esac
		    AC_MSG_RESULT($CXX)
        fi

	    # Calculate the right deployment target for this build. x86_64
        # support is only available in SDK 10.5, hence any build including
        # a x86_64 target needs 10.5. i386 came in in SDK 10.4, hence we
        # need 10.4 for a 32-bit build.
	    cur_target=`sw_vers -productVersion | sed 's/\(10\.[[0-9]]*\).*/\1/'`
	    if test ${cur_target} '>' 10.2; then
		    cur_target=10.3
		    if test ${enable_universalsdk}; then
			    if test "${UNIVERSAL_ARCHS}" = "all"; then
				    # Ensure that the default platform for a 
				    # 4-way universal build is OSX 10.5, 
				    # that's the first OS release where 
				    # 4-way builds make sense.
				    cur_target='10.5'
			    elif test "${UNIVERSAL_ARCHS}" = "3-way"; then
				    cur_target='10.5'
			    elif test "${UNIVERSAL_ARCHS}" = "intel"; then
				    cur_target='10.5'
			    elif test "${UNIVERSAL_ARCHS}" = "64-bit"; then
				    cur_target='10.5'
			    elif test "${UNIVERSAL_ARCHS}" = "32-bit"; then
				    cur_target='10.4'
			    fi
		    else
			    if test `/usr/bin/arch` = "i386"; then
				    # On Intel macs default to a deployment
				    # target of 10.4, that's the first OSX
				    # release with Intel support.
				    cur_target="10.4"
			    fi
		    fi
	    fi
	    CONFIGURE_MACOSX_DEPLOYMENT_TARGET=${MACOSX_DEPLOYMENT_TARGET-${cur_target}}
        
        # Make sure that deployment target is >= SDK
        case "${UNIVERSALSDK}" in
        */MacOSX10.4u.sdk)
            if [test ${cur_target} '>' 10.4;] then
	            AC_MSG_ERROR([Invalid MACOSX_DEPLOYMENT_TARGET=$CONFIGURE_MACOSX_DEPLOYMENT_TARGET for SDK 10.4. Specify --enable-universalsdk=/Developer/SDKs/MacOSX${cur_target}.sdk])
            fi
            ;;
        */MacOSX10.5.sdk)
            if [test ${cur_target} '>' 10.5;] then
	            AC_MSG_ERROR([Invalid MACOSX_DEPLOYMENT_TARGET=$CONFIGURE_MACOSX_DEPLOYMENT_TARGET for SDK 10.5. Specify --enable-universalsdk=/Developer/SDKs/MacOSX${cur_target}.sdk])
            fi
            ;;
        */MacOSX10.6.sdk)
            if [test ${cur_target} '>' 10.6;] then
	            AC_MSG_ERROR([Invalid MACOSX_DEPLOYMENT_TARGET=$CONFIGURE_MACOSX_DEPLOYMENT_TARGET for SDK 10.6. Specify --enable-universalsdk=/Developer/SDKs/MacOSX${cur_target}.sdk])
            fi
            ;;
        esac
	    
        # Set compile, link and pre-processor flags dependent on architecture
	    if test "${enable_universalsdk}"; then

	        # Make sure that MACOSX_DEPLOYMENT_TARGET is set in the 
	        # environment with a value that is the same as what we'll use
	        # in the Makefile to ensure that we'll get the same compiler
	        # environment during configure and build time.
	        MACOSX_DEPLOYMENT_TARGET="$CONFIGURE_MACOSX_DEPLOYMENT_TARGET"
	        export MACOSX_DEPLOYMENT_TARGET
	        EXPORT_MACOSX_DEPLOYMENT_TARGET=''

		    UNIVERSAL_ARCH_FLAGS=""
	        if test "$UNIVERSAL_ARCHS" = "32-bit" ; then
		        UNIVERSAL_ARCH_FLAGS="-arch ppc -arch i386"
		        ARCH_RUN_32BIT=""
		        LIPO_32BIT_FLAGS=""
            elif test "$UNIVERSAL_ARCHS" = "64-bit" ; then
		        UNIVERSAL_ARCH_FLAGS="-arch ppc64 -arch x86_64"
		        LIPO_32BIT_FLAGS=""
		        ARCH_RUN_32BIT="true"
            elif test "$UNIVERSAL_ARCHS" = "all" ; then
		        UNIVERSAL_ARCH_FLAGS="-arch i386 -arch ppc -arch ppc64 -arch x86_64"
		        LIPO_32BIT_FLAGS="-extract ppc7400 -extract i386"
		        ARCH_RUN_32BIT="/usr/bin/arch -i386 -ppc"
            elif test "$UNIVERSAL_ARCHS" = "intel" ; then
		        UNIVERSAL_ARCH_FLAGS="-arch i386 -arch x86_64"
		        LIPO_32BIT_FLAGS="-extract i386"
		        ARCH_RUN_32BIT="/usr/bin/arch -i386"
            elif test "$UNIVERSAL_ARCHS" = "3-way" ; then
		        UNIVERSAL_ARCH_FLAGS="-arch i386 -arch ppc -arch x86_64"
		        LIPO_32BIT_FLAGS="-extract ppc7400 -extract i386"
		        ARCH_RUN_32BIT="/usr/bin/arch -i386 -ppc"
		    else
	            AC_MSG_ERROR([proper usage is --with-universal-arch=ppc|i386|ppc64|x86_64|32-bit|64-bit|all|intel|3-way])
		    fi
		    CXXFLAGS="${UNIVERSAL_ARCH_FLAGS} ${CXXFLAGS}"
		    if test "${UNIVERSALSDK}" != "/"; then
			    CPPFLAGS="-isysroot ${UNIVERSALSDK} ${CPPFLAGS}"
			    LDFLAGS="-isysroot ${UNIVERSALSDK} ${LDFLAGS}"
			    CXXFLAGS="-isysroot ${UNIVERSALSDK} ${CXXFLAGS}"
		    fi
	    fi
	    ;;
	OSF*)
	    BASECFLAGS="$BASECFLAGS -mieee"
	    ;;
    esac
    ;;
*)
    case $ac_sys_system in
    OpenUNIX*|UnixWare*)
	BASECFLAGS="$BASECFLAGS -K pentium,host,inline,loop_unroll,alloca "
	;;
    OSF*)
	BASECFLAGS="$BASECFLAGS -ieee -std"
    	;;
    SCO_SV*)
	BASECFLAGS="$BASECFLAGS -belf -Ki486 -DSCO5"
	;;
    esac
    ;;
esac

#############################################################################
# Checks for debugging and profiling options                                #
#############################################################################
# Check for debugging option
AC_MSG_CHECKING(whether to enable debugging)
AC_ARG_ENABLE([debug],
              AS_HELP_STRING([--enable-debug],
                             [turn on debugging [default=no]]),
              [enable_debug="$enableval"],
              [enable_debug="no"])
if test "x$enable_debug" = "xyes"; then
  CFLAGS="$CFLAGS -g"
  CXXFLAGS="$CXXFLAGS -g"
  FFLAGS="$FFLAGS -g"
  AC_DEFINE([G_DEBUG], [1], [Define if debugging is turned on])
  AC_MSG_RESULT(yes)
else
  AC_MSG_RESULT(no)
fi

# Check for profiling option
AC_MSG_CHECKING(whether to enable profiling)
AC_ARG_ENABLE([profiling],
              AS_HELP_STRING([--enable-profiling],
                             [turn on profiling [default=no]]),
              [enable_profiling="$enableval"],
              [enable_profiling="no"])
if test "x$enable_profiling" = "xyes"; then
  CFLAGS="$CFLAGS -pg"
  CXXFLAGS="$CXXFLAGS -pg"
  FFLAGS="$FFLAGS -pg"
  AC_DEFINE([G_PROFILE], [1], [Define if profiling is turned on])
  AC_MSG_RESULT(yes)
else
  AC_MSG_RESULT(no)
fi


#############################################################################
# Set linker options                                                        #
#############################################################################
case $ac_sys_system/$ac_sys_release in
Darwin/*)
    # Use -undefined dynamic_lookup whenever possible (10.3 and later).
    # This allows an extension to be used in any Python
    #if test ${MACOSX_DEPLOYMENT_TARGET} '>' 10.2; then
        if test "${enable_universalsdk}"; then
            LDFLAGS="${UNIVERSAL_ARCH_FLAGS} -isysroot ${UNIVERSALSDK} ${LDFLAGS}"
        fi
    #fi
    ;;
esac



# Checks for libraries.
ROOT_PATH

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE



#############################################################################
# Check for cfitsio library                                                 #
# ------------------------------------------------------------------------- #
# We check first in the flexIRF directory, then in the standard paths and  #
# finally we perform system dependent checks. These are:                    #
# - Darwin: /opt/local (MacPorts)                                           #
# - Linux:  headers in /usr/include/libcfitsio0 (OpenSUSE)                  #
#           /usr/local (FreeBSD)                                            #
#############################################################################
has_cfitsio="no"
CFITSIO=
LIBS_CFITSIO=
LIBDIR_CFITSIO=
INCDIR_CFITSIO=
AC_SUBST(CFITSIO)
AC_SUBST(LIBS_CFITSIO)
AC_SUBST(LIBDIR_CFITSIO)
AC_SUBST(INCDIR_CFITSIO)

# Check if we want to use cfitsio
AC_ARG_WITH([cfitsio],
            [AS_HELP_STRING([--with-cfitsio],
                            [Use CFITSIO library [default=yes]])],
            [],
            [with_cfitsio=yes])

# If we want to use cfitsio, then search the library now
if [test "x$with_cfitsio" = "xyes"]; then

    # Set host specific cfitsio dependencies
    cfitsio_deps=
    case $host in
        *-*-solaris*) cfitsio_deps="-lm -lsocket" ;;
        *)            cfitsio_deps="-lm" ;;
    esac

    # First check if cfitsio exists in $flexIRF_prefix
    if [test -f $flexIRF_prefix/lib/libcfitsio$SHAREDSUFFIX -a \
             -f $flexIRF_prefix/include/fitsio.h]; then
        add_flexIRF_prefix="yes"
        has_cfitsio="yes"
        LIBDIR_CFITSIO="$flexIRF_prefix/lib"
        INCDIR_CFITSIO="$flexIRF_prefix/include"
    else

        # Next search for cfitsio in standard path
        AC_CHECK_LIB([cfitsio], [ffpss],
                     [AC_MSG_NOTICE([cfitsio found])
                      AC_CHECK_HEADER([fitsio.h],
                                      [has_cfitsio="yes"
                                       AC_MSG_NOTICE([cfitsio headers found])],
                                      [], [])],
                     [], [${cfitsio_deps}])

        # If not found then make additional checks
        if [test "x$has_cfitsio" != "xyes";] then
            case $ac_sys_system in
	            Darwin*) 
                    if [test -f /opt/local/lib/libcfitsio$SHAREDSUFFIX -a \
                             -f /opt/local/include/fitsio.h]; then
                        add_opt_local="yes"
                        has_cfitsio="yes"
                        LIBDIR_CFITSIO="/opt/local/lib"
                        INCDIR_CFITSIO="/opt/local/include"
                    fi
                    ;;
                *)
                    # OpenSUSE kluge
                    if [test -f /usr/lib/libcfitsio$SHAREDSUFFIX -a \
                             -f /usr/include/libcfitsio0/fitsio.h]; then
                        has_cfitsio="yes"
                        LIBDIR_CFITSIO="/usr/lib"
                        INCDIR_CFITSIO="/usr/include/libcfitsio0"
                    fi
                    if [test -f /usr/lib64/libcfitsio$SHAREDSUFFIX -a \
                             -f /usr/include/libcfitsio0/fitsio.h]; then
                        has_cfitsio="yes"
                        LIBDIR_CFITSIO="/usr/lib64"
                        INCDIR_CFITSIO="/usr/include/libcfitsio0"
                    fi
                    # Scientific Linux, CentOS kluge
                    if [test -f /usr/lib/libcfitsio$SHAREDSUFFIX -a \
                             -f /usr/include/cfitsio/fitsio.h]; then
                        has_cfitsio="yes"
                        LIBDIR_CFITSIO="/usr/lib"
                        INCDIR_CFITSIO="/usr/include/cfitsio"
                    fi
                    if [test -f /usr/lib64/libcfitsio$SHAREDSUFFIX -a \
                             -f /usr/include/cfitsio/fitsio.h]; then
                        has_cfitsio="yes"
                        LIBDIR_CFITSIO="/usr/lib64"
                        INCDIR_CFITSIO="/usr/include/cfitsio"
                    fi
                    # FreeBSD kluge
                    if [test -f /usr/local/lib/libcfitsio$SHAREDSUFFIX -a \
                             -f /usr/local/include/fitsio.h]; then
                        has_cfitsio="yes"
                        add_usr_local="yes"
                        LIBDIR_CFITSIO="/usr/local/lib"
                        INCDIR_CFITSIO="/usr/local/include"
                    fi
                    if [test -f /usr/local/lib64/libcfitsio$SHAREDSUFFIX -a \
                             -f /usr/local/include/fitsio.h]; then
                        has_cfitsio="yes"
                        add_usr_local64="yes"
                        LIBDIR_CFITSIO="/usr/local/lib64"
                        INCDIR_CFITSIO="/usr/local/include"
                    fi
                    ;;
            esac
        fi
    
    fi

fi

# If we have ncurses and readline then add readline support to flexIRF
if [test "x$has_cfitsio" = "xyes"]; then
    AC_DEFINE([HAVE_LIBCFITSIO], [1], [Define if cfitsio library is available])
    LIBS="${LIBS} -lcfitsio ${cfitsio_deps}"
    LIBS_CFITSIO="-lcfitsio ${cfitsio_deps}"
    CFITSIO="cfitsio"
fi
AM_CONDITIONAL(WITH_CFITSIO, test "x$has_cfitsio" = "xyes")

#############################################################################
# Add compiler flags                                                        #
#############################################################################
if [test "x$INCDIR_READLINE" != x]; then
    CPPFLAGS="$CPPFLAGS -I$INCDIR_READLINE"
fi
if [test "x$INCDIR_CFITSIO" != x]; then
    CPPFLAGS="$CPPFLAGS -I$INCDIR_CFITSIO"
fi
if [test "x$add_flexIRF_prefix" = "xyes"]; then
    LDFLAGS="$LDFLAGS -L$flexIRF_prefix/lib"
    TEST_ENV_DIR="$TEST_ENV_DIR:$flexIRF_prefix/lib"
fi
if [test "x$add_opt_local" = "xyes"]; then
    LDFLAGS="$LDFLAGS -L/opt/local/lib"
    #TEST_ENV_DIR="$TEST_ENV_DIR:/opt/local/lib"
fi
if [test "x$add_usr_local" = "xyes"]; then
    LDFLAGS="$LDFLAGS -L/usr/local/lib"
    TEST_ENV_DIR="$TEST_ENV_DIR:/usr/local/lib"
fi
if [test "x$add_usr_local64" = "xyes"]; then
    LDFLAGS="$LDFLAGS -L/usr/local/lib64"
    TEST_ENV_DIR="$TEST_ENV_DIR:/usr/local/lib64"
fi



# For SWIG
#AC_DISABLE_STATIC
#AC_PROG_LIBTOOL
#AM_PATH_PYTHON(3.4)
#AC_PATH_PROG([SWIG], [swig])
#SWIG_ENABLE_CXX
#SWIG_PYTHON

# Do we have swig?
#AC_PATH_PROG([SWIG], [swig])
#if test "$SWIG"; then
#  has_swig="yes"
#fi
#AM_PATH_PYTHON(2.2)
#AC_PROG_SWIG
#SWIG_ENABLE_CXX
#SWIG_PYTHON
#AX_PKG_SWIG(1.3.17, [], [ AC_MSG_ERROR([SWIG is required to build..]) ])
#AX_SWIG_ENABLE_CXX
#AX_SWIG_MULTI_MODULE_SUPPORT
#AX_SWIG_PYTHON

#check for swig
#AC_CHECK_PROGS([SWIG],[swig],["noswig"])


#############################################################################
# Checks if Python bindings are to be build                                 #
# ------------------------------------------------------------------------- #
# Python binding requires Python and either the files flexIRF_wrap.cpp and #
# flexIRF.py or swig to create these files.  The following conditionals    #
# are set:                                                                  #
# WITH_PYTHON - Python bindings will be built                               #
# HAVE_SWIG   - swig is available                                           #
#############################################################################
# Initialise flags
enable_python="no"
has_python="no"
has_python_h="no"
has_swig="no"

# Python binding enabled?
AC_ARG_ENABLE([python-binding],
              AS_HELP_STRING([--enable-python-binding],
                             [build python binding [default=yes]]),
              [ac_enable_python_binding="$enableval"],
              [ac_enable_python_binding="yes"])
if test "x$ac_enable_python_binding" = "xno"; then
  AC_MSG_NOTICE([Python binding for girf will not be built])
  AC_MSG_NOTICE([Enable Python binding module building with --enable-python-binding])
else

  # Do we have Python and Python.h?
  AM_PATH_PYTHON([2.2],
                 [has_python="yes"],
                 [AC_MSG_WARN([Python >= 2.2 not found. Python >= 2.2 is required to build python binding. Python can be obtained from http://www.python.org])])

  # It would really be weired if Python has not been found, as barely every system has Python. Maybe
  # it was not found because the autoconf is so old, and in an early version of the AM_PATH_PYTHON
  # there were no [Action-if-found] and [Action-if-not-found] arguments, hence $has_python is possibly
  # badly set. However, the Python executable and version should be set correctly, and we can test
  # for this to see whether Python is really there or not.
  if test "x$has_python" = "xno"; then
    AM_PYTHON_CHECK_VERSION([$PYTHON], [${PYTHON_VERSION}],
                            [has_python="yes"
                             AC_MSG_NOTICE([Wow! You're running a pretty old system. Found Python nevertheless:-)])],
                            [AC_MSG_WARN([Python not found.])])
  fi

  # If we have Python then verify the presence of the header and set some flags
  if test "x$has_python" = "xyes"; then

    # Set opening and closing symbols to make setup.py compatible for versions 2 and 3
    if test ${PYTHON_VERSION} '<' 3; then
      PYTHON_OPEN=" "
      PYTHON_CLOSE=""
    else
      PYTHON_OPEN="("
      PYTHON_CLOSE=")"
    fi
    AC_SUBST(PYTHON_OPEN)  
    AC_SUBST(PYTHON_CLOSE)

    # Do we have distutils?
    AC_MSG_CHECKING([for the distutils Python package])
    ac_distutils_result=`$PYTHON -c "import distutils" 2>&1`
    if test -z "$ac_distutils_result"; then
      AC_MSG_RESULT([yes])
    else
      AC_MSG_RESULT([no])
    fi

    # If we have distutils then get python path from distutils ...
    if test -z "$ac_distutils_result"; then
      if test ${PYTHON_VERSION} '<' 3; then
        python_path=`$PYTHON -c "import distutils.sysconfig; \
                                 print distutils.sysconfig.get_python_inc();"`
      else
        python_path=`$PYTHON -c "import distutils.sysconfig; \
                                 print(distutils.sysconfig.get_python_inc());"`
      fi                                         
      if test -n "${python_path}"; then
        AC_CHECK_HEADERS($python_path/Python.h, [has_python_h="yes"], [has_python_h="no"]) 
        python_path="-I$python_path"
      fi
      PYTHON_CPPFLAGS=$python_path
      CPPFLAGS="${CPPFLAGS} ${python_path}"
      AC_SUBST([PYTHON_CPPFLAGS])

    # ... otherwise check for includes relative to exectuable (obsolete?)
    else                             
                             
      # Determine include path for python
      AC_PATH_PROG(PY_INCLUDE, python)  
      PY_INCLUDE=`echo $PY_INCLUDE | sed -e "s/bin/include/"`
      PY_INCLUDE="$PY_INCLUDE$PYTHON_VERSION"
  
      # Check if we have Python.h
      AC_CHECK_HEADERS($PY_INCLUDE/Python.h, [has_python_h="yes"], [has_python_h="no"])
  
    fi

    # Signal missing Python headers
    if test "x$has_python_h" = "xno"; then
      AC_MSG_WARN([No Python header file "Python.h" found. Please install appropriate Python development package.])
    fi
  
  fi

  # Do we have swig?
  AC_PATH_PROG([SWIG], [swig])
  if test "$SWIG"; then
    has_swig="yes"
  fi


fi
AM_CONDITIONAL(HAVE_SWIG, test "x$has_swig" = "xyes")
AM_CONDITIONAL(WITH_PYTHON, test "x$enable_python" = "xyes")

# Set Python environment
if test "x$enable_python" = "xyes"; then

  # Initialise Python environment
  PYTHON_BUILD_PREFIX=
  PYTHON_EXTRA_LIBS=

  # Solaris Sun Studio compiler kluge (to prevent from using pycc for swig
  # compilation)
  case $host in
    *-*-solaris*)
      if test "${CC}" = cc; then
        PYTHON_BUILD_PREFIX="CC=CC"
        PYTHON_EXTRA_LIBS="Cstd"
      fi
      ;;
  esac

  # Set Python environment
  AC_SUBST(PYTHON_BUILD_PREFIX)
  AC_SUBST(PYTHON_EXTRA_LIBS)

fi








AC_CONFIG_FILES([Makefile
                 flexirf/Makefile
                 src/Makefile
                 pyext/Makefile
                 pyext/setup.py])



# Checks for library functions.
AC_FUNC_MALLOC

AC_OUTPUT


#############################################################################
# Print configuration summary to console                                    #
#############################################################################
echo
echo "  flexIRF configuration summary"
echo "  =============================="
if test "x$has_cfitsio" = "xyes"; then
  echo "  * FITS I/O support             (yes)   $LIBDIR_CFITSIO $INCDIR_CFITSIO"
else
  echo "  - FITS I/O support             (no)    no cfitsio library found"
fi

# Dump next step information
echo
echo "Now type 'make'"
echo
