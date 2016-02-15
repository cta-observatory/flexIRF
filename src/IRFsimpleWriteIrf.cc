/* ======================================================================== *\                                      
!                                                                                                                            
!   Author(s): Tarek Hassan, John E Ward, Javier Rico     12/2014 <mailto:thassan@ifae.es>
!                                                                                                                               
!   Copyright: CTA Software Development, 2014                                                                           
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
    AxisType axis_type = kBins;
    VarType  var_type  = kEnergy;
    bool               islog    = 1;
    int                size     = axisEdges.size();
    float*             axisData  = axisEdges.data();

//  Build GIRFAxis object.
    GIRFAxis* axis = new GIRFAxisBins(var_type,size,axisData,islog);

//  Build GIRFPdf object, using both pdfData and the generated axis.
	GIRFPdf*   myIrfToStore   = new GIRFPdf(kAeff,kNumber);
	myIrfToStore->SetData(pdfData.data());
	myIrfToStore->AddAxis(axis);

//  Build GIRF object, introduce the generated GIRFPdf object and write it into a fits file.
	GIRF irf;
	irf.AddPdf(myIrfToStore);
	irf.Write("!testEffArea.fits");

	return 0;
}//Ends main
