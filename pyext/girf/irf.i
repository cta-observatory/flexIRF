
%module irf

/* __ Headers needed for compilation _____________________________________ */
%{
#define SWIG
#include <stddef.h>

%}

/* __ Include standard typemaps for vectors and strings __________________ */
%include stl.i

/* __ Inform about base classes __________________________________________ */
%import(module="girf.convert") "convert.i";
%import(module="girf.simpleTest")  "simpleTest.i";
/* __ girf _____________________________________________________________ */

%include "GIRF.i"
