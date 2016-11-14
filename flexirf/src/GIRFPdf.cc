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
#include "GIRFAxisBins.h"
#include "GIRFAxisParam.h"
#include "GIRFUtils.h"
#include <stdlib.h>
#include <iostream>
#include <string.h>


//ROOT includes
#ifdef WITH_ROOT  // ROOT found in the installation
	#include "rootincludes.h"
#endif

using namespace std;

////////////////////////////////////////////////////////////////
// 
// Construct pdf table with type
//
flexIRF::GIRFPdf::GIRFPdf(PdfVar pdftype, PdfFunc pdffunc, unsigned long naxes) : fPdfFunc(pdffunc), fData(0), fIsEmpty(1) {
	fPdfVar = pdftype;
	fAxis.reserve(naxes);
}



////////////////////////////////////////////////////////////////
//
// 		Draw Pdf content
//	TODO: Just 1 and 2 dimensional IRFs can be currently drawn.
void flexIRF::GIRFPdf::Draw(string filename, string drawOption) const {


#ifdef WITH_ROOT  // ROOT found in the installation

	if (fIsEmpty) {
		cout << "Pdf is empty" << endl;
		return;
	}
	// Check how many dimensions does the Pdf have:
	int dimensions = fAxis.size();

	if (dimensions == 0) {
		cout << "ERROR: Cannot draw empty Pdf." << endl;
		return;
	} else if (dimensions > 2){
		cout << "ERROR: Only Pdfs with dimension lower than 3 are currently supported." << endl;
		return;
	} else if (dimensions == 1){
		GIRFAxisParam* axisParam = dynamic_cast<GIRFAxisParam*>(fAxis[0]);
		GIRFAxisBins* axisBins = dynamic_cast<GIRFAxisBins*>(fAxis[0]);
		if (axisParam){

			cout << "Formula: " << axisParam->GetFormula().data() << endl;
			TF1* pdf = new TF1(GetExtName().data(), axisParam->GetFormula().data(), (float) axisParam->GetRangeMin(), (float) axisParam->GetRangeMax());
			for (int i=0;i<axisParam->GetNumPars();i++) pdf->SetParameter(i,fData[i]);
			TCanvas c1;
			TString xTitle = (TString)axisParam->GetVarName() + " [" + (TString)axisParam->GetVarUnit() + "]";
			if (axisParam->GetScale() == kLog10) xTitle = "log " + (TString)axisParam->GetVarName() + " [" + (TString)axisParam->GetVarUnit() + "]";
			TString yTitle = (TString)GetVarName() + " [" + (TString)GetVarUnit() + "]";;
			pdf->SetTitle((TString)GetExtName());
			pdf->GetXaxis()->SetTitle(xTitle);
			pdf->GetYaxis()->SetTitle(yTitle);
			if (drawOption.find("logy")) c1.SetLogy();
			pdf->Draw();
			if (filename == "") c1.SaveAs("plot.png");
			else c1.SaveAs(filename.data());
			return;
		}
		if (axisBins){
			TH1F *pdf = new TH1F("pdf", GetExtName().data(), axisBins->GetSize(), axisBins->GetAxisBins().data());
			for (int i=1;i<=axisBins->GetSize();i++){
				pdf->SetBinContent(i,fData[i-1]);
			}
			TCanvas c1;
			TString xTitle = (TString)axisBins->GetVarName() + " [" + (TString)axisBins->GetVarUnit() + "]";
			TString yTitle = (TString)GetVarName() + " [" + (TString)GetVarUnit() + "]";
			if (axisBins->GetScale() == kLog10) xTitle = "log " + (TString)axisBins->GetVarName() + " [" + (TString)axisBins->GetVarUnit() + "]";
			pdf->GetXaxis()->SetTitle(xTitle);
			pdf->GetYaxis()->SetTitle(yTitle);
			pdf->SetStats(0);
			std::size_t logY = drawOption.find("logY");
			if (logY!=std::string::npos) c1.SetLogy();
			pdf->Draw();
			if (filename == "") c1.SaveAs("plot.png");
			else c1.SaveAs(filename.data());
		}
	} else if (dimensions == 2){
//		GIRFAxis axisx = fAxis[0];
//		GIRFAxis axisy = fAxis[1];
//		TH2F *pdf = new TH2F("pdf", GetExtName().data(), axisx.GetSize(), GetData());
//		for (int i=1;i<=axis.GetSize();i++){
//			pdf->SetBinContent(i,fData[i-1]);
//		}
//		pdf->Draw();
		GIRFAxisParam* axisParam1 = dynamic_cast<GIRFAxisParam*>(fAxis[0]);
		GIRFAxisBins* axisBins1 = dynamic_cast<GIRFAxisBins*>(fAxis[0]);
		GIRFAxisParam* axisParam2 = dynamic_cast<GIRFAxisParam*>(fAxis[1]);
		GIRFAxisBins* axisBins2 = dynamic_cast<GIRFAxisBins*>(fAxis[1]);
		if (axisParam1 || axisParam2){
			cout << "2D Parametrized axis are not yet supported... (yet!) :)" << endl;
			return;
		}
		else if (axisBins1 && axisBins2){
			TH2F *pdf = new TH2F("pdf", GetExtName().data(), axisBins1->GetSize()-1, axisBins1->GetAxisBins().data(), axisBins2->GetSize()-1, axisBins2->GetAxisBins().data());

			for (int i=0;i<axisBins1->GetSize();i++){
				for (int j=0;j<axisBins2->GetSize();j++){
//					cout << "Data[" << i+1 << "," << j+1 << "] = fData[" << (i*(axisBins2->GetSize()))+j << "] = " << fData[(i*(axisBins1->GetSize()))+j] << endl;
					pdf->SetBinContent(i+1, j+1, fData[(i*(axisBins2->GetSize()))+j]);
				}
			}
			TCanvas c1;
			pdf->Draw();
			c1.SaveAs("plot.png");
		}
	}


#endif
#ifndef WITH_ROOT  // ROOT found in the installation
	cout << "WARNING: GIRFPdf::Draw is implemented using ROOT libraries (not found in your system during compilation)." << endl;
#endif

}


