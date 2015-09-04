#ifndef G_IRF_Axis
#define G_IRF_Axis

#include <string>
#include "fitsio.h"

class GIRFAxis
{
 public: 

//  enum FormatType {kNoFormatType=0,							Posible solución para escribir un axis dentro del header del "Data HDU"
//		 kHDU,
//		 kHeader,
//		 kFormatMax};     // allowed axis types

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

  virtual inline AxisType GetAxisType() const {return fAxisType;}
  virtual inline VarType GetVarType() const {return fVarType;}

  virtual float GetRangeMin() const {return 0;}
  virtual float GetRangeMax() const {return 0;}
  virtual int   GetSize()     const {return 0;}

  virtual inline std::string GetExtName() const {return GetVarName() + "_" + GetTypeName();}
  virtual std::string GetTypeName() const;
  virtual std::string GetVarName() const;


  virtual bool operator==(const GIRFAxis& otherAxis){return 0;}													//TH: We will constantly check if Axis are equal... (when adding new Pdfs)
  
  virtual int Write(fitsfile* fptr,int& iaxis,int* status){*status=WRITE_ERROR;return *status;}
  virtual int WriteAxis(fitsfile* fptr,int iaxis,long size,float* data,int* status);
  virtual int IsAlreadyPresent(fitsfile* fptr,const GIRFAxis&,int* status){*status=READ_ERROR;return *status;};



 protected:
  virtual int CheckAxisConsistency();
  virtual void SetAxisType(AxisType type) {fAxisType=type;}

}; 

#endif

