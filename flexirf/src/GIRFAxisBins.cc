/* ======================================================================== *\
!
 !   Author(s): Javier Rico     12/2013 <mailto:jrico@ifae.es>
 !
 !   Copyright: CTA Software Development, 2013
 !
 \* ======================================================================== */

//////////////////////////////////////////////////////////////////////////////
//
// GIRFAxisBins
//
// Class to represent axes describing the organization of data
// in the GIRF object
// The axis represents bins
//
//////////////////////////////////////////////////////////////////////////////
#include <iostream>

#include "GIRFAxisBins.h"
#include "string.h"

using namespace std;

////////////////////////////////////////////////////////////////
// 
// Construct empty axis object
//
GIRFAxisBins::GIRFAxisBins() : GIRFAxis(), fIsLog(0), fAxisBinsFilled(0){
	SetAxisType(kBins);
}

////////////////////////////////////////////////////////////////
// 
// Construct axis object with predefined size
//
GIRFAxisBins::GIRFAxisBins(std::vector<float>::size_type size, bool islog) :
		fIsLog(islog), fAxisBinsFilled(0) {
	SetAxisType(kBins);
	fAxisBins.reserve(size);
}

////////////////////////////////////////////////////////////////
// 
// Construct axis object with predefined data
//
GIRFAxisBins::GIRFAxisBins(VarType vartype, bool islog) :
		GIRFAxis(vartype), fIsLog(islog), fAxisBinsFilled(0) {
	SetAxisType(kBins);
}

////////////////////////////////////////////////////////////////
// 
// Construct axis object with predefined size
//
GIRFAxisBins::GIRFAxisBins(VarType vartype, std::vector<float>::size_type size,
		bool islog) :
		GIRFAxis(vartype), fIsLog(islog), fAxisBinsFilled(0) {
	SetAxisType(kBins);
	fAxisBins.reserve(size);
}
////////////////////////////////////////////////////////////////
// 
// Construct axis object with predefined vectors
//
GIRFAxisBins::GIRFAxisBins(VarType vartype, std::vector<float> bins, bool islog) :
		GIRFAxis(vartype), fIsLog(islog) {
	SetAxisType(kBins);
	fAxisBins = bins;

	int status = CheckAxisConsistency();
	if (status)
		cout << "GIRFAxisBins::GIRFAxisBins: Warning: Axis is NOT consistent ("
				<< status << ")" << endl;
}

////////////////////////////////////////////////////////////////
// 
// Construct axis object with predefined vectors (using arrays)
//
GIRFAxisBins::GIRFAxisBins(VarType vartype, std::vector<float>::size_type size,
		float* bins, bool islog) :
		GIRFAxis(vartype), fIsLog(islog) {
	SetAxisType(kBins);
	SetAxis(size, bins);
}


////////////////////////////////////////////////////////////////
//
// Construct axis object directly reading from current HDU
//
GIRFAxisBins::GIRFAxisBins(fitsfile* fptr,int* status)
{
	SetAxisType(kBins);

	long int nRows;
	int nCol, anynull;
	float nullfloat = 0.0F;
	char card[FLEN_CARD]; /* Standard string lengths defined in fitsio.h */

	fits_get_num_rows(fptr, &nRows, status);
	fits_get_num_cols(fptr, &nCol, status);
	float farray[nRows];
	//TODO: For now, just get one column. In the future maybe Axis should have several columns (low/high bin edges)
	fits_read_col (fptr, TFLOAT, 1, 1, 1, nRows, &nullfloat, &farray, &anynull, status);
	fAxisBins.assign(farray,farray+nRows);
	fAxisBinsFilled=1;
	fIsLog=0;

	fits_read_key_str(fptr, "VARTYPE", card, NULL, status);
	SetVarType((VarType)atoi(card));
}






////////////////////////////////////////////////////////////////
// 
// Check that the vector describe consistently the axis
//
int GIRFAxisBins::CheckAxisConsistency() {
	int status = GIRFAxis::CheckAxisConsistency();

	if (fAxisBins.size() == 0) {
		cout << "ERROR: empty Axis" << endl;
		status++;
	}
	// check if bins are correctly defined
	for (unsigned ibin = 0; ibin < fAxisBins.size() - 1; ibin++)
		if (fAxisBins[ibin] >= fAxisBins[ibin + 1]){
			cout << "ERROR: Axis Bins need to be in increasing order." << endl;
			status++;
		}

	// return number of inconsitencies found
	return status;
}

