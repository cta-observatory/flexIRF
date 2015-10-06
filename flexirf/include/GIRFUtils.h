#ifndef G_IRFUtils
#define G_IRFUtils

#include <string>
#include <vector>
#include <GIRFAxis.h>

namespace GIRFUtils{

	void GoToLastAxisHDU(fitsfile* fptr);
	int GetLastAxisID(fitsfile* fptr);
	int GetLastAxisID(string filename);
	int GetLastPdfID(string filename);
	int GetLastPdfID(fitsfile* fptr);

};

#endif

