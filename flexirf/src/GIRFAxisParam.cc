/* ======================================================================== *\
!
!   Author(s): Javier Rico     12/2013 <mailto:jrico@ifae.es>
!
!   Copyright: CTA Software Development, 2013
!
\* ======================================================================== */

//////////////////////////////////////////////////////////////////////////////
//
// GIRFAxisParam
//
// Class to represent axes describing the organization of data
// in the GIRF object
// The axis represents a parameterization
//
//////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "GIRFAxisParam.h"

using namespace std;

////////////////////////////////////////////////////////////////
// 
// Construct empty axis object
//
GIRFAxisParam::GIRFAxisParam() :
  GIRFAxis()
{ 
}

////////////////////////////////////////////////////////////////
// 
// Construct axis object with predefined size
//
GIRFAxisParam::GIRFAxisParam(std::vector<float>::size_type size,bool islog) :
fIsLog(islog)
{
  fAxisParam.reserve(size);
}

////////////////////////////////////////////////////////////////
// 
// Construct axis object with predefined data
//
GIRFAxisParam::GIRFAxisParam(VarType vartype,bool islog) :
  GIRFAxis(vartype), fIsLog(islog)
{
  SetAxisType(kParam);
}

////////////////////////////////////////////////////////////////
// 
// Construct axis object with predefined size
//
GIRFAxisParam::GIRFAxisParam(VarType vartype,std::vector<float>::size_type size,bool islog) :
  GIRFAxis(vartype), fIsLog(islog)
{
  SetAxisType(kParam);
  fAxisParam.reserve(size);
}
////////////////////////////////////////////////////////////////
// 
// Construct axis object with predefined vectors
//
GIRFAxisParam::GIRFAxisParam(VarType vartype,std::vector<float> bins,bool islog) :
  GIRFAxis(vartype), fIsLog(islog)
{
  SetAxisType(kParam);
  fAxisParam     = bins;

  int status=CheckAxisConsistency();
  if(status)
    cout << "GIRFAxisParam::GIRFAxisParam: Warning: Axis is NOT consistent (" << status << ")" << endl;
}

////////////////////////////////////////////////////////////////
// 
// Construct axis object with predefined vectors (using arrays)
//
GIRFAxisParam::GIRFAxisParam(VarType vartype,std::vector<float>::size_type size,float* bins,bool islog) :
  GIRFAxis(vartype), fIsLog(islog)
{
  SetAxisType(kParam);
  SetAxis(size,bins);
}

////////////////////////////////////////////////////////////////
// 
// Construct axis object directly reading from HDU
//
GIRFAxisParam::GIRFAxisParam(fitsfile* fptr,int* status)
{

	//TODO: Read everything from fits file.


}


////////////////////////////////////////////////////////////////
//
// Check that the vector describe consistently the axis
//
int GIRFAxisParam::CheckAxisConsistency()
{
  int status = GIRFAxis::CheckAxisConsistency();
  return status;
}

////////////////////////////////////////////////////////////////
// 
// Check that the vector describe consistently the axis
//
void GIRFAxisParam::SetAxis(std::vector<float>::size_type size,float* bins)
{
  // Clear vector
  fAxisParam.clear();

  // Resize vector
  fAxisParam.reserve(size);

  // Fill vector with arrays
  for(std::vector<float>::size_type i=0;i<size;i++)
    fAxisParam.push_back(bins[i]);
}

////////////////////////////////////////////////////////////////
// 
// Write the axis to the specified file pointer
//
int GIRFAxisParam::Write(fitsfile* fptr, int& lastID, int* status)
{
  // fill the data array
  std::vector<float>::size_type axisSize = fAxisParam.size();
  float* axisdata = new float[axisSize];
  for(std::vector<float>::size_type ibin=0;ibin<axisSize;ibin++)
    axisdata[ibin] = fAxisParam[ibin];
  
  // write the axis header and data
  WriteAxis(fptr, int(axisSize), axisdata, lastID, status);

  return *status;
}
