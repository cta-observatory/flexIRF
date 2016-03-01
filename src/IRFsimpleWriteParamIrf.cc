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
// Declare functions here
//



////////////////////////////////////////////////////////////////////////////////
//
// Main code starts here


int main()
{

//	Axis parameterization will be done in log scale
    bool               islog    = 1;

//  Instanciate AxisParameterization class, to define a parameterization.
	AxisParameterization param;
//	Define parameterization formula, valid range and number of parameters. All will be stored into the FITS file with dedicated keywords.
//	Note ROOT's "TF1" sintax is currently used.
	param.formula = "gaus";
	param.validRangeLow= -2;
	param.validRangeHigh= 2;
	param.numParameters= 3;

//	Create the axis (in this case, of GIRFAxisParam type) and specify its type, and scale (log).
	GIRFAxisParam* axis1 = new GIRFAxisParam(kEnergy, islog);
	axis1->SetAxisParam(param);
	axis1->Print();

//	Create arbitrary parameters of an effective area gaussian fit:
	vector<float> pdfData;
	pdfData.push_back(2.68E06);
	pdfData.push_back(1.83);
	pdfData.push_back(1.03);

//  Build GIRFPdf object, using both pdfData and the generated axis.
	GIRFPdf*   myIrfToStore   = new GIRFPdf(kAeff,kNumber);
	myIrfToStore->SetData(pdfData.data());
	myIrfToStore->AddAxis(axis1);

//  Build GIRF object, introduce the generated GIRFPdf object and write it into a fits file.
	GIRF irf;
	irf.AddPdf(myIrfToStore);
	irf.Write("!examples/testParamEffArea.fits");

	return 0;
}//Ends main
