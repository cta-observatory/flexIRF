#ifndef G_IRF_Axis
#define G_IRF_Axis

#include <string>
#include "fitsio.h"
#include <iostream>

using namespace std;

namespace flexIRF{

	enum AxisType {
		kNoAxisType = 0,
		kBins,
		kParam,
		kAxisMax
	};     // allowed axis types

	enum VarType {
		kNoVarType = 0,
		kEnergy,
		kEnergy_true,
		kEnergy_rec,
		kTheta,
		kPhi,
		kRad,
		kMigra,
		kID,
		kDetX,
		kDetY,
		kVarMax
	}; // allowed variables

	enum ScaleType {
		kNoScaleType = 0,
		kLinear,
		kLog10,
		kSqrt,
		kScaleMax
	}; // allowed variables


	struct AxisRange {
		VarType varType;
		float lowRange;
		float highRange;
		bool required;
	};

	class GIRFAxis {
	public:

		// for the time being units are:
		// Energy: GeV
		// Energy_true: GeV
		// Energy_rec: GeV
		// Theta:  degree
		// Phi  :  degree
		// ID:     PDG particle ID numbering scheme

	private:
		AxisType fAxisType;  		// Type of the axis (bins or parameterization)
		VarType fVarType;   		// Variable contained in axis (energy, theta, phi...)
		ScaleType fScaleType;		// Scale in which the variable is stored (linear, log...)
//		bool fIsLog;        		// if true, the interpolation/parameterization will be done on the log of the variable TODO: deprecated!

	public:
		GIRFAxis();                	// create new empty axis
		GIRFAxis(VarType vartype); 	// create new axis for given variable

		virtual ~GIRFAxis(){};

		virtual inline void SetVarType(VarType varType) {fVarType = varType;}
		virtual void SetVarType(std::string axisVarName);

		virtual inline AxisType GetAxisType() const {return fAxisType;}
		virtual inline VarType GetVarType() const {return fVarType;}

		virtual inline void SetScale(ScaleType scaleType) {fScaleType = scaleType;}
		virtual void SetScale(std::string scaleVarName);

		virtual inline ScaleType GetScale() const {return fScaleType;}
		virtual std::string 	GetScaleName() const {return GetScaleName(fScaleType);}
		static std::string 	GetScaleName(ScaleType scaleType);

	//  virtual GIRFAxis* GetAxis(fitsfile* fptr, int axisID, int* status);

		virtual float 	GetRangeMin() const {return 0;}
		virtual float 	GetRangeMax() const {return 0;}
		virtual int 	GetSize() const {return 0;}

//		virtual bool 		IsLog() const {return fIsLog;}
//		virtual inline void	SetLog(bool isLog=1) {fIsLog=isLog;}

		virtual inline std::string 	GetExtName() const {return GetVarName() + "_" + GetTypeName();}
		virtual std::string 		GetTypeName() const;
		virtual std::string 		GetVarName() const {return GetVarName(fVarType);}
		static std::string 			GetVarName(VarType varType);
		virtual std::string 		GetVarUnit() const;

		virtual bool operator==(const GIRFAxis& otherAxis) {return 1;}//TH: We will constantly check if Axis are equal... (when adding new Pdfs)

		virtual int const 	Write(fitsfile* fptr, int& axisID, int* status) {*status = WRITE_ERROR;	return *status;}
		virtual int const	WriteAxis(fitsfile* fptr, long size, float* data, int& lastID, int* status);
		virtual void const	Print() {};

		virtual bool 	ContainsRange(AxisRange axisRange) {return 0;}

		virtual inline void Resize(int lbin, int hbin){ cout << "ERROR: Resizing Axis of uncertain type." << endl;}
		virtual inline void Resize(float lValue, float hValue){ cout << "ERROR: Resizing Axis of uncertain type." << endl;}
		virtual inline void Resize(float lValue, float hValue, int *lbin, int *hbin){ cout << "ERROR: Resizing Axis of uncertain type." << endl;}
		virtual int		CheckAxisConsistency();

	protected:
		virtual bool const	CheckAxisExists(fitsfile* fptr, int& axisID, int* status) {return 0;}
		virtual void 		SetAxisType(AxisType type) {fAxisType = type;}
		virtual void 		SetAxisType(std::string axisTypeName);

	};
}
#endif

