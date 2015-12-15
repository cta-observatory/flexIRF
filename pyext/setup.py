#############################################################################
# Disutils setup file for gammalib Python extension module                  #
# ------------------------------------------------------------------------- #
# Copyright (C) 2011-2014 Juergen Knoedlseder                               #
# ------------------------------------------------------------------------- #
#                                                                           #
#  This program is free software: you can redistribute it and/or modify     #
#  it under the terms of the GNU General Public License as published by     #
#  the Free Software Foundation, either version 3 of the License, or        #
#  (at your option) any later version.                                      #
#                                                                           #
#  This program is distributed in the hope that it will be useful,          #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of           #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
#  GNU General Public License for more details.                             #
#                                                                           #
#  You should have received a copy of the GNU General Public License        #
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.    #
#                                                                           #
#############################################################################

from distutils.core import setup, Extension
from distutils import sysconfig
import glob
import os
import sys
import re

# Define package name
pkg_name = 'girf'

# Define core modules
core_modules = ['convert', 'simpleTest', 'irf']

# Detect execution mode
is_build = True
if (len(sys.argv) >= 2):
	if (sys.argv[1] == 'install'):
		is_build = False
if is_build:
	sys.stdout.write("Python extension module building:\n")
else:
	sys.stdout.write("Python extension module installation:\n")

# Darwin MacPorts kluge: Remove -L/opt/local/lib path from LDSHARED as this
# overrides the GammaLib settings. Otherwise, MacPorts libraries would be
# used instead of the libraries selected by GammaLib
if sys.platform.lower().startswith("darwin"):
	
	# Get configuration variables
	cvars = sysconfig.get_config_vars()
	
	# Extract LDSHARED
	ldshared_old = cvars.get('LDSHARED')
	ldshared_new = ''
	ldshared_rm  = ''
	for option in ldshared_old.split(' '):
		if option.lstrip().find('-L/opt/local/lib') != 0:
			ldshared_new += option + ' '
		else:
			ldshared_rm += option + ' '
	if (len(ldshared_rm) > 0):
		sys.stdout.write("Removed following option(s) from LDSHARED: "+ldshared_rm+"\n")	
	# Set LDSHARED
	cvars['LDSHARED'] = ldshared_new

# Set default include directories, install files, library paths and libraries
include_dirs         = ['../include']
install_files        = glob.glob('../pyext/*.i')
library_dirs         = ['../src/.libs', '../flexirf/.libs', '/home/thassan/Paquetes/root/v5-34-21/lib']
runtime_library_dirs = ['../src/.libs', '../flexirf/.libs', '/home/thassan/Paquetes/root/v5-34-21/lib']
libraries            = ['irm']

# Add install directory to runtime library path
if os.path.exists('/usr/local/lib'):
	runtime_library_dirs.append('/usr/local/lib')

# Add cfitsio library only if available
if '' != '#':
	sys.stdout.write("Link cfitsio into Python bindings.\n")
	libraries.append('cfitsio')

# Add application include files. The method we use here is not very
# intelligent as it simple will add all names that start with 'ct'
# in the src directory. Once we added the ctools script we had to
# add an explicit filter to avoid appending those.
dirs = glob.glob('../flexirf/include')
for dir in dirs:
	if len(dir) > 0 :
		path = os.path.abspath(dir)
		include_dirs.append(path)


# Add extra libraries
extra_libs = ('-lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread  -lcfitsio -lm -lflexirf ').split(' ')
for extra_lib in extra_libs:
	if extra_lib.lstrip().find('-l') == 0:
		lib = extra_lib.lstrip().lstrip('-l')
		if len(lib) > 0:
			libraries.append(lib)


# Get extra library paths (for cfitsio)
extra_lib_paths = (' -L/home/thassan/Paquetes/root/v5-34-21/lib').split(' ')
for extra_lib_path in extra_lib_paths:
	if extra_lib_path.lstrip().find('-L') == 0:
		dir = extra_lib_path.lstrip().lstrip('-L')
		if len(dir) > 0:
			library_dirs.append(dir)
			runtime_library_dirs.append(dir)

# Get extra include directories
extra_inc_paths = (' -I/home/thassan/Paquetes/anaconda3/envs/cta/include/python3.4m -I/home/thassan/Paquetes/root/v5-34-21/include').split(' ')
for extra_inc_path in extra_inc_paths:
	if extra_inc_path.lstrip().find('-I') == 0:
		dir = extra_inc_path.lstrip().lstrip('-I')
		if len(dir) > 0:
			include_dirs.append(dir)

