#ifndef G_IRFConfig
#define G_IRFConfig

#include <string>
#include <vector>
#include <GIRFRange.h>

class GIRFConfig {
public:

	//////////////////////////////////////////////////////////
	//						TODO
	//			Ranges need to be added!!!
	//	The GIRF.GetPdf should select the ranges they want.
	//
	//////////////////////////////////////////////////////////
	enum MolProfile {
		kNoMolProfile = 0,
		kAverage,
		kSummer,
		kWinter,
		kSpring,
		kAutumn,
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

	inline std::vector<GIRFAxis::AxisRange> GetAxisRanges(){return fGIRFRange.GetAxisRanges();}
	inline void AddAxisRange(GIRFAxis::VarType varType, float valueMin, float valueMax){fGIRFRange.AddAxisRange(varType, valueMin, valueMax);}

	void SetDefault();
	void Print();
};

#endif

