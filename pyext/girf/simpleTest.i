
%module simpleTest

/* __ Headers needed for compilation _____________________________________ */
%{
//FITS includes
//#include "GIRF.h"
//#include "rootincludes.h"
//#include <iostream>
//using namespace std;

extern int main();
%}

/* __ Include standard typemaps for vectors and strings __________________ */
%include stl.i

/* __ Inform about base classes __________________________________________ */
// %import(module="girf.simpleTest") "IRFsimpleTests.i";

/* __ girf _____________________________________________________________ */
//%include "std_string.i"
//%include "std_vector.i"
//%include "argcargv.i"

%include "IRFsimpleTests.i"

extern int main();