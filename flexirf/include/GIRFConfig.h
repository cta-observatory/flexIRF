#ifndef G_IRFConfig
#define G_IRFConfig

#include <string>
#include <vector>
#include <GIRFRange.h>


enum MolProfile {
	kNoMolProfile = 0,
	kAverageProf,
	kSummerProf,
	kWinterProf,
	kSpringProf,
	kAutumnProf,
	kMolProfileMax
};

enum NSBLevel {
	kNoNSBLevel = 0, kExtragalactic, kGalactic, kNSBLevelMax
};

enum ObsMode {
	kNoObsMode = 0, kPoint, kDivergent, kObsModeMax
};
enum SubArray {
	kNoSubArray = 0, kFull, kLSTs, kMSTSSTs, kSubArrayMax
};
enum Trigger {
	kNoTrigger = 0, kStandard, kHighDTs, kTriggerMax
};

struct HWStatus {
	float reflectivity;
	Trigger trigger;
};

struct Weather {
	float Transmitance;
	MolProfile molProfile;
};

class GIRFConfig {
public:

	//////////////////////////////////////////////////////////
	//						TODO
	//			Ranges need to be added!!!
	//	The GIRF.GetPdf should select the ranges they want.
	//
	//////////////////////////////////////////////////////////

	// TODO
	// Add Event selection... Pointing direction... Etc....

private:
	Weather fWeather;
	NSBLevel fNSBLevel;
	ObsMode fObsMode;
	SubArray fSubArray;
	HWStatus fHWStatus;
	GIRFRange fGIRFRange;
	int LoadConfigFile(std::string filename);

public:

	GIRFConfig();
	GIRFConfig(std::string filename);
	virtual ~GIRFConfig() {};

	inline std::vector<AxisRange> GetAxisRanges() const {return fGIRFRange.GetAxisRanges();}
	inline void AddAxisRange(VarType varType, float valueMin, float valueMax){fGIRFRange.AddAxisRange(varType, valueMin, valueMax);}
	inline void AddAxisRange(VarType varType, float valueMin, float valueMax, bool required){fGIRFRange.AddAxisRange(varType, valueMin, valueMax, required);}

	void SetDefault();
	void Print();
};

#endif

