#ifndef G_IRF_Pdf
#define G_IRF_Pdf

#include <iostream>
#include <vector>
#include "fitsio.h"
#include "GIRFAxis.h"

class GIRFPdf
{
 public: 
  enum PdfVar   {kNoPdfVar=0,
		 kEfficiency,
		 kEDispersion,
		 kPsf,
		 kBkgRate,
		 kBkgRateSqDeg,
		 kDiffSens,
		 kAeff,
		 kAeffNoTheta2Cut,
		 kPdfVarMax}; // allowed Pdfs   

  enum PdfFunc  {kNoPdfFunc=0,
		 kNumber,
		 kGaussian,
		 k2DGaussian,
		 kPdfFuncMax}; // parametrization for the Pdf

 private:
  PdfVar  fPdfVar;
  PdfFunc fPdfFunc;
  std::vector<GIRFAxis*> fAxis;
  float*  fData;

 public:
  GIRFPdf(PdfVar pdftype=kNoPdfVar,PdfFunc pdffunc=kNoPdfFunc,std::vector<GIRFAxis*>::size_type naxes=0); // create new pdf table for a given PdfVar
  virtual ~GIRFPdf(){};

  inline int    AddAxis(GIRFAxis* axis)  {fAxis.push_back(axis); return int(fAxis.size())-1;} // insert axis in the list and return its id
  inline void   SetData(float* data)     {fData = data;}
  inline float* GetData()                const {return fData;} 
  inline float* GetDataEntry(int ientry) const {return fData+ientry*GetNEntriesPerBin(fPdfFunc);}
  inline long   GetSize()                const {long tot=1;for(uint i=0;i<fAxis.size();i++) tot*=int(fAxis[i]->GetSize()); return tot;}
  virtual inline std::string GetExtName() const {return GetVarName() + "_" + GetFuncName();}
  virtual std::string GetFuncName() const;
  virtual std::string GetVarName() const;
  virtual std::string GetVarUnit() const;




  inline int GetNEntriesPerBin(PdfFunc func) const
  {
    switch(func)
      {
      case kNumber:
	return 1;
      case kGaussian:
	return 2;
      case k2DGaussian:
	return 4;
      default:
	return 0;
      }
  }   
  
  virtual int Write(fitsfile* fptr,int ipdf,int& iaxis,int* status);

};
  
#endif