# Set extension definition for core modules
all_modules = []
ext_modules = []
py_modules  = []

# Optionally enable OpenMP
#if '@OPENMP_TRUE@' != '#':
#	extra_compile_args = ['@OPENMP_CXXFLAGS@']
#	extra_link_args    = ['@OPENMP_CXXFLAGS@']
#else:
extra_compile_args = []
extra_link_args    = []

# Loop over all core modules
for module in core_modules:
	
	
	# TH: Print for testing
	print ("--- Module: ", module, " ---")
	print ("libraries:", libraries) 
	print ("library_dirs:", library_dirs) 
	print ("runtime_library_dirs:", runtime_library_dirs) 
	print ("extra_compile_args:", extra_compile_args)
	print ("extra_link_args:", extra_link_args)
	
	
	# Define module
	ext_module = Extension('_'+module, [pkg_name+'/'+module+'_wrap.cpp'], \
	                       include_dirs=include_dirs, \
                           libraries=libraries,
                           library_dirs=library_dirs,
                           runtime_library_dirs=runtime_library_dirs,
                           extra_compile_args=extra_compile_args,
                           extra_link_args=extra_link_args)
	all_modules.append(module)
	ext_modules.append(ext_module)
	py_modules.append(pkg_name+'.'+module)


# Build list of class names
classes = []
casts   = []
for module in all_modules:

	# Set file names
	filename = pkg_name+"/"+module+".py"
	infile  = open(filename, 'r')

	# Loop over all lines of input file
	for line in infile:

		# Search class names
		if line.find("class ") != -1:
			pos  = line.find("(")
			name = line[6:pos]
			classes.append(name)

		# Search casts
		elif line.find("def cast_") != -1:
			pos     = line.find("(")
			name    = line[4:pos]
			casts.append(name)

	# Close file
	infile.close()

# Sort
classes.sort()
casts.sort()

# Build names string
names   = ""
n_names = 0
for name in classes:
	if n_names > 0:
		names += ", "
	names   += "'"+name+"'"
	n_names += 1
for name in casts:
	if n_names > 0:
		names += ", "
	names   += "'"+name+"'"
	n_names += 1

# Create gammalib/__init__.py file
file = open(pkg_name+'/__init__.py', 'w')
file.write('# This file is created automatically by setup.py\n')
file.write('# Please do not modify, unless you exactly know what you do.\n')
file.write('__all__ = [')
file.write(names)
file.write(']\n')
for module in all_modules:
	file.write('from girf.'+module+' import *\n')

# Remove useless and annoying `*_swigregister` entries from the namespace
# see https://cta-redmine.irap.omp.eu/issues/832
file.write("""
bad_entries = [entry for entry in list(locals())
               if entry.endswith('_swigregister')]
for entry in bad_entries:
    del locals()[entry]
#del girf
""")
file.close()

# Set data files to install.
data_files=[('/usr/local/share/girf/swig', install_files)]

# Setup
setup(name='irm',
      version='0.6',
      description='girf Python bindings',
      author='Tarek Hassan',
      author_email='thassan@ifae.es',
      maintainer='Tarek Hassan',
      maintainer_email='thassan@ifae.es',
      url='https://bitbucket.org/TarekH/irm',
      packages=[pkg_name],
      ext_package=pkg_name,
      ext_modules=ext_modules,
      py_modules=py_modules,
      data_files=data_files,
      license="GNU GPL v3",
      long_description="Versatile toolbox for the analysis of astronomical gamma-ray data",
      platforms=["Linux", "Solaris", "Mac OS-X", "Unix", "BSD"],
	  )

# Post-processing. A symbolic link is created to the directory containing the
# shared wrapper libraries so that we can add this directory to the PYTHONPATH
# for testing.
if is_build:
	try:
		os.chdir('build')
		os.remove('girf')
		sys.stdout.write("Removed symbolic link build/girf.\n")
	except:
		pass
	try:
		path = glob.glob('lib.*/girf')
		os.symlink(path[0], 'girf')
		sys.stdout.write("Created symbolic link build/girf.\n")
	except:
		sys.stdout.write("WARNING: Failed to create symbolic link build/girf.\n")
