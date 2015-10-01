#ifndef G_IRFConfig
#define G_IRFConfig

#include <string>
#include <GIRFAxis.h>

class GIRFRange {
public:

	struct AxisRange {
		GIRFAxis::VarType varType;
		float lowRange;
		float highRange;
	};

	// TODO
	// Add Event selection... Pointing direction... Etc....

private:
	std::vector<AxisRange*> fAxisRanges;
//	int LoadConfigFile(std::string filename);

public:
    inline void AddAxisRange(AxisRange* axisRange)  {fAxisRanges.push_back(axisRange);} 		// insert axis range in the list
    void AddAxisRange(GIRFAxis::VarType varType, float valueMin, float valueMax); 		// insert axis range in the list

	GIRFRange();
	virtual ~GIRFRange() {};
};

#endif

