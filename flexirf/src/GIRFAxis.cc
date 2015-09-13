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

using namespace std;

////////////////////////////////////////////////////////////////
// 
// Construct empty axis object
//
GIRFAxis::GIRFAxis() :
		fAxisType(kNoAxisType), fVarType(GIRFAxis::kNoVarType) {
}

////////////////////////////////////////////////////////////////
// 
// Construct empty axis object
//
GIRFAxis::GIRFAxis(VarType vartype) :
		fAxisType(kNoAxisType), fVarType(vartype) {
}

////////////////////////////////////////////////////////////////
//
// Get axis object from fitsfile
//
GIRFAxis* GIRFAxis::GetAxis(fitsfile* fptr, int axisID, GIRFAxis::AxisType axisType, int* status) {

	int currenthdu = fptr->HDUposition;

	char card[FLEN_CARD]; /* Standard string lengths defined in fitsio.h */
	int hdutype = BINARY_TBL, hdunum;
	char axisIDkeyword[20];
	sprintf(axisIDkeyword, "%d", axisID);
	GIRFAxis* IRFAxis;

	fits_get_num_hdus(fptr, &hdunum, status);
	for (int hdupos = 1; hdupos <= hdunum; hdupos++) /* Main loop through each extension */
	{
		fits_movabs_hdu(fptr, hdupos, &hdutype, status);
		if (hdutype == BINARY_TBL) {
			if (!fits_read_key_str(fptr, "HDUCLAS2", card, NULL, status)) {
				if (!strcmp(card, "AXIS")) {
					if (!fits_read_key_str(fptr, "HDUCLAS4", card, NULL, status)) {
						if (!strcmp(card, axisIDkeyword)) {
							if (axisType == GIRFAxis::kBins) IRFAxis = new GIRFAxisBins(fptr, status);
							if (axisType == GIRFAxis::kParam) IRFAxis = new GIRFAxisParam(fptr, status);
						}
					}
				}
			}
		}
		if (*status == KEY_NO_EXIST) *status = 0;
		if (*status) break;
	}

	fits_movabs_hdu(fptr, currenthdu + 1, NULL, status);
	return IRFAxis;
}

////////////////////////////////////////////////////////////////
//
// Get axis object from fitsfile
//
GIRFAxis::AxisType GIRFAxis::CheckAxisType(fitsfile* fptr, int axisID, int* status) {

	int currenthdu = fptr->HDUposition;

	GIRFAxis::AxisType axisType = GIRFAxis::kNoAxisType;
	char card[FLEN_CARD]; /* Standard string lengths defined in fitsio.h */
	int hdutype = BINARY_TBL, hdunum;
	char axisIDkeyword[20];
	sprintf(axisIDkeyword, "%d", axisID);

	fits_get_num_hdus(fptr, &hdunum, status);

	for (int hdupos = 1; hdupos <= hdunum; hdupos++) /* Main loop through each extension */
	{
		fits_movabs_hdu(fptr, hdupos, &hdutype, status);
		if (hdutype == BINARY_TBL) {
			if (!fits_read_key_str(fptr, "HDUCLAS2", card, NULL, status)) {
				if (!strcmp(card, "AXIS")) {
					if (!fits_read_key_str(fptr, "HDUCLAS4", card, NULL, status)) { 	// Now we know this is the axis we want.
						if (!strcmp(card, axisIDkeyword)) {
							if (!fits_read_key_str(fptr, "HDUCLAS3", card, NULL, status)) {
								if (!strcmp(card, "BINS")) axisType = GIRFAxis::kBins;
								else if (!strcmp(card, "PARAM")) axisType = GIRFAxis::kParam;
								else axisType = GIRFAxis::kNoAxisType;
							}
						}
					}
				}
			}
		}
		if (*status == KEY_NO_EXIST) *status = 0;
		if (*status) break;

	}
//TODO: Handle status and errors: No axis with that ID... Unknown axis type... etc...
	fits_movabs_hdu(fptr, currenthdu + 1, NULL, status);
	return axisType;
}

////////////////////////////////////////////////////////////////
// 
// Check that the vector describe consistently the axis
//
int GIRFAxis::CheckAxisConsistency() {
	int status = 0;
	if (fVarType <= kNoVarType || fVarType >= kVarMax)
		status++;

	return status;
}



