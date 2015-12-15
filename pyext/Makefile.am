

# CPPFLAGS = -I$(top_builddir)/include -I$(top_builddir)/flexirf/include @ROOTCFLAGS@
# LDFLAGS = -L@ROOTLIBDIR@ -L$(top_builddir)/flexirf 


# Sources that are required for the build. All targets in the list will be built
# before anything. These are honored by 'make all', 'make check' and 'make install'.
# We use this here to make sure that all these targets are build by swig before we
# start compiling the Python modules.
BUILT_SOURCES = girf/irf_wrap.cpp  girf/irf.py \
				girf/convert_wrap.cpp  girf/convert.py \
                girf/simpleTest_wrap.cpp girf/simpleTest.py 


# Dependencies. Obtain the list of dependencies by executing ls.
DEPFILES = $(shell ls girf/*.Pi 2> /dev/null)
include $(DEPFILES)


SWIG_INTERFACE = girf/convert.i

CLEAN_FILES = $(BUILT_SOURCES)
CLEANFILES = $(CLEAN_FILES) $(DEPFILES)
SWIGCOMPILE = $(SWIG) -c++ -python -Wall -I$(top_builddir)/flexirf/include \
				@ROOTCFLAGS@ 
				
# Things we want to do when 'make' is invoked.
all-local: build

# Build the girf extension module
build: $(BUILT_SOURCES)
	@PYTHON_BUILD_PREFIX@ $(PYTHON) setup.py build_ext
		
# Do nothing for 'make check'
#check-local:

# Install the girf extension module
install-exec-local:
	$(PYTHON) setup.py install --prefix=$(prefix)

# Remove build directory and test libraries
clean-local:
	rm -rf build
	rm -rf girf/__init__.py? girf/_*.so girf/*.pyc



# Rule to build SWIG wrappers
swig: $(BUILT_SOURCES)

# General rule for core modules
%_wrap.cpp %.py: %.i
	if $(SWIGCOMPILE) -MMD -MF "$*.Tpi" -o $@ -outdir girf $<; \
	then mv -f "$*.Tpi" "$*.Pi"; else rm -f "$*.Tpi"; exit 1; fi
								
#install-exec-local:
#	$(PYTHON) setup.py install --prefix=$(prefix)

#build: girf/convert_wrap.cpp girf/convert.py
#	$(PYTHON) setup.py build

#girf/convert.py: girf/convert_wrap.cpp
#girf/convert_wrap.cpp: $(SWIG_INTERFACE)
#	if $(SWIGCOMPILE) -MMD -MF "convert.Tpi" -o girf/convert_wrap.cpp $<; \
#	then mv -f "convert.Tpi" "convert.Pi"; else rm -f "convert.Tpi"; exit 1; fi