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
		GIRFAxis() {
}

////////////////////////////////////////////////////////////////
// 
// Construct axis object with predefined size
//
GIRFAxisParam::GIRFAxisParam(std::vector<float>::size_type size, bool islog) :
		fIsLog(islog) {
	fAxisParam.reserve(size);
}

////////////////////////////////////////////////////////////////
// 
// Construct axis object with predefined data
//
GIRFAxisParam::GIRFAxisParam(VarType vartype, bool islog) :
		GIRFAxis(vartype), fIsLog(islog) {
	SetAxisType(kParam);
}

////////////////////////////////////////////////////////////////
// 
// Construct axis object with predefined size
//
GIRFAxisParam::GIRFAxisParam(VarType vartype,
		std::vector<float>::size_type size, bool islog) :
		GIRFAxis(vartype), fIsLog(islog) {
	SetAxisType(kParam);
	fAxisParam.reserve(size);
}
////////////////////////////////////////////////////////////////
// 
// Construct axis object with predefined vectors
//
GIRFAxisParam::GIRFAxisParam(VarType vartype, std::vector<float> bins,
		bool islog) :
		GIRFAxis(vartype), fIsLog(islog) {
	SetAxisType(kParam);
	fAxisParam = bins;

	int status = CheckAxisConsistency();
	if (status)
		cout
				<< "GIRFAxisParam::GIRFAxisParam: Warning: Axis is NOT consistent ("
				<< status << ")" << endl;
}

////////////////////////////////////////////////////////////////
// 
// Construct axis object with predefined vectors (using arrays)
//
GIRFAxisParam::GIRFAxisParam(VarType vartype,
		std::vector<float>::size_type size, float* bins, bool islog) :
		GIRFAxis(vartype), fIsLog(islog) {
	SetAxisType(kParam);
	SetAxis(size, bins);
}

////////////////////////////////////////////////////////////////
// 
// Construct axis object directly reading from HDU
//
GIRFAxisParam::GIRFAxisParam(fitsfile* fptr, int* status) {

	//TODO: Read everything from fits file.

}

////////////////////////////////////////////////////////////////
//
// Check if axis contains AxisRange
//
bool GIRFAxisParam::ContainsRange(GIRFAxis::AxisRange axisRange){
	if (axisRange.varType != this->GetVarType()) return 0;							//Sanity check
	else{
		if (axisRange.lowRange > this->GetRangeMin() && axisRange.highRange < this->GetRangeMax()) return 1;
		else return 0;
	}
}

////////////////////////////////////////////////////////////////
//
// Check that the vector describe consistently the axis
//
int GIRFAxisParam::CheckAxisConsistency() {
	int status = GIRFAxis::CheckAxisConsistency();
	return status;
}

////////////////////////////////////////////////////////////////
// 
// Check that the vector describe consistently the axis
//
void GIRFAxisParam::SetAxis(std::vector<float>::size_type size, float* bins) {
	// Clear vector
	fAxisParam.clear();

	// Resize vector
	fAxisParam.reserve(size);

	// Fill vector with arrays
	for (std::vector<float>::size_type i = 0; i < size; i++)
		fAxisParam.push_back(bins[i]);
}

////////////////////////////////////////////////////////////////
// 
// Write the axis to the specified file pointer
//
int GIRFAxisParam::Write(fitsfile* fptr, int& axisID, int* status) {
	// fill the data array
	std::vector<float>::size_type axisSize = fAxisParam.size();
	float* axisdata = new float[axisSize];
	for (std::vector<float>::size_type ibin = 0; ibin < axisSize; ibin++)
		axisdata[ibin] = fAxisParam[ibin];

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
bool GIRFAxisParam::CheckAxisExists(fitsfile* fptr, int& axisID, int* status) {

	bool exists = 0;

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
	return exists;
}