////////////////////////////////////////////////////////////////
//
// Check the last Axis ID present within the fits file
//
int GIRFAxis::GetLastAxisID(string filename) {

	fitsfile *fptr; /* FITS file pointer, defined in fitsio.h */
	int status = 0;   		// must be initialized (0 means ok)
	char card[FLEN_CARD]; /* Standard string lengths defined in fitsio.h */
	int single = 0, hdupos, nkeys, ii;
	int lastID = 0;

	cout << "Opening file " << filename.data() << endl;
	if (!fits_open_file(&fptr, filename.data(), READONLY, &status)) {
		lastID = GetLastAxisID(fptr);
	}
	if (fits_close_file(fptr, &status))
		cout << "GIRF::Write Error: cannot close file (error code: " << status
				<< ")" << endl;
	return lastID;
}

int GIRFAxis::GetLastAxisID(fitsfile* fptr) {

	int currenthdu = fptr->HDUposition;

	int status = 0;   		// must be initialized (0 means ok)
	char card[FLEN_CARD]; /* Standard string lengths defined in fitsio.h */
	int single = 0, hdutype = BINARY_TBL, hdunum, nkeys, ii;
	int lastID = 0;
	fits_get_num_hdus(fptr, &hdunum, &status);

	for (int hdupos = 1; hdupos <= hdunum; hdupos++) /* Main loop through each extension */
	{
		fits_movabs_hdu(fptr, hdupos, &hdutype, &status);
		if (hdutype == BINARY_TBL) {
			if (!fits_read_key_str(fptr, "HDUCLAS3", card, NULL, &status)) {
				if (!strcmp(card, GetTypeName().data())) {
					if (!fits_read_key_str(fptr, "HDUCLAS4", card, NULL,
							&status)) {
						if (atoi(card) > lastID) {
							lastID = atoi(card);
						}
					}
				}
			}
		}
		if (status == KEY_NO_EXIST) status = 0;
		if (status) break;

	}
	fits_movabs_hdu(fptr, currenthdu + 1, NULL, &status);
	return lastID;
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
		axisVarType = "log10(TeV)";
		break;
	case kEnergy_true:
		axisVarType = "log10(TeV)";
		break;
	case kEnergy_rec:
		axisVarType = "log10(TeV)";
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
int GIRFAxis::WriteAxis(fitsfile* fptr, long size, float* data, int& lastID,
		int* status) {


	char extname[20], varname[20], form[20], unit[20];
	sprintf(extname, "%s", GetExtName().data());
	sprintf(varname, "%s", GetVarName().data());
	sprintf(form, "%s", "E");
	sprintf(unit, "%s", GetVarUnit().data());
	char *ttype[] = { varname };
	char *tform[] = { form }; // One column with float single precision (4 bytes)
	char *tunit[] = { unit };

	if (fits_create_tbl(fptr, BINARY_TBL, 0, 1, ttype, tform, tunit, extname,
			status))
		cout
				<< "GIRFAxis::Write Error: problem writing axis header (error code: "
				<< *status << ")" << endl;

	char keyword[9];
	char chval[20];
	char comment[70];
	ushort usval;

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

// Write in the first column, from first row
	if (fits_write_col(fptr, TFLOAT, 1, 1, 1, size, data, status))
		cout
				<< "GIRFAxis::WriteAxis Error: problem writing axis data (error code: "
				<< *status << ")" << endl;

// Add class keywords to the HDU.
	sprintf(keyword, "HDUCLASS");
	sprintf(chval, "CTA");
	sprintf(comment, "FITS file following the CTA data format.");
	if (fits_write_key(fptr, TSTRING, keyword, &chval, comment, status))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< *status << ")" << endl;
	sprintf(keyword, "HDUCLAS1");
	sprintf(chval, "IRM");
	sprintf(comment, "Instrument Response Model HDU.");
	if (fits_write_key(fptr, TSTRING, keyword, &chval, comment, status))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< *status << ")" << endl;
	sprintf(keyword, "HDUCLAS2");
	sprintf(chval, "AXIS");
	sprintf(comment, "Axis HDU.");
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
//	int a = CheckAxisType(fptr, 1, status);
//	if (a == 1) {
//		IRFAxisbins = (GIRFAxisBins*) GetAxis(fptr, 1, (GIRFAxis::AxisType) a, status);
//		IRFAxisbins->Print();
//	}


	//Good spot for axis tests
	sprintf(keyword, "HDUCLAS4");
	lastID = GetLastAxisID(fptr) + 1;
	usval = ushort(lastID);
	sprintf(comment, "Axis ID");
	if (fits_write_key(fptr, TUSHORT, keyword, &usval, comment, status))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< *status << ")" << endl;

	return *status;
}
