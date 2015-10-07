/* ======================================================================== *\
!
 !   Author(s): Javier Rico     12/2013 <mailto:jrico@ifae.es>
 !
 !   Copyright: CTA Software Development, 2013
 !
 \* ======================================================================== */

//////////////////////////////////////////////////////////////////////////////
//
// GIRF
//
// Class to hold, read/write CTA IRF 
//
//////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "GIRF.h"
#include "fitsio.h"


#include <string.h>

using namespace std;

////////////////////////////////////////////////////////////////
// 
// Create GIRF object: either for read/write FITS files
//
GIRF::GIRF() : fStatus(0), fFitsPtr(NULL) {

}

////////////////////////////////////////////////////////////////
//
// Create GIRF object: either for read/write FITS files
//
GIRF::GIRF(string filename) : fStatus(0) {
	fFilename=filename;

	// create output file
	if (fits_create_file(&fFitsPtr, fFilename.data(), &fStatus)) CheckStatus();
}





////////////////////////////////////////////////////////////////
//
// 		Check GIRF status
//
//  TODO: Handle errors, check status, show warnings...
//
int GIRF::CheckStatus(){
	if (fStatus) fits_report_error(stderr, fStatus);
	return fStatus;
}


////////////////////////////////////////////////////////////////
//
// Get axis type with specific ID
//
GIRFAxis::AxisType GIRF::CheckAxisType(int axisID) {

	int currenthdu = fFitsPtr->HDUposition;

	GIRFAxis::AxisType axisType = GIRFAxis::kNoAxisType;
	char card[FLEN_CARD]; /* Standard string lengths defined in fitsio.h */
	int hdutype = BINARY_TBL, hdunum;
	char axisIDkeyword[20];
	sprintf(axisIDkeyword, "%d", axisID);

	fits_get_num_hdus(fFitsPtr, &hdunum, &fStatus);

	for (int hdupos = 1; hdupos <= hdunum; hdupos++) /* Main loop through each extension */
	{
		fits_movabs_hdu(fFitsPtr, hdupos, &hdutype, &fStatus);
		if (hdutype == BINARY_TBL) {
			if (!fits_read_key_str(fFitsPtr, "HDUCLAS2", card, NULL, &fStatus)) {
				if (!strcmp(card, "AXIS")) {
					if (!fits_read_key_str(fFitsPtr, "HDUCLAS4", card, NULL, &fStatus)) { 	// Now we know this is the axis we want.
						if (!strcmp(card, axisIDkeyword)) {
							if (!fits_read_key_str(fFitsPtr, "HDUCLAS3", card, NULL, &fStatus)) {
								if (!strcmp(card, "BINS")) axisType = GIRFAxis::kBins;
								else if (!strcmp(card, "PARAM")) axisType = GIRFAxis::kParam;
								else axisType = GIRFAxis::kNoAxisType;
							}
						}
					}
				}
			}
		}
		if (fStatus == KEY_NO_EXIST) fStatus = 0;
		if (fStatus) break;

	}
//TODO: Handle status and errors: No axis with that ID... Unknown axis type... etc...
	fits_movabs_hdu(fFitsPtr, currenthdu + 1, NULL, &fStatus);
	return axisType;
}

////////////////////////////////////////////////////////////////
//
// Write IRF into file, return status (0 if ok)
// (use filename="-" for stdout, or
// start it with "!" to force overwriting)
//
int GIRF::Write() {

	if (fFilename.empty()){
		cout << "ERROR: No filename specified." << endl;
		return 1;
	}

	// write primary HDU
	if (fits_create_img(fFitsPtr, BYTE_IMG, 0, NULL, &fStatus))
		cout << "GIRF::Write Error: cannot write primary header (error code: "
				<< fStatus << ")" << endl;

	// write extra keywords (so far just an example)
	char keyword[] = "ORIGIN";
	char keyval[] = "CTA Consortium";
	char comment[] = "Organization responsible";
	if (fits_write_key(fFitsPtr, TSTRING, keyword, &keyval, comment, &fStatus))
		cout << "GIRF::Write Error: cannot write keyword (error code: "
				<< fStatus << ")" << endl;

	// write HDUCLASS hierarchy
	sprintf(keyword, "HDUCLASS");
	sprintf(keyval, "CTA");
	sprintf(comment, "FITS file following the CTA data format.");
	if (fits_write_key(fFitsPtr, TSTRING, keyword, &keyval, comment, &fStatus))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< fStatus << ")" << endl;
	sprintf(keyword, "HDUCLAS1");
	sprintf(keyval, "IRM");
	sprintf(comment, "Instrument Response Model HDU.");
	if (fits_write_key(fFitsPtr, TSTRING, keyword, &keyval, comment, &fStatus))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< fStatus << ")" << endl;
	sprintf(keyword, "HDUCLAS2");
	sprintf(keyval, "PRIMARY");
	sprintf(comment, "PRIMARY HDU.");
	if (fits_write_key(fFitsPtr, TSTRING, keyword, &keyval, comment, &fStatus))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< fStatus << ")" << endl;

	// write pdf blocks and associated axes

	for (vector<GIRFPdf*>::iterator pdf = fPdfList.begin();
			pdf != fPdfList.end(); ++pdf)
		if ((*pdf)->Write(fFitsPtr, &fStatus))
			cout << "GIRF::Write Error: cannot write pdf (error code: "
					<< fStatus << ")" << endl;

	// close output file
	if (fits_close_file(fFitsPtr, &fStatus))
		cout << "GIRF::Write Error: cannot close file (error code: " << fStatus
				<< ")" << endl;

	return fStatus;
}

