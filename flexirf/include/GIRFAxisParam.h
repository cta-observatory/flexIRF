#ifndef G_IRF_AxisParam
#define G_IRF_AxisParam

#include <vector>
#include "GIRFAxis.h"

class GIRFAxisParam : public GIRFAxis
{
 private:
  std::vector<float> fAxisParam;
  bool fIsLog;                    // if true, the parameterization will use the log of the variable

 public:
  GIRFAxisParam();           // create new empty bin axis
  GIRFAxisParam(std::vector<float>::size_type size,bool islog=false);   // create new bin axis with size
  GIRFAxisParam(VarType vartype,bool islog=false); // create new axis 
  GIRFAxisParam(VarType vartype,std::vector<float>::size_type size,bool islog=false); // create new axis 
  GIRFAxisParam(VarType vartype,std::vector<float> bins,bool islog=false); // create new axis 
  GIRFAxisParam(VarType vartype,std::vector<float>::size_type size,float* bins,bool islog=false); // create new axis 
  GIRFAxisParam(fitsfile* fptr,int* status);

  bool ContainsRange(AxisRange axisRange);

  virtual ~GIRFAxisParam(){};

  virtual inline void SetAxisParam(std::vector<float> axisbins) {fAxisParam=axisbins;}
  virtual inline void SetAxis(std::vector<float> axisbins)      {SetAxisParam(axisbins);}
  virtual        void SetAxis(std::vector<float>::size_type size,float* bins);

  virtual inline int   GetSize()                                {return int(fAxisParam.size());}
  
  virtual int Write(fitsfile* fptr, int& axisID, int* status);
  virtual bool CheckAxisExists(fitsfile* fptr, int& axisID, int* status);

 protected:
  virtual int CheckAxisConsistency(); // return 0 if axis is consistent
}; 

#endif

