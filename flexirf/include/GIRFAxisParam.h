#ifndef G_IRF_AxisParam
#define G_IRF_AxisParam

#include <vector>
#include <string>
#include "GIRFAxis.h"

class GIRFAxisParam: public GIRFAxis {


public:
	struct AxisParameterization{
		std::vector<float> constants;							// Array containing the constant parameters used in fFormula.
		float validRangeLow;
		float validRangeHigh;
		int numParameters;
		int numConstants;
		string formula;
	};

private:

	AxisParameterization fAxisParam;
	bool fAxisParameterizationFilled;

public:
	GIRFAxisParam();           // create new empty bin axis
	GIRFAxisParam(VarType vartype, bool islog = false); // create new axis
	GIRFAxisParam(VarType vartype, AxisParameterization axisParam, bool islog = false); // create new axis
	GIRFAxisParam(VarType vartype, std::vector<float>::size_type size,
			bool islog = false); // create new axis
	GIRFAxisParam(VarType vartype, std::vector<float> bins, bool islog = false); // create new axis
	GIRFAxisParam(VarType vartype, std::vector<float>::size_type size,
			float* bins, bool islog = false); // create new axis
	GIRFAxisParam(fitsfile* fptr, int* status);

	bool ContainsRange(AxisRange axisRange);

	virtual ~GIRFAxisParam() {};

	virtual string 		GetFormula(){return fAxisParam.formula;}
	virtual inline int 	GetNumVars() { return int(fAxisParam.numParameters);}
	virtual float 		GetRangeMin() const {return float(fAxisParam.validRangeLow);}
	virtual float 		GetRangeMax() const {return float(fAxisParam.validRangeHigh);}
	virtual inline int 	GetSize() const { return int(fAxisParam.constants.size());}

	virtual inline void SetAxis(std::vector<float> axisbins) { SetAxisConstants(axisbins);}
	virtual void 		SetAxis(std::vector<float>::size_type size, float* bins);
	virtual void 		SetAxisParam(AxisParameterization axisParam);
	virtual inline void SetAxisConstants(std::vector<float> axisConstants) { fAxisParam.constants = axisConstants;}
	virtual void 		SetFormula(string formula, std::vector<float>::size_type numParameters, float* parameters, int numVariables);

	virtual int const	Write(fitsfile* fptr, int& axisID, int* status);
	virtual bool const	CheckAxisExists(fitsfile* fptr, int& axisID, int* status);

	virtual inline void Resize(int lbin, int hbin) {;}
	virtual inline void Resize(float lValue, float hValue) {;}
	virtual inline void Resize(float lValue, float hValue, int *lbin, int *hbin) {;}
	virtual int 		CheckAxisConsistency(); // return 0 if axis is consistent

	virtual void const			Print();

protected:

	virtual inline bool CheckFormulaEmpty(){if (fAxisParam.formula.empty()) return 1; else return 0;}
	virtual bool CheckAxisParameterizationFilled();

};

#endif

