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
	bool fFITSopened;

public:
	GIRF();
	GIRF(std::string filename);
	virtual ~GIRF() {};

	inline int AddPdf(GIRFPdf* pdf) { fPdfList.push_back(pdf); return int(fPdfList.size()) - 1;}  // insert pdf in the list and return its id

	int CheckStatus();

	GIRFAxis* ReadAxis(int axisID);
	inline GIRFPdf GetPdf(int pdfPos){GIRFPdf pdf = *fPdfList[pdfPos]; return pdf;};
	GIRFPdf ReadPdf(GIRFPdf::PdfVar pdfVar, GIRFConfig config);

	int Write();
	inline int Write(std::string filename){fFilename=filename; return Write();};
	int OpenFITS();
	int CreateFITS();

protected:

	int CheckAxisHDUpos(int axisID);
	GIRFAxis::AxisType CheckAxisType(int axisID);
	vector<int> FindAxisRange(GIRFAxis::AxisRange axisRange);
	vector< vector<int> > FindAxisRanges(std::vector<GIRFAxis::AxisRange> axisRanges);
	vector<int> FindPdfs(vector< vector<int> > axisIDs);
};

#endif

