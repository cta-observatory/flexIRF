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

	int const numBins=9;

//	Create arbitrary energy bin "low edges" values (numBins+1):
	vector<float> axisEdges;
	for (int i=0;i<(numBins+1);i++) axisEdges.push_back(-2+(i*0.5));

//	Create arbitrary effective area values (numBins):
	vector<float> pdfData;
	pdfData.push_back(600);
	pdfData.push_back(2200);
	pdfData.push_back(10000);
	pdfData.push_back(40000);
	pdfData.push_back(90000);
	pdfData.push_back(200000);
	pdfData.push_back(300000);
	pdfData.push_back(300000);
	pdfData.push_back(300000);

//	Define required information to build a GIRFAxis object.
	flexIRF::AxisType axis_type = flexIRF::kBins;							// Not necessary, as it is already fixed by using the "GIRFAxisBins"
	flexIRF::VarType  var_type  = flexIRF::kEnergy;
	flexIRF::ScaleType  scale_type  = flexIRF::kLog10;
    int                size     = axisEdges.size();
    float*             axisData  = axisEdges.data();

//  Build GIRFAxis object.
    flexIRF::GIRFAxis* axis = new flexIRF::GIRFAxisBins(var_type,size,axisData,scale_type);

//  Build GIRFPdf object, using both pdfData and the generated axis.
    flexIRF::GIRFPdf*   myIrfToStore   = new flexIRF::GIRFPdf(flexIRF::kAeff,flexIRF::kNumber);
	myIrfToStore->SetData(pdfData.data());
	myIrfToStore->AddAxis(axis);

//  Build GIRF object, introduce the generated GIRFPdf object and write it into a fits file.
	flexIRF::GIRF irf;
	irf.AddPdf(myIrfToStore);
	cout << "Writing IRF element using IMAGE serialization into examples/testEffAreaIMAGE.fits" << endl;
	irf.Write("!examples/testEffAreaIMAGE.fits", "IMAGE");
	cout << "Writing IRF element using BINTABLE serialization into examples/testEffAreaBINTABLE.fits" << endl;
	irf.Write("!examples/testEffAreaBINTABLE.fits", "BINTABLE");

	return 0;
}//Ends main
