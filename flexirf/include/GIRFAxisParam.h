#ifndef G_IRF_AxisParam
#define G_IRF_AxisParam

#include <vector>
#include <string>
#include "GIRFAxis.h"

class GIRFAxisParam: public GIRFAxis {
private:
	std::vector<float> fAxisParam;
	int fNumVariables;
	string fFormula;
	bool fIsLog; // if true, the parameterization will use the log of the variable

public:
	GIRFAxisParam();           // create new empty bin axis
	GIRFAxisParam(std::vector<float>::size_type size, bool islog = false); // create new bin axis with size
	GIRFAxisParam(VarType vartype, bool islog = false); // create new axis
	GIRFAxisParam(VarType vartype, std::vector<float>::size_type size,
			bool islog = false); // create new axis
	GIRFAxisParam(VarType vartype, std::vector<float> bins, bool islog = false); // create new axis
	GIRFAxisParam(VarType vartype, std::vector<float>::size_type size,
			float* bins, bool islog = false); // create new axis
	GIRFAxisParam(fitsfile* fptr, int* status);

	bool ContainsRange(AxisRange axisRange);

	virtual ~GIRFAxisParam() {};

	virtual inline void SetAxisParam(std::vector<float> axisbins) { fAxisParam = axisbins;}
	virtual inline void SetAxis(std::vector<float> axisbins) { SetAxisParam(axisbins);}
	virtual void 		SetAxis(std::vector<float>::size_type size, float* bins);
	virtual void 		SetFormula(string formula, std::vector<float>::size_type numParameters, float* parameters, int numVariables);
	virtual inline int 	GetSize() { return int(fAxisParam.size());}

	virtual int 	Write(fitsfile* fptr, int& axisID, int* status);
	virtual bool 	CheckAxisExists(fitsfile* fptr, int& axisID, int* status);

	virtual inline void Resize(int lbin, int hbin) {;}
	virtual inline void Resize(float lValue, float hValue) {;}
	virtual inline void Resize(float lValue, float hValue, int *lbin, int *hbin) {;}
	virtual int CheckAxisConsistency(); // return 0 if axis is consistent

protected:

	virtual inline bool CheckFormulaEmpty(){if (fFormula.empty()) return 1; else return 0;}

};

#endif

