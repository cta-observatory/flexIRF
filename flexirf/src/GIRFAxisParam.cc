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
#include "string.h"

using namespace std;

////////////////////////////////////////////////////////////////
// 
// Construct empty axis object
//
flexIRF::GIRFAxisParam::GIRFAxisParam() :
		GIRFAxis(), fAxisParameterizationFilled(0){

	SetAxisType(kParam);
}

////////////////////////////////////////////////////////////////
// 
// Construct axis object with predefined data
//
flexIRF::GIRFAxisParam::GIRFAxisParam(VarType varType, ScaleType scaleType) :
		GIRFAxis(varType), fAxisParameterizationFilled(0){
	SetAxisType(kParam);
	SetScale(scaleType);
}


////////////////////////////////////////////////////////////////
// 
// Construct axis object with predefined vectors (using arrays)
//
flexIRF::GIRFAxisParam::GIRFAxisParam(VarType varType, AxisParameterization axisParam,
		ScaleType scaleType) : GIRFAxis(varType) {
	SetAxisType(kParam);
	fAxisParam = axisParam;
	SetScale(scaleType);

	// Check if the axis is properly parameterized
	CheckAxisParameterizationFilled();
}


////////////////////////////////////////////////////////////////
//
// Construct axis object directly reading from HDU
//
flexIRF::GIRFAxisParam::GIRFAxisParam(fitsfile* fptr, int* status) {

	SetAxisType(kParam);

	long int nRows;
	int nCol, anynull;
	float nullfloat = 0.0F;
	char card[FLEN_CARD]; /* Standard string lengths defined in fitsio.h */
	char *longcard;
	float validRangeLow, validRangeHigh;
	int numVars;

	fits_get_num_rows(fptr, &nRows, status);
	fits_get_num_cols(fptr, &nCol, status);
	float farray[nRows];
	//TODO: For now, just get one column. In the future maybe Axis should have several columns (low/high bin edges)
	fits_read_col(fptr, TFLOAT, 1, 1, 1, nRows, &nullfloat, &farray, &anynull,
			status);

	fits_read_key_str(fptr, "NUMVARS", card, NULL, status);
	numVars = atoi(card);

	fits_read_key_longstr(fptr, "FORMULA", &longcard, NULL, status);
	SetFormula((string) longcard, numVars);

	fits_read_key_str(fptr, "VARTYPE", card, NULL, status);
	SetVarType((VarType) atoi(card));

	fits_read_key_str(fptr, "VRANGE_L", card, NULL, status);
	fAxisParam.validRangeLow = atof(card);

	fits_read_key_str(fptr, "VRANGE_H", card, NULL, status);
	fAxisParam.validRangeHigh = atof(card);

//	SetValidRange();

	fits_read_key_str(fptr, "SCALE", card, NULL, status);
	SetScale((string) card);

	//TODO: Read everything from fits file.
	CheckAxisParameterizationFilled();

}

////////////////////////////////////////////////////////////////
//
// Check if axis contains AxisRange
//
bool flexIRF::GIRFAxisParam::ContainsRange(AxisRange axisRange) {

	//TODO: Modify as soon as I add the range of validity!!!
	if (!fAxisParameterizationFilled) return 0;

	if (axisRange.varType != GetVarType())
		return 0;							//Sanity check
	else {
		if (axisRange.lowRange >= GetRangeMin()
				&& axisRange.highRange <= GetRangeMax()){
			return 1;
		}

		else
			return 0;
	}
}



void flexIRF::GIRFAxisParam::SetAxisParam(AxisParameterization axisParam){

	fAxisParam = axisParam;
	// Check if the axis is properly parameterized
	CheckAxisParameterizationFilled();

}


////////////////////////////////////////////////////////////////
//
// 		Set the parametrized formula of the axis.
//
void flexIRF::GIRFAxisParam::SetFormula(string formula,
		std::vector<float>::size_type numParameters) {


	fAxisParam.formula = formula;
	// Fill vector with arrays

	fAxisParam.numParameters = numParameters;

	CheckAxisParameterizationFilled();
}

