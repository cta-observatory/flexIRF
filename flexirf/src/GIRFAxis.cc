/* ======================================================================== *\
!
!   Author(s): Javier Rico     12/2013 <mailto:jrico@ifae.es>
!
!   Copyright: CTA Software Development, 2013
!
\* ======================================================================== */

//////////////////////////////////////////////////////////////////////////////
//
// GIRFAxis
//
// Abstract class to represent axes describing the organization of data
// in the GIRF object. Inherited by GIRFAxisBins and GIRFAxisParam
// to describe the two types of supported axes
//
//////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "GIRFAxis.h"

using namespace std;

////////////////////////////////////////////////////////////////
// 
// Construct empty axis object
//
GIRFAxis::GIRFAxis() :
  fAxisType(kNoAxisType), fVarType(GIRFAxis::kNoVarType)
{ 
}

////////////////////////////////////////////////////////////////
// 
// Construct empty axis object
//
GIRFAxis::GIRFAxis(VarType vartype) :
  fAxisType(kNoAxisType), fVarType(vartype)
{ 
}
////////////////////////////////////////////////////////////////
// 
// Check that the vector describe consistently the axis
//
int GIRFAxis::CheckAxisConsistency()
{
  int status = 0;
  if(fVarType<=kNoVarType || fVarType>=kVarMax) status++;
  
  return status;
}
////////////////////////////////////////////////////////////////
// 
// Write the header of the axis HDU
//
int GIRFAxis::WriteAxis(fitsfile* fptr,int iaxis,long size,float* data,int* status)
{
  // write the axis header
  if(fits_create_img(fptr,FLOAT_IMG,1,&size,status))
    cout << "GIRFAxis::Write Error: problem writing axis header (error code: " << *status <<")" << endl;
  
  char keyword[9];
  char chval[20];
  char comment[70];
  ushort usval;

   // write axis name
  sprintf(keyword,"EXTNAME");
  sprintf(chval,"AXIS%03d",iaxis);
  sprintf(comment,"Axis HDU");
  if(fits_write_key(fptr,TSTRING,keyword,&chval,comment,status))
    cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: " << *status <<")" << endl;
  
   // write axis type
  sprintf(keyword,"AXISTYPE");
  usval = ushort(fAxisType);
  sprintf(comment,"Axis Type (see GIRFAxis.h for details)");
  if(fits_write_key(fptr,TUSHORT,keyword,&usval,comment,status))
    cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: " << *status <<")" << endl;
  
   // write variable type
  sprintf(keyword,"VARTYPE");
  usval = ushort(fVarType);
  sprintf(comment,"Variable Type (see GIRFAxis.h for details)");
  if(fits_write_key(fptr,TUSHORT,keyword,&usval,comment,status))
    cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: " << *status <<")" << endl;

  // write the axis data
  long fpixel[1] = {1}; // first entry of the axis to be saved (1 is first, not 0)
  if(fits_write_pix(fptr,TFLOAT,fpixel,size,data,status))
    cout << "GIRFAxis::WriteAxis Error: problem writing axis data (error code: " << *status <<")" << endl;  
  
  return *status;
}
