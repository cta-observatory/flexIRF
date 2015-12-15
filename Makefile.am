## Process this file with automake to produce Makefile.in

# Include M4 macros
ACLOCAL_AMFLAGS = -I m4


AUTOMAKE_OPTIONS = foreign

# Subdirectories
SUBDIRS = flexirf src pyext


lib_LTLIBRARIES = libirm.la
libirm_la_SOURCES =
# Dummy C++ source to cause C++ linking.
nodist_EXTRA_libirm_la_SOURCES = dummy.cxx
libirm_la_LIBADD = \
			flexirf/libflexirf.la 
#			src/libirmmacros.la 

# Automatically reconfigure libtool
LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck