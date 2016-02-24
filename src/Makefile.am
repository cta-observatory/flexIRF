
# what flags you want to pass to the C compiler & linker
#AM_CPPFLAGS = -I$(top_builddir)/include -I$(top_builddir)/flexirf/include @ROOTCFLAGS@		
AM_CPPFLAGS = -I$(top_builddir)/include -I$(top_builddir)/flexirf/include @ROOTCFLAGS@

#AM_LDFLAGS = -L@ROOTLIBDIR@ @ROOTGLIBS@ @ROOTLIBS@ @LIBS@  -L$(top_builddir)/flexirf -lstdc++ -lgirf
AM_LDFLAGS = -L@ROOTLIBDIR@ @ROOTGLIBS@ @ROOTLIBS@ @LIBS@ -L$(top_builddir)/flexirf -lflexirf


# Program name
#bin_PROGRAMS = IRFsimpleTests
#bin_PROGRAMS = IRFconverter_root2FITS IRFsimpleTests
bin_PROGRAMS = $(top_builddir)/bin/IRFconverter_root2FITS $(top_builddir)/bin/IRFgenerator_rootio $(top_builddir)/bin/IRFmerger_fitsio $(top_builddir)/bin/IRFsimpleReadIrf $(top_builddir)/bin/IRFsimpleWriteIrf $(top_builddir)/bin/IRFsimpleTests

#sources = IRFconverter_root2FITS.cc IRFsimpleTests.cc

# Libtool library
#noinst_LTLIBRARIES = libirmmacros.la
#libirmmacros_la_SOURCES = $(sources)

__top_builddir__bin_IRFconverter_root2FITS_SOURCES = IRFconverter_root2FITS.cc
__top_builddir__bin_IRFconverter_root2FITS_CPPFLAGS = ${AM_CPPFLAGS}
__top_builddir__bin_IRFconverter_root2FITS_LDFLAGS = ${AM_LDFLAGS} -L$(top_builddir)/flexirf -lflexirf

__top_builddir__bin_IRFgenerator_rootio_SOURCES = IRFgenerator_rootio.cc
__top_builddir__bin_IRFgenerator_rootio_CXXFLAGS = ${AM_CPPFLAGS}
__top_builddir__bin_IRFgenerator_rootio_LDFLAGS = ${AM_LDFLAGS}  

__top_builddir__bin_IRFmerger_fitsio_SOURCES = IRFmerger_fitsio.cc
__top_builddir__bin_IRFmerger_fitsio_CXXFLAGS = ${AM_CPPFLAGS}
__top_builddir__bin_IRFmerger_fitsio_LDFLAGS = ${AM_LDFLAGS}  

__top_builddir__bin_IRFsimpleWriteIrf_SOURCES = IRFsimpleWriteIrf.cc
__top_builddir__bin_IRFsimpleWriteIrf_CXXFLAGS = ${AM_CPPFLAGS}
__top_builddir__bin_IRFsimpleWriteIrf_LDFLAGS = ${AM_LDFLAGS} 

__top_builddir__bin_IRFsimpleReadIrf_SOURCES = IRFsimpleReadIrf.cc
__top_builddir__bin_IRFsimpleReadIrf_CXXFLAGS = ${AM_CPPFLAGS}
__top_builddir__bin_IRFsimpleReadIrf_LDFLAGS = ${AM_LDFLAGS} 

# __top_builddir__bin_IRFsimpleTests_SOURCES = IRFsimpleTests.cc
# __top_builddir__bin_IRFsimpleTests_CXXFLAGS = ${AM_CPPFLAGS}
# __top_builddir__bin_IRFsimpleTests_LDFLAGS = ${AM_LDFLAGS}  
      
                      





