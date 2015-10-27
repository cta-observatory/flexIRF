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
#include "GIRFAxisBins.h"
#include "GIRFAxisParam.h"
#include <stdio.h>
#include <string.h>

#include <GIRFUtils.h>

using namespace std;

////////////////////////////////////////////////////////////////
// 
// Construct empty axis object
//
GIRFAxis::GIRFAxis() :
		fAxisType(kNoAxisType), fVarType(GIRFAxis::kNoVarType), fIsLog(0) {
}

////////////////////////////////////////////////////////////////
// 
// Construct empty axis object
//
GIRFAxis::GIRFAxis(VarType vartype) :
		fAxisType(kNoAxisType), fVarType(vartype), fIsLog(false) {
}


////////////////////////////////////////////////////////////////
// 
// Check that the vector describe consistently the axis
//
int GIRFAxis::CheckAxisConsistency() {
	int status = 0;
	if (fVarType <= kNoVarType || fVarType >= kVarMax){
		cout << "ERROR: Axis has no VarType!" << endl;
		status++;
	}
	return status;
}


////////////////////////////////////////////////////////////////
//
// Return the axis name for EXTNAME
//
std::string GIRFAxis::GetTypeName() const {

	string axisType;

	switch (fAxisType) {
	case kBins:
		axisType = "BINS";
		break;
	case kParam:
		axisType = "PARAM";
		break;
	default:
		cout << "Incorrect axis type.\n";
		break;
	}

	return axisType;
}

////////////////////////////////////////////////////////////////
//
// Return the axis name for EXTNAME
//
std::string GIRFAxis::GetVarName() const {

	string axisVarType;

	switch (fVarType) {
	case kEnergy:
		axisVarType = "ENERGY";
		break;
	case kEnergy_true:
		axisVarType = "TENERGY";
		break;
	case kEnergy_rec:
		axisVarType = "RENERGY";
		break;
	case kTheta:
		axisVarType = "THETA";
		break;
	case kPhi:
		axisVarType = "PHI";
		break;
	case kID:
		axisVarType = "ID";
		break;
	default:
		cout << "Incorrect variable type.\n";
		return axisVarType;
	}

	return axisVarType;
}


////////////////////////////////////////////////////////////////
//
// Return the axis name for EXTNAME
//
std::string GIRFAxis::GetVarName(VarType varType) {

	string axisVarType;

	switch (varType) {
	case kEnergy:
		axisVarType = "ENERGY";
		break;
	case kEnergy_true:
		axisVarType = "TENERGY";
		break;
	case kEnergy_rec:
		axisVarType = "RENERGY";
		break;
	case kTheta:
		axisVarType = "THETA";
		break;
	case kPhi:
		axisVarType = "PHI";
		break;
	case kID:
		axisVarType = "ID";
		break;
	default:
		cout << "Incorrect variable type.\n";
		return axisVarType;
	}

	return axisVarType;
}




////////////////////////////////////////////////////////////////
//
// Set the axis type using FITS format keywords
//
void GIRFAxis::SetAxisType(string axisTypeName) {

	if (axisTypeName == "BINS") {
		fAxisType = kBins;
	} else if (axisTypeName == "PARAM") {
		fAxisType = kParam;
	} else {
		cout << "Incorrect axis type.\n";
	}
	return;
}

////////////////////////////////////////////////////////////////
//
// Set the axis variable type using FITS format keywords
//
void GIRFAxis::SetVarType(string axisVarName) {

	if (axisVarName == "ENERGY") {
		fVarType = kEnergy;
	} else if (axisVarName == "TENERGY") {
		fVarType = kEnergy_true;
	} else if (axisVarName == "RENERGY") {
		fVarType = kEnergy_rec;
	} else if (axisVarName == "THETA") {
		fVarType = kTheta;
	} else if (axisVarName == "PHI") {
		fVarType = kPhi;
	} else if (axisVarName == "ID") {
		fVarType = kID;
	} else {
		cout << "Incorrect axis variable type.\n";
	}
	return;

}

////////////////////////////////////////////////////////////////
//
// Return the axis name for EXTNAME
//
std::string GIRFAxis::GetVarUnit() const {

	string axisVarType;

	switch (fVarType) {
	case kEnergy:
		axisVarType = "TeV";
		break;
	case kEnergy_true:
		axisVarType = "TeV";
		break;
	case kEnergy_rec:
		axisVarType = "TeV";
		break;
	case kTheta:
		axisVarType = "deg";
		break;
	case kPhi:
		axisVarType = "deg";
		break;
	case kID:
		axisVarType = "";
		break;
	default:
		cout << "Incorrect variable type.\n";
		return axisVarType;
	}

	return axisVarType;
}

