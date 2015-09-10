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
	cout << "axis1.GetRangeMin() = " << axis1.GetRangeMin() << endl;
	cout << "axis2.GetRangeMin() = " << axis2.GetRangeMin() << endl;

	cout << "axis1.GetRangeMax() = " << axis1.GetRangeMax() << endl;
	cout << "axis2.GetRangeMax() = " << axis2.GetRangeMax() << endl;

	cout << "(axis1==axis2) = " << (axis1==axis2) << endl;

	cout << "axis1.GetExtName() = " << axis1.GetExtName().data() << endl;
	int lastID = axis1.GetLastAxisID("alltest.fits");
	cout << "lastID = " << lastID << endl;;



	return 0;
}//Ends main
