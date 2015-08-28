#ifndef G_IRF_AxisBins
#define G_IRF_AxisBins

#include <vector>
#include "GIRFAxis.h"

class GIRFAxisBins : public GIRFAxis
{
 private:
  std::vector<float> fAxisBins;
  bool fIsLog;                    // if true, the interpolation will be done on the log of the variable

 public:
  GIRFAxisBins();           // create new empty bin axis
  GIRFAxisBins(std::vector<float>::size_type size,bool islog=false);   // create new bin axis with size
  GIRFAxisBins(VarType vartype,bool islog=false); // create new axis 
  GIRFAxisBins(VarType vartype,std::vector<float>::size_type size,bool islog=false); // create new axis 
  GIRFAxisBins(VarType vartype,std::vector<float> bins,bool islog=false); // create new axis 
  GIRFAxisBins(VarType vartype,std::vector<float>::size_type size,float* bins,bool islog=false); // create new axis 

  virtual ~GIRFAxisBins(){};

  virtual bool operator==(const GIRFAxis& otherAxis);													//TH: We will constantly check if Axis are equal... (when adding new Pdfs)

  virtual inline void SetAxisBins(std::vector<float> axisbins) {fAxisBins=axisbins;}
  virtual inline void SetAxis(std::vector<float> axisbins)     {SetAxisBins(axisbins);}
  virtual void SetAxis(std::vector<float>::size_type size,float* bins);

  virtual inline float GetRangeMin() const {return fAxisBins[0];}
  virtual inline float GetRangeMax() const {return fAxisBins[fAxisBins.size()];}
  virtual inline int   GetSize()     const {return int(fAxisBins.size());}

  virtual int Write(fitsfile* fptr,int& iaxis,int* status);
  
 protected:
  virtual int CheckAxisConsistency(); // return 0 if axis is consistent
}; 

#endif