////////////////////////////////////////////////////////////////
// 
// Write the header of the axis HDU
//
int const GIRFAxis::WriteAxis(fitsfile* fptr, long size, float* data, int& lastID,
		int* status) {


	char extname[20], varname[20], form[20], unit[20];
	sprintf(extname, "%s", GetExtName().data());
	sprintf(varname, "%s", GetVarName().data());
	sprintf(form, "%s", "E");
	sprintf(unit, "%s", GetVarUnit().data());
	char *ttype[] = { varname };
	char *tform[] = { form }; // One column with float single precision (4 bytes)
	char *tunit[] = { unit };

	GIRFUtils::GoToLastAxisHDU(fptr);

//	if (fits_create_tbl(fptr, BINARY_TBL, 0, 1, ttype, tform, tunit, extname,
//			status))
	if (fits_insert_btbl(fptr, 0, 1, ttype, tform, tunit, extname, 0, status))
		cout
				<< "GIRFAxis::Write Error: problem writing axis header (error code: "
				<< *status << ")" << endl;

	char keyword[FLEN_KEYWORD];
	char chval[FLEN_CARD];
	char comment[FLEN_COMMENT];
	ushort usval;
	float floatValue;

// write axis type
	sprintf(keyword, "AXISTYPE");
	usval = ushort(fAxisType);
	sprintf(comment, "Axis Type (see GIRFAxis.h for details)");
	if (fits_write_key(fptr, TUSHORT, keyword, &usval, comment, status))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< *status << ")" << endl;

// write variable type
	sprintf(keyword, "VARTYPE");
	usval = ushort(fVarType);
	sprintf(comment, "Variable Type (see GIRFAxis.h for details)");
	if (fits_write_key(fptr, TUSHORT, keyword, &usval, comment, status))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< *status << ")" << endl;

// write IsLog
	sprintf(keyword, "ISLOG");
	usval = ushort(fIsLog);
	sprintf(comment, "If true, logarithm of the variable is stored");
	if (fits_write_key(fptr, TUSHORT, keyword, &usval, comment, status))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< *status << ")" << endl;


// Write keywords of each specific Axis type:
    GIRFAxisBins* binsPointer = dynamic_cast<GIRFAxisBins*>(this);
    if (binsPointer) {
    	//TODO:Any GIRFAxisBins specific keyword?
    }

    GIRFAxisParam* paramPointer = dynamic_cast<GIRFAxisParam*>(this);
    if (paramPointer) {
    	//TODO: write expression of parametrization
    	sprintf(keyword, "FORMULA");
    	sprintf(comment, "Parameterized formula");
    	if (fits_write_key_longstr(fptr, keyword, paramPointer->GetFormula().data(), comment, status))
    		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
    				<< *status << ")" << endl;

    	sprintf(keyword, "NUMVARS");
    	usval = ushort(paramPointer->GetNumVars());
    	sprintf(comment, "Number of variables used in the parameterization");
    	if (fits_write_key(fptr, TUSHORT, keyword, &usval, comment, status))
    		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
    				<< *status << ")" << endl;

    	sprintf(keyword, "VRANGE_L");
    	floatValue = float(paramPointer->GetRangeMin());
    	sprintf(comment, "Low edge of the parameterization valid range");
    	if (fits_write_key(fptr, TFLOAT, keyword, &floatValue, comment, status))
    		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
    				<< *status << ")" << endl;

    	sprintf(keyword, "VRANGE_H");
    	floatValue = float(paramPointer->GetRangeMax());
    	sprintf(comment, "High edge of the parameterization valid range");
    	if (fits_write_key(fptr, TFLOAT, keyword, &floatValue, comment, status))
    		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
    				<< *status << ")" << endl;

    }

// Write in the first column, from first row
	if (fits_write_col(fptr, TFLOAT, 1, 1, 1, size, data, status))
		cout
				<< "GIRFAxis::WriteAxis Error: problem writing axis data (error code: "
				<< *status << ")" << endl;

// Add class keywords to the HDU.
	sprintf(keyword, "HDUCLASS");
	sprintf(chval, "CTA");
	sprintf(comment, "FITS file following the CTA data format");
	if (fits_write_key(fptr, TSTRING, keyword, &chval, comment, status))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< *status << ")" << endl;
	sprintf(keyword, "HDUCLAS1");
	sprintf(chval, "IRM");
	sprintf(comment, "Instrument Response Model HDU");
	if (fits_write_key(fptr, TSTRING, keyword, &chval, comment, status))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< *status << ")" << endl;
	sprintf(keyword, "HDUCLAS2");
	sprintf(chval, "AXIS");
	sprintf(comment, "Axis HDU");
	if (fits_write_key(fptr, TSTRING, keyword, &chval, comment, status))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< *status << ")" << endl;
	sprintf(keyword, "HDUCLAS3");
	sprintf(chval, "%s", GetTypeName().data());
	sprintf(comment, "Variable Type (see GIRFAxis.h for details)");
	if (fits_write_key(fptr, TSTRING, keyword, &chval, comment, status))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< *status << ")" << endl;


	// Only for testing purposes.
//	GIRFAxisBins* IRFAxisbins;
//	int a = GIRFUtils::CheckAxisType(fptr, 1, status);
//	if (a == 1) {
//		IRFAxisbins = (GIRFAxisBins*) GetAxis(fptr, 1, (GIRFAxis::AxisType) a, status);
//		IRFAxisbins->Print();
//	}


	//Good spot for axis tests
	sprintf(keyword, "HDUCLAS4");
	lastID = GIRFUtils::GetLastAxisID(fptr) + 1;
	usval = ushort(lastID);
	sprintf(comment, "Axis ID");
	if (fits_write_key(fptr, TUSHORT, keyword, &usval, comment, status))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< *status << ")" << endl;

	//Required for the long string format.
	if (fits_write_key_longwarn(fptr,status))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< *status << ")" << endl;


	return *status;
}