//GIRFAxis GIRF::GetAxis(fitsfile *fptr, int axisID, int* status){
//
//	GIRFAxis::AxisType axisType = GIRFUtils::CheckAxisType(fptr, axisID,status);
//
//
//	switch (axisType){
//	case GIRFAxis::kBins:
//		GIRFAxisBins axis;
//		axis.Read(fitsfile *fptr, int axisID, int* status);
//		break;
//	case GIRFAxis::kParam:
//
//		break;
//	default:
//	    cout << "Invalid axis type\n";
//	    return NULL;
//	    break;
//	}
//}


////////////////////////////////////////////////////////////////
//
// Get axis object from fitsfile
//
GIRFAxis GIRF::GetAxis(int axisID) {

	int currenthdu = fFitsPtr->HDUposition;

	GIRFAxis::AxisType axisType = CheckAxisType(axisID);
	int hduPos = CheckAxisHDUpos(axisID);
	//TODO: Handle status!!

	char card[FLEN_CARD]; /* Standard string lengths defined in fitsio.h */
	int hdutype = BINARY_TBL, hdunum;
	char axisIDkeyword[20];
	sprintf(axisIDkeyword, "%d", axisID);
	GIRFAxis IRFAxis;

	fits_movabs_hdu(fFitsPtr, hduPos, NULL, &fStatus);

	switch (axisType){

	case GIRFAxis::kBins:{
		GIRFAxisBins axisBins(fFitsPtr, &fStatus);
		CheckStatus();
		fits_movabs_hdu(fFitsPtr, currenthdu + 1, NULL, &fStatus);
		return axisBins;
	}
	case GIRFAxis::kParam:{
		GIRFAxisParam axisParam(fFitsPtr, &fStatus);
		CheckStatus();
		fits_movabs_hdu(fFitsPtr, currenthdu + 1, NULL, &fStatus);
		return axisParam;
	}
	default:

		cout << "Invalid axis type\n";
		CheckStatus();
		fits_movabs_hdu(fFitsPtr, currenthdu + 1, NULL, &fStatus);
		return IRFAxis;
		break;
	}

}




GIRFPdf GIRF::GetPdf(GIRFPdf::PdfVar pdfVar, GIRFConfig config) {

	// Find all axis containing the valid range. Get IDs
	std::vector<GIRFRange::AxisRange> axisRanges = config.GetAxisRanges();

	for(std::vector<GIRFRange::AxisRange>::iterator axisRange = axisRanges.begin(); axisRange != axisRanges.end(); ++axisRange) {
		cout << "axisRange->varType = " << axisRange->varType << ", lowRange = " << axisRange->lowRange << ", highRange = " << axisRange->highRange << endl;
	}
	vector<int> ids = GIRFUtils::FindAxisRanges(fFilename, axisRanges);


	for(std::vector<int>::iterator foundAxisID = ids.begin(); foundAxisID != ids.end(); ++foundAxisID) {
		cout << "foundAxisID = " << *foundAxisID << endl;
	}
	// List all Pdfs of pdfVar type, and check if they point to selected Axis (from the IDs)

	// If found, extract Pdf values within the correct range:
	// 		- Define each axis the Pdf points to, in the correct ranges
	// 		- With the axis defined, set Pdf values for the correct ranges
	// 		- return Pdf



	GIRFPdf extractedPdf;

	return extractedPdf;

}


////////////////////////////////////////////////////////////////
//
// Get axis type from fitsfile
//
int GIRF::CheckAxisHDUpos(int axisID) {

	int currenthdu = fFitsPtr->HDUposition;

	GIRFAxis::AxisType axisType = GIRFAxis::kNoAxisType;
	char card[FLEN_CARD]; /* Standard string lengths defined in fitsio.h */
	int hdutype = BINARY_TBL, hdunum;
	char axisIDkeyword[20];
	sprintf(axisIDkeyword, "%d", axisID);

	fits_get_num_hdus(fFitsPtr, &hdunum, &fStatus);

	for (int hdupos = 1; hdupos <= hdunum; hdupos++) /* Main loop through each extension */
	{
		fits_movabs_hdu(fFitsPtr, hdupos, &hdutype, &fStatus);
		if (hdutype == BINARY_TBL) {
			if (!fits_read_key_str(fFitsPtr, "HDUCLAS2", card, NULL, &fStatus)) {
				if (!strcmp(card, "AXIS")) {
					if (!fits_read_key_str(fFitsPtr, "HDUCLAS4", card, NULL, &fStatus)) { 	// Now we know this is the axis we want.
						if (!strcmp(card, axisIDkeyword)) {
							if (!fits_read_key_str(fFitsPtr, "HDUCLAS3", card, NULL, &fStatus)) {
								fits_movabs_hdu(fFitsPtr, currenthdu + 1, NULL, &fStatus);
								return hdupos;
							}
						}
					}
				}
			}
		}
		if (fStatus == KEY_NO_EXIST) fStatus = 0;
		if (fStatus) {
			CheckStatus();
			break;
		}

	}
	cout << "WARNING: No axis found with ID = " << axisID << endl;
//TODO: Handle status and errors: No axis with that ID... Unknown axis type... etc...
	fits_movabs_hdu(fFitsPtr, currenthdu + 1, NULL, &fStatus);
	return axisType;
}











