#ifndef G_IRF
#define G_IRF

#include <vector>
#include <string>

#include "GIRFAxis.h"
#include "GIRFAxisBins.h"
#include "GIRFAxisParam.h"
#include "GIRFPdf.h"
#include "GIRFConfig.h"
#include "GIRFRange.h"
#include "GIRFUtils.h"

class GIRF {
private:
	std::vector<GIRFPdf*> fPdfList;
	std::string fFilename;
	fitsfile* fFitsPtr;
	int fStatus;

public:
	GIRF();
	GIRF(std::string filename);
	virtual ~GIRF() {};

	inline int AddPdf(GIRFPdf* pdf) { fPdfList.push_back(pdf); return int(fPdfList.size()) - 1;}  // insert pdf in the list and return its id

	int CheckStatus();

	GIRFAxis GetAxis(int axisID);
	GIRFPdf GetPdf(GIRFPdf::PdfVar pdfVar, GIRFConfig config);
	inline GIRFPdf GetPdf(std::string filename, GIRFPdf::PdfVar pdfVar, GIRFConfig config){fFilename=filename; return GetPdf(pdfVar, config);};

	int Write();
	inline int Write(std::string filename){fFilename=filename; return Write();};


protected:

	int CheckAxisHDUpos(int axisID);
	GIRFAxis::AxisType CheckAxisType(int axisID);

	void GoToLastAxisHDU(fitsfile* fptr);
};

#endif

