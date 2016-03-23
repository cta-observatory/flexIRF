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
flexIRF::GIRF::GIRF() : fStatus(0), fFitsPtr(NULL), fFITSopened(0) {

}

////////////////////////////////////////////////////////////////
//
// Create GIRF object: either for read/write FITS files
//
flexIRF::GIRF::GIRF(string filename) : fStatus(0) {
	fFilename=filename;

	//First try to open existing fits file.
	if (OpenFITS()){
		fFITSopened=1;
		CheckStatus();
	}
	else {
		cout << "ERROR: FITS file " << fFilename.data() << " not found.";
	}
}

////////////////////////////////////////////////////////////////
//
// 		Try to open an existing FITS file
//
//  	TODO: Handle errors, check status, show warnings...
//
int flexIRF::GIRF::OpenFITS(){
	if (!fits_open_file(&fFitsPtr, fFilename.data(), READWRITE, &fStatus)) return 1;
	else return 0;
}

////////////////////////////////////////////////////////////////
//
// 		Creates (and overwrites!) a FITS file
//
//  	TODO: Handle errors, check status, show warnings...
//
int flexIRF::GIRF::CreateFITS(){
	if (!fits_create_file(&fFitsPtr, fFilename.data(), &fStatus)) return 1;
	else return 0;
}



////////////////////////////////////////////////////////////////
//
// 		Check GIRF status
//
//  TODO: Handle errors, check status, show warnings...
//
int flexIRF::GIRF::CheckStatus(){
	if (fStatus) fits_report_error(stderr, fStatus);
	return fStatus;
}


