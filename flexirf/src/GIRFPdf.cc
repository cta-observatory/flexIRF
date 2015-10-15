/* ======================================================================== *\
!
 !   Author(s): Javier Rico     12/2013 <mailto:jrico@ifae.es>
 !
 !   Copyright: CTA Software Development, 2013
 !
 \* ======================================================================== */
//////////////////////////////////////////////////////////////////////////////
//
// GIRFPdf
//
// Class to represent data describing a PDF of the IRF
//
//////////////////////////////////////////////////////////////////////////////

#include "GIRFPdf.h"
#include "GIRFAxis.h"
#include "GIRFUtils.h"
#include <iostream>
#include <string.h>

using namespace std;

////////////////////////////////////////////////////////////////
// 
// Construct pdf table with type
//
GIRFPdf::GIRFPdf(PdfVar pdftype, PdfFunc pdffunc,
		std::vector<GIRFAxis*>::size_type naxes) :
		fPdfVar(pdftype), fPdfFunc(pdffunc), fData(0) {
	fAxis.reserve(naxes);
}

////////////////////////////////////////////////////////////////
//
// Return the axis name for EXTNAME
//
std::string GIRFPdf::GetFuncName() const {

	string axisType;

	switch (fPdfFunc) {
	case kNumber:
		axisType = "NUM";
		break;
	case kGaussian:
		axisType = "GAUSS";
		break;
	case k2DGaussian:
		axisType = "2DGAUSS";
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
std::string GIRFPdf::GetVarName() const {

	string axisVarType;

	switch (fPdfVar) {
	case kEfficiency:
		axisVarType = "EFFIC";
		break;
	case kEDispersion:
		axisVarType = "EDISP";
		break;
	case kPsf:
		axisVarType = "PSF";
		break;
	case kBkgRate:
		axisVarType = "BGRATE";
		break;
	case kBkgRateSqDeg:
		axisVarType = "BGRATESQDEG";
		break;
	case kDiffSens:
		axisVarType = "DIFFSENS";
		break;
	case kAeff:
		axisVarType = "AEFF";
		break;
	case kAeffNoTheta2Cut:
		axisVarType = "AEFFNOT2CUT";
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
std::string GIRFPdf::GetVarUnit() const {

	string axisVarType;

	switch (fPdfVar) {
	case kEfficiency:
		axisVarType = "";
		break;
	case kEDispersion:
		axisVarType = "log10(TeV)";
		break;
	case kPsf:
		axisVarType = "deg";
		break;
	case kBkgRate:
		axisVarType = "events*s^-1";
		break;
	case kBkgRateSqDeg:
		axisVarType = "events*s^-1*deg^-2";
		break;
	case kDiffSens:
		axisVarType = "erg*cm^-2*s^-1";
		break;
	case kAeff:
		axisVarType = "m^2";
		break;
	case kAeffNoTheta2Cut:
		axisVarType = "m^2";
		break;
	default:
		cout << "Incorrect variable type.\n";
		return axisVarType;
	}

	return axisVarType;
}



////////////////////////////////////////////////////////////////
//
// Write the pdf and the associated axes to the specified
// file pointer
//
int GIRFPdf::Write(fitsfile* fptr, int* status) {
	// create arrays with size and first entry of every dimension to be saved (1 is first, not 0)
	int naxis = int(fAxis.size());
	long* naxes = new long[naxis];
	long* fpixel = new long[naxis];
	for (std::vector<GIRFAxis*>::size_type jaxis = 0; jaxis < naxis; jaxis++) {
		naxes[jaxis] = int(fAxis[jaxis]->GetSize());
		fpixel[jaxis] = 1;
	}
	vector<int> axisIDs;
	axisIDs.reserve(naxis);
	int axisID;
	// write associated axis blocks
	int firstaxis = 1;
	for (vector<GIRFAxis*>::iterator axis = fAxis.begin(); axis != fAxis.end();
			++axis){
		if ((*axis)->Write(fptr, axisID, status))
					cout << "GIRFPdf::Write Error: cannot write axis (error code: "
							<< *status << ")" << endl;
		else axisIDs.push_back(axisID);
	}

	// write the pdf header
	if (fits_create_img(fptr, FLOAT_IMG, naxis, naxes, status))
		cout
				<< "GIRFPdf::Write Error: problem writing axis header (error code: "
				<< *status << ")" << endl;

	// write keywords to the header
	char keyword[9];
	char chval[20];
	char comment[70];
	ushort usval;


	// write the indeces linking to the relevant axes
	for (int jaxis = 0; jaxis < naxis; jaxis++) {
		sprintf(keyword, "AXISID%d", jaxis + 1);
		usval = ushort(axisIDs[jaxis]);
		sprintf(comment, "Axis describing dimension #%d", jaxis + 1);
		if (fits_write_key(fptr, TUSHORT, keyword, &usval, comment, status))
			cout << "GIRFPdf::Write Error: cannot write keyword (error code: "
					<< *status << ")" << endl;
		sprintf(keyword, "CTYPE%d", jaxis + 1);
		sprintf(chval, "%s", GetVarName().data());
		sprintf(comment, "Axis %d units", jaxis + 1);
			if (fits_write_key(fptr, TSTRING, keyword, &chval, comment, status))
				cout << "GIRFPdf::Write Error: cannot write keyword (error code: "
						<< *status << ")" << endl;
	}

	// TODO: Check if it already exists!
	// write pdf name
	sprintf(keyword, "EXTNAME");
	sprintf(chval, "%s", GetExtName().data());
	sprintf(comment, "Pdf Data HDU");
	if (fits_write_key(fptr, TSTRING, keyword, &chval, comment, status))
		cout << "GIRFPdf::Write Error: cannot write keyword (error code: "
				<< *status << ")" << endl;

	// write pdf units name
	sprintf(keyword, "BUNIT");
	sprintf(chval, "%s", GetVarUnit().data());
	sprintf(comment, "Pdf units");
		if (fits_write_key(fptr, TSTRING, keyword, &chval, comment, status))
			cout << "GIRFPdf::Write Error: cannot write keyword (error code: "
					<< *status << ")" << endl;

	// write pdf var
	sprintf(keyword, "PDFVAR");
	usval = ushort(fPdfVar);
	sprintf(comment,
			"Variable whose pdf is parameterized (see GIRFPdf.h for details)");
	if (fits_write_key(fptr, TUSHORT, keyword, &usval, comment, status))
		cout << "GIRFPdf::Write Error: cannot write keyword (error code: "
				<< *status << ")" << endl;

	// write pdf function
	sprintf(keyword, "PDFFunc");
	usval = ushort(fPdfFunc);
	sprintf(comment, "Function describing pdf (see GIRFPdf.h for details)");
	if (fits_write_key(fptr, TUSHORT, keyword, &usval, comment, status))
		cout << "GIRFPdf::Write Error: cannot write keyword (error code: "
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
	sprintf(chval, "DATA");
	sprintf(comment, "Data HDU.");
	if (fits_write_key(fptr, TSTRING, keyword, &chval, comment, status))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< *status << ")" << endl;
	sprintf(keyword, "HDUCLAS3");
	sprintf(chval, "%s", GetVarName().data());
	sprintf(comment, "Variable whose pdf is parameterized (see GIRFPdf.h for details)");
	if (fits_write_key(fptr, TSTRING, keyword, &chval, comment, status))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< *status << ")" << endl;
	//Get the last Pdf ID of the same class
	int pdfID = GIRFUtils::GetLastPdfID(fptr)+1;

	sprintf(keyword, "HDUCLAS4");
	usval = ushort(pdfID);
	sprintf(comment, "Pdf ID");
	if (fits_write_key(fptr, TUSHORT, keyword, &usval, comment, status))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< *status << ")" << endl;

	// write the pdf data
	long nentries = GetSize();
	if (fits_write_pix(fptr, TFLOAT, fpixel, nentries, fData, status))
		cout << "GIRFPdf::Write Error: problem writing axis data (error code: "
				<< *status << ")" << endl;

	return *status;
}



////////////////////////////////////////////////////////////////
//
// Write the pdf and the associated axes to the specified
// file pointer
//
void GIRFPdf::Print() const {

	int iAxis=0;
	cout << "Printing Axes:" << endl;
	for(std::vector<GIRFAxis*>::const_iterator axis = fAxis.begin(); axis != fAxis.end(); ++axis, iAxis++) {
		cout << "Printing Axis #" << iAxis+1 << endl;
		(*axis)->Print();
	}
	cout << "Printing Pdf content:" << endl;
	int iData=0;
	for(std::vector<GIRFAxis*>::const_iterator axis = fAxis.begin(); axis != fAxis.end(); ++axis, iData++) {
		cout << "Printing Axis #1" << iAxis+1 << endl;
		for (int i=0;i<(*axis)->GetSize();i++){
			cout << fData[i] << endl;
		}
	}

}


