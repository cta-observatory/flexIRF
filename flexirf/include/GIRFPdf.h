#ifndef G_IRF_Pdf
#define G_IRF_Pdf

#include <iostream>
#include <vector>
#include "fitsio.h"
#include "GIRFAxis.h"


enum PdfVar {kNoPdfVar=0,
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

class GIRFPdf
{

 private:
  PdfVar  fPdfVar;
  PdfFunc fPdfFunc;
  std::vector<GIRFAxis*> fAxis;
  bool fIsEmpty;
  float*  fData;

 public:
  GIRFPdf(PdfVar pdftype=kNoPdfVar,PdfFunc pdffunc=kNoPdfFunc,unsigned long naxes=0); // create new pdf table for a given PdfVar
  virtual ~GIRFPdf(){};

  inline int    AddAxis(GIRFAxis* axis)  {if (!axis->CheckAxisConsistency()) fAxis.push_back(axis); return int(fAxis.size());} // insert axis in the list
  virtual void 	Draw(string filename, string drawOption) const;
  inline float* GetData()                const {return fData;}
  inline float* GetDataEntry(int ientry) const {return fData+ientry*GetNEntriesPerBin(fPdfFunc);}
  inline long   GetSize()                const {long tot=1;for(uint i=0;i<fAxis.size();i++) tot*=int(fAxis[i]->GetSize()); return tot;}
  virtual inline std::string GetExtName() const {return GetVarName() + "_" + GetFuncName();}
  virtual std::string GetFuncName() const;
  virtual std::string GetVarName() const;
  virtual std::string GetVarUnit() const;
  virtual inline bool IsEmpty () const {return fIsEmpty;}
  void   		SetData(float* data);

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

  virtual int Write(fitsfile* fptr,int* status);
  virtual void Print() const;
};

#endif

