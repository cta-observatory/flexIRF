#ifndef G_IRFUtils
#define G_IRFUtils

#include <string>
#include <vector>
#include <GIRFAxis.h>
#include "GIRFRange.h"

namespace GIRFUtils{

	void GoToLastAxisHDU(fitsfile* fptr);
	int GetLastAxisID(fitsfile* fptr);
	int GetLastAxisID(string filename);
	int GetLastPdfID(string filename);
	int GetLastPdfID(fitsfile* fptr);
	vector<int> FindAxisRanges(string filename, std::vector<GIRFRange::AxisRange> axisRanges);
};

#endif