////////////////////////////////////////////////////////////////
//
// Get axis type with specific ID
//
flexIRF::AxisType flexIRF::GIRF::CheckAxisType(int axisID) {

	int currenthdu = fFitsPtr->HDUposition;

	AxisType axisType = kNoAxisType;
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
								if (!strcmp(card, "BINS")) axisType = kBins;
								else if (!strcmp(card, "PARAM")) axisType = kParam;
								else axisType = kNoAxisType;
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
// Get axis variable with specific ID
//
flexIRF::VarType flexIRF::GIRF::CheckAxisVarType(int axisID) {

	int currenthdu = fFitsPtr->HDUposition;

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
					if (!fits_read_key_str(fFitsPtr, "HDUCLAS4", card, NULL, &fStatus)) {
						if (!strcmp(card, axisIDkeyword)) {
							// Now we know this is the axis we want.
							if (!fits_read_key_str(fFitsPtr, "VARTYPE", card, NULL, &fStatus)) {
								return static_cast<VarType>(atoi(card));
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
	return kNoVarType;
}




////////////////////////////////////////////////////////////////
//
// Write IRF into file, return status (0 if ok)
// (use filename="-" for stdout, or
// start it with "!" to force overwriting)
//
int flexIRF::GIRF::Write() {

	if (fFilename.empty()){
		cout << "ERROR: No filename specified." << endl;
		return 1;
	} else if (fSerialization.empty()) {
		cout << "WARNING: No serialization specified. Using default IMAGE serialization" << endl;
		fSerialization="IMAGE";
	}


	if (!fFITSopened) if (!CreateFITS()) cout << "ERROR: Could not create FITS file." << endl;

	// write primary HDU
	if (Write_PHDU()) {
		cout << "ERROR: Impossible to write primary HDU." << endl;
		return 1;
	}

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


////////////////////////////////////////////////////////////////
//
// 			Write primary HDU
//
int flexIRF::GIRF::Write_PHDU() {


	if (fits_create_img(fFitsPtr, BYTE_IMG, 0, NULL, &fStatus))
		cout << "GIRF::Write Error: cannot write primary header (error code: "
				<< fStatus << ")" << endl;


	// write extra keywords (so far just an example)
	char keyword[FLEN_KEYWORD] = "ORIGIN";
	char keyval[FLEN_VALUE] = "CTA Consortium";
	char comment[FLEN_COMMENT] = "Organization responsible";
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
	if (fSerialization=="IMAGE") sprintf(keyval, "IMAGE");
	else if (fSerialization=="BINTABLE") sprintf(keyval, "BINTABLE");
	sprintf(comment, "Serialization used to store IRF");
	if (fits_write_key(fFitsPtr, TSTRING, keyword, &keyval, comment, &fStatus))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< fStatus << ")" << endl;

	sprintf(keyword, "HDUCLAS3");
	sprintf(keyval, "PRIMARY");
	sprintf(comment, "PRIMARY HDU.");
	if (fits_write_key(fFitsPtr, TSTRING, keyword, &keyval, comment, &fStatus))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
				<< fStatus << ")" << endl;

	return 0;
}


////////////////////////////////////////////////////////////////
//
// 			Write PDF.
//
int flexIRF::GIRF::Write_PDF() {

	for (vector<GIRFPdf*>::iterator pdf = fPdfList.begin();
			pdf != fPdfList.end(); ++pdf){
		if ((*pdf)->Write(fFitsPtr, fSerialization,  &fStatus)){
			cout << "GIRF::Write Error: cannot write pdf (error code: "
					<< fStatus << ")" << endl;
			return fStatus;
		}
	}
	return 0;
}



////////////////////////////////////////////////////////////////
//
// Get axis type from fitsfile
//
int flexIRF::GIRF::CheckAxisHDUpos(int axisID) {

	int currenthdu = fFitsPtr->HDUposition;

	AxisType axisType = kNoAxisType;
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



////////////////////////////////////////////////////////////////
//
// Get axis object from fitsfile
//
flexIRF::GIRFAxis* flexIRF::GIRF::ReadAxis(int axisID) {

	int currenthdu = fFitsPtr->HDUposition;

	AxisType axisType = static_cast<AxisType>(CheckAxisType(axisID));

	int hduPos = CheckAxisHDUpos(axisID);
	//TODO: Handle status!!

	char card[FLEN_CARD]; /* Standard string lengths defined in fitsio.h */
	int hdutype = BINARY_TBL, hdunum;
	char axisIDkeyword[20];
	sprintf(axisIDkeyword, "%d", axisID);

	fits_movabs_hdu(fFitsPtr, hduPos, NULL, &fStatus);

	switch (axisType){

	case kBins:{
		GIRFAxisBins *axisBins = new GIRFAxisBins(fFitsPtr, &fStatus);
		CheckStatus();
		fits_movabs_hdu(fFitsPtr, currenthdu + 1, NULL, &fStatus);
		return axisBins;
	}
	case kParam:{
		GIRFAxisParam *axisParam = new GIRFAxisParam(fFitsPtr, &fStatus);
		CheckStatus();
		fits_movabs_hdu(fFitsPtr, currenthdu + 1, NULL, &fStatus);
		return axisParam;
	}
	default:{
		cout << "Invalid axis type\n";
		CheckStatus();
		fits_movabs_hdu(fFitsPtr, currenthdu + 1, NULL, &fStatus);
		return NULL;
		break;
	}
	}

}

////////////////////////////////////////////////////////////////
//
// Get axis object from fitsfile with the range defined
// within AxisRanges
//
flexIRF::GIRFAxis* flexIRF::GIRF::ReadAxis(int axisID, vector<AxisRange> axisRanges) {

	int currenthdu = fFitsPtr->HDUposition;

	AxisType axisType = CheckAxisType(axisID);
	VarType varType = CheckAxisVarType(axisID);

	int hduPos = CheckAxisHDUpos(axisID);
	//TODO: Handle status!!

	char card[FLEN_CARD]; /* Standard string lengths defined in fitsio.h */
	int hdutype = BINARY_TBL, hdunum;
	char axisIDkeyword[20];
	sprintf(axisIDkeyword, "%d", axisID);

	fits_movabs_hdu(fFitsPtr, hduPos, NULL, &fStatus);

	switch (axisType){

	case kBins:{
		GIRFAxisBins *axisBins = new GIRFAxisBins(fFitsPtr, &fStatus);
		for(std::vector<AxisRange>::iterator axisRange = axisRanges.begin(); axisRange != axisRanges.end(); ++axisRange) {
			if (axisRange->varType == varType) axisBins->Resize(axisRange->lowRange, axisRange->highRange);
		}
		CheckStatus();
		fits_movabs_hdu(fFitsPtr, currenthdu + 1, NULL, &fStatus);
		return dynamic_cast<GIRFAxis*>(axisBins);
	}
	case kParam:{
		GIRFAxisParam *axisParam = new GIRFAxisParam(fFitsPtr, &fStatus);
		CheckStatus();
		fits_movabs_hdu(fFitsPtr, currenthdu + 1, NULL, &fStatus);
		return dynamic_cast<GIRFAxis*>(axisParam);
	}
	default:{
		cout << "Invalid axis type\n";
		CheckStatus();
		fits_movabs_hdu(fFitsPtr, currenthdu + 1, NULL, &fStatus);
		return NULL;
		break;
	}
	}

}



////////////////////////////////////////////////////////////////
//
// 		Search and extract GIRFPdf object from fitsfile
//		with specific  PdfVar and GIRFConfig.
//
flexIRF::GIRFPdf* flexIRF::GIRF::ReadPdf(PdfVar pdfVar, GIRFConfig config) {

	GIRFPdf* extractedPdf = new GIRFPdf();

	//**************************************************************//
	// 		Find all axis containing the valid range.
	//**************************************************************//

	std::vector<AxisRange> axisRanges = config.GetAxisRanges();

	vector< vector<int> > ids = FindAxisRanges(axisRanges);

	if (ids.empty()) {
		cout << "Required Pdf dependencies are not present within the FITS file" << endl;
		return extractedPdf;
	}

//	cout << "ids:" << endl;
//	for(std::vector< vector<int> >::iterator idrow = ids.begin(); idrow != ids.end(); ++idrow) {
//		for(std::vector<int>::iterator id = idrow->begin(); id != idrow->end(); ++id) {
//			cout << " " << *id << " ";
//		}
//		cout << endl;
//	}


	//**************************************************************//
	// 		Find all pdfs containing the valid axis IDs.
	//**************************************************************//

	vector<int> foundPdfs = FindPdfs(ids, pdfVar);

	// Several IRFs may contain the valid axis IDs!!
	//**************************************************************//
	// 		Several IRFs may contain the valid axis IDs
	// 		Choose the preferable one. TODO!!
	//**************************************************************//
	int chosenPdfID = PickPreferredPdf(foundPdfs, config);
	if (chosenPdfID == 0) {
		cout << "Required Pdf dependencies are not present within the FITS file." << endl;
		return extractedPdf;
	}
	//**************************************************************//
	// 		Extract Pdf ID.
	//**************************************************************//

//	cout << "Pdf #" << chosenPdfID << " was chosen." << endl;
	extractedPdf = ReadPdf(chosenPdfID, config);

//	for(std::vector<int>::iterator foundAxisID = ids.begin(); foundAxisID != ids.end(); ++foundAxisID) {
//		cout << "foundAxisID = " << *foundAxisID << endl;
//	}
	// List all Pdfs of pdfVar type, and check if they point to selected Axis (from the IDs)

	// If found, extract Pdf values within the correct range:
	// 		- Define each axis the Pdf points to, in the correct ranges
	// 		- With the axis defined, set Pdf values for the correct ranges
	// 		- return Pdf


	if (extractedPdf->IsEmpty()) {
		cout << "Extracted pdf is empty!" << endl;
		return NULL;
	}
	else return extractedPdf;

}

////////////////////////////////////////////////////////////////
//
// 		Read GIRFPdf object from fitsfile with a specific
//		pdfID
//
flexIRF::GIRFPdf* flexIRF::GIRF::ReadPdf(int pdfID, GIRFConfig config) {

	std::vector<AxisRange> axisRanges = config.GetAxisRanges();

	vector<int> pdfAxes = GetPdfAxisIDs(pdfID);
	PdfVar pdfVar = ReadPdfVar(pdfID);
	PdfFunc pdfFunc = ReadPdfFunc(pdfID);


	GIRFPdf* pdfOut = new GIRFPdf(pdfVar, pdfFunc);

	for(std::vector<int>::iterator axisID = pdfAxes.begin(); axisID != pdfAxes.end(); ++axisID) {
		pdfOut->AddAxis(ReadAxis(*axisID, axisRanges));
	}

	pdfOut->SetData(ReadPdfData(pdfID, pdfAxes, config.GetAxisRanges()));


	return pdfOut;
}



////////////////////////////////////////////////////////////////
//
// 		Read GIRFPdf::PdfVar object from fitsfile with a
//		specific pdfID
//
flexIRF::PdfVar flexIRF::GIRF::ReadPdfVar(int pdfID) {

	int currenthdu = fFitsPtr->HDUposition;

	int status = 0;   		// must be initialized (0 means ok)
	char card[FLEN_CARD]; /* Standard string lengths defined in fitsio.h */
	int numaxes = 0, hdutype, hdunum;
	fits_get_num_hdus(fFitsPtr, &hdunum, &status);
	char axisIDkeyword[20];

	for (int hdupos = 1; hdupos <= hdunum; hdupos++) /* Main loop through each extension */
	{
		fits_movabs_hdu(fFitsPtr, hdupos, &hdutype, &status);
		if (hdutype == IMAGE_HDU) {
			if (!fits_read_key_str(fFitsPtr, "HDUCLAS2", card, NULL, &status)) {
				if (!strcmp(card, "DATA")) {
					if (!fits_read_key_str(fFitsPtr, "HDUCLAS4", card, NULL, &status)) {
						if (atoi(card) == pdfID){
							// Current HDU is the pdf we want.
							if (!fits_read_key_str(fFitsPtr, "PDFVAR", card, NULL, &status)) {
								PdfVar pdfVar = static_cast<PdfVar>(atoi(card));
								return pdfVar;
							}
						}
					}
				}
			}
		}
		if (status == KEY_NO_EXIST) status = 0;
		if (status) break;

	}
	fits_movabs_hdu(fFitsPtr, currenthdu + 1, NULL, &status);

	cout << "ERROR: Pdf with no PdfVar!" << endl;
	return static_cast<PdfVar>(0);

}

////////////////////////////////////////////////////////////////
//
// 		Read GIRFPdf::PdfFunc object from fitsfile with a
//		specific pdfID
//
flexIRF::PdfFunc flexIRF::GIRF::ReadPdfFunc(int pdfID) {

	int currenthdu = fFitsPtr->HDUposition;

	int status = 0;   		// must be initialized (0 means ok)
	char card[FLEN_CARD]; /* Standard string lengths defined in fitsio.h */
	int numaxes = 0, hdutype, hdunum;
	fits_get_num_hdus(fFitsPtr, &hdunum, &status);
	char axisIDkeyword[20];

	for (int hdupos = 1; hdupos <= hdunum; hdupos++) /* Main loop through each extension */
	{
		fits_movabs_hdu(fFitsPtr, hdupos, &hdutype, &status);
		if (hdutype == IMAGE_HDU) {
			if (!fits_read_key_str(fFitsPtr, "HDUCLAS2", card, NULL, &status)) {
				if (!strcmp(card, "DATA")) {
					if (!fits_read_key_str(fFitsPtr, "HDUCLAS4", card, NULL, &status)) {
						if (atoi(card) == pdfID){
							// Current HDU is the pdf we want.
							if (!fits_read_key_str(fFitsPtr, "PDFFUNC", card, NULL, &status)) {
								PdfFunc pdfVar = static_cast<PdfFunc>(atoi(card));
								return pdfVar;
							}
						}
					}
				}
			}
		}
		if (status == KEY_NO_EXIST) status = 0;
		if (status) break;

	}
	fits_movabs_hdu(fFitsPtr, currenthdu + 1, NULL, &status);

	cout << "ERROR: Pdf with no PdfFunc!" << endl;
	return static_cast<PdfFunc>(0);

}

////////////////////////////////////////////////////////////////
//
// 		Read data from fitsfile from a Data HDU with a
//		specific pdfID
//
float*  flexIRF::GIRF::ReadPdfData(int pdfID, vector<int> pdfAxes, vector<AxisRange> axisRanges){

	vector<long> lBins, hBins, inc;
	int lBin, hBin, anynull, axisSize;
	long nBins=0;

	int iAxis=0;
	float *array, nulval = 0.;
	AxisType axisType;
	VarType varType;

	for(std::vector<int>::iterator axisID = pdfAxes.begin(); axisID != pdfAxes.end(); ++axisID, iAxis++) {
		GIRFAxis *axis;
		lBin=0; hBin = 0;
		axisType = CheckAxisType(*axisID);
		varType = CheckAxisVarType(*axisID);
		switch (axisType){
			case kBins:{
				axis = dynamic_cast<GIRFAxisBins*>(ReadAxis(*axisID));
				axisSize=axis->GetSize()-1;
				break;
			}
			case kParam:{
				axis = dynamic_cast<GIRFAxisParam*>(ReadAxis(*axisID));
				axisSize=axis->GetSize();
				break;
			}
		}
		for(std::vector<AxisRange>::iterator axisRange = axisRanges.begin(); axisRange != axisRanges.end(); ++axisRange) {
			if (varType == axisRange->varType){
//				cout << "Axis #" << *axisID << " is resized using lowRange = " << axisRange->lowRange << " & highRange = " << axisRange->highRange << endl;
				axis->Resize(axisRange->lowRange, axisRange->highRange, &lBin, &hBin);
//				cout << "lBin = " << lBin << " and hBin = " << hBin << endl;
				break;
			}
		}
		if (lBin==0 && hBin == 0){
			lBins.push_back(1);
			hBins.push_back(axisSize);
		} else {
			lBins.push_back(lBin+1);	//TODO: CHECK!!!!!!!!! Donde va el +1!!?!??!?
			hBins.push_back(hBin-1);
		}
//		cout << "Axis #" << *axisID << " with axis size = " << axis->GetSize() << endl;
		if (nBins == 0) nBins=axisSize;
		else nBins*=axisSize;
		inc.push_back(1);
	}

//	cout << "lBins = [ ";
//	for(std::vector<long>::iterator lbin = lBins.begin(); lbin != lBins.end(); ++lbin) {
//		cout << *lbin << " ";
//	}
//	cout << "]" << endl;

//	cout << "hBins = [ ";
//	for(std::vector<long>::iterator hbin = hBins.begin(); hbin != hBins.end(); ++hbin) {
//		cout << *hbin << " ";
//	}
//	cout << "]" << endl;

//	cout << "nBins = " << nBins << endl;

	array = (float *) malloc(nBins * sizeof(float));

	GIRFUtils::GoToPdfHDU(fFitsPtr, pdfID);

	if (fits_read_subset(fFitsPtr, TFLOAT, lBins.data(), hBins.data(), inc.data(), &nulval, array, &anynull, &fStatus)){
		CheckStatus();
		return NULL;
	}

	return array;

}







////////////////////////////////////////////////////////////////
//
// 		Return all axis IDs matching AxisRange.
//
vector<int> flexIRF::GIRF::FindAxisRange(AxisRange axisRange){
	vector<int> foundAxisID;

	int currenthdu = fFitsPtr->HDUposition;				//TODO: do we need to know the current position? I leave it just to make sure...

	int status = 0;   		// must be initialized (0 means ok)
	char card[FLEN_CARD]; /* Standard string lengths defined in fitsio.h */
	int single = 0, hdutype, hdunum;
	fits_get_num_hdus(fFitsPtr, &hdunum, &status);

	AxisType axisType;
	for (int hdupos = 1; hdupos <= hdunum; hdupos++) /* Main loop through each extension */
	{
		fits_movabs_hdu(fFitsPtr, hdupos, &hdutype, &status);
		if (hdutype == BINARY_TBL) {
			if (!fits_read_key_str(fFitsPtr, "HDUCLAS2", card, NULL, &status)) {
				if (!strcmp(card, "AXIS")) {
					if (!fits_read_key_str(fFitsPtr, "VARTYPE", card, NULL, &status)) {
						if (atoi(card) == axisRange.varType) {
							if (!fits_read_key_str(fFitsPtr, "AXISTYPE", card, NULL, &status)) {
								axisType=static_cast<AxisType>(atoi(card));
								if (!fits_read_key_str(fFitsPtr, "HDUCLAS4", card, NULL, &status)) {
									switch(axisType){
										case kBins:{
											GIRFAxisBins *axis = dynamic_cast<GIRFAxisBins*>(ReadAxis(atoi(card)));
											if (axis && axis->ContainsRange(axisRange)) foundAxisID.push_back(atoi(card));
											break;
										}
										case kParam:{
											GIRFAxisParam *axis = dynamic_cast<GIRFAxisParam*>(ReadAxis(atoi(card)));
											if (axis && axis->ContainsRange(axisRange)) foundAxisID.push_back(atoi(card));
											break;
										}
										default:{
											cout << "ERROR: Invalid axis type\n";
											CheckStatus();
											fits_movabs_hdu(fFitsPtr, currenthdu + 1, NULL, &fStatus);
											break;
										}
									}

								}
							}
						}
					}
				}
			}
		}
		if (status == KEY_NO_EXIST) status = 0;
		if (status) break;

	}

	if (foundAxisID.empty()) cout << "WARNING: Axis of type " <<  GIRFAxis::GetVarName(axisRange.varType).data() << " between " << axisRange.lowRange << " and " << axisRange.highRange << " is not present in the FITS file." << endl;

	fits_movabs_hdu(fFitsPtr, currenthdu + 1, NULL, &status);

	return foundAxisID;
}

////////////////////////////////////////////////////////////////
//
// 		Return all axis IDs matching AxisRanges.
//
//		TODO: For now, just takes all AxisRanges present in the
//		FITS file, and ignores the rest.
vector< vector<int> > flexIRF::GIRF::FindAxisRanges(std::vector<AxisRange> axisRanges){

	vector< vector<int> > axisIDs, emptyVector;
	vector<int> foundIDs;
	int loop=0;
	for(std::vector<AxisRange>::iterator axisRange = axisRanges.begin(); axisRange != axisRanges.end(); ++axisRange, loop++) {
		foundIDs = FindAxisRange(*axisRange);
//		for(vector<int>::iterator foundID = foundIDs.begin(); foundID != foundIDs.end(); ++foundID){
//			cout << "In loop " << loop << " found axis ID = " << *foundID << endl;
//		}
		if (!foundIDs.empty()) axisIDs.push_back(foundIDs);
		else if (axisRange->required){
			cout << "ERROR: Required axis of type " << GIRFAxis::GetVarName(axisRange->varType).data() << " between " << axisRange->lowRange << " and " << axisRange->highRange << " is not present in the FITS file!!! Exiting..." << endl;
			return emptyVector;
		}
	}

	return axisIDs;
}



////////////////////////////////////////////////////////////////
//
// 		Return Pdfs IDs pointing at all axisIDs.
//
vector<int> flexIRF::GIRF::FindPdfs(vector< vector<int> > axisIDs, PdfVar pdfVar){

	vector<int> pdfAxes;


	vector<int> pdfsIDOfCorrectType = FindPdfsOfType(pdfVar);

	vector< vector<int> > foundPdfsAxisIDs = GetPdfAxisIDs(pdfsIDOfCorrectType);


//	cout << "foundPdfsAxisIDs:" << endl;
//	for(std::vector< vector<int> >::iterator pdf = foundPdfsAxisIDs.begin(); pdf != foundPdfsAxisIDs.end(); ++pdf) {
//		cout << "This pdf points to the axes: ";
//		for(std::vector<int>::iterator pdfID = pdf->begin(); pdfID != pdf->end(); ++pdfID) {
//			cout << *pdfID << ", ";
//		}
//		cout << endl;
//	}

//	cout << "axisIDs:" << endl;
//	for(std::vector< vector<int> >::iterator axis = axisIDs.begin(); axis != axisIDs.end(); ++axis) {
//		cout << "This axis range points axes: ";
//		for(std::vector<int>::iterator axisID = axis->begin(); axisID != axis->end(); ++axisID) {
//			cout << *axisID << ", ";
//		}
//		cout << endl;
//	}
	// Now we have the axis IDs of each Pdf of correct type.
	// We need to check if any of these Pdfs point to the correct axis IDs.

	vector<int> correctPdfIDs = FindPdfsPointingToAxisIDs(axisIDs, pdfsIDOfCorrectType, foundPdfsAxisIDs);

//	cout << "correctPdfIDs: ";
//	for(std::vector<int>::iterator pdfID = correctPdfIDs.begin(); pdfID != correctPdfIDs.end(); ++pdfID) {
//		cout << *pdfID << " ";
//	}
//	cout << endl;

	return correctPdfIDs;
}

////////////////////////////////////////////////////////////////
//
// 		Return all pdf IDs matching the Pdf type pdfVar.
//
vector<int> flexIRF::GIRF::FindPdfsOfType(PdfVar pdfVar){

	vector<int> foundPdfIDs;

	int currenthdu = fFitsPtr->HDUposition;				//TODO: do we need to know the current position? I leave it just to make sure...

	int status = 0;   		// must be initialized (0 means ok)
	char card[FLEN_CARD]; /* Standard string lengths defined in fitsio.h */
	int numaxes = 0, hdutype, hdunum;
	fits_get_num_hdus(fFitsPtr, &hdunum, &status);
	char axisIDkeyword[20];

	for (int hdupos = 1; hdupos <= hdunum; hdupos++) /* Main loop through each extension */
	{
		fits_movabs_hdu(fFitsPtr, hdupos, &hdutype, &status);
		if (hdutype == IMAGE_HDU) {
			if (!fits_read_key_str(fFitsPtr, "HDUCLAS2", card, NULL, &status)) {
				if (!strcmp(card, "DATA")) {
					if (!fits_read_key_str(fFitsPtr, "PDFVAR", card, NULL, &status)) {
						if (atoi(card) == (int)pdfVar) {
							if (!fits_read_key_str(fFitsPtr, "HDUCLAS4", card, NULL, &status)) {
								foundPdfIDs.push_back(atoi(card));
							}
						}
					}
				}
			}
		}
		if (status == KEY_NO_EXIST) status = 0;
		if (status) break;

	}
	fits_movabs_hdu(fFitsPtr, currenthdu + 1, NULL, &status);

	return foundPdfIDs;
}

////////////////////////////////////////////////////////////////
//
// 		Return the axis IDs which the Pdf points at.
//
vector<int> flexIRF::GIRF::GetPdfAxisIDs(int pdfID){

	vector<int> foundAxisIDs;

	int currenthdu = fFitsPtr->HDUposition;

	int status = 0;   		// must be initialized (0 means ok)
	char card[FLEN_CARD]; /* Standard string lengths defined in fitsio.h */
	int numaxes = 0, hdutype, hdunum;
	fits_get_num_hdus(fFitsPtr, &hdunum, &status);
	char axisIDkeyword[20];

	for (int hdupos = 1; hdupos <= hdunum; hdupos++) /* Main loop through each extension */
	{
		fits_movabs_hdu(fFitsPtr, hdupos, &hdutype, &status);
		if (hdutype == IMAGE_HDU) {
			if (!fits_read_key_str(fFitsPtr, "HDUCLAS2", card, NULL, &status)) {
				if (!strcmp(card, "DATA")) {
					if (!fits_read_key_str(fFitsPtr, "HDUCLAS4", card, NULL, &status)) {
						if (atoi(card) == pdfID){
							// Current HDU is the pdf we want.
							if (!fits_read_key_str(fFitsPtr, "NAXIS", card, NULL, &status)) {
								numaxes=atoi(card);
								for (int i=1;i<=numaxes;i++){
									sprintf(axisIDkeyword, "AXISID%d", i);
									if (!fits_read_key_str(fFitsPtr, axisIDkeyword, card, NULL, &status)) {
										foundAxisIDs.push_back(atoi(card));
									}
								}
							}
						}
					}
				}
			}
		}
		if (status == KEY_NO_EXIST) status = 0;
		if (status) break;

	}
	fits_movabs_hdu(fFitsPtr, currenthdu + 1, NULL, &status);

	return foundAxisIDs;

}


////////////////////////////////////////////////////////////////
//
// 		Return the axis IDs which the Pdfs points at.
//
vector< vector<int> > flexIRF::GIRF::GetPdfAxisIDs(vector<int> pdfIDs){

	vector< vector<int> > foundPdfsAxisIDs;

	for(std::vector<int>::iterator pdfID = pdfIDs.begin(); pdfID != pdfIDs.end(); ++pdfID) {
//		cout << "Pdf #" << *pdfID << " is of correct type!!" << endl;
		foundPdfsAxisIDs.push_back(GetPdfAxisIDs(*pdfID));
	}

	return foundPdfsAxisIDs;
}

////////////////////////////////////////////////////////////////
//
// 		Return the pdf IDs pointing at the correct axis.
//
vector<int> flexIRF::GIRF::FindPdfsPointingToAxisIDs(vector< vector<int> > axisIDs, vector<int> pdfsIDs, vector< vector<int> > allPdfsAxisIDs){

	vector<int> foundPdfIDs;

	int ipdf=0;
	for(std::vector< vector<int> >::iterator pdf = allPdfsAxisIDs.begin(); pdf != allPdfsAxisIDs.end(); ++pdf, ipdf++) {
		if (CheckPdfPointsToAxisIDs(axisIDs, *pdf)) foundPdfIDs.push_back(pdfsIDs[ipdf]);
	}
	return foundPdfIDs;
}


////////////////////////////////////////////////////////////////
//
// 		Check if one pdf points to the required axes
//
bool flexIRF::GIRF::CheckPdfPointsToAxisIDs(vector< vector<int> > axisIDs, vector<int> pdfAxisIDs){


	bool axisRangePresent=0;
	for(std::vector< vector<int> >::iterator axisRange = axisIDs.begin(); axisRange != axisIDs.end(); ++axisRange) {
		for(std::vector<int>::iterator axisRangeID = axisRange->begin(); axisRangeID != axisRange->end(); ++axisRangeID) {
			for(std::vector<int>::iterator pdfaxisID = pdfAxisIDs.begin(); pdfaxisID != pdfAxisIDs.end(); ++pdfaxisID) {
				if (*pdfaxisID == *axisRangeID) axisRangePresent=1;
			}
		}
		if (!axisRangePresent) return 0;
	}
	return 1;
}

////////////////////////////////////////////////////////////////
//
// 		Check if one pdf points to the required axes
//
int flexIRF::GIRF::PickPreferredPdf(vector<int> foundPdfs, GIRFConfig config){

//	cout << "foundPdfs.size() = " << foundPdfs.size() << endl;
	if (foundPdfs.size() == 0) return 0;

	//TODO!!!!!!!!!!!!!
	return foundPdfs[0];

}




