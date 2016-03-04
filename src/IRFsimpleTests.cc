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


int main()
{
	flexIRF::GIRFAxisBins axis1;
	flexIRF::GIRFAxisBins axis2;

	vector<float> vect1, vect2;
	for (int i=0;i<10;i++){
		vect1.push_back(i*3.1416);
		vect2.push_back(i*3.14);
	}
	axis1.SetAxisBins(vect1);
	axis2.SetAxisBins(vect2);
	axis1.SetVarType(flexIRF::kEnergy);
//	cout << "axis1.GetRangeMin() = " << axis1.GetRangeMin() << endl;
//	cout << "axis2.GetRangeMin() = " << axis2.GetRangeMin() << endl;
//
//	cout << "axis1.GetRangeMax() = " << axis1.GetRangeMax() << endl;
//	cout << "axis2.GetRangeMax() = " << axis2.GetRangeMax() << endl;
//
//	cout << "(axis1==axis2) = " << (axis1==axis2) << endl;
//
//	cout << "axis1.GetExtName() = " << axis1.GetExtName().data() << endl;
	int lastID = flexIRF::GIRFUtils::GetLastAxisID("alltest.fits");
//	cout << "lastID = " << lastID << endl;

	flexIRF::GIRFAxisBins* axis3;
	flexIRF::GIRFAxisBins* axis4;
	axis3 = new flexIRF::GIRFAxisBins();
	axis4 = new flexIRF::GIRFAxisBins();
	axis3->SetAxisBins(vect1);
	axis4->SetAxisBins(vect2);
	cout << "axis3==axis4 = " << ((*axis3)==(*axis4)) << endl;


	cout << "Funciona" << endl;

	return 0;


//	GIRFAxis* axis1;
//	GIRFAxisParam* axis1 = new GIRFAxisParam(kEnergy, (bool)1);
//	AxisParameterization axisPar;
////	axisPar.constants.push_back(3.);
////	axisPar.constants.push_back(0.55);
////	axisPar.constants.push_back(9999.3);
////	axisPar.numConstants = 3;
//	axisPar.formula = "[1]*(x/[0])*exp([2])";
//	axisPar.validRangeLow= -2;
//	axisPar.validRangeHigh= 2;
//	axisPar.numParameters= 3;
//	axis1->SetAxisParam(axisPar);
//	axis1->Print();
//
//	vector<float> values;
//	values.push_back(1.);
//	values.push_back(2.);
//	values.push_back(3.);
//	values.push_back(4.);
//	values.push_back(5.);
//	values.push_back(6.);
//	values.push_back(7.);
//	values.push_back(8.);
//	values.push_back(9.);
//	values.push_back(10.);
//	values.push_back(11.);
//	values.push_back(12.);
//	values.push_back(13.);
//	values.push_back(14.);
//	values.push_back(15.);
//	values.push_back(16.);
//
//
////    GIRFAxis::AxisType axis_type = GIRFAxis::kBins;
////    GIRFAxis::VarType  var_type  = GIRFAxis::kEnergy;
////    bool               islog    = 1;
////    int                size     = 3;
////    float*             theaxis  = values.data();
////
////	axis1 = new GIRFAxisBins(var_type,size,theaxis,islog);
////	axis1->Print();
//
//
//	GIRFPdf*   mypdf   = new GIRFPdf(kAeff,kNumber);
//	vector<float> pdfdata;
////	pdfdata.reserve(15);
////	pdfdata.push_back(0);
//	pdfdata.push_back(0.354);
//	pdfdata.push_back(0.4559);
//	pdfdata.push_back(0.98);
//	pdfdata.push_back(1.54);
//	pdfdata.push_back(2.222);
//	pdfdata.push_back(2.4021);
//	pdfdata.push_back(2.4021);
//	pdfdata.push_back(2.4021);
//	pdfdata.push_back(2.4021);
//	pdfdata.push_back(2.4021);
//	pdfdata.push_back(2.4021);
//	pdfdata.push_back(2.4021);
//	pdfdata.push_back(2.4021);
//	pdfdata.push_back(2.4021);
//	pdfdata.push_back(2.4021);
//	mypdf->SetData(pdfdata.data());
//	mypdf->AddAxis(axis1);
//
//
//	GIRF* irf2 = new GIRF;
//	irf2->AddPdf(mypdf);
//	irf2->Write("!paramTest.fits");
//
//
	flexIRF::GIRFConfig config;
//	config.SetDefault();
	config.AddAxisRange(flexIRF::kEnergy,-1.7,-0.9, 1);
	config.AddAxisRange(flexIRF::kPhi, -1,1, 0);
//	config.Print();
	string filename = "paramTest.fits";
	flexIRF::GIRF irf(filename);
	flexIRF::GIRFPdf* effArea = dynamic_cast<flexIRF::GIRFPdf*>(irf.ReadPdf(flexIRF::kAeff, config));
	if (!effArea || effArea->IsEmpty()) return 2;
	effArea->Print();
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
////	effArea->Draw();





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
