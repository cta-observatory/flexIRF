
# what flags you want to pass to the C compiler & linker
#AM_CFLAGS = --pedantic -Wall -std=c99 -O2 
#AM_LDFLAGS = 

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
#bin_PROGRAMS = GIRF
#GIRF_SOURCES = src/GIRF.cc src/GIRFAxis.cc src/GIRFAxisBins.cc src/GIRFAxisParam.cc  src/GIRFConfig.cc  src/GIRFPdf.cc  src/GIRFRange.cc  src/GIRFUtils.cc include/GIRFAxisBins.h  include/GIRFAxis.h  include/GIRFAxisParam.h  include/GIRFConfig.h  include/GIRF.h  include/GIRFPdf.h  include/GIRFRange.h  include/GIRFUtils.h  include/rootincludes.h
#GIRF_CXXFLAGS = -I$(top_builddir)/include -I$(top_builddir)/flexirf/include -I$(top_builddir)/flexirf/src @ROOTCFLAGS@
#GIRF_LDADD = -L@ROOTLIBDIR@ @ROOTGLIBS@ @ROOTLIBS@ @LIBS@



## Process this file with automake to produce Makefile.in

# Set include directories
AM_CPPFLAGS = -I$(top_builddir)/flexirf/include \
				@ROOTCFLAGS@
AM_LDFLAGS = -L@ROOTLIBDIR@ @ROOTGLIBS@ @ROOTLIBS@ @LIBS@ 

# Define sources for this directory
sources = 	src/GIRFAxisBins.cc \
			src/GIRFAxis.cc \
			src/GIRFAxisParam.cc \
			src/GIRF.cc \
			src/GIRFConfig.cc \
			src/GIRFPdf.cc \
			src/GIRFRange.cc \
			src/GIRFUtils.cc

# Build libtool library
noinst_LTLIBRARIES = libflexirf.la
#lib_LTLIBRARIES = libflexirf.la
libflexirf_la_SOURCES = $(sources)
#libapp_la_LDFLAGS = -version-info 0:0:0