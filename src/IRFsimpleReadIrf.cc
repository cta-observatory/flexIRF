/* ======================================================================== *\                                      
!                                                                                                                            
!   Author(s): Tarek Hassan, John E Ward, Javier Rico     12/2014 <mailto:thassan@ifae.es>
!                                                                                                                               
!   Copyright: CTA Software Development, 2016
!                                                                                                                                          
\* ======================================================================== */

//GIRF includes
#include "GIRF.h"

using namespace std;


////////////////////////////////////////////////////////////////////////////////
//
// Main code starts here


int main(int argc, char **argv)
{

	//	Set filename storing the IRF information. If not specified as an input, a previously
    //	generated file (using bin/IRFsimpleWriteIrf executable) will be used.
	string filename;
	cout << "argc = " << argc << endl;
	if(argc == 2) // do more checking on the number of arguments below
	{
		filename = argv[1];
	} else {
		cout << "WARNING: GIRF FITS file has not been specified. Using standard file contained in the examples folder." << endl;
		filename = "examples/testEffArea.fits";
	}

//	Build mock CTA configuration file "GIRFConfig", and fill the ranges which we are interested to retrieve from the IRF file.
	GIRFConfig config;
	config.SetDefault();

//	Select energy range between -1.7 and 0.9. Set the axis range as mandatory (if range is not present, stop with an error).
	config.AddAxisRange(kEnergy,-1.7,0.9, 1);
//	Select Phi angle between -1 and 1. Set the axis range as optional (if range is not present in the file, continue with a warning).
	config.AddAxisRange(kPhi, -1,1, 0);
//	Print current GIRFConfig values.
	config.Print();

//	Build GIRF object
	GIRF irf(filename);
//	Extract desired IRF of type "kAeff" (effective area) using the GCTAConfig object we just generated
	GIRFPdf* effArea = dynamic_cast<GIRFPdf*>(irf.ReadPdf(kAeff, config));
	if (effArea && !effArea->IsEmpty()) {
		effArea->Print();
		effArea->Draw("", "logY");
		return 0;
	}
	cout << "ERROR: IRF element was not properly extracted from the FITS file." << endl;
	return 1;

	return 0;
}//Ends main