////////////////////////////////////////////////////////////////
//
// Return the axis name for EXTNAME
//
// These names were changed in order to comply with ctools
// version 1.1.
std::string flexIRF::GIRFPdf::GetExtName() const {

	string extName;

	switch (fPdfVar) {
		case kEfficiency:
			extName = "EFFIC";
			break;
		case kEDispersion:
			extName = "ENERGY DISPERSION";
			break;
		case kPsf:
			extName = "POINT SPREAD FUNCTION";
			break;
		case kBkgRate:
			extName = "BGRATE";
			break;
		case kBkgRateSqDeg:
			extName = "BGRATESQDEG";
			break;
		case kBkgRateSr:
			extName = "BACKGROUND";
			break;
		case kDiffSens:
			extName = "DIFFSENS";
			break;
		case kAeff:
			extName = "EFFECTIVE AREA";
			break;
		case kAeffNoTheta2Cut:
			extName = "EFFANOT2CUT";
			break;
		default:
			cout << "Incorrect variable type.\n";
			return extName;
	}

	return extName;
}


////////////////////////////////////////////////////////////////
//
// Return the pdf function name for EXTNAME
//
std::string flexIRF::GIRFPdf::GetFuncName() const {

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
std::string flexIRF::GIRFPdf::GetVarName() const {

	string pdfVarType;

	switch (fPdfVar) {
		case kEfficiency:
			pdfVarType = "EFFIC";
			break;
		case kEDispersion:
			pdfVarType = "MATRIX";
			break;
		case kPsf:
			pdfVarType = "RPSF";
			break;
		case kBkgRate:
			pdfVarType = "BGD";
			break;
		case kBkgRateSqDeg:
			pdfVarType = "BGRATESQDEG";
			break;
		case kBkgRateSr:
			pdfVarType = "BGD";
			break;
		case kDiffSens:
			pdfVarType = "DIFFSENS";
			break;
		case kAeff:
			pdfVarType = "EFFAREA";
			break;
		case kAeffNoTheta2Cut:
			pdfVarType = "EFFANOT2CUT";
			break;
		default:
			cout << "Incorrect variable type.\n";
			return pdfVarType;
	}

	return pdfVarType;
}

////////////////////////////////////////////////////////////////
//
// Return the axis name for EXTNAME
//
std::string flexIRF::GIRFPdf::GetVarUnit() const {

	//TODO: Build a separate GIRFPdf units enum, which is read/written from the FITS file.

	string pdfVarType;

	switch (fPdfVar) {
	case kEfficiency:
		pdfVarType = "";
		break;
	case kEDispersion:
		pdfVarType = "";
		break;
	case kPsf:
		pdfVarType = "deg";
		break;
	case kBkgRate:
		pdfVarType = "events*s^-1";
		break;
	case kBkgRateSqDeg:
		pdfVarType = "events*s^-1*deg^-2";
		break;
	case kBkgRateSr:
		pdfVarType = "1/s/MeV/sr";
		break;
	case kDiffSens:
		pdfVarType = "erg*cm^-2*s^-1";
		break;
	case kAeff:
		pdfVarType = "m^2";
		break;
	case kAeffNoTheta2Cut:
		pdfVarType = "m^2";
		break;
	default:
		cout << "Incorrect variable type.\n";
		return pdfVarType;
	}

	return pdfVarType;
}



////////////////////////////////////////////////////////////////
//
// Write the pdf and the associated axes to the specified
// file pointer
//
int flexIRF::GIRFPdf::Write(fitsfile* fptr, int* status) {

	if (fSerialization.empty()) {
		cout << "WARNING: No serialization specified. Using default IMAGE serialization" << endl;
		fSerialization="IMAGE";
	}

	if (fSerialization=="IMAGE") return Write_IMAGE(fptr, status);
	else if (fSerialization=="BINTABLE") return Write_BINTABLE(fptr, status);
	else {
		cout << "ERROR: Incorrect serialization type. Exitting...\n";
		return 1;
	}
}

////////////////////////////////////////////////////////////////
//
// Write the pdf and the associated axes to the specified
// file pointer. Only done if IMAGE serialization is used.
//
int flexIRF::GIRFPdf::Write_IMAGE(fitsfile* fptr, int* status) {

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
		cout << "GIRFPdf::Write Error: problem writing axis header (error code: "
				<< *status << ")" << endl;

	// write keywords to the header
	char keyword[9];
	char chval[20];
	char comment[70];
	ushort usval;
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

	int axisloop=1;
	for (vector<GIRFAxis*>::iterator axis = fAxis.begin(); axis != fAxis.end();
			++axis, axisloop++){
		sprintf(keyword, "CTYPE%d", axisloop);
		sprintf(chval, "%s", (*axis)->GetVarName().data());
		sprintf(comment, "Axis %d type", axisloop);
			if (fits_write_key(fptr, TSTRING, keyword, &chval, comment, status))
				cout << "GIRFPdf::Write Error: cannot write keyword (error code: "
						<< *status << ")" << endl;

		sprintf(keyword, "CUNIT%d", axisloop);
		sprintf(chval, "%s", (*axis)->GetVarUnit().data());
		sprintf(comment, "Axis %d units", axisloop);
			if (fits_write_key(fptr, TSTRING, keyword, &chval, comment, status))
				cout << "GIRFPdf::Write Error: cannot write keyword (error code: "
						<< *status << ")" << endl;


	}
	// write the indeces linking to the relevant axes
	for (int jaxis = 0; jaxis < naxis; jaxis++) {
		sprintf(keyword, "AXISID%d", jaxis + 1);
		usval = ushort(axisIDs[jaxis]);
		sprintf(comment, "Axis describing dimension #%d", jaxis + 1);
		if (fits_write_key(fptr, TUSHORT, keyword, &usval, comment, status))
			cout << "GIRFPdf::Write Error: cannot write keyword (error code: "
					<< *status << ")" << endl;
	}




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

//  For testing purposes:
//	float *array;
//	array = (float *) calloc(nentries+1, abs(FLOAT_IMG)/8);
//	for (int i=0;i<=nentries;i++) array[i]=fData[i];

//	if (fits_write_img(fptr, TFLOAT, 1, nentries, array, status))
//			cout << "GIRFPdf::Write Error: problem writing axis data (error code: "
//					<< *status << ")" << endl;
	if (fits_write_pix(fptr, TFLOAT, fpixel, nentries, fData, status))
		cout << "GIRFPdf::Write Error: problem writing axis data (error code: "
				<< *status << ")" << endl;

	return *status;
}

////////////////////////////////////////////////////////////////
//
// Write the pdf and the associated axes as a BINTABLE. Only
// done if BINTABLE serialization is used.
//
int flexIRF::GIRFPdf::Write_BINTABLE(fitsfile* fptr, int* status) {

	// create arrays with the total number of columns and the size of each of them
	const int naxis = int(fAxis.size());
	const int nColumns = (2*naxis)+1;
	int naxes[nColumns];

//	char *ttype[(2*naxis)+1];
//	char *tform[(2*naxis)+1];
//	char *tunit[(2*naxis)+1];

	vector<string> tType, tForm, tUnit;
//	Loop over axes, filling both low and high edges. TODO: Must be improved for other kind of axes.
	for (int jaxis = 0; jaxis < naxis; jaxis++) {
		naxes[2*jaxis] = int(fAxis[jaxis]->GetSize());
		naxes[(2*jaxis)+1] = int(fAxis[jaxis]->GetSize());
//		TODO: Only if stored column is a float!!!

		char temp1[30];
		sprintf(temp1, "%s_LO", (char*)fAxis[jaxis]->GetVarName().data());
		tType.push_back(temp1);

		char temp2[30];
		sprintf(temp2, "%s_HI", (char*)fAxis[jaxis]->GetVarName().data());
		tType.push_back(temp2);
		char temp3[30];
		sprintf(temp3, "%dE", naxes[2*jaxis]);  //TODO: add type to axis, so float require _LOW _HIGH while "integers" or "chars" not.
		tForm.push_back(temp3);
		char temp4[30];
		sprintf(temp4, "%dE", naxes[(2*jaxis)+1]);  //TODO: add type to axis, so float require _LOW _HIGH while "integers" or "chars" not.
		tForm.push_back(temp4);
		tUnit.push_back(fAxis[jaxis]->GetVarUnit().data());
		tUnit.push_back(fAxis[jaxis]->GetVarUnit().data());

	}
	int const pdfEntries = GetSize();
	char temp6[30];
	sprintf(temp6, "%s", GIRFPdf::GetVarName().data());
	tType.push_back(temp6);

	char temp5[30];
	sprintf(temp5, "%dE", pdfEntries);
//	cout << "pdfEntries = " << pdfEntries << endl;
	tForm.push_back(temp5);
	tUnit.push_back(GIRFPdf::GetVarUnit());


	// Filling of cfitsio bintable parameters (taken from gammalib::GFitsTable.cpp)
    // Initialise number of fields
    int tfields = 0;
    // Setup cfitsio column definition arrays
    char** ttype = NULL;
    char** tform = NULL;
    char** tunit = NULL;
    if (nColumns > 0) {
        ttype = new char*[nColumns];
        tform = new char*[nColumns];
        tunit = new char*[nColumns];
        for (int i = 0; i < nColumns; ++i) {
            ttype[i] = NULL;
            tform[i] = NULL;
            tunit[i] = NULL;
        }
        for (int i = 0; i < nColumns; ++i) {
            ttype[tfields] = (char*)tType[i].data();
            tform[tfields] = (char*)tForm[i].data();
            tunit[tfields] = (char*)tUnit[i].data();
            if (ttype[tfields] != NULL && tform[tfields] != NULL &&
                tunit[tfields] != NULL)
                tfields++;
        }
    }


	// write the pdf BINTABLE HDU.
	if (fits_create_tbl(fptr, BINARY_TBL, 0, nColumns,ttype, tform, tunit,GetExtName().data(), status))
		cout << "GIRFPdf::Write Error: problem writing axis header (error code: "
				<< *status << ")" << endl;

	// write keywords to the header
	char keyword[9];
	char chval[20];
	char comment[70];
	ushort usval;

	//	Loop over axes, writing both low and high edge columns. TODO: Must be improved for other kind of axes.
	for (std::vector<GIRFAxis*>::size_type jaxis = 0; jaxis < naxis; jaxis++) {
//		TODO: allow other variable types.
		GIRFAxisBins* axisPtr= dynamic_cast<GIRFAxisBins*>(fAxis[jaxis]);
		if (axisPtr){
			vector<float> axisBinsLow= axisPtr->GetAxisBins();
			vector<float> axisBinsHigh= axisPtr->GetAxisBins();
			axisBinsLow.pop_back();
			axisBinsHigh.erase(axisBinsHigh.begin());
			fits_write_col(fptr, TFLOAT, (2*jaxis)+1, 1, 1, naxes[2*jaxis], axisBinsLow.data(), status);
			fits_write_col(fptr, TFLOAT, (2*jaxis)+2, 1, 1, naxes[(2*jaxis)+1], axisBinsHigh.data(), status);
		}

	}

	fits_write_col(fptr, TFLOAT, (2*naxis)+1, 1, 1, pdfEntries, fData, status);

	// write tdym of pdf data field
	sprintf(keyword, "TDIM%d", nColumns);
	strcpy(chval, "(");
	for (int jaxis = 0; jaxis < naxis; jaxis++) {
		if (jaxis == 0) sprintf(chval, "%s%d",chval, naxes[2*jaxis]);
		else sprintf(chval, "%s,%d",chval, naxes[2*jaxis]);
	}
	sprintf(chval, "%s)",chval);
	sprintf(comment, "Dimensions of the field");
	if (fits_write_key(fptr, TSTRING, keyword, &chval, comment, status))
		cout << "GIRFAxis::WriteAxis Error: cannot write keyword (error code: "
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

	return *status;


}


////////////////////////////////////////////////////////////////
//
// Write the pdf and the associated axes to the specified
// file pointer
//
void flexIRF::GIRFPdf::Print(int sanity) const {

	int iAxis=0;
	int totalSize=1;
	cout << endl;
	cout << "*******************************************" << endl;
	cout << "***          Printing GIRFPdf           ***" << endl;
	cout << "*******************************************" << endl;
	cout << "" << endl;
	cout << "Printing Axes:" << endl;
	for(std::vector<GIRFAxis*>::const_iterator axis = fAxis.begin(); axis != fAxis.end(); ++axis, iAxis++) {
		cout << "Printing Axis #" << iAxis+1 << endl;
		if (dynamic_cast<GIRFAxisParam*>(*axis)) totalSize*=(*axis)->GetSize();
		else if (dynamic_cast<GIRFAxisBins*>(*axis)) totalSize*=(*axis)->GetSize()-1;
		(*axis)->Print();
	}
	if (totalSize > sanity) cout << "Printing Pdf content (just first " << sanity << " values):" << endl;
	else cout << "Printing Pdf content:" << endl;
	int iData=0;
	// TODO: This is obviously wrong... but at least printing some numbers...

	int pdfPrintedValues=0;
	for (int i=0;i<totalSize;i++){
		if (pdfPrintedValues < sanity){
			cout << "Data[" << i+1 << "] = " << fData[i] << endl;
			pdfPrintedValues++;
		}
	}
	cout << "*******************************************" << endl;
//	for(std::vector<GIRFAxis*>::const_iterator axis = fAxis.begin(); axis != fAxis.end(); ++axis, iData++) {
//		cout << "Printing Axis #" << iData+1 << endl;
//		for (int i=0;i<(*axis)->GetSize();i++){
//			cout << fData[i] << endl;
//		}
//	}

}

void   flexIRF::GIRFPdf::SetData(float* data){
	if (data==0) return;
	fData = data;
	fIsEmpty=0;
}








