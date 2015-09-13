#ifndef G_IRF_AxisBins
#define G_IRF_AxisBins

#include <vector>
#include "GIRFAxis.h"

class GIRFAxisBins : public GIRFAxis
{
 private:
  std::vector<float> fAxisBins;
  bool fIsLog;                    // if true, the interpolation will be done on the log of the variable
  bool fAxisBinsFilled;
 public:
  GIRFAxisBins();           // create new empty bin axis
  GIRFAxisBins(std::vector<float>::size_type size,bool islog=false);   // create new bin axis with size
  GIRFAxisBins(VarType vartype,bool islog=false); // create new axis 
  GIRFAxisBins(VarType vartype,std::vector<float>::size_type size,bool islog=false); // create new axis 
  GIRFAxisBins(VarType vartype,std::vector<float> bins,bool islog=false); // create new axis 
  GIRFAxisBins(VarType vartype,std::vector<float>::size_type size,float* bins,bool islog=false); // create new axis 
  GIRFAxisBins(fitsfile* fptr,int* status);
  virtual ~GIRFAxisBins(){};

  virtual bool operator==(const GIRFAxisBins& otherAxis);													//TH: We will constantly check if Axis are equal... (when adding new Pdfs)

  virtual inline void SetAxisBins(std::vector<float> axisbins) {fAxisBinsFilled=1; fAxisBins=axisbins;}
  inline std::vector<float> GetAxisBins() const {return fAxisBins;}

  virtual inline void SetAxis(std::vector<float> axisbins)     {SetAxisBins(axisbins);}
  virtual void SetAxis(std::vector<float>::size_type size,float* bins);

  virtual inline float GetRangeMin() const {if (fAxisBinsFilled) return fAxisBins[0]; else return 0;}
  virtual inline float GetRangeMax() const {if (fAxisBinsFilled) return fAxisBins[fAxisBins.size()-1]; else return 0;}
  virtual inline int   GetSize()     const {return int(fAxisBins.size());}

  virtual int Write(fitsfile* fptr, int& lastID, int* status);
  virtual int IsAlreadyPresent(fitsfile* fptr,int iaxis,long size,float* data,int* status);
  virtual void Print();
  virtual bool CheckAxisExists(fitsfile* fptr, int* status);

 protected:
  virtual int CheckAxisConsistency(); // return 0 if axis is consistent
}; 

#endif

