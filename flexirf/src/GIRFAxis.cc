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
// Return the axis name for EXTNAME
//
virtual std::string GIRFAxis::GetVarName() const {

	string axisName, axisType, axisVarType;

    switch(fVarType)
      {
      case kEnergy: //Energy Resolution for E (kEnergy) v. Offset (kTheta)
    	  axisVarType="ENERGY";
		  break;
      case kEnergy_true: //Energy Bias for E (kEnergy) v. Offset (kTheta)
    	  axisVarType="TENERGY";
		  break;
      case kEnergy_rec: //Angular Resolution for E (kEnergy) v. Offset (kTheta) v. Azimuth (kPhi)
    	  axisVarType="RENERGY";
		  break;
      case kTheta: //Angular Resolution for E (kEnergy) v. Offset (kTheta) v. Azimuth (kPhi)
    	  axisVarType="THETA";
		  break;
      case kPhi: //Angular Resolution for E (kEnergy) v. Offset (kTheta) v. Azimuth (kPhi)
    	  axisVarType="PHI";
		  break;
      case kID: //Angular Resolution for E (kEnergy) v. Offset (kTheta) v. Azimuth (kPhi)
    	  axisVarType="ID";
		  break;
      default:
    	  cout << "Incorrect variable type.\n";
    	  return axisName;
      }


    switch(fAxisType)
      {
      case kBins: //Energy Resolution for E (kEnergy) v. Offset (kTheta)
    	  axisVarType="BINS";
		  break;
      case kParam: //Energy Bias for E (kEnergy) v. Offset (kTheta)
    	  axisVarType="PARAM";
		  break;
      default:
        cout << "Incorrect axis type.\n";
        break;
      }

    axisName=axisType+"_"+axisVarType;

    return axisName;
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


//  TODO: Convert Axis HDUs into binary tables.

//	char keyword[9];
//	char chval[20];
//	char comment[70];
//	ushort usval;
//	// write axis name
//	sprintf(keyword,"EXTNAME");
//	sprintf(chval,"AXIS%03d",iaxis);
//	sprintf(comment,"Axis HDU");
//	if(fits_write_key(fptr,TSTRING,keyword,&chval,comment,status))
//	cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: " << *status <<")" << endl;
//
//	// write axis type
//	sprintf(keyword,"AXISTYPE");
//	usval = ushort(fAxisType);
//	sprintf(comment,"Axis Type (see GIRFAxis.h for details)");
//	if(fits_write_key(fptr,TUSHORT,keyword,&usval,comment,status))
//	cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: " << *status <<")" << endl;
//
//
//
//
//
//	// Add the axis header as a binary table:
//    if(fits_create_tbl(fptr,BINARY_TBL,0, 1, &size,status))
//	  cout << "GIRFAxis::Write Error: problem writing axis header (error code: " << *status <<")" << endl;
//    fits_create_tbl(fptr, BINARY_TBL, 0, tfields, ttype, tform, tunit,
//    		  extname, status);
//
//  int fits_create_tbl(fitsfile *fptr, int tbltype, LONGLONG naxis2, int tfields, char *ttype[],
//	   char *tform[], char *tunit[], char *extname, int *status)
  
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
