/* ======================================================================== *\                                      
!                                                                                                                            
!   Author(s): John E Ward, Javier Rico     12/2014 <mailto:jward@ifae.es>                                                           
!                                                                                                                               
!   Copyright: CTA Software Development, 2014                                                                           
!                                                                                                                                          
\* ======================================================================== */

//#include <iostream>
//#include <sstream>
//#include <fstream>
//#include <string>
//#include <iterator>
//#include <iomanip>
//#include <math.h>

//GIRF includes
#include "GIRF.h"

//FITS includes
//#include "fitsio.h"

//#include "rootincludes.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//
// Declare functions here
//



////////////////////////////////////////////////////////////////////////////////
//
// Main code starts here


int main()
{

//	Build mock CTA configuration file "GIRFConfig", and fill the ranges which we are interested to retrieve.
	GIRFConfig config;
	config.SetDefault();

//	Select energy range between -1.7 and -0.9. Set the axis range as mandatory (if range is not present, stop with an error).
	config.AddAxisRange(kEnergy,-2,2, 1);
//	Select Phi angle between -1 and 1. Set the axis range as optional (if range is not present in the file, continue with a warning).
	config.AddAxisRange(kPhi, -1,1, 0);
//	Print current CTAconfig values.
	config.Print();

//	Set filename storing the IRF information, previously generated with bin/IRFsimpleWriteIrf executable.
	string filename = "examples/testEffArea.fits";

//	Build GIRF object
	GIRF irf(filename);
//	Extract desired IRF of type "kAeff" (effective area) using the GCTAConfig object we just generated
	GIRFPdf* effArea = dynamic_cast<GIRFPdf*>(irf.ReadPdf(kAeff, config));
	effArea->Print();
	effArea->Draw("", "logY");

	return 0;
}//Ends main
