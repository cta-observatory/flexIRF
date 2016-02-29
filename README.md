
Project description
==============

FlexIRF, developed as a test bench for the future Cherenkov Telescope Array Data Models, is a tool to read/write an Instrument Response Function into a single FITS file. Due to the large number of external parameters affecting Imaging Atmospheric Cherenkov Telescopes performance, the IRF is required to be defined in a wide phase space to reduce systematic uncertaintie. This code attemps to provide more flexibility to current IRF generators. 

The code is written in C++, making use of the CFITSIO library. Few dependencies, to be removed in the future, make use of ROOT libraries (current standard in gamma-ray astronomy).


Dependencies
--------------

Currently, the software uses the following dependencies:

git dpkg-dev make g++ gcc binutils libx11-dev libxpm-dev libxft-dev libxext-dev cmake autoconf 

It also uses CFITSIO library ("libcfitsio3" "libcfitsio3-dev" packages on ubuntu machines) and swig, although with no python working wrapper yet.

ROOT is currently also used for some executables, but will be removed in the close future.


Installation
--------------

It is currently only compiling properly in linux machines. 

Build git version using:

```shell
./autogen.sh
./configure
make
```

Although we DON'T recommend it, if you want to link generated libraries into your "/usr/local/share" do (sudo) make install.

Documentation
--------------

Although this project still lacks propper documentation, several very simple macros were created to show simple use cases of the different classes:

IRFsimpleWriteIrf.cc:

This simple macro generates a fits file "examples/testEffArea.fits" containing a mock effective area into a FITS file. To run it, simply execute:

```shell
bin/IRFsimpleWriteIrf
```

It shows how "AxisType", "VarType" and other variables are used in the definition of a "GIRFAxis" object, and then how the IRF element is created using a "GIRFPdf" and introduced into the generic "GIRF".


IRFsimpleReadIrf.cc:

This script reads a the previously generated "examples/testEffArea.fits", loading it into a GIRF object, extracting the effective area as a "GIRFPdf" and printing (& drawing, if you have ROOT properly installed, generating the "plot.png" file). To run it, execute:

```shell
bin/IRFsimpleReadIrf
```
Note the use of the "GIRFConfig" class, used to extract the IRF with a specific configuration and valid range. 


IRFsimpleWriteParamIrf.cc: 

Similarly as in the "IRFsimpleWriteIrf", this script writes a parameterized effective area (1D gaussian) into a FITS file, stored in "examples/testParamEffArea.fits":

```shell
bin/IRFsimpleWriteParamIrf
```


**More examples will soon be added.**


The following are older and more complicated (working, but nearly deprecated) examples. 

IRFconverter_root2FITS:

This tool takes a dummy configuration file and (for now), the performance file "Performance_2Q_Aar_50h_20150217.root". Run the following
command from the build directory to convert the root histograms in the performance file into the fits format being developed at IFAE.

Example -

```shell
bin/IRFconverter_root2FITS dummy/dummyconfig.txt prodfiles/Performance_2Q_Aar_50h_20150505.root
```

Output will be

alltest.fits

IRFgenerator_rootio:

For the moment, this program reads in a dummy configuration file (located in dummy/dummyconfig.txt) 
and a CTA MC analysis file (located in prodfiles/DESY.d20140105.Erec1.V3.ID0NIM2.prod2-Tenerife-NS.N.2NN-MST.180000s.root) 
and generates three fits files ( Ebias_offaxis.fits (2D), ERes_offaxis.fits (2D) and AngRes_offaxis.fits (3D) ) in the build directory

Example -

In the parent directory run

```shell
./bin/IRFgenerator_rootio dummy/dummyconfig.txt prodfiles/DESY.d20140105.Erec1.V3.ID0NIM2.prod2-Tenerife-NS.N.2NN-MST.180000s.root
```

Output will be 

ERes_offaxis.fits, Ebias_offaxis.fits and AngRes_offaxis.fits

