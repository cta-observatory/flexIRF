#ifndef G_IRF_Range
#define G_IRF_Range

#include <string>
#include <vector>
#include <GIRFAxis.h>

class GIRFRange {

private:
	std::vector<GIRFAxis::AxisRange> fAxisRanges;
//	int LoadConfigFile(std::string filename);

public:
    inline void AddAxisRange(GIRFAxis::AxisRange axisRange)  {fAxisRanges.push_back(axisRange);} 		// insert axis range in the list
    void AddAxisRange(GIRFAxis::VarType varType, float valueMin, float valueMax); 		// insert axis range in the list

	GIRFRange();
	virtual ~GIRFRange() {};
	inline std::vector<GIRFAxis::AxisRange> GetAxisRanges(){return fAxisRanges;}
	inline uint GetNumAxisRanges(){return fAxisRanges.size();}

};

#endif

