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

namespace flexIRF{
	class GIRF {

	#ifdef SWIG
	public:
	#else
	private:
	#endif
	std::vector<GIRFPdf*> fPdfList;
	std::string fFilename;
	std::string fSerialization;
	fitsfile* fFitsPtr;
	int fStatus;
	bool fFITSopened;

	public:
	GIRF();
	GIRF(std::string filename);
	virtual ~GIRF() {};

	inline int 				AddPdf(GIRFPdf* pdf) { fPdfList.push_back(pdf); return int(fPdfList.size()) - 1;}  // insert pdf in the list and return its id

	int 					CheckStatus();

	GIRFAxis* 				ReadAxis(int axisID);
	GIRFAxis*				ReadAxis(int axisID, vector<AxisRange> axisRanges);
	inline GIRFPdf 			GetPdf(int pdfPos){GIRFPdf pdf = *fPdfList[pdfPos]; return pdf;};
	GIRFPdf* 				ReadPdf(PdfVar pdfVar, GIRFConfig config);
	GIRFPdf* 				ReadPdf(int pdfID, GIRFConfig config);
	//	GIRFPdf* 				ReadPdf(int pdfID);

	inline void 			SetSerialization(std::string serialization){fSerialization=serialization;};
	int 					Write();
	inline int 				Write(std::string filename, std::string serialization="IMAGE"){fFilename=filename; fSerialization=serialization; return Write();};
	int 					OpenFITS();
	int 					CreateFITS();

	#ifdef SWIG
	public:
	#else
	protected:
	#endif

	int 					CheckAxisHDUpos(int axisID);
	AxisType 				CheckAxisType(int axisID);
	VarType 				CheckAxisVarType(int axisID);
	bool 					CheckPdfPointsToAxisIDs(vector< vector<int> > axisIDs, vector<int> pdfAxisIDs);
	vector<int> 			FindAxisRange(AxisRange axisRange);
	vector< vector<int> > 	FindAxisRanges(std::vector<AxisRange> axisRanges);
	vector<int> 			FindPdfs(vector< vector<int> > axisIDs, PdfVar pdfVar);
	vector<int> 			FindPdfsOfType(PdfVar pdfVar);
	vector<int> 			GetPdfAxisIDs(int pdfID);
	vector< vector<int> > 	GetPdfAxisIDs(vector<int> pdfIDs);
	vector<int> 			FindPdfsPointingToAxisIDs(vector< vector<int> > axisIDs, vector<int> pdfsIDs, vector< vector<int> > allPdfsAxisIDs);
	int 					PickPreferredPdf(vector<int> foundPdfs, GIRFConfig config);
	PdfVar 					ReadPdfVar(int pdfID);
	PdfFunc 				ReadPdfFunc(int pdfID);
	float*  				ReadPdfData(int pdfID, vector<int> pdfAxes, vector<AxisRange> axisRanges);
	int 					Write_PHDU();
	int 					Write_PDF();

	};
}


#endif

