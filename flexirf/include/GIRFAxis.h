#ifndef G_IRF_Axis
#define G_IRF_Axis

#include <string>
#include "fitsio.h"

class GIRFAxis
{
 public: 
  enum AxisType {kNoAxisType=0,
		 kBins,
		 kParam,
		 kAxisMax};     // allowed axis types

  enum VarType  {kNoVarType=0,
		 kEnergy,
		 kEnergy_true,
		 kEnergy_rec,
		 kTheta,
		 kPhi,
		 kID,
		 kVarMax}; // allowed variables   

  // for the time being units are:
  // Energy: GeV 
  // Energy_true: GeV
  // Energy_rec: GeV
  // Theta:  degree
  // Phi  :  degree
  // ID:     PDG particle ID numbering scheme

 private:
  AxisType  fAxisType;  // kind of axis (bins or parameterization)
  VarType   fVarType;   // variable described by axis

 public:
  GIRFAxis();                // create new empty axis
  GIRFAxis(VarType vartype); // create new axis for given variable

  virtual ~GIRFAxis(){};
  
  virtual inline void SetVarType(VarType vartype) {fVarType=vartype;}
 
  virtual inline VarType GetVarType()             {return fVarType;}

  virtual float GetRangeMin(){return 0;}
  virtual float GetRangeMax(){return 0;}
  virtual int   GetSize()    {return 0;}
  
  virtual int Write(fitsfile* fptr,int& iaxis,int* status){*status=WRITE_ERROR;return *status;}
  virtual int WriteAxis(fitsfile* fptr,int iaxis,long size,float* data,int* status);

 protected:
  virtual int CheckAxisConsistency();
  virtual void SetAxisType(AxisType type) {fAxisType=type;}
}; 

#endif

