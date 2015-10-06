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

#include "GIRF.h"
#include "fitsio.h"
#include "GIRFConfig.h"
#include "GIRFPdf.h"

#include <string>

using namespace std;

////////////////////////////////////////////////////////////////
// 
// Create GIRF object: either for read/write FITS files
//
GIRF::GIRF() {

}

////////////////////////////////////////////////////////////////
//
// Create GIRF object: either for read/write FITS files
//
GIRF::GIRF(string filename) {
	fFilename=filename;
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

	fitsfile* fptr; // pointer to output file
	int status = 0;   // must be initialized (0 means ok)

	// create output file
	if (fits_create_file(&fptr, fFilename.data(), &status))
		cout << "GIRF::Write Error: cannot create file (error code: " << status
				<< ")" << endl;

	// write primary HDU
	if (fits_create_img(fptr, BYTE_IMG, 0, NULL, &status))
		cout << "GIRF::Write Error: cannot write primary header (error code: "
				<< status << ")" << endl;

	// write extra keywords (so far just an example)
	char keyword[] = "ORIGIN";
	char keyval[] = "CTA Consortium";
	char comment[] = "Organization responsible";
	if (fits_write_key(fptr, TSTRING, keyword, &keyval, comment, &status))
		cout << "GIRF::Write Error: cannot write keyword (error code: "
				<< status << ")" << endl;

	// write HDUCLASS hierarchy
	sprintf(keyword, "HDUCLASS");
	sprintf(keyval, "CTA");
	sprintf(comment, "FITS file following the CTA data format.");
	if (fits_write_key(fptr, TSTRING, keyword, &keyval, comment, &status))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< status << ")" << endl;
	sprintf(keyword, "HDUCLAS1");
	sprintf(keyval, "IRM");
	sprintf(comment, "Instrument Response Model HDU.");
	if (fits_write_key(fptr, TSTRING, keyword, &keyval, comment, &status))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< status << ")" << endl;
	sprintf(keyword, "HDUCLAS2");
	sprintf(keyval, "PRIMARY");
	sprintf(comment, "PRIMARY HDU.");
	if (fits_write_key(fptr, TSTRING, keyword, &keyval, comment, &status))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< status << ")" << endl;

	// write pdf blocks and associated axes

	for (vector<GIRFPdf*>::iterator pdf = fPdfList.begin();
			pdf != fPdfList.end(); ++pdf)
		if ((*pdf)->Write(fptr, &status))
			cout << "GIRF::Write Error: cannot write pdf (error code: "
					<< status << ")" << endl;

	// close output file
	if (fits_close_file(fptr, &status))
		cout << "GIRF::Write Error: cannot close file (error code: " << status
				<< ")" << endl;

	return status;
}


GIRFPdf GIRF::GetPdf(GIRFPdf::PdfVar pdfVar, GIRFConfig config) {

	// Find all axis containing the valid range. Get IDs
	std::vector<GIRFRange::AxisRange> axisRanges = config.GetAxisRanges();

	for(std::vector<GIRFRange::AxisRange>::iterator axisRange = axisRanges.begin(); axisRange != axisRanges.end(); ++axisRange) {
		cout << "axisRange->varType = " << axisRange->varType << ", lowRange = " << axisRange->lowRange << ", highRange = " << axisRange->highRange << endl;;
	}
	vector<int> ids = GIRFUtils::FindAxisRanges(fFilename, axisRanges);



	// List all Pdfs of pdfVar type, and check if they point to selected Axis (from the IDs)

	// If found, extract Pdf values within the correct range:
	// 		- Define each axis the Pdf points to, in the correct ranges
	// 		- With the axis defined, set Pdf values for the correct ranges
	// 		- return Pdf



	GIRFPdf extractedPdf;

	return extractedPdf;

}









