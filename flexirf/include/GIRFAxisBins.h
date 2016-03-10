#ifndef G_IRF_AxisBins
#define G_IRF_AxisBins

#include <vector>
#include "GIRFAxis.h"

namespace flexIRF{
	class GIRFAxisBins : public GIRFAxis
	{
	 private:
	  std::vector<float> fAxisBins;
	  bool fAxisBinsFilled;

	 public:
	  GIRFAxisBins();           // create new empty bin axis
	  GIRFAxisBins(std::vector<float>::size_type size, ScaleType scaleType);   // create new bin axis with size
	  GIRFAxisBins(flexIRF::VarType vartype, ScaleType scaleType); // create new axis
	  GIRFAxisBins(flexIRF::VarType vartype,std::vector<float>::size_type size, ScaleType scaleType); // create new axis
	  GIRFAxisBins(flexIRF::VarType vartype,std::vector<float> bins, ScaleType scaleType); // create new axis
	  GIRFAxisBins(flexIRF::VarType vartype,std::vector<float>::size_type size,float* bins, ScaleType scaleType); // create new axis
	  GIRFAxisBins(fitsfile* fptr,int* status);
	  virtual ~GIRFAxisBins(){};

	  virtual bool operator==(const GIRFAxisBins& otherAxis);													//TH: We will constantly check if Axis are equal... (when adding new Pdfs)

	  bool ContainsRange(flexIRF::AxisRange axisRange);

	  virtual inline void 		SetAxisBins(std::vector<float> axisbins) {fAxisBinsFilled=1; fAxisBins=axisbins;}
	  inline std::vector<float> GetAxisBins() const {return fAxisBins;}

	  virtual inline void 	SetAxis(std::vector<float> axisbins)     {SetAxisBins(axisbins);}
	  virtual void 			SetAxis(std::vector<float>::size_type size,float* bins);

	  virtual inline float GetRangeMin() const {if (fAxisBinsFilled) return fAxisBins[0]; else return 0;}						//TODO: Improve!! they must be in order!!
	  virtual inline float GetRangeMax() const {if (fAxisBinsFilled) return fAxisBins[fAxisBins.size()-1]; else return 0;}		//TODO: Improve!! they must be in order!!
	  virtual inline int   GetSize()     const {return int(fAxisBins.size()-1);}

	  virtual int const 	Write(fitsfile* fptr, int& axisID, int* status);
	  virtual void const	Print();
	  virtual bool const	CheckAxisExists(fitsfile* fptr, int& axisID, int* status);

	  inline void 	ResizeBins(int lbin, int hbin){ fAxisBins.erase(fAxisBins.begin(),fAxisBins.begin()+lbin);fAxisBins.erase(fAxisBins.begin()+(hbin-lbin),fAxisBins.end());}
	  void 			Resize(float lValue, float hValue);
	  void 			Resize(float lValue, float hValue, int *lbin, int *hbin);
	  virtual int 	CheckAxisConsistency(); // return 0 if axis is consistent

	 protected:
	};
}
#endif

