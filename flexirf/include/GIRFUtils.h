#ifndef G_IRFUtils
#define G_IRFUtils

#include <string>
#include <vector>
#include <GIRFAxis.h>
#include "GIRFRange.h"

namespace GIRFUtils{

	int GetLastAxisID(fitsfile* fptr);
	int GetLastAxisID(string filename);
	int GetLastPdfID(string filename);
	int GetLastPdfID(fitsfile* fptr);

	void GoToLastAxisHDU(fitsfile* fptr);
	void GoToPdfHDU(fitsfile* fptr, int pdfID);

	vector<int> FindAxisRanges(string filename, std::vector<AxisRange> axisRanges);
	vector<int> FindAxisRange(fitsfile *fptr, AxisRange axisRange);
	GIRFAxis GetAxis(fitsfile *fptr, int axisID);


};

#endif

