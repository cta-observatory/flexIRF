
%module convert

/* __ Headers needed for compilation _____________________________________ */
%{
#include "GIRF.h"
#include "rootincludes.h"

%}

/* __ Include standard typemaps for vectors and strings __________________ */
%include stl.i

/* __ girf _____________________________________________________________ */

%include "std_string.i"
%include "std_vector.i"
%include "argcargv.i"

%include "irfConvert.i"