////////////////////////////////////////////////////////////////
//
// 		Set the valid range of the parameterization.
//
void flexIRF::GIRFAxisParam::SetValidRange(float validRangeLow, float validRangeHigh) {


	fAxisParam.validRangeLow = validRangeLow;
	fAxisParam.validRangeHigh = validRangeHigh;

	CheckAxisParameterizationFilled();
}


////////////////////////////////////////////////////////////////
// 
// Write the axis to the specified file pointer
//
int const flexIRF::GIRFAxisParam::Write(fitsfile* fptr, int& axisID, int* status) {
	// fill the data array
	float* axisdata;

	if (!CheckAxisExists(fptr, axisID, status)) {
		// write the axis header and data
		WriteAxis(fptr, 0, axisdata, axisID, status);
	}

	return *status;
}

////////////////////////////////////////////////////////////////
//
// Check if the Axis already exists within the fits file
//
bool const flexIRF::GIRFAxisParam::CheckAxisExists(fitsfile* fptr, int& axisID, int* status) {


	int currenthdu = fptr->HDUposition;

	char card[FLEN_CARD]; /* Standard string lengths defined in fitsio.h */
	int single = 0, hdutype = BINARY_TBL, hdunum, nkeys, ii;

	fits_get_num_hdus(fptr, &hdunum, status);
	for (int hdupos = 1; hdupos <= hdunum; hdupos++) /* Main loop through each extension */
	{
		fits_movabs_hdu(fptr, hdupos, &hdutype, status);
		if (hdutype == BINARY_TBL) {
			if (!fits_read_key_str(fptr, "HDUCLAS2", card, NULL, status)) {
				if (!strcmp(card, "AXIS")) {
					if (!fits_read_key_str(fptr, "VARTYPE", card, NULL,
							status)) {
						if ((ushort) atoi(card)
								== (ushort) this->GetVarType()) {
							if (!fits_read_key_str(fptr, "HDUCLAS3", card, NULL,
									status)) {
								if (!strcmp(card, "PARAM")
										&& this->GetAxisType() == kParam) {
									GIRFAxisParam* IRFAxis = new GIRFAxisParam(
											fptr, status);
									if ((*IRFAxis) == (*this)) {
										if (!fits_read_key_str(fptr, "HDUCLAS4",
												card, NULL, status)) {
											axisID = (ushort) atoi(card);
											return TRUE;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		if (*status == KEY_NO_EXIST)
			*status = 0;
		if (*status)
			break;
	}

	fits_movabs_hdu(fptr, currenthdu + 1, NULL, status);
	return FALSE;
}

////////////////////////////////////////////////////////////////
//
// Check that the vector describe consistently the axis
//
int flexIRF::GIRFAxisParam::CheckAxisConsistency() {
	int status = GIRFAxis::CheckAxisConsistency();

	if (!fAxisParameterizationFilled) status++;
	//TODO: Check if the function contains the correct number of constants and variables.??
	return status;
}

////////////////////////////////////////////////////////////////
//
// 	Print the content of the parameterized axis.
//
void const flexIRF::GIRFAxisParam::Print() {
	cout << "GIRFAxisParam:" << endl;
	cout << "formula = " << fAxisParam.formula << endl;
	cout << "numParameters = " << fAxisParam.numParameters << endl;
	cout << "valid Range = { " << fAxisParam.validRangeLow << ", " << fAxisParam.validRangeHigh << "}" << endl;
	//TODO: print constants
}

////////////////////////////////////////////////////////////////
//
// Check if the parameterized axis is filled propperly
//
bool flexIRF::GIRFAxisParam::CheckAxisParameterizationFilled(){


	if (fAxisParam.validRangeLow < fAxisParam.validRangeHigh && fAxisParam.validRangeLow != fAxisParam.validRangeHigh){
		if (!fAxisParam.formula.empty()){
			fAxisParameterizationFilled=1;
			return 1;
		}
	}
	return 0;
}

