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
// Check that the vector describe consistently the axis
//
int GIRFAxisBins::CheckAxisConsistency() {
	int status = GIRFAxis::CheckAxisConsistency();

	// check if bins are correctly defined
	for (unsigned ibin = 0; ibin < fAxisBins.size() - 1; ibin++)
		if (fAxisBins[ibin] >= fAxisBins[ibin + 1])
			status++;

	// return number of inconsitencies found
	return status;
}

////////////////////////////////////////////////////////////////
//
// Check if both axis are identical
//
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
}

////////////////////////////////////////////////////////////////
// 
// Write the axis to the specified file pointer
//
int GIRFAxisBins::Write(fitsfile* fptr, int& lastID, int* status) {
	// fill the data array
	std::vector<float>::size_type axisSize = fAxisBins.size();
	float* axisdata = new float[axisSize];
	for (std::vector<float>::size_type ibin = 0; ibin < axisSize; ibin++)
		axisdata[ibin] = fAxisBins[ibin];

	// write the axis header and data
	WriteAxis(fptr, int(axisSize), axisdata, lastID, status);

	return *status;
}


////////////////////////////////////////////////////////////////
//
// 		Search for identical axis within the fits file
//
int GIRFAxisBins::IsAlreadyPresent(fitsfile* fptr,int iaxis,long size,float* data,int* status){

//	std::vector<float>::size_type axisSize = fAxisBins.size();
//	float* axisdata = new float[axisSize];
//	for (std::vector<float>::size_type ibin = 0; ibin < axisSize; ibin++)
//		axisdata[ibin] = fAxisBins[ibin];
//
//	// write the axis header and data
//	WriteAxis(fptr, iaxis++, int(axisSize), axisdata, status);

	return *status;
}





