
# what flags you want to pass to the C compiler & linker
#AM_CPPFLAGS = -I$(top_builddir)/include -I$(top_builddir)/flexirf/include @ROOTCFLAGS@		#Compiles!!!
AM_CPPFLAGS = -I$(top_builddir)/include -I$(top_builddir)/flexirf/include @ROOTCFLAGS@

#AM_LDFLAGS = -L@ROOTLIBDIR@ @ROOTGLIBS@ @ROOTLIBS@ @LIBS@  -L$(top_builddir)/flexirf -lstdc++ -lgirf
AM_LDFLAGS = -L@ROOTLIBDIR@ @ROOTGLIBS@ @ROOTLIBS@ @LIBS@ -L$(top_builddir)/flexirf -lflexirf


# Program name
#bin_PROGRAMS = IRFsimpleTests
bin_PROGRAMS = IRFconverter_root2FITS IRFsimpleTests
#bin_PROGRAMS = IRFconverter_root2FITS IRFgenerator_rootio IRFmerger_fitsio IRFsimpleTests

#sources = IRFconverter_root2FITS.cc IRFsimpleTests.cc

# Libtool library
#noinst_LTLIBRARIES = libirmmacros.la
#libirmmacros_la_SOURCES = $(sources)

IRFconverter_root2FITS_SOURCES = IRFconverter_root2FITS.cc
IRFconverter_root2FITS_CPPFLAGS = ${AM_CPPFLAGS}
IRFconverter_root2FITS_LDFLAGS = ${AM_LDFLAGS} -L$(top_builddir)/flexirf -lflexirf

#IRFgenerator_rootio_SOURCES = IRFgenerator_rootio.cc
#IRFgenerator_rootio_CXXFLAGS = @ROOTCFLAGS@
#IRFgenerator_rootio_LDADD = -L@ROOTLIDIR@ @ROOTGLIBS@ @ROOTLIBS@

#IRFmerger_fitsio_SOURCES = IRFmerger_fitsio.cc
#IRFmerger_fitsio_CXXFLAGS = @ROOTCFLAGS@
#IRFmerger_fitsio_LDADD = -L@ROOTLIDIR@ @ROOTGLIBS@ @ROOTLIBS@

IRFsimpleTests_SOURCES = IRFsimpleTests.cc
IRFsimpleTests_CXXFLAGS = ${AM_CPPFLAGS}
IRFsimpleTests_LDFLAGS = ${AM_LDFLAGS}  

# Build libtool library for ctools. This is needed to provide
# a shared library that can be loaded for python bindings. We do
# not have to build this library if no python bindings are used.
      
                      





