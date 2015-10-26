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
GIRFAxisParam::GIRFAxisParam() :
		GIRFAxis(), fAxisParameterizationFilled(0){

}

////////////////////////////////////////////////////////////////
// 
// Construct axis object with predefined data
//
GIRFAxisParam::GIRFAxisParam(VarType vartype, bool islog) :
		GIRFAxis(vartype), fAxisParameterizationFilled(0){
	SetAxisType(kParam);
	SetLog(islog);
}

////////////////////////////////////////////////////////////////
// 
// Construct axis object with predefined size
//
GIRFAxisParam::GIRFAxisParam(VarType vartype,
		std::vector<float>::size_type size, bool islog) :
		GIRFAxis(vartype), fAxisParameterizationFilled(0){
	SetAxisType(kParam);
	fAxisParam.constants.reserve(size);
	SetLog(islog);
}

////////////////////////////////////////////////////////////////
// 
// Construct axis object with predefined vectors
//
GIRFAxisParam::GIRFAxisParam(VarType vartype, std::vector<float> bins,
		bool islog) :
		GIRFAxis(vartype), fAxisParameterizationFilled(0){
	SetAxisType(kParam);
	fAxisParam.constants = bins;
	SetLog(islog);

}

////////////////////////////////////////////////////////////////
// 
// Construct axis object with predefined vectors (using arrays)
//
GIRFAxisParam::GIRFAxisParam(VarType vartype,
		std::vector<float>::size_type size, float* bins, bool islog) :
		GIRFAxis(vartype) {
	SetAxisType(kParam);
	SetAxis(size, bins);
	SetLog(islog);
}

////////////////////////////////////////////////////////////////
// 
// Construct axis object with predefined vectors (using arrays)
//
GIRFAxisParam::GIRFAxisParam(VarType vartype, AxisParameterization axisParam,
		bool islog) : GIRFAxis(vartype) {
	fAxisParam = axisParam;
	SetLog(islog);

	// Check if the axis is properly parameterized
	CheckAxisParameterizationFilled();
}


////////////////////////////////////////////////////////////////
//
// Construct axis object directly reading from HDU
//
GIRFAxisParam::GIRFAxisParam(fitsfile* fptr, int* status) {

	SetAxisType(kParam);

	long int nRows;
	int nCol, anynull;
	float nullfloat = 0.0F;
	char card[FLEN_CARD]; /* Standard string lengths defined in fitsio.h */
	char *longcard;
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
	SetFormula((string) longcard, nRows, farray, numVars);


	fits_read_key_str(fptr, "VARTYPE", card, NULL, status);
	SetVarType((VarType) atoi(card));

	fits_read_key_str(fptr, "ISLOG", card, NULL, status);
	SetLog((bool)atoi(card));

	//TODO: Read everything from fits file.
	CheckAxisParameterizationFilled();

}

////////////////////////////////////////////////////////////////
//
// Check if axis contains AxisRange
//
bool GIRFAxisParam::ContainsRange(GIRFAxis::AxisRange axisRange) {

	//TODO: Modify as soon as I add the range of validity!!!
	if (!fAxisParameterizationFilled) return 0;

	if (axisRange.varType != GetVarType())
		return 0;							//Sanity check
	else {
		if (axisRange.lowRange > GetRangeMin()
				&& axisRange.highRange < GetRangeMax())
			return 1;
		else
			return 0;
	}
}


////////////////////////////////////////////////////////////////
// 
// 		Set the fAxisParam content.
//
void GIRFAxisParam::SetAxis(vector<float>::size_type size, float* bins) {
	// Clear vector
	fAxisParam.constants.clear();

	// Resize vector
	fAxisParam.constants.reserve(size);

	// Fill vector with arrays
	for (std::vector<float>::size_type i = 0; i < size; i++)
		fAxisParam.constants.push_back(bins[i]);
	// Check if the axis is properly parameterized
	CheckAxisParameterizationFilled();
}

void GIRFAxisParam::SetAxisParam(AxisParameterization axisParam){

	fAxisParam = axisParam;
	// Check if the axis is properly parameterized
	CheckAxisParameterizationFilled();

}


////////////////////////////////////////////////////////////////
//
// 		Set the parametrized formula of the axis.
//
void GIRFAxisParam::SetFormula(string formula,
		std::vector<float>::size_type numParameters, float* parameters,
		int numVariables) {

	fAxisParam.constants.reserve(numParameters);
	fAxisParam.formula = formula;
	// Fill vector with arrays
	for (std::vector<float>::size_type i = 0; i < numParameters; i++)
		fAxisParam.constants.push_back(parameters[i]);

	CheckAxisParameterizationFilled();
}

////////////////////////////////////////////////////////////////
// 
// Write the axis to the specified file pointer
//
int const GIRFAxisParam::Write(fitsfile* fptr, int& axisID, int* status) {
	// fill the data array
	std::vector<float>::size_type axisSize = fAxisParam.constants.size();
	float* axisdata = new float[axisSize];
	for (std::vector<float>::size_type ibin = 0; ibin < axisSize; ibin++)
		axisdata[ibin] = fAxisParam.constants[ibin];

	if (!CheckAxisExists(fptr, axisID, status)) {
		// write the axis header and data
		WriteAxis(fptr, int(axisSize), axisdata, axisID, status);
	}

	return *status;
}

////////////////////////////////////////////////////////////////
//
// Check if the Axis already exists within the fits file
//
bool const GIRFAxisParam::CheckAxisExists(fitsfile* fptr, int& axisID, int* status) {


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
int GIRFAxisParam::CheckAxisConsistency() {
	int status = GIRFAxis::CheckAxisConsistency();

	if (!fAxisParameterizationFilled) status++;
	//TODO: Check if the function contains the correct number of constants and variables.??
	return status;
}

////////////////////////////////////////////////////////////////
//
// 	Print the content of the parameterized axis.
//
void const GIRFAxisParam::Print() {

}

////////////////////////////////////////////////////////////////
//
// Check if the parameterized axis is filled propperly
//
bool GIRFAxisParam::CheckAxisParameterizationFilled(){

	if (fAxisParam.validRangeLow < fAxisParam.validRangeLow && fAxisParam.validRangeLow != fAxisParam.validRangeHigh){
		if (!fAxisParam.formula.empty()){
			if (fAxisParam.numConstants == (int) fAxisParam.constants.size()){
				fAxisParameterizationFilled=1;
				return 1;
			}
		}
	}
	return 0;
}