////////////////////////////////////////////////////////////////
//
// Check if both axis are identical. For now, only ranges and bins
//	TODO: check values inside
bool GIRFAxisBins::operator==(const GIRFAxisBins& otherAxis) {
	if (otherAxis.GetAxisType() == this->GetAxisType() && otherAxis.GetVarType() == this->GetVarType()){
		if (otherAxis.GetRangeMax() == this->GetRangeMax() && otherAxis.GetRangeMin() == this->GetRangeMin() && otherAxis.GetSize() == this->GetSize()){
			vector<float> otherAxisBins = otherAxis.GetAxisBins();
			if (otherAxisBins==otherAxisBins) return 1;
			else return 0;
		} else return 0;
	} else return 0;
}


////////////////////////////////////////////////////////////////
//
// Check if axis contains AxisRange
//
bool GIRFAxisBins::ContainsRange(GIRFAxis::AxisRange axisRange){
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
void GIRFAxisBins::SetAxis(std::vector<float>::size_type size, float* bins) {
	// Clear vector
	fAxisBins.clear();

	// Resize vector
	fAxisBins.reserve(size);

	// Fill vector with arrays
	for (std::vector<float>::size_type i = 0; i < size; i++)
		fAxisBins.push_back(bins[i]);
	fAxisBinsFilled=1;
}

////////////////////////////////////////////////////////////////
// 
// Write the axis to the specified file pointer
//
int GIRFAxisBins::Write(fitsfile* fptr, int& axisID, int* status) {
	// fill the data array
	std::vector<float>::size_type axisSize = fAxisBins.size();
	float* axisdata = new float[axisSize];
	for (std::vector<float>::size_type ibin = 0; ibin < axisSize; ibin++)
		axisdata[ibin] = fAxisBins[ibin];


	if (!CheckAxisExists(fptr, axisID, status)){
		// write the axis header and data
		WriteAxis(fptr, int(axisSize), axisdata, axisID, status);
	}
	return *status;
}


////////////////////////////////////////////////////////////////
//
// Print Axis content
//
void GIRFAxisBins::Print()
{
	//TODO: Improve print output.
	std::vector<float>::size_type axisSize = fAxisBins.size();
	for (std::vector<float>::size_type ibin = 0; ibin < axisSize; ibin++) cout << "fAxisBins[" << ibin << "] = " << fAxisBins[ibin] << endl;
}



////////////////////////////////////////////////////////////////
//
// 		Check if the Axis already exists within the fits file
//
bool GIRFAxisBins::CheckAxisExists(fitsfile* fptr, int& axisID, int* status) {

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
					if (!fits_read_key_str(fptr, "VARTYPE", card, NULL, status)) {
						if ((ushort)atoi(card) == (ushort)this->GetVarType()) {
							if (!fits_read_key_str(fptr, "HDUCLAS3", card, NULL, status)) {
								if (!strcmp(card, "BINS") && this->GetAxisType() == kBins) {
									GIRFAxisBins* IRFAxis = new GIRFAxisBins(fptr, status);
									if ((*IRFAxis)==(*this)) {
										if (!fits_read_key_str(fptr, "HDUCLAS4", card, NULL, status)) {
											axisID=(ushort)atoi(card);
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
		if (*status == KEY_NO_EXIST) *status = 0;
		if (*status) break;
	}

	fits_movabs_hdu(fptr, currenthdu + 1, NULL, status);
	return exists;
}

////////////////////////////////////////////////////////////////
//
// 		Resize Axis, checking internal axis limit values
//
void GIRFAxisBins::Resize(float lValue, float hValue){
	int lbin, hbin;
	bool lBinFilled=0,hBinFilled=0;
	for (std::vector<float>::size_type ibin = 0; ibin < fAxisBins.size(); ibin++) {
		if ((fAxisBins[ibin]-lValue) > 0.000001 && !lBinFilled){ lbin = ibin-1; lBinFilled=1;}
		if ((fAxisBins[ibin]-hValue) >= 0 && !hBinFilled){ hbin = ibin+1; hBinFilled=1;}
	}
	ResizeBins(lbin, hbin);
}

////////////////////////////////////////////////////////////////
//
// 		Resize Axis, checking internal axis limit values,
// 		returning lbin and hbin.
//
void GIRFAxisBins::Resize(float lValue, float hValue, int *lbin, int *hbin){

	bool lBinFilled=0,hBinFilled=0;
	for (std::vector<float>::size_type ibin = 0; ibin < fAxisBins.size(); ibin++) {
//		cout << "fAxisBins[" << ibin << "] = " << fAxisBins[ibin] << " and lValue = " << lValue << endl;
		if ((fAxisBins[ibin]-lValue) > 0.000001 && !lBinFilled){ *lbin = ibin-1; lBinFilled=1;}
		if ((fAxisBins[ibin]-hValue) >= 0 && !hBinFilled){ *hbin = ibin+1; hBinFilled=1;}
	}
//	cout << "lbin = " << *lbin << " & hbin = " << *hbin << endl;
	ResizeBins((*lbin), (*hbin));
}


