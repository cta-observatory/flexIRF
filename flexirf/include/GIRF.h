#ifndef G_IRF
#define G_IRF

#include <vector>
#include <string>

#include "GIRFAxis.h"
#include "GIRFAxisBins.h"
#include "GIRFAxisParam.h"
#include "GIRFPdf.h"
#include "GIRFConfig.h"

class GIRF 
{
 private:
  std::vector<GIRFPdf*>   fPdfList;
  

 public:
  GIRF();               // create new fits file with empty primary image
  virtual ~GIRF(){};

  inline int AddPdf(GIRFPdf* pdf)     {fPdfList.push_back(pdf);   return int(fPdfList.size())-1;}  // insert pdf in the list and return its id

  int Write(std::string filename);
  GIRFPdf GetPdf(GIRFPdf::PdfVar pdfVar, GIRFConfig config);

};

#endif

