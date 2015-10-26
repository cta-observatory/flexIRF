/* ======================================================================== *\                                      
!                                                                                                                            
!   Author(s): John E Ward, Javier Rico     12/2014 <mailto:jward@ifae.es>                                                           
!                                                                                                                               
!   Copyright: CTA Software Development, 2014                                                                           
!                                                                                                                                          
\* ======================================================================== */

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iterator>
#include <iomanip>
#include <math.h>

//GIRF includes
#include "GIRF.h"

//FITS includes
#include "fitsio.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//
// Declare functions here
//



////////////////////////////////////////////////////////////////////////////////
//
// Main code starts here


int main(int argc, char **argv)
{
	GIRFAxisBins axis1;
	GIRFAxisBins axis2;

	vector<float> vect1, vect2;
	for (int i=0;i<10;i++){
		vect1.push_back(i*3.1416);
		vect2.push_back(i*3.14);
	}
	axis1.SetAxisBins(vect1);
	axis2.SetAxisBins(vect2);
	axis1.SetVarType(GIRFAxis::kEnergy);
//	cout << "axis1.GetRangeMin() = " << axis1.GetRangeMin() << endl;
//	cout << "axis2.GetRangeMin() = " << axis2.GetRangeMin() << endl;
//
//	cout << "axis1.GetRangeMax() = " << axis1.GetRangeMax() << endl;
//	cout << "axis2.GetRangeMax() = " << axis2.GetRangeMax() << endl;
//
//	cout << "(axis1==axis2) = " << (axis1==axis2) << endl;
//
//	cout << "axis1.GetExtName() = " << axis1.GetExtName().data() << endl;
	int lastID = GIRFUtils::GetLastAxisID("alltest.fits");
//	cout << "lastID = " << lastID << endl;

	GIRFAxisBins* axis3;
	GIRFAxisBins* axis4;
	axis3 = new GIRFAxisBins();
	axis4 = new GIRFAxisBins();
	axis3->SetAxisBins(vect1);
	axis4->SetAxisBins(vect2);
//	cout << "axis3==axis4 = " << ((*axis3)==(*axis4)) << endl;


//	GIRFConfig config;
////	config.SetDefault();
//	config.AddAxisRange(GIRFAxis::kEnergy,-1.7,-0.9, 1);
//	config.AddAxisRange(GIRFAxis::kPhi, -1,1, 0);
////	config.Print();
//	string filename = "/home/thassan/Workspace/IRM_devel/build/alltest.fits";
//	GIRF irf(filename);
//	GIRFPdf* effArea = dynamic_cast<GIRFPdf*>(irf.ReadPdf(GIRFPdf::kBkgRate, config));
//	if (!effArea || effArea->IsEmpty()) return 2;
////	effArea.Print();
//	effArea->Draw();



//	GIRFConfig config;
////	config.SetDefault();
//	config.AddAxisRange(GIRFAxis::kEnergy_true, -1,1, 1);
//	config.AddAxisRange(GIRFAxis::kEnergy_rec, -1,1, 1);
//	config.AddAxisRange(GIRFAxis::kPhi, -1,1, 0);
////	config.Print();
//	string filename = "/home/thassan/Workspace/IRM_devel/build/alltest.fits";
//	GIRF irf(filename);
//	GIRFPdf* effArea = dynamic_cast<GIRFPdf*>(irf.ReadPdf(GIRFPdf::kEDispersion, config));
//	if (!effArea || effArea->IsEmpty()) return 2;
////	effArea->Print();
//	effArea->Draw();





//	GIRF irf2;
//	irf2.AddPdf(effArea);
//	irf2.Write("!/home/thassan/Workspace/IRM_devel/build/outTest.fits");

//	vector<int> test;
//	test.push_back(1);
//	test.push_back(2);
//	test.push_back(3);
//
//	int *test2 = test.data();
//
//	test.erase(test.begin(),test.begin()+0);
//	test.erase(test.begin()+3,test.end());
//
//	cout << "test.size() = " << test.size() << endl;
//	cout << "test[0] = " << test[0] << endl;
//	cout << "test[1] = " << test[1] << endl;
//	cout << "test[3] = " << test[3] << endl;
//
//	cout << "test2[0] = " << test2[0] << endl;
//	cout << "test2[1] = " << test2[1] << endl;
//	cout << "test2[3] = " << test2[3] << endl;


	return 0;
}//Ends main
